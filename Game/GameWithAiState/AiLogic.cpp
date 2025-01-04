#include "AiLogic.h"
#include "Pieces.h"


AiLogic::AiLogic(GameDataRef data): _data(data){
    _board = new Board(data);
    _board->Init();
}

AiLogic::~AiLogic(){
    delete _board;
}


int AiLogic::getPositionValue(Piece* piece) {
    if (!piece) return 0;

    Coordinate pos = piece->getBoardPosition();
    int x = pos.x;
    int y = pos.y;

    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;

    switch (piece->getType()) {
        case piece->PieceType::Pawn:return pawnPositionValues[y][x];
        case piece->PieceType::Knight: return knightPositionValues[y][x];
        case piece->PieceType::Bishop: return bishopPositionValues[y][x];
        case piece->PieceType::Rook: return rookPositionValues[y][x];
        case piece->PieceType::Queen: return queenPositionValues[y][x];
        case piece->PieceType::King: return kingPositionValues[y][x];
        default:
            return 0;
    }
}



int AiLogic::evaluatePosition(int color) {
    int score = 0;
    
    for (Piece* piece : _board->enemyPieces(color)) {
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            continue;
        }
        std::cout << piece->getBoardPosition().x << " " << piece->getBoardPosition().y << " "<<piece->getValue()<< std::endl;
        score += piece->getValue();
        score += getPositionValue(piece);
    }
    for (Piece* piece : _board->playerPieces(color)) {
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            continue;
        }
        std::cout << piece->getBoardPosition().x << " " << piece->getBoardPosition().y << " "<<piece->getValue()<< std::endl;
        score -= piece->getValue();
        score -= getPositionValue(piece); 
    }

    if(_board->isCheckmate(1 - color)){
        score -= 10000;
    }
    if(_board->isCheckmate(color)){
        score += 10000;
    }

    return score;
}


int AiLogic::minimax(int depth, int color, bool maximizingPlayer, int alpha, int beta) {
    // Sprawdź warunki końcowe
    if (depth == 0 || _board->isCheckmate(color) || _board->isStalemate(color)) {
        return evaluatePosition(color);
    }

    if (maximizingPlayer) {
        int maxEval = -10000;
        auto allMoves = _board->getAllMoves(color);

        for (const auto& move : allMoves) {
            Piece* movedPiece = move.first;
            Coordinate target = move.second;
            Piece* capturedPiece = _board->getPieceAt(target.x, target.y);

            if (!movedPiece) continue;

            Coordinate originalPosition = movedPiece->getBoardPosition();

            // Wykonaj ruch
            movedPiece->simulateMove(target.x, target.y);
            if (capturedPiece) capturedPiece->simulateMove(-1, -1);

            // Rekurencja
            int eval = minimax(depth - 1, 1 - color, false, alpha, beta);

            // Cofnij ruch
            movedPiece->simulateMove(originalPosition.x, originalPosition.y);
            if (capturedPiece) capturedPiece->simulateMove(target.x, target.y);

            // Aktualizuj wynik
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            // Przytnij
            if (beta <= alpha) break;
        }
        return maxEval;

    } else {
        int minEval = 10000;
        auto allMoves = _board->getAllMoves(color);

        for (const auto& move : allMoves) {
            Piece* movedPiece = move.first;
            Coordinate target = move.second;
            Piece* capturedPiece = _board->getPieceAt(target.x, target.y);

            if (!movedPiece) continue;

            Coordinate originalPosition = movedPiece->getBoardPosition();

            // Wykonaj ruch
            movedPiece->simulateMove(target.x, target.y);
            if (capturedPiece) capturedPiece->simulateMove(-1, -1);

            // Rekurencja
            int eval = minimax(depth - 1, 1 - color, true, alpha, beta);

            // Cofnij ruch
            movedPiece->simulateMove(originalPosition.x, originalPosition.y);
            if (capturedPiece) capturedPiece->simulateMove(target.x, target.y);

            // Aktualizuj wynik
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            // Przytnij
            if (beta <= alpha) break;
        }
        return minEval;
    }
}



std::pair<Piece*, Coordinate> AiLogic::getBestMove(int depth, int color) {
    int bestScore = -10000;
    std::pair<Piece*, Coordinate> bestMove;
    int alpha = -10000, beta = 10000;

    auto allMoves = _board->getAllMoves(color);

    for (const auto& move : allMoves) {
        Piece* movedPiece = move.first;
        Coordinate target = move.second;
        Piece* capturedPiece = _board->getPieceAt(target.x, target.y);

        Coordinate originalPosition = movedPiece->getBoardPosition();

        // Wykonaj ruch
        movedPiece->simulateMove(target.x, target.y);
        if (capturedPiece) capturedPiece->simulateMove(-1, -1);

        // Oblicz ocenę
        int score = minimax(depth - 1, 1 - color, false, alpha, beta);

        // Cofnij ruch
        movedPiece->simulateMove(originalPosition.x, originalPosition.y);
        if (capturedPiece) capturedPiece->simulateMove(target.x, target.y);

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



void AiLogic::aiMove(int color) {
    std::pair<Piece*, Coordinate> bestMove = getBestMove(5, color);
    Piece* movedPiece = bestMove.first;
    Coordinate target = bestMove.second;


    Piece* capturedPiece = _board->getPieceAt(target.x, target.y);


    if (capturedPiece) _board->removePiece(target.x, target.y, nullptr);
    // Wykonaj ruch
    movedPiece->move(target.x, target.y);
    
}



