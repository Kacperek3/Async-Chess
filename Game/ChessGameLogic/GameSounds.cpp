#include "GameSounds.h"

GameSounds::GameSounds(GameDataRef data) : _data(data) {
    
}

GameSounds::~GameSounds() {
    std::cout << "GameSounds destructor" << std::endl;
}

void GameSounds::Init() {
    _data->soundManager.LoadSoundBuffer("MOVE", "../assets/Sounds/move-self.wav");
    _moveSound = _data->soundManager.CreateSound("MOVE");

    _data->soundManager.LoadSoundBuffer("CAPTURE", "../assets/Sounds/capture.wav");
    _captureSound = _data->soundManager.CreateSound("CAPTURE");

    _data->soundManager.LoadSoundBuffer("START_GAME", "../assets/Sounds/game-start.wav");
    _startGameSound = _data->soundManager.CreateSound("START_GAME");

    _data->soundManager.LoadSoundBuffer("END_GAME", "../assets/Sounds/game-end.wav");
    _endGameSound = _data->soundManager.CreateSound("END_GAME");

    _data->soundManager.LoadSoundBuffer("CHECK", "../assets/Sounds/move-check.wav");
    _CheckSound = _data->soundManager.CreateSound("CHECK");
}

void GameSounds::PlayMoveSound() {
    if (!_isGameSoundOn) {
        return;
    }
    _moveSound.play();
}

void GameSounds::PlayCaptureSound() {
    if (!_isGameSoundOn) {
        return;
    }
    _captureSound.play();
}

void GameSounds::PlayCheckSound() {
    if (!_isGameSoundOn) {
        return;
    }
    _CheckSound.play();
}

void GameSounds::PlayStartGameSound() {
    if (!_isGameSoundOn) {
        return;
    }
    _startGameSound.play();
}

void GameSounds::PlayEndGameSound() {
    if (!_isGameSoundOn) {
        return;
    }
    _endGameSound.play();
}

void GameSounds::ToggleGameSound() {
    _isGameSoundOn = !_isGameSoundOn;
}