#include "Clock.h"

ClockWidget::ClockWidget(GameDataRef data) : _data(data) {
    
}

ClockWidget::~ClockWidget() {
    std::cout << "ClockWidget destructor" << std::endl;
    delete _textFieldBlack;
    delete _textFieldWhite;
    delete countdownClockBlack;
    delete countdownClockWhite;
}

void ClockWidget::Init() {
    _data->assetManager.LoadTexture("START_BUTTON", "../assets/GameWithFriendState/Buttons/StartButton.png");
    _data->assetManager.LoadTexture("START_BUTTON_HOVER", "../assets/GameWithFriendState/Buttons/StartButton_hover.png");
    _startButton.setTexture(_data->assetManager.GetTexture("START_BUTTON"));
    _startButton.setPosition(610, 271);
    _startButton.setScale(0.90, 0.90);

    _data->assetManager.LoadTexture("CLOCK_ICON_WHITE_1", "../assets/ClockWhiteICON1.png");
    _data->assetManager.LoadTexture("CLOCK_ICON_WHITE_2", "../assets/ClockWhiteICON2.png");
    _data->assetManager.LoadTexture("CLOCK_ICON_WHITE_3", "../assets/ClockWhiteICON3.png");
    _data->assetManager.LoadTexture("CLOCK_ICON_WHITE_4", "../assets/ClockWhiteICON4.png");
    _clockIconWhite.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_WHITE_1"));
    _clockIconWhite.setScale(0.55, 0.55);
    _clockIconWhite.setPosition(460, 8);


    _data->assetManager.LoadTexture("CLOCK_ICON_BLACK_1", "../assets/ClockBlackICON1.png");
    _data->assetManager.LoadTexture("CLOCK_ICON_BLACK_2", "../assets/ClockBlackICON2.png");
    _data->assetManager.LoadTexture("CLOCK_ICON_BLACK_3", "../assets/ClockBlackICON3.png");
    _data->assetManager.LoadTexture("CLOCK_ICON_BLACK_4", "../assets/ClockBlackICON4.png");
    _clockIconBlack.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_BLACK_1"));
    _clockIconBlack.setScale(0.55, 0.55);
    _clockIconBlack.setPosition(460, 666);


    _data->assetManager.LoadFont("Poppins", "../assets/fonts/Poppins-Light.ttf");
    _font = _data->assetManager.GetFont("Poppins");

   
    countdownClockBlack = new sf::Clock();
    countdownClockWhite = new sf::Clock();

    _textFieldBlack = new sf::Text();
    _textFieldBlack->setFont(_font);
    _textFieldBlack->setCharacterSize(18);
    _textFieldBlack->setFillColor(sf::Color(200,200,200));
    _textFieldBlack->setPosition(530, 10);
    _textFieldBlack->setOutlineColor(sf::Color::Black);
    _textFieldBlack->setOutlineThickness(0);
    _textFieldBlack->setString("00:00");
    _textFieldBlack->setStyle(sf::Text::Bold);


    _textFieldWhite = new sf::Text();
    _textFieldWhite->setFont(_font);
    _textFieldWhite->setCharacterSize(18);
    _textFieldWhite->setFillColor(sf::Color( 80, 80, 80));
    _textFieldWhite->setPosition(530, 668);
    _textFieldWhite->setOutlineColor(sf::Color::White);
    _textFieldWhite->setOutlineThickness(0);
    _textFieldWhite->setString("00:00");
    _textFieldWhite->setStyle(sf::Text::Bold);



    _data->assetManager.LoadTexture("BACKGROUND_TO_TEXTFIELD", "../assets/TextClockBackground.png");
    _data->assetManager.LoadTexture("BACKGROUND_TO_TEXTFIELD_POINTED", "../assets/TextClockBackgroundPointed.png");
    _data->assetManager.LoadTexture("BACKGROUND_TO_TEXTFIELD_LOW_TIME", "../assets/TextClockBackgroundLowTime.png");
    _data->assetManager.LoadTexture("BACKGROUND_TO_TEXTFIELD_BLACK", "../assets/TextClockBackgroundBlack.png");
    _data->assetManager.LoadTexture("BACKGROUND_TO_TEXTFIELD_BLACK_POINTED", "../assets/TextClockBackgroundBlackPointed.png");


    _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_BLACK"));
    _backgroudn_to_textFieldBlack.setScale(0.8, 0.65);
    _backgroudn_to_textFieldBlack.setPosition(480, 5);


    _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD"));
    _backgroudn_to_textFieldWhite.setScale(0.8, 0.65);
    _backgroudn_to_textFieldWhite.setPosition(480, 663);
}


void ClockWidget::StartButtonPressed() {
    if(_data->inputManager.IsSpriteClicked(_backgroudn_to_textFieldBlack, sf::Mouse::Left, _data->window)){
        _whichClockPointed = BLACK;
        _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_BLACK_POINTED"));
        _textFieldBlack->setFillColor(sf::Color(255,255,255)); // czysty bialy
        _isClockBlackIconVisible = false;

        _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD"));
        _textFieldWhite->setFillColor(sf::Color(80,80,80));
        _isClockWhiteIconVisible = true;
    }
    else if(_data->inputManager.IsSpriteClicked(_backgroudn_to_textFieldWhite, sf::Mouse::Left, _data->window)){
        _whichClockPointed = WHITE;
        _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_POINTED"));
        _textFieldWhite->setFillColor(sf::Color(0,0,0)); // czysty bialy
        _isClockWhiteIconVisible = false;

        _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_BLACK"));
        _textFieldBlack->setFillColor(sf::Color(200,200,200)); // taki szarawy
        _isClockBlackIconVisible = true;
    }


    if (_data->inputManager.IsSpriteClicked(_startButton, sf::Mouse::Left, _data->window)) {
        try {
            //konwertowanie czasu z inputText na int i zapisanie go w remainingTimeInSeconds
            int minutes = std::stoi(inputTextBlack.substr(0, 2));
            int seconds = std::stoi(inputTextBlack.substr(3, 2));
            remainingTimeInSecondsBlack = minutes * 60 + seconds;
            countdownClockBlack->restart();
            isCountdownActiveBlack = false;

            // to samo ale dla bialego
            minutes = std::stoi(inputTextWhite.substr(0, 2));
            seconds = std::stoi(inputTextWhite.substr(3, 2));
            remainingTimeInSecondsWhite = minutes * 60 + seconds;
            countdownClockWhite->restart();
            isCountdownActiveWhite = true;

            //Ustawienie aby na poczatku zegar bialego byl podswietlony
            _whichClockPointed = WHITE;
            _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_BLACK"));
            _textFieldBlack->setFillColor(sf::Color(200,200,200));
            _isClockBlackIconVisible = true;

            _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_POINTED"));
            _textFieldWhite->setFillColor(sf::Color(0,0,0));
            _isClockWhiteIconVisible = false;

            _isClockTimeSet = true;
        } catch (...) {

            std::cout << "Nieprawidłowy format czasu" << std::endl;
            _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_LOW_TIME"));
            _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_LOW_TIME"));
            _isClockTimeSet = false;
        }
    }
}

void ClockWidget::inputTime(sf::Event event) {

    if(_whichClockPointed == BLACK){
        inputTimeBlack(event);
    }
    else if(_whichClockPointed == WHITE){
        inputTimeWhite(event);
    }
}

void ClockWidget::inputTimeBlack(sf::Event event) {
    if (std::isdigit(event.text.unicode)) {
        if(inputTextBlack.size() > 4){
            return;
        }
        inputTextBlack += static_cast<char>(event.text.unicode);
    }
    else if (event.text.unicode == 8 && !inputTextBlack.empty()) {
        inputTextBlack.pop_back();

        if(inputTextBlack.size() == 2){
            inputTextBlack.pop_back();
        }
    }


    if(inputTextBlack.size() == 2){
        inputTextBlack += ":";
    }
    _textFieldBlack->setString(inputTextBlack);
}

void ClockWidget::inputTimeWhite(sf::Event event) {
    if (std::isdigit(event.text.unicode)) {
        if(inputTextWhite.size() > 4){
            return;
        }

        inputTextWhite += static_cast<char>(event.text.unicode);
    }
    else if (event.text.unicode == 8 && !inputTextWhite.empty()) {
        inputTextWhite.pop_back();

        if(inputTextWhite.size() == 2){
            inputTextWhite.pop_back();
        }
    }

    if(inputTextWhite.size() == 2){
        inputTextWhite += ":";
    }

    _textFieldWhite->setString(inputTextWhite);
}

void ClockWidget::Update(){
    if(_data->inputManager.IsSpriteHover(_startButton, sf::Mouse::Left, _data->window)){
        _startButton.setTexture(_data->assetManager.GetTexture("START_BUTTON_HOVER"));
    } 
    else {
        _startButton.setTexture(_data->assetManager.GetTexture("START_BUTTON"));
    }

   sf::Time elapsed;

    if (isCountdownActiveBlack) {
        elapsed = countdownClockBlack->getElapsedTime();
        if (elapsed.asSeconds() >= 1) {
            remainingTimeInSecondsBlack -= static_cast<int>(elapsed.asSeconds());
            countdownClockBlack->restart();

            int whichAnimation = remainingTimeInSecondsBlack % 4 + 1;
            if (whichAnimation == 1) {
                _clockIconWhite.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_WHITE_4"));
            } else if (whichAnimation == 2) {
                _clockIconWhite.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_WHITE_3"));
            } else if (whichAnimation == 3) {
                _clockIconWhite.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_WHITE_2"));
            } else if (whichAnimation == 4) {
                _clockIconWhite.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_WHITE_1"));
            }



            if (remainingTimeInSecondsBlack <= 0) {
                isCountdownActiveBlack = false;
                remainingTimeInSecondsBlack = 0;
                std::cout << "Koniec odliczania dla czarnego!" << std::endl;
            }

            // Aktualizuj tekst wyświetlany w _textFieldBlack
            int minutes = remainingTimeInSecondsBlack / 60;
            int seconds = remainingTimeInSecondsBlack % 60;
            std::ostringstream timeStream;
            timeStream << std::setw(2) << std::setfill('0') << minutes << ":"
                       << std::setw(2) << std::setfill('0') << seconds;
            _textFieldBlack->setString(timeStream.str());
        }
    }

    if (isCountdownActiveWhite) {
        elapsed = countdownClockWhite->getElapsedTime();
        if (elapsed.asSeconds() >= 1) {
            remainingTimeInSecondsWhite -= static_cast<int>(elapsed.asSeconds());
            countdownClockWhite->restart();

            int whichAnimation = remainingTimeInSecondsWhite % 4 + 1;
            if (whichAnimation == 1) {
                _clockIconBlack.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_BLACK_4"));
            } else if (whichAnimation == 2) {
                _clockIconBlack.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_BLACK_3"));
            } else if (whichAnimation == 3) {
                _clockIconBlack.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_BLACK_2"));
            } else if (whichAnimation == 4) {
                _clockIconBlack.setTexture(_data->assetManager.GetTexture("CLOCK_ICON_BLACK_1"));
            }


            if (remainingTimeInSecondsWhite <= 0) {
                isCountdownActiveWhite = false;
                remainingTimeInSecondsWhite = 0;
                std::cout << "Koniec odliczania dla białego!" << std::endl;
            }

            // Aktualizuj tekst wyświetlany w _textFieldWhite
            int minutes = remainingTimeInSecondsWhite / 60;
            int seconds = remainingTimeInSecondsWhite % 60;
            std::ostringstream timeStream;
            timeStream << std::setw(2) << std::setfill('0') << minutes << ":"
                       << std::setw(2) << std::setfill('0') << seconds;
            _textFieldWhite->setString(timeStream.str());
        }
    }
}

void ClockWidget::togglePlayerTime() {
    if (_whichClockPointed == BLACK) {
        isCountdownActiveBlack = false;
        isCountdownActiveWhite = true;

        countdownClockWhite->restart();

        _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_BLACK"));
        _textFieldBlack->setFillColor(sf::Color(200,200,200));
        _isClockWhiteIconVisible = false;


        if (remainingTimeInSecondsWhite <= 40) {
            _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_LOW_TIME"));
        } else {
            _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_POINTED"));
            _textFieldWhite->setFillColor(sf::Color(0,0,0));
            _isClockBlackIconVisible = true;
        }

        _whichClockPointed = WHITE;
    } 
    else if (_whichClockPointed == WHITE) {
        isCountdownActiveWhite = false;
        isCountdownActiveBlack = true;

        countdownClockBlack->restart();

        _backgroudn_to_textFieldWhite.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD"));
        _textFieldWhite->setFillColor(sf::Color( 80, 80, 80));
        _isClockBlackIconVisible = false;

        if (remainingTimeInSecondsBlack <= 40) {
            _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_LOW_TIME"));
        } else {
            _backgroudn_to_textFieldBlack.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD_BLACK_POINTED"));
            _textFieldBlack->setFillColor(sf::Color(255,255,255));
            _isClockWhiteIconVisible = true;
        }

        _whichClockPointed = BLACK;
    }
}


void ClockWidget::rotatePositionClocks(){
    //530, 10
    //480, 5

    //530, 668
    //480, 663

     if (whoOnTop) {
        _backgroudn_to_textFieldWhite.setPosition(480, 5);
        _backgroudn_to_textFieldBlack.setPosition(480, 663);

        _textFieldWhite->setPosition(530, 10);
        _textFieldBlack->setPosition(530, 668);

        _clockIconBlack.setPosition(460, 8);
        _clockIconWhite.setPosition(460, 666);

    } else {
        _backgroudn_to_textFieldWhite.setPosition(480, 663);
        _backgroudn_to_textFieldBlack.setPosition(480, 5);

        _textFieldWhite->setPosition(530, 668);
        _textFieldBlack->setPosition(530, 10);

        _clockIconBlack.setPosition(460, 666);
        _clockIconWhite.setPosition(460, 8);
    }
    whoOnTop = !whoOnTop;
}



void ClockWidget::Draw() {
    _data->window.draw(_startButton);
    _data->window.draw(_backgroudn_to_textFieldBlack);
    _data->window.draw(*_textFieldBlack);

    _data->window.draw(_backgroudn_to_textFieldWhite);
    _data->window.draw(*_textFieldWhite);

    if(_isClockWhiteIconVisible){
        _data->window.draw(_clockIconWhite);
    }

    if(_isClockBlackIconVisible){
        _data->window.draw(_clockIconBlack);
    }
}



