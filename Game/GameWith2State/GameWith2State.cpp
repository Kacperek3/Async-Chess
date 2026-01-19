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

    _clockWidget = new ClockWidget(_data);
    _capturedPieces = new CapturedPieces(_data);
    _decorations = new Decorations(_data);
    _pawnPromotion = new PawnPromotion(_data);
    _gameSounds = new GameSounds(_data);
    _gameOver = new GameOver(_data);

    _data->assetManager.LoadTexture("PLAYER_2_LOGO", "assets/ProfilePictures/Yeti_100x100.png");
    _data->assetManager.LoadTexture("PLAYER_1_LOGO", "assets/ProfilePictures/Gingerbread_100x100.png");
    _data->assetManager.LoadFont("Poppins", "assets/fonts/Poppins-Light.ttf");
    _font = _data->assetManager.GetFont("Poppins");

    _player2Logo.setTexture(_data->assetManager.GetTexture("PLAYER_2_LOGO"));
    _player2Logo.setPosition(658, 50);
    _player1Logo.setTexture(_data->assetManager.GetTexture("PLAYER_1_LOGO"));
    _player1Logo.setPosition(658,50);

    _player1LogoText = new sf::Text();
    _player1LogoText->setFont(_font);
    _player1LogoText->setCharacterSize(18);
    _player1LogoText->setFillColor(sf::Color(200,200,200));
    _player1LogoText->setPosition(654, 170);
    _player1LogoText->setOutlineColor(sf::Color::Black);
    _player1LogoText->setOutlineThickness(0);
    _player1LogoText->setString("Player 1 turn");
    _player1LogoText->setStyle(sf::Text::Bold);
    _player2LogoText = new sf::Text();
    _player2LogoText->setFont(_font);
    _player2LogoText->setCharacterSize(18);
    _player2LogoText->setFillColor(sf::Color(200,200,200));
    _player2LogoText->setPosition(654, 170);
    _player2LogoText->setOutlineColor(sf::Color::Black);
    _player2LogoText->setOutlineThickness(0);
    _player2LogoText->setString("Player 2 turn");
    _player2LogoText->setStyle(sf::Text::Bold);

    _clockWidget->Init();
    _capturedPieces->Init();
    _decorations->Init("Player 1", "Player 2");
    _pawnPromotion->Init();
    _gameSounds->Init();
    _gameOver->Init();
}

void GameWith2State::HandleInput() {
    sf::Event event;
    while (_data->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _data->window.close();
            return;
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                _data->stateManager.AddState(StateRef(new MenuState(_data)), true);
                return;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                startDragging(mousePos);


                if(_isGameOver){
                    bool userChoice = _gameOver->UserInput();
                    if(userChoice == newGame){
                        _data->stateManager.AddState(StateRef(new GameWith2State(_data)), true);
                    }
                    else if(userChoice == goToMainMenu){
                        _data->stateManager.AddState(StateRef(new MenuState(_data)), true);
                    }
                    
                }

                if(_board._dataAboutPawnPromotion._isPawnPromotion){
                    int choice = _pawnPromotion->ChoicePiece();
                    if(choice != 0){
                        if(choice == 1){
                            _board.addQueen(_board._dataAboutPawnPromotion._pawnColor, _board._dataAboutPawnPromotion._pawnX, _board._dataAboutPawnPromotion._pawnY);
                        }
                        else if(choice == 2){
                            _board.addRook(_board._dataAboutPawnPromotion._pawnColor, _board._dataAboutPawnPromotion._pawnX, _board._dataAboutPawnPromotion._pawnY);
                        }
                        else if (choice == 3){
                            _board.addBishop(_board._dataAboutPawnPromotion._pawnColor, _board._dataAboutPawnPromotion._pawnX, _board._dataAboutPawnPromotion._pawnY);
                        }
                        else if (choice == 4) {
                            _board.addKnight(_board._dataAboutPawnPromotion._pawnColor, _board._dataAboutPawnPromotion._pawnX, _board._dataAboutPawnPromotion._pawnY);
                        }
                        _board._dataAboutPawnPromotion._isPawnPromotion = false;
                        _board._dataAboutPawnPromotion._pawnColor = 2;
                        _board._dataAboutPawnPromotion._pawnX = -1;
                        _board._dataAboutPawnPromotion._pawnY = -1;

                        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
                        _board.rotatePieces();
                        _clockWidget->togglePlayerTime();
                        _clockWidget->rotatePositionClocks();
                        _capturedPieces->RotateCapturedPieces();
                        _decorations->rotatePositionDecorations();   
                    }
                }


                if (!clickedOnPiece(mousePos)) {
                    clickedOnField(mousePos);
                } 

                if(_clockWidget->StartButtonPressed()) continue;
                else if(_clockWidget->MenuButtonPressed()){
                    _data->stateManager.AddState(StateRef(new MenuState(_data)),false);
                }
                else if(_clockWidget->ResumeButtonPressed()) continue;
                else if(_clockWidget->PauseButtonPressed()) continue;
                else if(_clockWidget->NewGameButtonPressed()){
                    _data->stateManager.AddState(StateRef(new GameWith2State(_data)), true);
                }


                _isClockTimeSet = _clockWidget->getIsClockTimeSet();
                if(_isClockTimeSet && !_isGameStartSoundPlayed){
                    _gameSounds->PlayStartGameSound();
                    _isGameStartSoundPlayed = true;
                }

            } else if (event.mouseButton.button == sf::Mouse::Right) {
                selectedPiece = nullptr;

                if (isDragging) {
                    draggedPiece->simulateMove(draggedPiece->getBoardPosition().x, draggedPiece->getBoardPosition().y);
                    isDragging = false;
                    draggedPiece = nullptr;
                    dragOffset = sf::Vector2f(0, 0);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                stopDragging(mousePos);
            }
        }

        if (event.type == sf::Event::TextEntered) {
            _clockWidget->inputTime(event);
        }
    }
}


bool GameWith2State::clickedOnPiece(const sf::Vector2f& mousePosition) {
    for (auto& piece : _board.b_pieces) {
        if (_data->inputManager.IsSpriteClicked(piece->getSprite(), sf::Mouse::Left, _data->window) && piece->getColor() == currentPlayerTurn) {
            if (selectedPiece == nullptr) {
                selectedPiece = dynamic_cast<Piece*>(piece);
            } 
            else {
                if (selectedPiece == piece) {
                    selectedPiece = nullptr;
                } 
                else {
                    selectedPiece = dynamic_cast<Piece*>(piece);
                }
            }
            return true;
        }
    }
    return false;
}
void GameWith2State::clickedOnField(const sf::Vector2f& mousePosition) {
    if (selectedPiece != nullptr) {
        int snappedX = int(mousePosition.x / 75);
        int snappedY = int((mousePosition.y - 50) / 75);
        bool iscapture = false;
        if (selectedPiece->isValidMove(snappedX, snappedY) && !_board.isKingInCheckAfterMove(selectedPiece, Coordinate(snappedX, snappedY))) {
            if (_board.isEnemyPieceAt(snappedX, snappedY, selectedPiece->getColor())) {
                _board.removePiece(snappedX, snappedY, _capturedPieces);
                iscapture = true;
            }

            selectedPiece->move(snappedX, snappedY);
            if(!_board._dataAboutPawnPromotion._isPawnPromotion){
                currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
                _board.rotatePieces();
                _clockWidget->togglePlayerTime();
                _clockWidget->rotatePositionClocks();
                _capturedPieces->RotateCapturedPieces();
                _decorations->rotatePositionDecorations();   
            }
            if (_board.isCheckmate(currentPlayerTurn)) {
                _gameSounds->PlayEndGameSound();
                _isGameOver = true;
                _isMovingAllowed = false;
                int winner = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
                _gameOver->whoWins(winner, checkmate);
            }
            else if (_board.isStalemate(currentPlayerTurn)) {
                
            }
            else if(_board.isKingInCheck(currentPlayerTurn)){
                _gameSounds->PlayCheckSound();
            }
            else if(!iscapture){
                _gameSounds->PlayMoveSound();
            }
            else if(iscapture){
                _gameSounds->PlayCaptureSound();
            }

            selectedPiece = nullptr;
        } 
        else{
            selectedPiece = nullptr;
        }
    }
}

void GameWith2State::startDragging(const sf::Vector2f& mousePosition) {
    for (auto& piece : _board.b_pieces) {
        if (_data->inputManager.IsSpriteHover(piece->getSprite(), sf::Mouse::Left, _data->window) && piece->getColor() == currentPlayerTurn) {
            isDragging = true;
            draggedPiece = dynamic_cast<Piece*>(piece);
            //dragOffset = mousePosition - draggedPiece->getPosition();
            sf::Vector2f temp;
            temp.x = mousePosition.x - draggedPiece->getSprite().getGlobalBounds().width / 2;
            temp.y = mousePosition.y - draggedPiece->getSprite().getGlobalBounds().height / 2;
            draggedPiece->move(temp);

            draggedPiece->move(mousePosition);
            break;
        }
    }
}

void GameWith2State::stopDragging(sf::Vector2f& mousePosition) {
    isDragging = false;

    int snappedX = int(mousePosition.x / 75);
    int snappedY = int((mousePosition.y - 50) / 75);
    
    bool isCapture = false; 

    if (draggedPiece->isValidMove(snappedX, snappedY) && 
        !_board.isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY)) 
        && _isClockTimeSet
        && !_board._dataAboutPawnPromotion._isPawnPromotion) {

        if (_board.isEnemyPieceAt(snappedX, snappedY, draggedPiece->getColor())) {
            std::cout << "Zbito" << std::endl;
            _board.removePiece(snappedX, snappedY, _capturedPieces);
            isCapture = true; 
        }

        draggedPiece->move(snappedX, snappedY);
        _clockWidget->ResumeGame();
        if(!_board._dataAboutPawnPromotion._isPawnPromotion){
            currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
            _board.rotatePieces();
            _clockWidget->togglePlayerTime();
            _clockWidget->rotatePositionClocks();
            _capturedPieces->RotateCapturedPieces();
            _decorations->rotatePositionDecorations();   
        }        

        if (_board.isCheckmate(currentPlayerTurn)) {
            _gameSounds->PlayEndGameSound();
            _isGameOver = true;
            _isMovingAllowed = false;
            int winner = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
            _gameOver->whoWins(winner, checkmate);
        }
        else if (_board.isStalemate(currentPlayerTurn)) {
            std::cout << "Pat" << std::endl;
        }
        else if(_board.isKingInCheck(currentPlayerTurn)){
            _gameSounds->PlayCheckSound();
        }
        else if (!isCapture) {
            _gameSounds->PlayMoveSound();
        }
        else if(isCapture){
            _gameSounds->PlayCaptureSound();
        }
        selectedPiece = nullptr;
    } else {
        draggedPiece->simulateMove(draggedPiece->getBoardPosition().x, draggedPiece->getBoardPosition().y);
    }

    draggedPiece = nullptr;
    dragOffset = sf::Vector2f(0, 0);
}


void GameWith2State::Update() {
    for(auto& piece : _board.b_pieces){
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            std::cout << "Usunieto pionka" << std::endl;
            _board.removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y, nullptr);
        }
    }

    bool isWinnerbyTime = _clockWidget->Update();
    if(isWinnerbyTime){
        _isGameOver = true;
        _isMovingAllowed = false;
        int winner = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
        _gameOver->whoWins(winner, checkmate);
    }
    _gameOver->Update();
}

void GameWith2State::Draw() {
    sf::View view = sf::View(sf::FloatRect(0, 0, 800, 700));
    _data->window.setView(view);
    _data->window.clear(sf::Color( 58, 58, 58 ));
    
    _board.drawBoard(_data->window, false);

    if (isDragging && draggedPiece && _isMovingAllowed) {
        if(_isClockTimeSet){
            _board.showPossibleMoves(_data->window, draggedPiece);
            _board.showPossibleCaptures(_data->window, draggedPiece);
            _board.markPieceField(_data->window, draggedPiece);
        }
        sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);

        sf::Vector2f temp;
        temp.x = mousePos.x - draggedPiece->getSprite().getGlobalBounds().width / 2;
        temp.y = mousePos.y - draggedPiece->getSprite().getGlobalBounds().height / 2;
        draggedPiece->move(temp);
    }
    else if(selectedPiece && _isClockTimeSet && _isMovingAllowed){
        _board.showPossibleMoves(_data->window, selectedPiece);
        _board.showPossibleCaptures(_data->window, selectedPiece);
        _board.markPieceField(_data->window, selectedPiece);
    }

    

    _board.showCheck(_data->window, currentPlayerTurn);
    _board.drawPieces(_data->window, draggedPiece);
    _capturedPieces->Draw();
    _decorations->Draw();
    _clockWidget->Draw();

    if(currentPlayerTurn == WHITE){
        _data->window.draw(_player1Logo);
        _data->window.draw(*_player1LogoText);
    } else{
        _data->window.draw(_player2Logo);
        _data->window.draw(*_player2LogoText);
    }
    
    if(_board._dataAboutPawnPromotion._isPawnPromotion){
        _pawnPromotion->ChangePosition(_board._dataAboutPawnPromotion._pawnX, 50, _board._dataAboutPawnPromotion._pawnColor);
        _pawnPromotion->Draw();
    }



    if(_isGameOver) _gameOver->Draw();

    _data->window.display();
}



void GameWith2State::ClearObjects() {
    _board.deleteObjects();
    delete _player1LogoText;
    delete _player2LogoText;
    delete _decorations;
    delete _clockWidget;
    delete _capturedPieces;
    delete _pawnPromotion;
    delete _gameSounds;
    _data->soundManager.ClearSounds();
    _data->assetManager.clearAssets();
}