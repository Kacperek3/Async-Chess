#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"
#include <iostream>


class King : public Piece {
public:
    King(int color, int boardX, int boardY, Board* board);
    void move(int boardX, int boardY) override;
    King();
    ~King();

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int boardX, int boardY) override;
    std::vector<Coordinate> getPossibleMoves() override;
    std::vector<Coordinate> getPossibleCaptures() override;

    bool isFirstMove() const { return firstMove; }
    void rotatePiece() override;
    
private:
    bool firstMove = true;
    
};
