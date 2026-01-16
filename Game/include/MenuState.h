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
    void ClearObjects() override;


private:
    GameDataRef _data;
    
    sf::Font _font;
    

    sf::Sprite _backgroundSprite;    
    sf::Sprite _playWithFriendSprite;    
    sf::Sprite _playWithAISprite;      

    void UpdateSpriteTexture(sf::Sprite& sprite, const std::string& normalTexture, const std::string& hoverTexture);
};
