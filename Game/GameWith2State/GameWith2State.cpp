#include "GameWith2State.h"
#include "MenuState.h"

GameWith2State::GameWith2State(GameDataRef data): _data(data), _board(data) {
}



void GameWith2State::Init(){
    currentPlayerTurn = WHITE;
    isDragging = false;
    draggedPiece = nullptr;
    dragOffset = sf::Vector2f(0, 0);

    _board.Init();

    //rysowanie pozostałych elementów

    _data->assetManager.LoadTexture("START_BUTTON", "../assets/GameWithFriendState/Buttons/StartButton.png");
    _data->assetManager.LoadTexture("START_BUTTON_HOVER", "../assets/GameWithFriendState/Buttons/StartButton_hover.png");
    _startButton.setTexture(_data->assetManager.GetTexture("START_BUTTON"));
    _startButton.setPosition(610, 271);
    _startButton.setScale(0.90, 0.90);

    _data->assetManager.LoadFont("Poppins", "../assets/fonts/Poppins-Thin.ttf");
    _font = _data->assetManager.GetFont("Poppins");

    _textField = std::make_unique<sf::Text>();
    _textField->setFont(_font);
    _textField->setCharacterSize(24);
    _textField->setFillColor(sf::Color::Black);
    _textField->setPosition(625, 30);
    _textField->setOutlineColor(sf::Color::White);
    _textField->setOutlineThickness(1);
    _textField->setString("00:00");
    _textField->setStyle(sf::Text::Bold);




    _data->assetManager.LoadTexture("BACKGROUND_TO_TEXTFIELD", "../assets/TextClockBackground.png");
    _backgroudn_to_textField1.setTexture(_data->assetManager.GetTexture("BACKGROUND_TO_TEXTFIELD"));
    _backgroudn_to_textField1.setPosition(620, 25);

    // _backgroud_to_textField1 = new sf::RectangleShape(sf::Vector2f(150, 50));
    // _backgroud_to_textField1->setFillColor(sf::Color::White);
    // _backgroud_to_textField1->setOutlineColor(sf::Color::Black);
    // _backgroud_to_textField1->setOutlineThickness(2);
    // _backgroud_to_textField1->setPosition(620, 25);
}

void GameWith2State::HandleInput() {
    sf::Event event;
    while (_data->window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                _data->window.close();
                return;
                break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Escape){
                    //powrot do menu
                    std::cout << "Escape" << std::endl;
                    _data->stateManager.AddState(StateRef(new MenuState(_data)), true);
                    return;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //rozpoczecie przesuwania pionka
                    sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);


                    if (_data->inputManager.IsSpriteHover(_startButton, sf::Mouse::Left, _data->window)) {
                        // Pobierz czas z _textField i aktywuj odliczanie
                        try {
                            int minutes = std::stoi(inputText.substr(0, 2));
                            int seconds = std::stoi(inputText.substr(3, 2));
                            remainingTimeInSeconds = minutes * 60 + seconds;
                            countdownClock.restart();
                            isCountdownActive = true;
                        } catch (...) {
                            std::cout << "Nieprawidłowy format czasu" << std::endl;
                        }
                    }



                    startDragging(mousePos);
                }
                else if(event.mouseButton.button == sf::Mouse::Right and isDragging){
                    //przywracanie pionka na miejsce po kliknieciu prawym przyciskiem myszy
                    draggedPiece->simulateMove(draggedPiece->getBoardPosition().x, draggedPiece->getBoardPosition().y);
                    isDragging = false;
                    draggedPiece = nullptr;
                    dragOffset = sf::Vector2f(0, 0);
                }
                break;

            case sf::Event::MouseButtonReleased:
                //koniec przesuwania pionka
                if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                    sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                    stopDragging(mousePos);
                }
                break;

           case sf::Event::TextEntered:
               if (std::isdigit(event.text.unicode)) {
                   // Add digit to input
                   inputText += static_cast<char>(event.text.unicode);
               }
               else if (event.text.unicode == 58) { // Colon
                   // Add colon to input
                   inputText += ":";
                }
                else if (event.text.unicode == 8 && !inputText.empty()) { // Backspace
                   // Remove last character
                   inputText.pop_back();
               }
                _textField->setString(inputText);
               break;
           

            default:    
                break;
        }
    }
}

void GameWith2State::startDragging(const sf::Vector2f& mousePosition) {
    for (auto& piece : _board.b_pieces) {
        if (_data->inputManager.IsSpriteHoverAccurate(piece->getSprite(), sf::Mouse::Left, _data->window) && piece->getColor() == currentPlayerTurn) {
            isDragging = true;
            draggedPiece = dynamic_cast<Piece*>(piece);
            dragOffset = mousePosition - draggedPiece->getPosition();
            break;
        }
    }
}

void GameWith2State::stopDragging(sf::Vector2f& mousePosition) {
    isDragging = false;

    float snappedX = int(mousePosition.x / 75);
    float snappedY = int(mousePosition.y / 75);
    
    if (draggedPiece->isValidMove(snappedX, snappedY) && 
        !_board.isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY))) {
        if (_board.isEnemyPieceAt(snappedX, snappedY, draggedPiece->getColor())) {
            std::cout << "Zbito" << std::endl;
            _board.removePiece(snappedX, snappedY);
        }

        draggedPiece->move(snappedX, snappedY);
        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
        _board.rotatePieces();

        if (_board.isCheckmate(currentPlayerTurn)) {
            std::cout << "Szach" << std::endl;
        }
        if (_board.isStalemate(currentPlayerTurn)) {
            std::cout << "Pat" << std::endl;
        }
        
    } else {
        draggedPiece->simulateMove(draggedPiece->getBoardPosition().x, draggedPiece->getBoardPosition().y);
    }

    draggedPiece = nullptr;
    dragOffset = sf::Vector2f(0, 0);
}


void GameWith2State::Update() {
    // Usuwanie pionka, który doszedł do końca planszy
    for(auto& piece : _board.b_pieces){
        if(piece->getPosition().x == -1 && piece->getPosition().y == -1){
            _board.removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y);
        }
    }

    if(_data->inputManager.IsSpriteHover(_startButton, sf::Mouse::Left, _data->window)){
        _startButton.setTexture(_data->assetManager.GetTexture("START_BUTTON_HOVER"));
    } else {
        _startButton.setTexture(_data->assetManager.GetTexture("START_BUTTON"));
    }



    if (isCountdownActive) {
        sf::Time elapsed = countdownClock.getElapsedTime();
        if (elapsed.asSeconds() >= 1) {
            remainingTimeInSeconds -= static_cast<int>(elapsed.asSeconds());
            countdownClock.restart();

            if (remainingTimeInSeconds <= 0) {
                isCountdownActive = false;
                remainingTimeInSeconds = 0;
                std::cout << "Koniec odliczania!" << std::endl;
            }

            // Aktualizuj tekst wyświetlany w _textField
            int minutes = remainingTimeInSeconds / 60;
            int seconds = remainingTimeInSeconds % 60;
            std::ostringstream timeStream;
            timeStream << std::setw(2) << std::setfill('0') << minutes << ":"
                    << std::setw(2) << std::setfill('0') << seconds;
            _textField->setString(timeStream.str());
        }
    }
}

void GameWith2State::Draw() {

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    _data->window.setView(view);
    _data->window.clear(sf::Color(40, 20, 2));
    
    _board.drawBoard(_data->window, false);

    if (isDragging && draggedPiece) {
        _board.showPossibleMoves(_data->window, draggedPiece);
        _board.showPossibleCaptures(_data->window, draggedPiece);
        _board.markPieceField(_data->window, draggedPiece);

        sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
        draggedPiece->move(mousePos - dragOffset);
    }
    _board.showCheck(_data->window, currentPlayerTurn);
    _board.drawPieces(_data->window, draggedPiece);

   // _data->window.draw(sidePanel);
    _data->window.draw(_startButton);
    //_data->window.draw(*_backgroud_to_textField1);
    _data->window.draw(_backgroudn_to_textField1);
    _data->window.draw(*_textField);
   
    _data->window.display();
}



void GameWith2State::ClearObjects() {
    _board.deleteObjects();
    _data->assetManager.clearAssets();
    //delete _backgroud_to_textField1;
}
