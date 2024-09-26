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

//class GameWith2State;
class Board {
public:

    sf::RenderWindow *window;

    Board(sf::RenderWindow* window);  
    void draw(sf::RenderWindow& window, bool showCoordinates);  
    void drawPieces(sf::RenderWindow& window, Piece* draggedPiece);
    void drawBoard(sf::RenderWindow& window, bool showCoordinates);
    bool isEmpty(int x, int y);
    bool isEnemyPieceAt(int boardX, int boardY, int color) const;
    void removePiece(int boardX, int boardY);
    bool isKingInCheck(int color);
    bool canKingMove(int color);
    bool canPreventCheck(int color);
    bool isWithinBounds(int x, int y);
    bool isCheckmate(int color);
    std::vector<Piece*> b_pieces; // wektor przechowujący wskaźniki na obiekty klasy Piece
    bool isKingInCheckAfterMove(Piece* movedPiece, Coordinate targetPosition);
    void showPossibleMoves(sf::RenderWindow& window, Piece* piece); // Pokazuje możliwe ruchy dla przesuwanej bierki
    void showPossibleCaptures(sf::RenderWindow& window, Piece* piece); // Pokazuje możliwe bicia dla przesuwanej bierki
    void markPieceField(sf::RenderWindow& window, Piece* piece); // Podkreśla przesuwaną bierkę
    void promotePawn(Piece* pawn);
    bool isStalemate(int color);

private:
    
    Piece* findKing(int color);
    std::vector<Piece*> playerPieces(int color);
    std::vector<Piece*> enemyPieces(int color) const;
    std::vector<Coordinate> getValidMoves(Piece* piece);
    std::vector<Coordinate> getValidCaptures(Piece* piece);
    void undoMove(Piece* piece, Coordinate originalPosition, Piece* capturedPiece);
};