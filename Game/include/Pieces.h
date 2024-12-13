// pieces.h
#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Positions.h"
#include <iostream>

#define WHITE 0
#define BLACK 1

#define UP 1
#define DOWN -1

class Board;

struct Coordinate {
    int x, y;
    Coordinate() : x(0), y(0) {}  
    Coordinate(int x, int y) : x(x), y(y) {}
};


class Piece {
public:
    enum class PieceType {
        Pawn, Rook, Knight, Bishop, Queen, King
    };

    Piece(int color, sf::Vector2f position, PieceType type, int boardX, int boardY);
    Piece();
    virtual ~Piece();

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void simulateMove(int boardX, int boardY);
    virtual void move(int boardX, int boardY);
    virtual void move(sf::Vector2f newPosition);
    virtual bool isValidMove(int boardX, int boardY) = 0;
    virtual std::vector<Coordinate> getPossibleMoves() = 0;
    virtual std::vector<Coordinate> getPossibleCaptures() = 0;
    virtual void rotatePiece();


    sf::Sprite& getSprite();
    sf::Vector2f getPosition();
    Coordinate getBoardPosition();
    int getColor() { return m_color; }
    PieceType getType() { return m_type; }
    

protected:
    Board* board; // wskaźnik na planszę
    sf::Texture texture; // tekstura figury
    sf::Sprite sprite; // sprite figury
    sf::Vector2f m_position; // pozycja figury na planszy
    int m_color; // kolor figury
    PieceType m_type; // typ figury
    Coordinate boardPosition; // pozycja figury na planszy w postaci współrzędnych planszy

};
