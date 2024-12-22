// pawn.cpp
#include "pawn.h"
#include "Board.h"

Pawn::Pawn() {
    
}

Pawn::Pawn(int color, int boardX, int boardY, Board* board, int direction, sf::Texture& pawnTexture)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Pawn, boardX, boardY) 
{
    this->direction = direction;   
    this->board = board;
    sprite.setTexture(pawnTexture);
    sprite.setPosition(m_position);
}



std::vector<Coordinate> Pawn::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    // Ruch do przodu o jedno pole
    if (board->isEmpty(boardPosition.x, boardPosition.y + direction)) {
        possibleMoves.push_back(Coordinate(boardPosition.x, boardPosition.y + direction));
    }

    // Ruch do przodu o dwa pola, tylko na pierwszym ruchu
    if (firstMove && board->isEmpty(boardPosition.x, boardPosition.y + direction) &&
        board->isEmpty(boardPosition.x, boardPosition.y + 2 * direction)) {
        possibleMoves.push_back(Coordinate(boardPosition.x, boardPosition.y + 2 * direction));
    }

    // Ruch po skosie (atak), jeśli na tym polu znajduje się figura przeciwnika
    if (board->isEnemyPieceAt(boardPosition.x + 1, boardPosition.y + direction, m_color)) {
        possibleMoves.push_back(Coordinate(boardPosition.x + 1, boardPosition.y + direction));
    }
    if (board->isEnemyPieceAt(boardPosition.x - 1, boardPosition.y + direction, m_color)) {
        possibleMoves.push_back(Coordinate(boardPosition.x - 1, boardPosition.y + direction));
    }

    // Możliwość dodania funkcji dla bicia w przelocie (en passant) oraz promocji pionka

    return possibleMoves;
}


std::vector<Coordinate> Pawn::getPossibleCaptures(){
    std::vector<Coordinate> possibleCaptures;

    // Ruch po skosie (atak), jeśli na tym polu znajduje się figura przeciwnika
    if (board->isEnemyPieceAt(boardPosition.x + 1, boardPosition.y + direction, m_color)) {
        possibleCaptures.push_back(Coordinate(boardPosition.x + 1, boardPosition.y + direction));
    }
    if (board->isEnemyPieceAt(boardPosition.x - 1, boardPosition.y + direction, m_color)) {
        possibleCaptures.push_back(Coordinate(boardPosition.x - 1, boardPosition.y + direction));
    }

    return possibleCaptures;
}

bool Pawn::isValidMove(int boardX, int boardY) {
    
    // Ruch do przodu o jedno pole
    if (boardX == boardPosition.x && boardY == boardPosition.y + direction) {
        if (board->isEmpty(boardX, boardY)) {
            return true;
        }
    }

    // Ruch do przodu o dwa pola, tylko na pierwszym ruchu
    if (boardX == boardPosition.x && boardY == boardPosition.y + 2 * direction && firstMove) {
        if (board->isEmpty(boardX, boardY) && board->isEmpty(boardX, boardPosition.y + direction)) {
            return true;
        }
    }
    
    // Ruch po skosie (atak), jeśli na tym polu znajduje się figura przeciwnika
    if (abs(boardX - boardPosition.x) == 1 && boardY == boardPosition.y + direction) {
        if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            return true;
        }
    }

    return false;  // W innych przypadkach ruch jest niedozwolony
}

void Pawn::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
    firstMove = false;
    // Sprawdź, czy pionek doszedł do końca planszy
    if (boardY == 0 || boardY == 7) {
        board->window->clear();  // Wyczyść okno
        board->drawBoard(*board->window, false);  // Narysuj planszę
        board->drawPieces(*board->window,this);  // Narysuj pionki
        board->window->display();  // Update the window


        board->promotePawn(this);
        // Zastąp pionka królową
        move(-1, -1);  // Usuń pionka z planszy
        
    }
}

void Pawn::rotatePiece() {
    Piece::rotatePiece();
    direction *= -1;
}

Pawn::~Pawn() {}

void Pawn::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}