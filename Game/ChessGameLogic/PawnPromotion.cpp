#include "PawnPromotion.h"


PawnPromotion::PawnPromotion(GameDataRef data) : _data(data) {
}

void PawnPromotion::Init(int x, int y) {
    _promotionWindow = new sf::RectangleShape(sf::Vector2f(75, 300));
    _promotionWindow->setFillColor(sf::Color(0, 0, 0));
    _promotionWindow->setPosition(x, y);


    _data->assetManager.LoadTexture("wq", "../assets/pieces/chessCom1/wq.png");
    _data->assetManager.LoadTexture("wr", "../assets/pieces/chessCom1/wr.png");
    _data->assetManager.LoadTexture("wb", "../assets/pieces/chessCom1/wb.png");
    _data->assetManager.LoadTexture("wn", "../assets/pieces/chessCom1/wn.png");

    sf::Sprite queen(_data->assetManager.GetTexture("wq"));
    queen.setPosition(x*75, y + 15);

    sf::Sprite rook(_data->assetManager.GetTexture("wr"));
    rook.setPosition(x*75, y + 90);

    sf::Sprite bishop(_data->assetManager.GetTexture("wb"));
    bishop.setPosition(x*75, y + 165);

    sf::Sprite knight(_data->assetManager.GetTexture("wn"));
    knight.setPosition(x*75, y + 240);


    _WhitePiecesToChoice.push_back(queen);
    _WhitePiecesToChoice.push_back(rook);
    _WhitePiecesToChoice.push_back(bishop);
    _WhitePiecesToChoice.push_back(knight);
}

PawnPromotion::~PawnPromotion() {
    delete _promotionWindow;
}


void PawnPromotion::Draw() {
    _data->window.draw(*_promotionWindow);
    for(auto& piece : _WhitePiecesToChoice){
        _data->window.draw(piece);
    }

}

bool PawnPromotion::ChoicePiece() {
    bool spriteClicked = false;

    while(!spriteClicked){
        if(_data->inputManager.IsSpriteClicked(_WhitePiecesToChoice[0], sf::Mouse::Left, _data->window)){
            spriteClicked = true;
            return true;
        }
        else if(_data->inputManager.IsSpriteClicked(_WhitePiecesToChoice[1], sf::Mouse::Left, _data->window)){
            spriteClicked = true;
            return true;
        }
        else if(_data->inputManager.IsSpriteClicked(_WhitePiecesToChoice[2], sf::Mouse::Left, _data->window)){
            spriteClicked = true;
            return true;
        }
        else if(_data->inputManager.IsSpriteClicked(_WhitePiecesToChoice[3], sf::Mouse::Left, _data->window)){
            spriteClicked = true;
            return true;
        }
    }
    return false;
}


