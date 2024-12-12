#pragma once
#include <SFML/Graphics.hpp>
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
    
    sf::Font _font;
    

    sf::Sprite _backgroundSprite;     // Sprite tła
    sf::Sprite _playWithFriendSprite;    
    sf::Sprite _playWithAISprite;      // Sprite przycisku "Play with AI"

    void UpdateSpriteTexture(sf::Sprite& sprite, const std::string& normalTexture, const std::string& hoverTexture);
};
