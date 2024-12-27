#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "pawn.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"
#include "Knight.h"
#include "CapturedPieces.h"
#include "PawnPromotion.h"

class Board {
public:

    sf::RenderWindow *window;
    Board(GameDataRef data);
    void Init();
    ~Board();

    // funkcje odpowiedzialne za logikę gry
    void draw(sf::RenderWindow& window, bool showCoordinates);  
    void drawPieces(sf::RenderWindow& window, Piece* draggedPiece);
    void drawBoard(sf::RenderWindow& window, bool showCoordinates);
    bool isEmpty(int x, int y);
    bool isEnemyPieceAt(int boardX, int boardY, int color) const;
    bool isTeamPieceAt(int boardX, int boardY, int color) const;
    void removePiece(int boardX, int boardY, CapturedPieces* capturedPieces);
    bool isKingInCheck(int color);
    bool canKingMove(int color);
    bool canPreventCheck(int color);
    bool isWithinBounds(int x, int y);
    bool isCheckmate(int color);
    std::vector<Piece*> b_pieces;
    bool isKingInCheckAfterMove(Piece* movedPiece, Coordinate targetPosition);
    void showPossibleMoves(sf::RenderWindow& window, Piece* piece); 
    void showPossibleCaptures(sf::RenderWindow& window, Piece* piece); 
    void showCheck(sf::RenderWindow& window, int color);
    void markPieceField(sf::RenderWindow& window, Piece* piece);
    void promotePawn(Piece* pawn);
    bool isStalemate(int color);
    Piece* getPieceAt(int x, int y);
    void rotatePieces();
    void deleteObjects();


private:
    
    Piece* findKing(int color);
    std::vector<Piece*> playerPieces(int color) const;
    std::vector<Piece*> enemyPieces(int color) const;
    std::vector<Coordinate> getValidMoves(Piece* piece);
    std::vector<Coordinate> getValidCaptures(Piece* piece);
    void undoMove(Piece* piece, Coordinate originalPosition, Piece* capturedPiece);


    GameDataRef _data;
    // funkcje odpowiedzialne za rysowanie planszy i bierki itp
    sf::RectangleShape *tile;
    sf::RectangleShape *markedField;
    sf::CircleShape *circle;


    sf::Color *recColor;
    sf::Color *circleColor;


    PawnPromotion* _pawnPromotion;

};