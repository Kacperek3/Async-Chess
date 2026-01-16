#include "MenuState.h"
#include "GameWith2State.h"
#include "GameWithAiState.h"

MenuState::MenuState(GameDataRef data) : _data(data) {
    
}

void MenuState::Init(){
    _data->assetManager.LoadTexture("Background", "assets/Designer.png");
    _data->assetManager.LoadFont("Poppins-Thin", "assets/fonts/Poppins-Thin.ttf");

    _backgroundSprite.setTexture(_data->assetManager.GetTexture("Background"));
    _font = _data->assetManager.GetFont("Poppins-Thin");

    _data->assetManager.LoadTexture("PLAY_WITH_FRIEND", "assets/MenuAssets/Buttons/Button_play_friend.png");
    _data->assetManager.LoadTexture("PLAY_WITH_FRIEND_HOVER", "assets/MenuAssets/Buttons/Button_play_friend_hover.png");
    _playWithFriendSprite.setTexture(_data->assetManager.GetTexture("PLAY_WITH_FRIEND"));
    _playWithFriendSprite.setPosition(280, 100);

    _data->assetManager.LoadTexture("BUTTON_PLAY_AI", "assets/MenuAssets/Buttons/Button_play_ai.png");
    _data->assetManager.LoadTexture("BUTTON_PLAY_AI_HOVER", "assets/MenuAssets/Buttons/Button_play_ai_hover.png");
    _playWithAISprite.setTexture(_data->assetManager.GetTexture("BUTTON_PLAY_AI"));
    _playWithAISprite.setPosition(280, 200);
}

void MenuState::HandleInput() {
    sf::Event event;
    while (_data->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _data->window.close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
            
            if(_data->inputManager.IsSpriteClicked(_playWithFriendSprite, sf::Mouse::Left, _data->window)) {
                std::cout << "Play with friend selected\n";
                _data->stateManager.AddState(StateRef(new GameWith2State(_data)), true);
                _data->window.setSize(sf::Vector2u(800, 700)); 
            }
            else if (_data->inputManager.IsSpriteClicked(_playWithAISprite, sf::Mouse::Left, _data->window)) {
                _data->stateManager.AddState(StateRef(new GameWithAiState(_data)), true);
                _data->window.setSize(sf::Vector2u(800, 700));
            }
            
        }
    }
}

void MenuState::Update() {
    sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
    
    UpdateSpriteTexture(_playWithFriendSprite, "PLAY_WITH_FRIEND", "PLAY_WITH_FRIEND_HOVER");
    UpdateSpriteTexture(_playWithAISprite, "BUTTON_PLAY_AI", "BUTTON_PLAY_AI_HOVER");
}

void MenuState::UpdateSpriteTexture(sf::Sprite& sprite, const std::string& normalTexture, const std::string& hoverTexture) {
    if (_data->inputManager.IsSpriteHover(sprite, sf::Mouse::Left, _data->window)) {
        sprite.setTexture(_data->assetManager.GetTexture(hoverTexture));
    } else {
        sprite.setTexture(_data->assetManager.GetTexture(normalTexture));
    }
}


void MenuState::Draw() {
    _data->window.clear();
    _data->window.draw(_backgroundSprite);
    _data->window.draw(_playWithFriendSprite);
    _data->window.draw(_playWithAISprite);
    _data->window.display();
}


void MenuState::ClearObjects() {
    _data->assetManager.clearAssets();
}