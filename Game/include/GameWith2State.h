#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <iostream>
#include "State.h"
#include "Game.h"

class GameWith2State : public State {
public:
    GameWith2State(GameDataRef data);
    ~GameWith2State();

private:
    void Init() override;

    void HandleInput() override;
    void Update() override;
    void Draw() override;

    void startDragging(const sf::Vector2f& mousePosition);
    void stopDragging(sf::Vector2f& mousePosition);
    void toggleCoordinates();
    void rotateView();
    

    GameDataRef _data;
    Board _board;


    int currentPlayerTurn;
    sf::Vector2f dragOffset;
    bool isDragging = false;
    Piece* draggedPiece = nullptr;
    bool showCoordinates = false;
    bool isBoardRotated = false;
};
