#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

#define WHITE 0
#define BLACK 1

class CapturedPieces {
public:
    CapturedPieces(GameDataRef data);
    ~CapturedPieces();
    void Init();
    void AddCapturedPiece(std::string pieceName, int color);
    void Draw();
    
private:
    GameDataRef _data;
    std::vector<sf::Sprite> _capturedWhitePieces;
    std::vector<sf::Sprite> _capturedBlackPieces;


    sf::RectangleShape *_background1;
};