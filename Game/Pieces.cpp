// pieces.cpp
#include "Pieces.h"

Piece::Piece(int color, sf::Vector2f position, PieceType type, int boardX, int boardY)
    : m_color(color), m_position(position), m_type(type) 
{
    boardPosition = Coordinate(boardX, boardY);
}
sf::Sprite& Piece::getSprite() {
    return sprite;
}

sf::Vector2f Piece::getPosition() {
    return m_position;
}

Coordinate Piece::getBoardPosition(){
    return boardPosition;
}

void Piece::move(sf::Vector2f newPosition) {
    m_position = newPosition;
    sprite.setPosition(newPosition);
}

void Piece::move(int boardX, int boardY) {
    m_position = Position(boardX, boardY).getPixelPosition();
    boardPosition = Coordinate(boardX, boardY);
    sprite.setPosition(m_position);
}

Piece::Piece() {}

Piece::~Piece() {}
