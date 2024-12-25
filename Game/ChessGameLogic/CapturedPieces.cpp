#include "CapturedPieces.h"


CapturedPieces::CapturedPieces(GameDataRef data): _data(data) {
}

void CapturedPieces::Init() {
    _background1 = new sf::RectangleShape(sf::Vector2f(200, 340));
    _background1->setFillColor(sf::Color( 148, 148,148, 128));
    _background1->setPosition(600, 130);

}

void CapturedPieces::AddCapturedPiece(std::string pieceName, int color) {
    sf::Sprite sprite(_data->assetManager.GetTexture(pieceName));
    sprite.setScale(0.65, 0.65);
    if (color == WHITE) {
        _capturedWhitePieces.push_back(sprite);
    } else {
        _capturedBlackPieces.push_back(sprite);
    }
}


void CapturedPieces::Draw() {
    _data->window.draw(*_background1);

    int x = 0, y = 0;
    for (auto& piece : _capturedWhitePieces) {
        piece.setPosition(50 + x * 10, 2 + y * 50);
        _data->window.draw(piece);
        x++;
        if (x == 4) {
            x = 0;
            y++;
        }
    }

    x = 0;
    y = 0;
    for (auto& piece : _capturedBlackPieces) {
        piece.setPosition(50 + x * 10, 654 + y * 50);
        _data->window.draw(piece);
        x++;
        if (x == 4) {
            x = 0;
            y++;
        }
    }
}


CapturedPieces::~CapturedPieces() {
    std::cout << "CapturedPieces destructor" << std::endl;
    delete _background1;
}