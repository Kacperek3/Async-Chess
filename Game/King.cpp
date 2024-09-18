#include "King.h"
#include "Board.h"

King::King() {
    
}

King::King(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::King, boardX, boardY) 
{
    this->board = board;

    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wKing.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bKing.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

bool King::isValidMove(int boardX, int boardY) {
    int deltaX = abs(boardX - boardPosition.x);
    int deltaY = abs(boardY - boardPosition.y);

    // Król może przemieszczać się o jedno pole w dowolnym kierunku
    if (deltaX <= 1 && deltaY <= 1) {
        // Sprawdzenie, czy pole docelowe jest puste lub zawiera figurę przeciwnika
        if (board->isEmpty(boardX, boardY)) {
            return true;
        } 
        else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            board->removePiece(boardX, boardY);  // Zbijamy przeciwnika
            return true;
        }
    }

    return false;  // W innych przypadkach ruch jest niedozwolony
}

void King::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
}

King::~King() {}

void King::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
