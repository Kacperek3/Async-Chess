#include "Bishop.h"
#include "Board.h"

Bishop::Bishop() {
    
}

Bishop::Bishop(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Bishop, boardX, boardY) 
{
    this->board = board;
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wBishop.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bBishop.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

bool Bishop::isValidMove(int boardX, int boardY) {
    int dx = abs(boardX - boardPosition.x);
    int dy = abs(boardY - boardPosition.y);

    // Sprawdzenie, czy ruch jest po przekątnej (|deltaX| == |deltaY|)
    if (dx == dy) {
        // Sprawdzenie, czy droga do docelowej pozycji jest wolna
        int xDirection = (boardX - boardPosition.x > 0) ? 1 : -1;
        int yDirection = (boardY - boardPosition.y > 0) ? 1 : -1;
        
        for (int i = 1; i < dx; ++i) {  // Sprawdź każde pole na drodze
            if (!board->isEmpty(boardPosition.x + i * xDirection, boardPosition.y + i * yDirection)) {
                return false;  // Jeśli na drodze jest figura, ruch jest niedozwolony
            }
        }

        // Jeżeli docelowe pole jest puste, lub znajduje się na nim figura przeciwnika
        if (board->isEmpty(boardX, boardY)) {
            return true;
        } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            board->removePiece(boardX, boardY);  // Zbijamy przeciwnika
            return true;
        }
    }

    return false;  // W innych przypadkach ruch jest niedozwolony
}

void Bishop::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
}

Bishop::~Bishop() {}

void Bishop::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
