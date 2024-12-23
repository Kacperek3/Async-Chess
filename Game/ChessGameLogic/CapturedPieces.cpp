#include "CapturedPieces.h"


CapturedPieces::CapturedPieces(GameDataRef data): _data(data) {
}

void CapturedPieces::Init() {
   
}

void CapturedPieces::AddCapturedPiece(std::string pieceName, int color) {
    sf::Sprite sprite(_data->assetManager.GetTexture(pieceName));
    sprite.setScale(0.5, 0.5);
    if (color == WHITE) {
        _capturedWhitePieces[pieceName] = sprite;
    } else {
        _capturedBlackPieces[pieceName] = sprite;
    }
}


void CapturedPieces::Draw() {
    int x = 0;
    int y = 0;
    for (auto& piece : _capturedWhitePieces) {
        piece.second.setPosition(620 + x * 50, 150 + y * 50);
        _data->window.draw(piece.second);
        x++;
        if (x == 4) {
            x = 0;
            y++;
        }
    }

    x = 0;
    y = 0;
    for (auto& piece : _capturedBlackPieces) {
        piece.second.setPosition(620 + x * 50, 350 + y * 50);
        _data->window.draw(piece.second);
        x++;
        if (x == 4) {
            x = 0;
            y++;
        }
    }
}



CapturedPieces::~CapturedPieces() {
    std::cout << "CapturedPieces destructor" << std::endl;
}