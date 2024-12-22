#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"
#include <iostream>



class Pawn : public Piece {
public:
    Pawn(int color, int boardX, int boardY, Board* board, int direction, sf::Texture& pawnTexture);
    Pawn();
    ~Pawn();

    void draw(sf::RenderWindow& window) override;
    void move(int boardX, int boardY) override;
    bool isValidMove(int boardX, int boardY) override;
    std::vector<Coordinate> getPossibleMoves() override;
    std::vector<Coordinate> getPossibleCaptures() override;
    void rotatePiece() override;
    
    
private:
    bool firstMove = true;
    int direction;
};
