#include "AiLogic.h"
#include "Pieces.h"


AiLogic::AiLogic(GameDataRef data): _data(data){
    _board = new Board(data);
    _board->Init();
}

AiLogic::~AiLogic(){
    delete _board;
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
        rook->simulateMove(rookOriginalPosition.x, rookOriginalPosition.y);
    }
}

int AiLogic::getPositionValue(Piece* piece) {
    if (!piece) return 0;

    Coordinate pos = piece->getBoardPosition();
    int x = pos.x;
    int y = pos.y;

    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;

    switch (piece->getType()) {
        case piece->PieceType::Pawn:
            if(piece->getColor() == WHITE) return pawnWhitePositionValues[y][x];
            else return pawnBlackPositionValues[y][x];

        case piece->PieceType::Knight: return knightPositionValues[y][x];
        case piece->PieceType::Bishop: return bishopPositionValues[y][x];
        case piece->PieceType::Rook: return rookPositionValues[y][x];
        case piece->PieceType::Queen: return queenPositionValues[y][x];
        case piece->PieceType::King:
            if (piece->getColor() == WHITE) return kingPositionValuesWhite[y][x];
            else return kingPositionValuesBlack[y][x];

        default:
            return 0;
    }
}

bool AiLogic::canBeCaptured(Piece* piece) {
    if (!piece) return false;

    for (Piece* enemyPiece : _board->enemyPieces(piece->getColor())) {
        if (enemyPiece->isValidMove(piece->getBoardPosition().x, piece->getBoardPosition().y)) {
            return true;
        }
    }
    return false;
}


int AiLogic::canBeCapturedRecursive(Piece* piece) {
    // do zrobienia
    int score = 0;
    
    return 0;
}



float AiLogic::evaluatePositionBlack(int color, Piece* RecentlyMovedPiece, int depth) {
    float score = 0;
    
    for (Piece* piece : _board->enemyPieces(color)) {
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            continue;
        }
        score -= piece->getValue();
        score -= getPositionValue(piece);
    }
    for (Piece* piece : _board->playerPieces(color)) {
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            continue;
        }
        score += piece->getValue();
        score += getPositionValue(piece); 
    }

    if(canBeCaptured(RecentlyMovedPiece) && RecentlyMovedPiece->getColor() == BLACK){
        score -= RecentlyMovedPiece->getValue() - getPositionValue(RecentlyMovedPiece);
    }

    if(_board->isCheckmate(color)){
        score -= 5000 - depth *10;
    }
    if(_board->isCheckmate(1 - color)){
        score += 5000 + depth *10;
    }
    return score;
}

float AiLogic::evaluatePositionWhite(int color, Piece* RecentlyMovedPiece, int depth) {
    float score = 0;
    
    for (Piece* piece : _board->enemyPieces(color)) {
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            continue;
        }
        score += piece->getValue();
        score += getPositionValue(piece);
    }
    for (Piece* piece : _board->playerPieces(color)) {
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            continue;
        }
        score -= piece->getValue();
        score -= getPositionValue(piece); 
    }

    if(canBeCaptured(RecentlyMovedPiece) && RecentlyMovedPiece->getColor() == WHITE){
        score += RecentlyMovedPiece->getValue() + getPositionValue(RecentlyMovedPiece);
    }

    //bialy chce jak najmniej punktow 

    if(_board->isCheckmate(color)){ // sprawdzenie czy bialy nie dostal mata im mniejsza glebokosc tym lepiej

        score += 5000 + depth *10;
    }
    if(_board->isCheckmate(1 - color)){ // sprawdzenie czy bialy nie zamatowal przeciwnika im mniejsza glebokosc tym lepiej
        score -= 5000 - depth *10;
    }
    return score;
}


float AiLogic::minimax(int depth, int color, bool maximizingPlayer, float alpha, float beta, Piece* RecentlyMovedPiece) {
    if (depth == 0 || _board->isCheckmate(color) || _board->isStalemate(color)) {
        if (color == WHITE) 
            return evaluatePositionWhite(color, RecentlyMovedPiece, depth);
        else 
            return evaluatePositionBlack(color, RecentlyMovedPiece, depth);
    }

    if (maximizingPlayer) {
        float maxEval = -10000;
        auto allMoves = getAllMovesSorted(color);

        for (const auto& move : allMoves) {
            Piece* movedPiece = move.first;
            Coordinate target = move.second;

            if(movedPiece->getBoardPosition().x == -1 && movedPiece->getBoardPosition().y == -1){
                continue;
            }


            Piece* capturedPiece = _board->getPieceAt(target.x, target.y);

            if (!movedPiece) continue;

            Coordinate originalPosition = movedPiece->getBoardPosition();
            Coordinate rookOriginalPosition, rookTargetPosition;
            Piece* rook = nullptr;

            handleCastle(movedPiece, target, rook, rookOriginalPosition, rookTargetPosition);
            
            // Wykonaj ruch
            movedPiece->simulateMove(target.x, target.y);
            if (capturedPiece) capturedPiece->simulateMove(-1, -1);
            if (rook) rook->simulateMove(rookTargetPosition.x, rookTargetPosition.y);

            // Rekurencja
            float eval = minimax(depth - 1, 1 - color, false, alpha, beta, movedPiece);

            // Cofnij ruch
            movedPiece->simulateMove(originalPosition.x, originalPosition.y);
            if (capturedPiece) capturedPiece->simulateMove(target.x, target.y);
            undoCastle(rook, rookOriginalPosition);

            // Aktualizuj wynik
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            // Przytnij
            if (beta <= alpha) break;
        }
        return maxEval;

    } else {
        float minEval = 10000;
        auto allMoves = getAllMovesSorted(color);

        for (const auto& move : allMoves) {
            Piece* movedPiece = move.first;
            Coordinate target = move.second;

            if(movedPiece->getBoardPosition().x == -1 && movedPiece->getBoardPosition().y == -1){
                continue;
            }

            Piece* capturedPiece = _board->getPieceAt(target.x, target.y);

            if (!movedPiece) continue;

            Coordinate originalPosition = movedPiece->getBoardPosition();
            Coordinate rookOriginalPosition, rookTargetPosition;
            Piece* rook = nullptr;

            handleCastle(movedPiece, target, rook, rookOriginalPosition, rookTargetPosition);

            // Wykonaj ruch
            movedPiece->simulateMove(target.x, target.y);
            if (capturedPiece) capturedPiece->simulateMove(-1, -1);
            if (rook) rook->simulateMove(rookTargetPosition.x, rookTargetPosition.y);

            // Rekurencja
            float eval = minimax(depth - 1, 1 - color, true, alpha, beta, movedPiece);

            // Cofnij ruch
            movedPiece->simulateMove(originalPosition.x, originalPosition.y);
            if (capturedPiece) capturedPiece->simulateMove(target.x, target.y);
            undoCastle(rook, rookOriginalPosition);

            // Aktualizuj wynik
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            // Przytnij
            if (beta <= alpha) break;
        }
        return minEval;
    }
}


std::vector<std::pair<Piece*, Coordinate>> AiLogic::getAllMovesSorted(int color) {
    auto allMoves = _board->getAllMoves(color);
    std::vector<std::pair<Piece*, Coordinate>> sortedMoves(allMoves.begin(), allMoves.end());

    std::sort(sortedMoves.begin(), sortedMoves.end(), [this](const std::pair<Piece*, Coordinate>& a, const std::pair<Piece*, Coordinate>& b) {
        int valueA = 0;
        int valueB = 0;

        if (a.second.x >= 0 && a.second.y >= 0) {
            Piece* pieceAtA = _board->getPieceAt(a.second.x, a.second.y);
            if (pieceAtA) valueA = pieceAtA->getValue();
        }
        if (b.second.x >= 0 && b.second.y >= 0) {
            Piece* pieceAtB = _board->getPieceAt(b.second.x, b.second.y);
            if (pieceAtB) valueB = pieceAtB->getValue();
        }

        // Sortujemy według wartości figur do zbicia (malejąco)
        return valueA > valueB;
    });

    return sortedMoves;
}






std::pair<Piece*, Coordinate> AiLogic::getBestMove(int depth, int color) {
    float bestScore = -10000;
    std::pair<Piece*, Coordinate> bestMove;
    float alpha = -10000, beta = 10000;

    auto allMoves = getAllMovesSorted(color);

    for (const auto& move : allMoves) {
        Piece* movedPiece = move.first;
        Coordinate target = move.second;
        Piece* capturedPiece = _board->getPieceAt(target.x, target.y);

        Coordinate originalPosition = movedPiece->getBoardPosition();

        Coordinate rookOriginalPosition, rookTargetPosition;
        Piece* rook = nullptr;


        handleCastle(movedPiece, target, rook, rookOriginalPosition, rookTargetPosition);

        // Wykonaj ruch
        movedPiece->simulateMove(target.x, target.y);
        if (capturedPiece) capturedPiece->simulateMove(-1, -1);
        if(rook){
            rook->simulateMove(rookTargetPosition.x, rookTargetPosition.y);
        }

        // Oblicz ocenę
        float score = minimax(depth - 1, 1 - color, false, alpha, beta, movedPiece);

        // Cofnij ruch
        movedPiece->simulateMove(originalPosition.x, originalPosition.y);
        if (capturedPiece) capturedPiece->simulateMove(target.x, target.y);
        undoCastle(rook, rookOriginalPosition);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        alpha = std::max(alpha, score);
        if (beta <= alpha) break; // Przycinanie
    }

    std::cout << "Best Score: " << bestScore << std::endl;

    return bestMove;
}


std::pair<Piece*, Coordinate> AiLogic::getBestMoveIterative(int maxDepth, int color) {
    std::pair<Piece*, Coordinate> bestMove;
    for (int depth = 1; depth <= maxDepth; ++depth) {
        bestMove = getBestMove(depth, color);
    }
    return bestMove;
}




void AiLogic::aiMove(int color) {
    std::pair<Piece*, Coordinate> bestMove = getBestMoveIterative(5, color);
    Piece* movedPiece = bestMove.first;
    Coordinate target = bestMove.second;

    Piece* capturedPiece = _board->getPieceAt(target.x, target.y);


    Coordinate rookOriginalPosition, rookTargetPosition;
    Piece* rook = nullptr;


    handleCastle(movedPiece, target, rook, rookOriginalPosition, rookTargetPosition);

    if (capturedPiece) _board->removePiece(target.x, target.y, nullptr);
    // Wykonaj ruch
    movedPiece->move(target.x, target.y);
    if(rook) rook->move(rookTargetPosition.x, rookTargetPosition.y);
}





