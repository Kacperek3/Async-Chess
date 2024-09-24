#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"
#include <iostream>


class King : public Piece {
public:
    King(int color, int boardX, int boardY, Board* board);
    King();
    ~King();

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int boardX, int boardY) override;
    std::vector<Coordinate> getPossibleMoves() override;
    std::vector<Coordinate> getPossibleCaptures() override;
    
private:
    
};
