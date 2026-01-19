// pawn.cpp
#include "pawn.h"
#include "Board.h"

Pawn::Pawn() {
    
}

Pawn::Pawn(int color, int boardX, int boardY, Board* board, int direction, sf::Texture& pawnTexture)
    : Piece(color, Position(boardX, boardY).getPixelPosition(), Piece::PieceType::Pawn, boardX, boardY, 1) 
{
    this->direction = direction;   
    this->board = board;
    sprite.setTexture(pawnTexture);
    m_position = Position(boardX, boardY).getPixelPosition();
    sprite.setPosition(m_position);
}



std::vector<Coordinate> Pawn::getPossibleMoves() {
    std::vector<Coordinate> possibleMoves;

    if (board->isEmpty(boardPosition.x, boardPosition.y + direction)) {
        possibleMoves.push_back(Coordinate(boardPosition.x, boardPosition.y + direction));
    }

    if (firstMove && board->isEmpty(boardPosition.x, boardPosition.y + direction) &&
        board->isEmpty(boardPosition.x, boardPosition.y + 2 * direction)) {
        possibleMoves.push_back(Coordinate(boardPosition.x, boardPosition.y + 2 * direction));
    }

    if (board->isEnemyPieceAt(boardPosition.x + 1, boardPosition.y + direction, m_color)) {
        possibleMoves.push_back(Coordinate(boardPosition.x + 1, boardPosition.y + direction));
    }
    if (board->isEnemyPieceAt(boardPosition.x - 1, boardPosition.y + direction, m_color)) {
        possibleMoves.push_back(Coordinate(boardPosition.x - 1, boardPosition.y + direction));
    }


    return possibleMoves;
}


std::vector<Coordinate> Pawn::getPossibleCaptures(){
    std::vector<Coordinate> possibleCaptures;

    if (board->isEnemyPieceAt(boardPosition.x + 1, boardPosition.y + direction, m_color)) {
        possibleCaptures.push_back(Coordinate(boardPosition.x + 1, boardPosition.y + direction));
    }
    if (board->isEnemyPieceAt(boardPosition.x - 1, boardPosition.y + direction, m_color)) {
        possibleCaptures.push_back(Coordinate(boardPosition.x - 1, boardPosition.y + direction));
    }

    return possibleCaptures;
}

bool Pawn::isValidMove(int boardX, int boardY) {
    
    if (boardX == boardPosition.x && boardY == boardPosition.y + direction) {
        if (board->isEmpty(boardX, boardY)) {
            return true;
        }
    }

    if (boardX == boardPosition.x && boardY == boardPosition.y + 2 * direction && firstMove) {
        if (board->isEmpty(boardX, boardY) && board->isEmpty(boardX, boardPosition.y + direction)) {
            return true;
        }
    }
    
    if (abs(boardX - boardPosition.x) == 1 && boardY == boardPosition.y + direction) {
        if (board->isEnemyPieceAt(boardX, boardY, m_color)) {
            return true;
        }
    }

    return false;  
}

void Pawn::move(int boardX, int boardY, bool updateVisuals) {
    Piece::move(boardX, boardY, updateVisuals);
    firstMove = false;
    if (boardY == 0 || boardY == 7) {
            board->_dataAboutPawnPromotion._isPawnPromotion = true;
        board->_dataAboutPawnPromotion._pawnColor = m_color;
        board->_dataAboutPawnPromotion._pawnX = boardX;
        board->_dataAboutPawnPromotion._pawnY = boardY;

        move(-1, -1);  
        
    }
}

void Pawn::rotatePiece() {
    Piece::rotatePiece();
    direction *= -1;
}

Pawn::~Pawn() {}

void Pawn::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}