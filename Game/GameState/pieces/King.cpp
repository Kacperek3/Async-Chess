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

    if (color == WHITE) {
        filePath = (currentPath / "assets/pieces/chessCom1/wk.png").string();
    } else {
        filePath = (currentPath / "assets/pieces/chessCom1/bk.png").string();
    }

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(m_position);
}

std::vector<Coordinate> King::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    int directions[8][2] = {
        {1, 0},   // W prawo
        {1, 1},   // W prawo w dół
        {0, 1},   // W dół
        {-1, 1},  // W lewo w dół
        {-1, 0},  // W lewo
        {-1, -1}, // W lewo w górę
        {0, -1},  // W górę
        {1, -1}   // W prawo w górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x + dx;
        int y = boardPosition.y + dy;

        if (board->isWithinBounds(x, y)) {
            if (board->isEmpty(x, y) || board->isEnemyPieceAt(x, y, m_color)) {
                possibleMoves.push_back(Coordinate(x, y));
            }
        }
    }

    return possibleMoves;
}

std::vector<Coordinate> King::getPossibleCaptures() {
    std::vector<Coordinate> possibleCaptures;

    int directions[8][2] = {
        {1, 0},   // W prawo
        {1, 1},   // W prawo w dół
        {0, 1},   // W dół
        {-1, 1},  // W lewo w dół
        {-1, 0},  // W lewo
        {-1, -1}, // W lewo w górę
        {0, -1},  // W górę
        {1, -1}   // W prawo w górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x + dx;
        int y = boardPosition.y + dy;

        // Sprawdzanie, czy pozycja jest w granicach planszy
        if (board->isWithinBounds(x, y)) {
            // Jeśli na pozycji znajduje się bierka przeciwnika, dodajemy do możliwych bić
            if (board->isEnemyPieceAt(x, y, m_color)) {
                possibleCaptures.push_back(Coordinate(x, y));
            }
        }
    }

    return possibleCaptures;
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
            return true;
        }
    }

    return false;  // W innych przypadkach ruch jest niedozwolony
}

King::~King() {}

void King::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
