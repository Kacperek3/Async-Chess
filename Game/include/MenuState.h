#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "State.h"

class MenuState : public State {
public:
    MenuState(GameDataRef data);

    void Init() override;

    void HandleInput() override;
    void Update() override;
    void Draw() override;


private:
    GameDataRef _data;
    

    sf::Font font;
    sf::Text playWithAiText;
    sf::Text playWith2Text;
    sf::Text playByLanText;
    sf::Text exitText;


    sf::RectangleShape playWithAiRect;
    sf::RectangleShape playWith2Rect;
    sf::RectangleShape playByLanRect;
    sf::RectangleShape exitRect;

    sf::Texture backgroundTexture;   // Tekstura tła
    sf::Sprite backgroundSprite;     // Sprite tła

    void setupText(sf::Text& text, sf::RectangleShape& rect,const std::string& str, float yPos);
    void checkMouseHover(const sf::Vector2f& mousePos, sf::Text& text, sf::RectangleShape& rect);

    const sf::Color normalColor = sf::Color::White;
    const sf::Color hoverColor = sf::Color::Red;
};
