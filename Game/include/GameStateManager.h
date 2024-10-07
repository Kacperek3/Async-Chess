#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include <iostream>
#include "GameState.h"

class GameStateManager {
private:
    std::stack<GameState*> _states;  // Stos stanów

public:
    bool destroyCurrentState = false;
    void pushState(GameState* state);
    void popState();
    void closeAllStates();
    void closePoppedStates();
    GameState* getCurrentState();
    void handleInput();
    void update();
    void render();
};
