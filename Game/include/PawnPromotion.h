#pragma once

#include "Game.h"
#include <vector>
#include <iostream>

#define WHITE 0
#define BLACK 1

class PawnPromotion {
public:
    PawnPromotion(GameDataRef data);
    ~PawnPromotion();
    void Init(int x, int y);
    void Draw();
    bool ChoicePiece();
    
private:
    GameDataRef _data;
    bool whoOnTop = BLACK;
   
    sf::RectangleShape* _promotionWindow;

    std::vector<sf::Sprite> _WhitePiecesToChoice;
    std::vector<sf::Sprite> _BlackPiecesToChoice;


};