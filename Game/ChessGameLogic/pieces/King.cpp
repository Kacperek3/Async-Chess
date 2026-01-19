#include "King.h"
#include "Board.h"

King::King() {
    
}

King::King(int color, int boardX, int boardY, Board* board, sf::Texture& kingTexture)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::King, boardX, boardY, 1000) 
{
    this->board = board;
    sprite.setTexture(kingTexture);
    sprite.setPosition(m_position);
}

std::vector<Coordinate> King::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    int directions[8][2] = {
        {1, 0},   
        {1, 1},   
        {0, 1},   
        {-1, 1},  
        {-1, 0},  
        {-1, -1}, 
        {0, -1},  
        {1, -1}   
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
                if (board->isEmpty(5, boardPosition.y) && board->isEmpty(6, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(5, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(6, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(7, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(6, boardPosition.y)); 
                    isCastle = true;
                }
            }

            if (board->isEmpty(1, boardPosition.y) && board->isEmpty(2, boardPosition.y) &&
                board->isEmpty(3, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(2, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(3, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(0, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(2, boardPosition.y));  
                    isCastle = true;
                }
            }
        } else if (getColor() == BLACK) {
            if (board->isEmpty(5, boardPosition.y) && board->isEmpty(6, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(5, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(6, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(7, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(6, boardPosition.y));  
                    isCastle = true;
                }
            }

            if (board->isEmpty(1, boardPosition.y) && board->isEmpty(2, boardPosition.y) &&
                board->isEmpty(3, boardPosition.y) &&
                !board->isKingInCheckAfterMove(this, Coordinate(2, boardPosition.y)) &&
                !board->isKingInCheckAfterMove(this, Coordinate(3, boardPosition.y))) {
                Rook* rook = dynamic_cast<Rook*>(board->getPieceAt(0, boardPosition.y));
                if (rook && rook->getType() == Piece::PieceType::Rook && rook->isFirstMove()) {
                    possibleMoves.push_back(Coordinate(2, boardPosition.y));
                    isCastle = true;
                }
            }
        }
    }
    return possibleMoves;
}

std::vector<Coordinate> King::getPossibleCaptures() {
    std::vector<Coordinate> possibleCaptures;

    int directions[8][2] = {
        {1, 0},   
        {1, 1},   
        {0, 1},   
        {-1, 1},  
        {-1, 0},  
        {-1, -1}, 
        {0, -1},  
        {1, -1}   
    };

    for (auto& direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int x = boardPosition.x + dx;
        int y = boardPosition.y + dy;

        if (board->isWithinBounds(x, y)) {
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

    if (deltaX <= 1 && deltaY <= 1) {
        if (board->isEmpty(boardX, boardY)) {
            return true;
        } 
        else if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            return true;
        }
    }


    if (isFirstMove() && deltaY == 0) {
        if (getColor() == WHITE && boardX == 6 && boardY == this->boardPosition.y) {
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


    return false;  
}



void King::getIsCastle(bool &isCastle) {
    isCastle = this->isCastle;
    this->isCastle = false;
}


void King::move(int boardX, int boardY, bool updateVisuals) {
    Piece::move(boardX, boardY, updateVisuals);
    firstMove = false;
}
void King::rotatePiece() {
    Piece::rotatePiece();
}


King::~King() {
    std::cout << "Usunieto King" << std::endl;
}

void King::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
