#include "InputManager.h"

bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window) {
    if (sf::Mouse::isButtonPressed(button)) {
        sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);
        if (tempRect.contains(sf::Mouse::getPosition(window))) {
            return true;
        }
    }
    return false;
}

bool InputManager::IsSpriteHover(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window) {
    sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);
    if (tempRect.contains(sf::Mouse::getPosition(window))) {
        return true;
    }
    return false;
}



bool InputManager::IsTextClicked(sf::Text text, sf::Mouse::Button button, sf::RenderWindow &window) {
    if (sf::Mouse::isButtonPressed(button)) {
        sf::IntRect tempRect(text.getPosition().x, text.getPosition().y, text.getGlobalBounds().width, text.getGlobalBounds().height);
        if (tempRect.contains(sf::Mouse::getPosition(window))) {
            return true;
        }
    }
    return false;
}

sf::Vector2f InputManager::GetMousePosition(sf::RenderWindow &window) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePosition);
}