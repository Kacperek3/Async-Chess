#include "Knight.h"
#include "Board.h"

Knight::Knight() {
    
}

Knight::Knight(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Knight, boardX, boardY) 
{
    this->board = board;
    
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wKnight.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bKnight.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

bool Knight::isValidMove(int boardX, int boardY) {
    // Obliczenie różnicy między aktualną a docelową pozycją
    int dx = abs(boardX - boardPosition.x);
    int dy = abs(boardY - boardPosition.y);

    // Sprawdzenie, czy ruch jest w kształcie "L" (2 pola w jednym kierunku, 1 pole w drugim)
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        // Sprawdzenie, czy na docelowej pozycji znajduje się przeciwnik lub jest puste
        if (board->isEmpty(boardX, boardY)) {
            return true;  // Ruch do pustego pola jest dozwolony
        } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            board->removePiece(boardX, boardY);  // Zbijamy przeciwnika
            return true;
        }
    }
    
    return false;  // W innych przypadkach ruch jest niedozwolony
}


void Knight::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
}

Knight::~Knight() {}

void Knight::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
