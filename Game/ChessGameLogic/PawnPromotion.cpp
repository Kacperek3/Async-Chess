#include "PawnPromotion.h"

PawnPromotion::PawnPromotion(GameDataRef data) : _data(std::move(data)), _promotionWindow(sf::Vector2f(PROMOTION_WINDOW_WIDTH, PROMOTION_WINDOW_HEIGHT)) {
    _promotionWindow.setFillColor(sf::Color(255, 255, 255));
}

void PawnPromotion::Init() {
    SetupPieces();
}

void PawnPromotion::SetupPieces() {
    // Load textures for white pieces
    _whitePieces[0].setTexture(_data->assetManager.GetTexture("wq"));
    _whitePieces[1].setTexture(_data->assetManager.GetTexture("wr"));
    _whitePieces[2].setTexture(_data->assetManager.GetTexture("wb"));
    _whitePieces[3].setTexture(_data->assetManager.GetTexture("wn"));

    // Load textures for black pieces
    _blackPieces[0].setTexture(_data->assetManager.GetTexture("bq"));
    _blackPieces[1].setTexture(_data->assetManager.GetTexture("br"));
    _blackPieces[2].setTexture(_data->assetManager.GetTexture("bb"));
    _blackPieces[3].setTexture(_data->assetManager.GetTexture("bn"));
}

void PawnPromotion::ChangePosition(int x, int y, int color) {
    _color = color;
    _promotionWindow.setPosition(x * TILE_SIZE, y);

    int ypos = y + PIECE_OFFSET;
    if (_color == BLACK) {
        SetPiecePositions(_blackPieces, x, ypos);
    } else {
        SetPiecePositions(_whitePieces, x, ypos);
    }
}

void PawnPromotion::SetPiecePositions(std::array<sf::Sprite, 4>& pieces, int x, int y) {
    for (auto& piece : pieces) {
        piece.setPosition(x * TILE_SIZE + PIECE_OFFSET, y);
        y += TILE_SIZE;
    }
}

void PawnPromotion::Draw() {
    _data->window.draw(_promotionWindow);
    if (_color == BLACK) {
        DrawPieces(_blackPieces);
    } else {
        DrawPieces(_whitePieces);
    }
}

void PawnPromotion::DrawPieces(const std::array<sf::Sprite, 4>& pieces) {
    for (const auto& piece : pieces) {
        _data->window.draw(piece);
    }
}

int PawnPromotion::ChoicePiece() {
    const auto& pieces = (_color == WHITE) ? _whitePieces : _blackPieces;

    for (size_t i = 0; i < pieces.size(); ++i) {
        if (_data->inputManager.IsSpriteClicked(pieces[i], sf::Mouse::Left, _data->window)) {
            return static_cast<int>(i + 1);
        }
    }
    return 0;
}