#pragma once
#include <SFML/Graphics.hpp>

struct Position {
    int boardX, boardY; 
    sf::Vector2f pixelPosition;  

    Position(int x, int y) : boardX(x), boardY(y) {
        pixelPosition = sf::Vector2f(x * 75+5, y * 75 + 6); 
    }

    sf::Vector2f getPixelPosition() const {
        return pixelPosition;
    }
};
