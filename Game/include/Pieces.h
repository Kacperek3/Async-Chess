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

    Piece(int color, sf::Vector2f position, PieceType type, int boardX, int boardY, int value);
    Piece();
    virtual ~Piece();

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void simulateMove(int boardX, int boardY, bool updateVisuals = true);
    virtual void move(int boardX, int boardY, bool updateVisuals = true);
    virtual void move(sf::Vector2f newPosition);
    virtual bool isValidMove(int boardX, int boardY) = 0;
    virtual std::vector<Coordinate> getPossibleMoves() = 0;
    virtual std::vector<Coordinate> getPossibleCaptures() = 0;
    virtual void rotatePiece();

    int getValue() { return _value; }
    sf::Sprite& getSprite();
    sf::Vector2f getPosition();
    Coordinate getBoardPosition();
    int getColor() { return m_color; }
    PieceType getType() { return m_type; }

    std::string getTypeName() {
        if(m_color == BLACK){
            switch (m_type) {
            case PieceType::Pawn: return "bp";
            case PieceType::Rook: return "br";
            case PieceType::Knight: return "bn";
            case PieceType::Bishop: return "bb";
            case PieceType::Queen: return "bq";
            case PieceType::King: return "bk";
            }
        }

        if(m_color == WHITE){
            switch (m_type) {
            case PieceType::Pawn: return "wp";
            case PieceType::Rook: return "wr";
            case PieceType::Knight: return "wn";
            case PieceType::Bishop: return "wb";
            case PieceType::Queen: return "wq";
            case PieceType::King: return "wk";
            }
        }
        return "error";
    }

protected:
    int _value;
    Board* board; 
    sf::Sprite sprite; 
    sf::Vector2f m_position; 
    int m_color;
    PieceType m_type; 
    Coordinate boardPosition; 

};
