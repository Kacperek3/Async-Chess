#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include <iostream>
#include "GameState.h"

class GameStateManager {
private:
    std::stack<GameState*> _states;  // Stos stanów

public:
    void pushState(GameState* state);
    void popState();
    GameState* getCurrentState();
    void handleInput();
    void update();
    void render();
};
