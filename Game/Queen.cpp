#include "Queen.h"
#include "Board.h"

Queen::Queen() {
    
}

Queen::Queen(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Queen, boardX, boardY) 
{
    this->board = board;
    
    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string filePath;

    if (color == 0) {
        filePath = (currentPath / "assets/pieces/wQueen.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/bQueen.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

bool Queen::isValidMove(int boardX, int boardY) {
    int deltaX = abs(boardX - boardPosition.x);
    int deltaY = abs(boardY - boardPosition.y);

    // Królowa porusza się jak wieża (po prostej) lub jak goniec (po przekątnej)
    if (boardX == boardPosition.x || boardY == boardPosition.y) {
        // Ruch jak wieża
        return isValidRookMove(boardX, boardY);
    } else if (deltaX == deltaY) {
        // Ruch jak goniec
        return isValidBishopMove(boardX, boardY);
    }
    
    return false;  // W innych przypadkach ruch jest niedozwolony
}

bool Queen::isValidRookMove(int boardX, int boardY) {
    // Sprawdzenie, czy droga jest wolna w pionie lub poziomie (jak w wieży)
    if (boardX == boardPosition.x) {  // Ruch w pionie
        int direction = (boardY > boardPosition.y) ? 1 : -1;
        for (int i = 1; i < abs(boardY - boardPosition.y); ++i) {
            if (!board->isEmpty(boardX, boardPosition.y + i * direction)) {
                return false;
            }
        }
    } else if (boardY == boardPosition.y) {  // Ruch w poziomie
        int direction = (boardX > boardPosition.x) ? 1 : -1;
        for (int i = 1; i < abs(boardX - boardPosition.x); ++i) {
            if (!board->isEmpty(boardPosition.x + i * direction, boardY)) {
                return false;
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

bool Queen::isValidBishopMove(int boardX, int boardY) {
    // Sprawdzenie, czy droga jest wolna po przekątnej (jak w gońcu)
    int directionX = (boardX > boardPosition.x) ? 1 : -1;
    int directionY = (boardY > boardPosition.y) ? 1 : -1;

    for (int i = 1; i < abs(boardX - boardPosition.x); ++i) {
        if (!board->isEmpty(boardPosition.x + i * directionX, boardPosition.y + i * directionY)) {
            return false;
        }
    }

    if (board->isEmpty(boardX, boardY)) {
            return true;
    } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
        board->removePiece(boardX, boardY);  // Zbijamy przeciwnika
        return true;
    }

    return false;
}

void Queen::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
}

Queen::~Queen() {}

void Queen::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
