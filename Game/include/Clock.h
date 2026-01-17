#pragma once


#include <SFML/Graphics.hpp>
#include "Game.h"
#include <sstream>
#include <iomanip>
#include <iostream>


#define WHITE 0
#define BLACK 1
#define NOTHING_POINTED 2

#define BLACK_LOST 1
#define WHITE_LOST 2

#define OUT_OF_SCREEN -1000

enum GameStatus{
    BEFORE_GAME,
    GAME_RUNNING,
    GAME_PAUSE,
    GAME_END
};

class ClockWidget {

public:
    ClockWidget(GameDataRef data);
    ~ClockWidget();
    void Init();
    bool StartButtonPressed();
    bool PauseButtonPressed();
    bool ResumeButtonPressed();
    bool NewGameButtonPressed();
    bool MenuButtonPressed();
    void ResumeGame();
    void inputTime(sf::Event event);
    bool Update();
    void inputTimeBlack(sf::Event event);
    void inputTimeWhite(sf::Event event);
    void togglePlayerTime();
    void rotatePositionClocks();
    bool getIsClockTimeSet(){return _isClockTimeSet;}
    void Draw();

      
private:
    GameDataRef _data;


    sf::Sprite _clockIconWhite;
    bool _isClockWhiteIconVisible = false;

    sf::Sprite _clockIconBlack;
    bool _isClockBlackIconVisible = false;


    sf::Sprite _startButton;
    sf::Sprite _newGameButton;
    sf::Sprite _pauseButton;
    sf::Sprite _resumeButton;
    sf::Sprite _menuButton;
    enum GameStatus _gameStatus = BEFORE_GAME;

    sf::Font _font;

    int _whichClockPointed = NOTHING_POINTED;
    bool whoOnTop = BLACK;
    bool _isClockTimeSet = false;


    sf::Text *_textFieldBlack;
    std::string inputTextBlack = "";
    sf::Clock *countdownClockBlack;
    int remainingTimeInSecondsBlack = 0;
    bool isCountdownActiveBlack = false;
    sf::Sprite _backgroudn_to_textFieldBlack;

    sf::Text *_textFieldWhite;
    std::string inputTextWhite = "";
    sf::Clock *countdownClockWhite;
    int remainingTimeInSecondsWhite = 0;
    bool isCountdownActiveWhite = false;
    sf::Sprite _backgroudn_to_textFieldWhite;
};