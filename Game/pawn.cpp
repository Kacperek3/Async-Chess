// pawn.cpp
#include "pawn.h"
#include "Board.h"

Pawn::Pawn() {
    
}

Pawn::Pawn(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Pawn, boardX, boardY) 
{
    this->board = board;
    
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wPawn.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bPawn.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}


bool Pawn::isValidMove(int boardX, int boardY) {
    int direction = (m_color == WHITE) ? -1 : 1;  // Kierunek ruchu: do góry dla białych, na dół dla czarnych
    
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
            board->removePiece(boardX, boardY);
            return true;
        }
    }

    return false;  // W innych przypadkach ruch jest niedozwolony
}

void Pawn::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
    firstMove = false;
}

Pawn::~Pawn() {}

void Pawn::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}