#include "Rook.h"
#include "Board.h"

Rook::Rook() {
    
}

Rook::Rook(int color, int boardX, int boardY, Board* board, sf::Texture& rookTexture)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Rook, boardX, boardY, 5) 
{
    this->board = board;
    sprite.setTexture(rookTexture);
    std::cout << m_position.x << " " << m_position.y << std::endl;
    sprite.setPosition(m_position);
}

std::vector<Coordinate> Rook::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    int directions[4][2] = {
        {1, 0},   // W prawo
        {-1, 0},  // W lewo
        {0, 1},   // W dół
        {0, -1}   // W górę
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x;
        int y = boardPosition.y;

        while (true) {
            x += dx;
            y += dy;

            if (board->isWithinBounds(x, y)) {
                if (board->isEmpty(x, y)) {
                    possibleMoves.push_back(Coordinate(x, y));
                } else {
                    if (board->isEnemyPieceAt(x, y, m_color)) {
                        possibleMoves.push_back(Coordinate(x, y));
                    }
                    break;  
                }
            } else {
                break;
            }
        }
    }

    return possibleMoves;
}

std::vector<Coordinate> Rook::getPossibleCaptures() {
    std::vector<Coordinate> possibleCaptures;

    int directions[4][2] = {
        {1, 0},   
        {-1, 0},  
        {0, 1},   
        {0, -1}   
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x;
        int y = boardPosition.y;

        while (true) {
            x += dx;
            y += dy;

            if (board->isWithinBounds(x, y)) {
                if (board->isEnemyPieceAt(x, y, m_color)) {
                    possibleCaptures.push_back(Coordinate(x, y));
                    break;  
                } else if (!board->isEmpty(x, y)) {
                    break;  
                }
            } else {
                break;  
            }
        }
    }

    return possibleCaptures;
}

bool Rook::isValidMove(int boardX, int boardY) {
    if (!board->isWithinBounds(this->boardPosition.x, this->boardPosition.y) || !board->isWithinBounds(boardX, boardY)) {
        return false;  
    }
    if (boardX != boardPosition.x && boardY != boardPosition.y) {
        return false;
    }

    if (boardX == boardPosition.x) {  
        int direction = (boardY > boardPosition.y) ? 1 : -1;
        for (int i = 1; i < abs(boardY - boardPosition.y); ++i) {
            if (!board->isEmpty(boardX, boardPosition.y + i * direction)) {
                return false;
            }
        }
        
    } else if (boardY == boardPosition.y) {  
        int direction = (boardX > boardPosition.x) ? 1 : -1;
        for (int i = 1; i < abs(boardX - boardPosition.x); ++i) {
            if (!board->isEmpty(boardPosition.x + i * direction, boardY)) {
                return false;  
            }
        }
    }

    if (board->isEmpty(boardX, boardY)) {
        return true;
    } else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
        return true;
    }

    return false;
}

void Rook::move(int boardX, int boardY, bool updateVisuals) {
    Piece::move(boardX, boardY, updateVisuals);
    firstMove = false;
}
void Rook::rotatePiece() {
    Piece::rotatePiece();
}

Rook::~Rook() {}

void Rook::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
