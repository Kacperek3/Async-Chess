#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces.h"



class Queen : public Piece {
public:
    Queen(int color, int boardX, int boardY, Board* board, sf::Texture& queenTexture);
    Queen();
    ~Queen();

    void draw(sf::RenderWindow& window) override;
    bool isValidMove(int boardX, int boardY) override;
    bool isValidRookMove(int boardX, int boardY);
    bool isValidBishopMove(int boardX, int boardY);
    std::vector<Coordinate> getPossibleMoves() override;
    std::vector<Coordinate> getPossibleCaptures() override;
    void rotatePiece() override;
    
private:
    
};
