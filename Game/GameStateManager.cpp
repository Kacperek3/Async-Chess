#include "GameStateManager.h"

void GameStateManager::pushState(GameState* state) {
    _states.push(state);
}

void GameStateManager::popState() {
    if (!_states.empty()) {
        delete _states.top();  // Usuń bieżący stan
        _states.pop();
    }
}

void GameStateManager::closeAllStates() {
    while (!_states.empty()) {
        delete _states.top();  // Usuń bieżący stan
        _states.pop();
    }
}

void GameStateManager::closePoppedStates() {
    if (destroyCurrentState) {
        popState();
        destroyCurrentState = false;
    }
}

GameState* GameStateManager::getCurrentState() {
    if (!_states.empty()) {
        return _states.top();  // Zwróć bieżący stan
    }
    return nullptr;
}

void GameStateManager::handleInput() {
    if (getCurrentState()) {
        getCurrentState()->handleInput();  // Obsłuż wejście dla bieżącego stanu
    }
}

void GameStateManager::update() {
    if (getCurrentState()) {
        getCurrentState()->update();  // Aktualizuj bieżący stan
    }
}

void GameStateManager::render() {
    if (getCurrentState()) {
        getCurrentState()->render();  // Renderuj bieżący stan
    }
}