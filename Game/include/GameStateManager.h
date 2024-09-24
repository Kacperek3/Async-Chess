#pragma once
#include <stack>
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
