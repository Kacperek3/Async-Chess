#include "Decorations.h"

Decorations::Decorations(GameDataRef data): _data(data) {
}

void Decorations::Init(std::string player1Name, std::string Player2Name) {

    _data->assetManager.LoadFont("Poppins", "assets/fonts/Poppins-Light.ttf");
    _font = _data->assetManager.GetFont("Poppins");


    _textFieldNameWhite = new sf::Text();
    _textFieldNameWhite->setFont(_font);
    _textFieldNameWhite->setCharacterSize(14);
    _textFieldNameWhite->setFillColor(sf::Color(255,255,255));
    _textFieldNameWhite->setPosition(65, 655);
    _textFieldNameWhite->setOutlineColor(sf::Color::Black);
    _textFieldNameWhite->setString(player1Name);
    _textFieldNameWhite->setOutlineThickness(0);
    _textFieldNameWhite->setStyle(sf::Text::Bold);


    _textFieldNameBlack = new sf::Text();
    _textFieldNameBlack->setFont(_font);
    _textFieldNameBlack->setCharacterSize(14);
    _textFieldNameBlack->setFillColor(sf::Color(255,255,255));
    _textFieldNameBlack->setPosition(65, 5);
    _textFieldNameBlack->setOutlineColor(sf::Color::Black);
    _textFieldNameBlack->setString(Player2Name);
    _textFieldNameBlack->setOutlineThickness(0);
    _textFieldNameBlack->setStyle(sf::Text::Bold);


    _data->assetManager.LoadTexture("YETI", "assets/ProfilePictures/Yeti.png");
    _data->assetManager.LoadTexture("GINGERBREAD", "assets/ProfilePictures/Gingerbread.png");

    _profilePictureWhite.setTexture(_data->assetManager.GetTexture("GINGERBREAD"));
    _profilePictureWhite.setPosition(15, 655);

    _profilePictureBlack.setTexture(_data->assetManager.GetTexture("YETI"));
    _profilePictureBlack.setPosition(15, 3);

    _backround = new sf::RectangleShape(sf::Vector2f(150, 670));
    _backround->setFillColor(sf::Color(45, 45, 45));
    _backround->setPosition(635, 15);



    _data->assetManager.LoadTexture("SETTINGS", "assets/settings.png");
    _settings.setTexture(_data->assetManager.GetTexture("SETTINGS"));
    _settings.setPosition(608, 12);
}

void Decorations::rotatePositionDecorations(){
    if (whoOnTop) {
        _profilePictureWhite.setPosition(15, 3);
        _textFieldNameWhite->setPosition(65, 5);
        _profilePictureBlack.setPosition(15, 655);
        _textFieldNameBlack->setPosition(65, 655);
    } else {
        _profilePictureWhite.setPosition(15, 655);
        _textFieldNameWhite->setPosition(65, 655);
        _profilePictureBlack.setPosition(15, 3);
        _textFieldNameBlack->setPosition(65, 5);
    }
    whoOnTop = !whoOnTop;
}

void Decorations::Draw() { 
    _data->window.draw(_profilePictureBlack);
    _data->window.draw(*_textFieldNameBlack);

    _data->window.draw(_profilePictureWhite);
    _data->window.draw(*_textFieldNameWhite);

    _data->window.draw(*_backround);
    _data->window.draw(_settings);
}

Decorations::~Decorations() {
    delete _backround;
    delete _textFieldNameBlack;
}
