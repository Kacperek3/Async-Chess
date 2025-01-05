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
    // Sprawdź warunki końcowe
    if (depth == 0 || _board->isCheckmate(color) || _board->isStalemate(color)) {

        if(color == WHITE) return evaluatePositionWhite(color, RecentlyMovedPiece, depth);
        else return evaluatePositionBlack(color, RecentlyMovedPiece, depth);
    }

    if (maximizingPlayer) {
        float maxEval = -10000;
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
            float eval = minimax(depth - 1, 1 - color, false, alpha, beta, movedPiece);

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
        float minEval = 10000;
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
            float eval = minimax(depth - 1, 1 - color, true, alpha, beta, movedPiece);

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
    float bestScore = -10000;
    std::pair<Piece*, Coordinate> bestMove;
    float alpha = -10000, beta = 10000;

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
        float score = minimax(depth - 1, 1 - color, false, alpha, beta, movedPiece);

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



