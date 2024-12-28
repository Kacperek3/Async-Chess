#pragma once


#include <SFML/Audio.hpp>
#include "Game.h"
#include <sstream>
#include <iomanip>
#include <iostream>



class GameSounds {

public:
    GameSounds(GameDataRef data);
    ~GameSounds();
    void Init();
    void PlayMoveSound();
    void PlayCaptureSound();
    void PlayCheckSound();
    void PlayStartGameSound();
    void PlayEndGameSound();
    void ToggleGameSound();

private:
    GameDataRef _data;
    bool _isGameSoundOn = true;


    sf::Sound _moveSound;
    sf::Sound _captureSound;
    sf::Sound _startGameSound;
    sf::Sound _endGameSound;
    sf::Sound _CheckSound;

};