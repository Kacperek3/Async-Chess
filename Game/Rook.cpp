#include "Rook.h"
#include "Board.h"

Rook::Rook() {
    
}

Rook::Rook(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Rook, boardX, boardY) 
{
    this->board = board;
    
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wRook.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bRook.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

bool Rook::isValidMove(int boardX, int boardY) {
    // Sprawdzenie, czy ruch jest w linii prostej (po osi X lub Y)
    if (boardX != boardPosition.x && boardY != boardPosition.y) {
        return false; // Ruch jest nieprawidłowy, bo wieża nie może poruszać się po skosie
    }

    // Sprawdzenie, czy droga jest wolna
    if (boardX == boardPosition.x) {  // Ruch w pionie
        int direction = (boardY > boardPosition.y) ? 1 : -1;
        for (int i = 1; i < abs(boardY - boardPosition.y); ++i) {
            if (!board->isEmpty(boardX, boardPosition.y + i * direction)) {
                return false;  // Jeśli droga jest zablokowana, ruch jest nieprawidłowy
            }
        }
        
    } else if (boardY == boardPosition.y) {  // Ruch w poziomie
        int direction = (boardX > boardPosition.x) ? 1 : -1;
        for (int i = 1; i < abs(boardX - boardPosition.x); ++i) {
            if (!board->isEmpty(boardPosition.x + i * direction, boardY)) {
                return false;  // Jeśli droga jest zablokowana, ruch jest nieprawidłowy
            }
        }
    }

    // Sprawdzenie, czy docelowe pole jest puste, lub znajduje się tam figura przeciwnika
    if (board->isEmpty(boardX, boardY)) {
        return true;
    } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
        board->removePiece(boardX, boardY);  // Zbijamy przeciwnika
        return true;
    }

    return false;
}

void Rook::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
}

Rook::~Rook() {}

void Rook::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
