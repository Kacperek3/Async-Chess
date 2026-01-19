#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <iostream>
#include "State.h"
#include "Game.h"
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include "CapturedPieces.h"
#include "Decorations.h"
#include "PawnPromotion.h"
#include "GameSounds.h"
#include "GameOver.h"
#include "AiLogic.h"

class GameWithAiState : public State {
public:
    GameWithAiState(GameDataRef data);

private:
    void Init() override;

    void HandleInput() override;
    void Update() override;
    void Draw() override;
    void ClearObjects() override;

    void startDragging(const sf::Vector2f& mousePosition);
    void stopDragging(sf::Vector2f& mousePosition);

    bool clickedOnPiece(const sf::Vector2f& mousePosition);
    void clickedOnField(const sf::Vector2f& mousePosition);

    void rotateView();
    

    std::thread _aiThread;
    std::atomic<bool> _isAiThinking;

    GameDataRef _data;

    Piece *selectedPiece = nullptr;


    int currentPlayerTurn;
    sf::Vector2f dragOffset;
    bool isDragging = false;
    Piece* draggedPiece = nullptr;
    bool isBoardRotated = false;
    Coordinate dragStartPos;
    sf::Sprite fakeDragSprite; 
    bool isFakeDragging = false; 
    Piece* fakeDragOriginPiece = nullptr;

    bool _isClockTimeSet = false;


    PawnPromotion *_pawnPromotion;
    GameOver *_gameOver;
    CapturedPieces *_capturedPieces;
    Decorations *_decorations;
    GameSounds *_gameSounds;
    bool _isGameStartSoundPlayed = false;

    bool _isMovingAllowed = true;
    bool _isGameOver = false;
    sf::Sprite _aiLogo;
    sf::Font _font;
    sf::Sprite _playerLogo;
    sf::Text* _playerLogoText;
    sf::Text* _aiLogoText;

    sf::Sprite _newGameButton;
    sf::Sprite _menuButton;

    AiLogic *_aiLogic;
};
