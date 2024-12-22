#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"
#include <iostream>


class Bishop : public Piece {
public:
    Bishop(int color, int boardX, int boardY, Board* board, sf::Texture& bishopTexture);
    Bishop();
    ~Bishop();

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int boardX, int boardY) override;
    std::vector<Coordinate> getPossibleMoves() override;
    std::vector<Coordinate> getPossibleCaptures() override;
    void rotatePiece() override;
    
private:
    
};
