#include "AiLogic.h"
#include "Pieces.h"
#include <iomanip>
#include <iostream>
#include <algorithm>

AiLogic::AiLogic(GameDataRef data): _data(data){
    _board = new Board(data);
    _board->Init();

    initializeZobristTable();
    clearTT(); 
}

AiLogic::~AiLogic(){
    delete _board;
}

void AiLogic::initializeZobristTable() {
    std::mt19937_64 generator(123456); 
    std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);

    for (int square = 0; square < 64; ++square) {
        for (int piece = 0; piece < 12; ++piece) {
            zobristTable[square][piece] = distribution(generator);
        }
    }
}

void AiLogic::clearTT() {
    for (int i = 0; i < TT_SIZE; ++i) {
        transpositionTable[i].hash = 0;
    }
}

void AiLogic::storeTT(uint64_t hash, float score, int depth, int flag, Piece* bestPiece, Coordinate bestTarget) {
    int index = hash % TT_SIZE;
    if (transpositionTable[index].hash == 0 || transpositionTable[index].depth <= depth) {
        transpositionTable[index].hash = hash;
        transpositionTable[index].score = score;
        transpositionTable[index].depth = depth;
        transpositionTable[index].flag = flag;
        
        if (bestPiece) {
            transpositionTable[index].bestMoveSrcX = bestPiece->getBoardPosition().x;
            transpositionTable[index].bestMoveSrcY = bestPiece->getBoardPosition().y;
            transpositionTable[index].bestMoveDstX = bestTarget.x;
            transpositionTable[index].bestMoveDstY = bestTarget.y;
        } else {
            transpositionTable[index].bestMoveSrcX = -1;
        }
    }
}

bool AiLogic::probeTT(uint64_t hash, int depth, float alpha, float beta, float& score, Piece*& ttPiece, Coordinate& ttTarget) {
    int index = hash % TT_SIZE;
    TTEntry& entry = transpositionTable[index];

    if (entry.hash == hash) {
        if (entry.bestMoveSrcX != -1) {
            ttPiece = _board->getPieceAt(entry.bestMoveSrcX, entry.bestMoveSrcY);
            ttTarget = Coordinate(entry.bestMoveDstX, entry.bestMoveDstY);
        }

        if (entry.depth >= depth) {
            if (entry.flag == 0) { score = entry.score; return true; }
            if (entry.flag == 1 && entry.score <= alpha) { score = alpha; return true; }
            if (entry.flag == 2 && entry.score >= beta) { score = beta; return true; }
        }
    }
    return false;
}

uint64_t AiLogic::computeZobristHash(Board* board) {
    uint64_t hash = 0;
    for (Piece* piece : board->b_pieces) {
        Coordinate pos = piece->getBoardPosition();
        if (pos.x != -1 && pos.y != -1) {
            int pieceIndex = static_cast<int>(piece->getType()) + (piece->getColor() == WHITE ? 0 : 6);
            int squareIndex = pos.y * 8 + pos.x;
            hash ^= zobristTable[squareIndex][pieceIndex];
        }
    }
    return hash;
}

void AiLogic::handleCastle(Piece* movedPiece, const Coordinate& target, Piece*& rook, Coordinate& rookOriginalPosition, Coordinate& rookTargetPosition) {
    if (movedPiece->getType() == Piece::PieceType::King) {
        bool isWhite = (movedPiece->getColor() == WHITE);
        int y = isWhite ? 7 : 0;

        if (movedPiece->getBoardPosition().x == 4 && movedPiece->getBoardPosition().y == y) {
            if (target.x == 6 && target.y == y) { 
                rook = _board->getPieceAt(7, y);
            } else if (target.x == 2 && target.y == y) { 
                rook = _board->getPieceAt(0, y);
            }

            if (rook) {
                rookOriginalPosition = rook->getBoardPosition();
                rookTargetPosition = _board->getRookTargetForCastle(target);
            }
        }
    }
}

void AiLogic::undoCastle(Piece* rook, const Coordinate& rookOriginalPosition) {
    if (rook) {
        rook->simulateMove(rookOriginalPosition.x, rookOriginalPosition.y, false);
    }
}

float AiLogic::getPositionValue(Piece* piece) {
    if (!piece) return 0;
    Coordinate pos = piece->getBoardPosition();
    int x = pos.x;
    int y = pos.y;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;

    auto type = piece->getType();

    switch (type) {
        case Piece::PieceType::Pawn:
            return (piece->getColor() == WHITE) ? pawnWhitePositionValues[y][x] : pawnBlackPositionValues[y][x];
        case Piece::PieceType::Knight: return knightPositionValues[y][x];
        case Piece::PieceType::Bishop: return bishopPositionValues[y][x];
        case Piece::PieceType::Rook:   return rookPositionValues[y][x];
        case Piece::PieceType::Queen:  return queenPositionValues[y][x];
        case Piece::PieceType::King:
            if (piece->getColor() == WHITE) return kingPositionValuesWhite[y][x];
            else return kingPositionValuesBlack[y][x];
        default: return 0;
    }
}

float AiLogic::evaluate(int color) {
    float whiteScore = 0;
    float blackScore = 0;

    for (Piece* piece : _board->b_pieces) {
        if (piece->getBoardPosition().x == -1) continue;

        if (piece->getColor() == WHITE) {
            whiteScore += piece->getValue();
            whiteScore += getPositionValue(piece);
        } else {
            blackScore += piece->getValue();
            blackScore += getPositionValue(piece);
        }
    }

    float evaluation = whiteScore - blackScore;
    return (color == WHITE) ? evaluation : -evaluation;
}

float AiLogic::quiescence(float alpha, float beta, int color, int qDepth) {
    float standPat = evaluate(color);

    if (qDepth <= 0) return standPat;
    if (standPat >= beta) return beta;
    if (alpha < standPat) alpha = standPat;

    MoveList allMoves;
    getSortedMoves(color, allMoves, nullptr, Coordinate(-1, -1));

    for (const auto& move : allMoves) {
        Coordinate target(move.toX, move.toY);
        Piece* targetPiece = _board->getPieceAt(target.x, target.y);

        if (!targetPiece) continue;
        if (targetPiece->getType() == Piece::PieceType::King) continue;

        Piece* movedPiece = move.piece; 
        Coordinate originalPosition = movedPiece->getBoardPosition();

        movedPiece->simulateMove(target.x, target.y, false);
        targetPiece->simulateMove(-1, -1, false);

        float score = -quiescence(-beta, -alpha, 1 - color, qDepth - 1);

        movedPiece->simulateMove(originalPosition.x, originalPosition.y, false);
        targetPiece->simulateMove(target.x, target.y, false);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

float AiLogic::negaMax(int depth, int color, float alpha, float beta) {
    uint64_t currentHash = computeZobristHash(_board);
    float ttScore = 0;
    Piece* ttPiece = nullptr;
    Coordinate ttTarget = {-1, -1};

    if (probeTT(currentHash, depth, alpha, beta, ttScore, ttPiece, ttTarget)) {
        return ttScore;
    }

    if (depth <= 0) {
        return quiescence(alpha, beta, color, 4);
    }

    MoveList allMoves;
    getSortedMoves(color, allMoves, ttPiece, ttTarget);

    if (allMoves.count == 0) {
        if (_board->isKingInCheck(color)) {
            return -50000 + depth;
        } else {
            return 0;
        }
    }

    float maxScore = -100000;
    int flag = 1; // ALPHA

    Piece* bestPieceForTT = nullptr;
    Coordinate bestTargetForTT = {-1, -1};

    for (const auto& move : allMoves) {
        Piece* movedPiece = move.piece;
        Coordinate target(move.toX, move.toY);
        Piece* capturedPiece = _board->getPieceAt(target.x, target.y);
        Coordinate originalPosition = movedPiece->getBoardPosition();
        
        Coordinate rookOriginal, rookTarget;
        Piece* rook = nullptr;
        handleCastle(movedPiece, target, rook, rookOriginal, rookTarget);

        movedPiece->simulateMove(target.x, target.y, false);
        if (capturedPiece) capturedPiece->simulateMove(-1, -1, false);
        if (rook) rook->simulateMove(rookTarget.x, rookTarget.y, false);

        float currentScore = -negaMax(depth - 1, 1 - color, -beta, -alpha);

        movedPiece->simulateMove(originalPosition.x, originalPosition.y, false);
        if (capturedPiece) capturedPiece->simulateMove(target.x, target.y,false);
        undoCastle(rook, rookOriginal);

        if (currentScore >= beta) {
            storeTT(currentHash, beta, depth, 2, movedPiece, target);
            return beta;
        }
        if (currentScore > maxScore) {
            maxScore = currentScore;
            bestPieceForTT = movedPiece;
            bestTargetForTT = target;
            
            if (currentScore > alpha) {
                alpha = currentScore;
                flag = 0; // EXACT
            }
        }
    }

    storeTT(currentHash, maxScore, depth, flag, bestPieceForTT, bestTargetForTT);
    return maxScore;
}

void AiLogic::getSortedMoves(int color, MoveList& moves, Piece* ttPiece, Coordinate ttTarget) {
    _board->generateAllMoves(color, moves);

    std::sort(moves.begin(), moves.end(), [this, ttPiece, ttTarget](const Move& a, const Move& b) {
        bool isBestMoveA = (ttPiece && a.piece == ttPiece && a.toX == ttTarget.x && a.toY == ttTarget.y);
        bool isBestMoveB = (ttPiece && b.piece == ttPiece && b.toX == ttTarget.x && b.toY == ttTarget.y);
        
        if (isBestMoveA) return true;
        if (isBestMoveB) return false;

        int scoreA = 0;
        int scoreB = 0;

        Piece* victimA = _board->getPieceAt(a.toX, a.toY);
        Piece* victimB = _board->getPieceAt(b.toX, b.toY);

        if (victimA) scoreA = 10 * victimA->getValue() - a.piece->getValue();
        if (victimB) scoreB = 10 * victimB->getValue() - b.piece->getValue();

        return scoreA > scoreB;
    });
}

std::pair<Piece*, Coordinate> AiLogic::getBestMove(int depth, int color) {
    float bestScore = -100000;
    std::pair<Piece*, Coordinate> bestMove = {nullptr, {-1, -1}};
    float alpha = -100000, beta = 100000;

    MoveList allMoves;
    getSortedMoves(color, allMoves, nullptr, Coordinate(-1, -1));

    for (const auto& move : allMoves) {
        Piece* movedPiece = move.piece;
        Coordinate target(move.toX, move.toY);
        Piece* capturedPiece = _board->getPieceAt(target.x, target.y);
        Coordinate originalPosition = movedPiece->getBoardPosition();
        
        Coordinate rookOriginal, rookTarget;
        Piece* rook = nullptr;
        handleCastle(movedPiece, target, rook, rookOriginal, rookTarget);

        movedPiece->simulateMove(target.x, target.y, false);
        if (capturedPiece) capturedPiece->simulateMove(-1, -1, false);
        if (rook) rook->simulateMove(rookTarget.x, rookTarget.y,false);

        float score = -negaMax(depth - 1, 1 - color, -beta, -alpha);

        movedPiece->simulateMove(originalPosition.x, originalPosition.y, false);
        if (capturedPiece) capturedPiece->simulateMove(target.x, target.y, false);
        undoCastle(rook, rookOriginal);

        if (score > bestScore) {
            bestScore = score;
            bestMove = {movedPiece, target};
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    
    std::cout << "AI Best Score: " << bestScore << " at Depth: " << depth << std::endl;
    return bestMove;
}

void AiLogic::aiMove(int color) {
    std::cout << "AI thinking..." << std::endl;
    std::pair<Piece*, Coordinate> bestMove;
    
    for (int d = 1; d <= 5; ++d) {
        bestMove = getBestMove(d, color);
    }
    
    if (bestMove.first == nullptr) {
        std::cout << "MAT or PAT for ai" << std::endl;
        return;
    }

    Piece* movedPiece = bestMove.first;
    Coordinate target = bestMove.second;
    Piece* capturedPiece = _board->getPieceAt(target.x, target.y);
    
    Coordinate rookOriginal, rookTarget;
    Piece* rook = nullptr;
    handleCastle(movedPiece, target, rook, rookOriginal, rookTarget);

   if (capturedPiece) {
        capturedPiece->simulateMove(-1, -1, false); 
    }
    
    movedPiece->move(target.x, target.y, true); 
    
    if(rook) rook->move(rookTarget.x, rookTarget.y, true); 
}