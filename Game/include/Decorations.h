#pragma once

#include "Game.h"
#include <vector>
#include <iostream>

#define WHITE 0
#define BLACK 1

class Decorations {
public:
    Decorations(GameDataRef data);
    ~Decorations();
    void Init(std::string player1Name, std::string player2Name);
    void rotatePositionDecorations();
    void Draw();
    
private:
    GameDataRef _data;
    bool whoOnTop = BLACK;
   
    sf::Font _font;
    sf::Text *_textFieldNameBlack;
    sf::Text *_textFieldNameWhite;

    sf::RectangleShape* _backround;

    sf::Sprite _profilePictureWhite;
    sf::Sprite _profilePictureBlack;

    sf::Sprite _settings;
};