#include "King.h"
#include "Board.h"

King::King() {
    
}

King::King(int color, int boardX, int boardY, Board* board)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::King, boardX, boardY) 
{
    this->board = board;

    std::string filePath;
    if (color == 0) {
        filePath =  "../assets/pieces/chessCom1/wk.png";
    } else {
        filePath = "../assets/pieces/chessCom1/bk.png";
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


    if (isFirstMove()) {
        if (getColor() == WHITE) {
            // White kingside castling
            if (board->isEmpty(5, boardPosition.y) && board->isEmpty(6, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(5, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(6, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(7, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(6, boardPosition.y));  // Castling move
                }
            }

            // White queenside castling
            if (board->isEmpty(1, boardPosition.y) && board->isEmpty(2, boardPosition.y) &&
                board->isEmpty(3, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(2, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(3, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(0, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(2, boardPosition.y));  // Castling move
                }
            }
        } else if (getColor() == BLACK) {
            // Black kingside castling
            if (board->isEmpty(5, boardPosition.y) && board->isEmpty(6, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(5, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(6, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(7, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(6, boardPosition.y));  // Castling move
                }
            }

            // Black queenside castling
            if (board->isEmpty(1, boardPosition.y) && board->isEmpty(2, boardPosition.y) &&
                board->isEmpty(3, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(2, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(3, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(0, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(2, boardPosition.y));  // Castling move
                }
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

    //roszada

    if (isFirstMove() && deltaY == 0) {
        if (getColor() == WHITE && boardX == 6 && boardY == this->boardPosition.y) {
            // Check if squares between the king and the rook are empty and the king is not in check
            if (board->isEmpty(5, boardY) && board->isEmpty(6, boardY) &&
                !board->isKingInCheckAfterMove(this, Coordinate(5, boardY)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(6, boardY))) {
                // Check if the rook has not moved and is in the correct position
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(7, boardY));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    board->getPieceAt(7, boardY)->move(5, boardY);  // Move rook
                    return true;
                }
            }
        }

        // Queenside castling (long castling)
        if (getColor() == WHITE && boardX == 2 && boardY == this->boardPosition.y) {
            if (board->isEmpty(1, boardY) && board->isEmpty(2, boardY) && board->isEmpty(3, boardY) &&
                !board->isKingInCheckAfterMove(this, Coordinate(2, boardY)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(3, boardY))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(0, boardY));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    board->getPieceAt(0, boardY)->move(3, boardY);  // Move rook
                    return true;
                }
            }
        }

        // Kingside castling for black
        if (getColor() == BLACK && boardX == 6 && boardY == this->boardPosition.y) {
            if (board->isEmpty(5, boardY) && board->isEmpty(6, boardY) &&
                !board->isKingInCheckAfterMove(this, Coordinate(5, boardY)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(6, boardY))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(7, boardY));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    board->getPieceAt(7, boardY)->move(5, boardY);  // Move rook
                    return true;
                }
            }
        }

        // Queenside castling for black
        if (getColor() == BLACK && boardX == 2 && boardY == this->boardPosition.y) {
            if (board->isEmpty(1, boardY) && board->isEmpty(2, boardY) && board->isEmpty(3, boardY) &&
                !board->isKingInCheckAfterMove(this, Coordinate(2, boardY)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(3, boardY))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(0, boardY));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    board->getPieceAt(0, boardY)->move(3, boardY);  // Move rook
                    return true;
                }
            }
        }
    }


    return false;  // W innych przypadkach ruch jest niedozwolony
}

void King::move(int boardX, int boardY) {
    Piece::move(boardX, boardY);
    firstMove = false;
}
void King::rotatePiece() {
    Piece::rotatePiece();
}


King::~King() {}

void King::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
