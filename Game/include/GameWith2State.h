#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <iostream>
#include "GameState.h"
#include "GameStateManager.h"
#include "MenuState.h"

class GameWith2State : public GameState {
public:
    GameWith2State(GameStateManager* gsm, sf::RenderWindow* window);
    ~GameWith2State();

private:
    void handleInput() override;
    void update() override;
    void render() override;

    void startDragging(const sf::Vector2f& mousePosition);
    void stopDragging(sf::Vector2f& mousePosition);
    void toggleCoordinates();
    void rotateView();
    
    GameStateManager* gsm;
    sf::RenderWindow* window;

    Board board;

    int currentPlayerTurn;
    sf::Vector2f dragOffset;
    bool isDragging = false;
    Piece* draggedPiece = nullptr;
    bool showCoordinates = false;
    bool isBoardRotated = false;
};
