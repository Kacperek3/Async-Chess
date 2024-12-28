#pragma once

#include "Game.h"
#include "Board.h"
#include <array>
#include <iostream>


constexpr int TILE_SIZE = 75;
constexpr int PROMOTION_WINDOW_WIDTH = 75;
constexpr int PROMOTION_WINDOW_HEIGHT = 300;
constexpr int PIECE_OFFSET = 5;

class PawnPromotion {
public:
    explicit PawnPromotion(GameDataRef data);
    ~PawnPromotion() = default;

    void Init();
    void Draw();
    int ChoicePiece();
    void ChangePosition(int x, int y, int color);

private:
    GameDataRef _data;

    int _x, _y;
    int _color;

    sf::RectangleShape _promotionWindow;

    std::array<sf::Sprite, 4> _whitePieces;
    std::array<sf::Sprite, 4> _blackPieces;

    void SetupPieces();
    void DrawPieces(const std::array<sf::Sprite, 4>& pieces);
    void SetPiecePositions(std::array<sf::Sprite, 4>& pieces, int x, int y);
};