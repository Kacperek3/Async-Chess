#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"


class Rook : public Piece {
public:
    Rook(int color, int boardX, int boardY, Board* board);
    Rook();
    ~Rook();

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int boardX, int boardY) override;
    std::vector<Coordinate> getPossibleMoves() override;
    std::vector<Coordinate> getPossibleCaptures() override;
    
    
private:
    
};
