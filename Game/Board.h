#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "pawn.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"
#include "Knight.h"


class Board {
public:
    Board();  
    void draw(sf::RenderWindow& window);  
    bool isEmpty(int x, int y);
    bool isEnemyPieceAt(int boardX, int boardY, int color) const;
    void removePiece(int boardX, int boardY);

    std::vector<Piece*> b_pieces; // wektor przechowujący wskaźniki na obiekty klasy Piece
private:
    
};