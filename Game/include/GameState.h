#pragma once
#include <SFML/Graphics.hpp>

class GameState {
public:
    virtual void handleInput() = 0;  // Obsługa wejścia
    virtual void update() = 0;       // Aktualizacja stanu
    virtual void render() = 0;  // Renderowanie
    virtual ~GameState() = default;  // Wirtualny destruktor
};
