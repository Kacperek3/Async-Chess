#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <iostream>
#include "State.h"
#include "Game.h"
#include <string>
#include <memory>
#include "Clock.h"
#include "CapturedPieces.h"
#include "Decorations.h"
#include "PawnPromotion.h"
#include "GameSounds.h"
#include "GameOver.h"

class GameWith2State : public State {
public:
    GameWith2State(GameDataRef data);

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
    

    GameDataRef _data;
    Board _board;

    Piece *selectedPiece = nullptr;


    int currentPlayerTurn;
    sf::Vector2f dragOffset;
    bool isDragging = false;
    Piece* draggedPiece = nullptr;
    bool isBoardRotated = false;

    ClockWidget *_clockWidget;
    bool _isClockTimeSet = false;
    sf::Sprite _player1Logo;
    sf::Font _font;
    sf::Sprite _player2Logo;
    sf::Text* _player1LogoText;
    sf::Text* _player2LogoText;

    PawnPromotion *_pawnPromotion;
    GameOver *_gameOver;
    CapturedPieces *_capturedPieces;
    Decorations *_decorations;
    GameSounds *_gameSounds;
    bool _isGameStartSoundPlayed = false;

    bool _isMovingAllowed = true;
    bool _isGameOver = false;
};
