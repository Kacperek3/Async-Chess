#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"



class Queen : public Piece {
public:
    Queen(int color, int boardX, int boardY, Board* board);
    Queen();
    ~Queen();

    void draw(sf::RenderWindow& window) override;
    void move(int boardX, int boardY) override;
    bool isValidMove(int boardX, int boardY);
    bool isValidRookMove(int boardX, int boardY);
    bool isValidBishopMove(int boardX, int boardY);
    
private:
    
};
