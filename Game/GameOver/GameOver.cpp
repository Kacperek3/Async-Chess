#include "GameOver.h"


GameOver::GameOver(GameDataRef data): _data(data) {
}

void GameOver::Init() {
    _data->assetManager.LoadFont("Poppins", "assets/fonts/Poppins-Light.ttf");
    _font = _data->assetManager.GetFont("Poppins");

    _data->assetManager.LoadTexture("PLAY_AGAIN_BUTTON", "assets/GameOverAssets/PlayAgainButton.png");
    _data->assetManager.LoadTexture("PLAY_AGAIN_BUTTON_HOVER", "assets/GameOverAssets/PlayAgainButton_hover.png");
    _playAgainButton.setTexture(_data->assetManager.GetTexture("PLAY_AGAIN_BUTTON"));
    _playAgainButton.setPosition(215, 250);

    _data->assetManager.LoadTexture("GO_TO_MENU_BUTTON", "assets/GameOverAssets/GoToMenuButton.png");
    //_data->assetManager.LoadTexture("GO_TO_MENU_BUTTON_HOVER", "../assets/GameOverAssets/GoToMenuButton_hover.png");
    _goToMenuButton.setTexture(_data->assetManager.GetTexture("GO_TO_MENU_BUTTON"));
    _goToMenuButton.setPosition(215, 320);



    _textFielHeader = new sf::Text("Black Wins", _font, 30);
    _textFielHeader->setFillColor(sf::Color::White);
    _textFielHeader->setPosition(240, 160);
    _textFielHeader->setStyle(sf::Text::Bold);

    _textFielReason = new sf::Text("placeholder", _font, 17);
    _textFielReason->setFillColor(sf::Color::White);
    _textFielReason->setPosition(255, 200);

    _backround = new sf::RectangleShape(sf::Vector2f(300, 250));
    _backround->setFillColor(sf::Color(45, 45, 45));
    _backround->setPosition(170, 150);

    _backgroundToHeader = new sf::RectangleShape(sf::Vector2f(300, 80));
    _backgroundToHeader->setFillColor(sf::Color(35, 35, 35));
    _backgroundToHeader->setPosition(170, 150);
}


void GameOver::whoWins(int color, int reason) {
    if(color == WHITE){
        _textFielHeader->setString("White Wins");
    }
    else{
        _textFielHeader->setString("Black Wins");
    }


    if(reason == checkmate){
        _textFielReason->setString("by Checkmate");
    }
    else if(reason == stalemate){
        _textFielReason->setString("by Stalemate");
    }
    else if(reason == timeEnd){
        _textFielReason->setString("by enemy Time End");
    }
    else if(reason == resignation){
        _textFielReason->setString("by enemy Resignation");
        _textFielReason->setPosition(225, 200);
    }

}

void GameOver::Update() {
    if(_data->inputManager.IsSpriteHover(_playAgainButton, sf::Mouse::Left, _data->window)){
        _playAgainButton.setTexture(_data->assetManager.GetTexture("PLAY_AGAIN_BUTTON_HOVER"));
    } 
    else {
        _playAgainButton.setTexture(_data->assetManager.GetTexture("PLAY_AGAIN_BUTTON"));
    }
}


bool GameOver::UserInput() {
    if(_data->inputManager.IsSpriteClicked(_playAgainButton, sf::Mouse::Left, _data->window)){
        return newGame;
    }
    return 0;
}

void GameOver::Draw() {
    _data->window.draw(*_backround);
    _data->window.draw(*_backgroundToHeader);
    _data->window.draw(*_textFielHeader);
    _data->window.draw(*_textFielReason);
    _data->window.draw(_playAgainButton);
    _data->window.draw(_goToMenuButton);
}

GameOver::~GameOver() {
    delete _backround;
    delete _backgroundToHeader;
    delete _textFielHeader;
}