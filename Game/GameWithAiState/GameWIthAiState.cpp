#include "GameWithAiState.h"
#include "MenuState.h"

GameWithAiState::GameWithAiState(GameDataRef data): _data(data){
}



void GameWithAiState::Init(){
    currentPlayerTurn = WHITE;
    isDragging = false;
    draggedPiece = nullptr;
    dragOffset = sf::Vector2f(0, 0);

    
    _aiLogic = new AiLogic(_data);


    _capturedPieces = new CapturedPieces(_data);
    _decorations = new Decorations(_data);
    _pawnPromotion = new PawnPromotion(_data);
    _gameSounds = new GameSounds(_data);
    _gameOver = new GameOver(_data);
    _capturedPieces->Init();
    _decorations->Init();
    _pawnPromotion->Init();
    _gameSounds->Init();
    _gameOver->Init();
}

void GameWithAiState::HandleInput() {
    sf::Event event;
    while (_data->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _data->window.close();
            return;
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // Powrót do menu
                std::cout << "Escape" << std::endl;
                _data->stateManager.AddState(StateRef(new MenuState(_data)), true);
                return;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Rozpoczęcie przesuwania pionka
                sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                startDragging(mousePos);


                if(_isGameOver){
                    bool userChoice = _gameOver->UserInput();
                    if(userChoice == newGame){
                        _data->stateManager.AddState(StateRef(new GameWithAiState(_data)), true);
                    }
                    else if(userChoice == goToMainMenu){
                        _data->stateManager.AddState(StateRef(new MenuState(_data)), true);
                    }
                    
                }

                if(_aiLogic->_board->_dataAboutPawnPromotion._isPawnPromotion){
                    int choice = _pawnPromotion->ChoicePiece();
                    if(choice != 0){
                        if(choice == 1){
                            _aiLogic->_board->addQueen(_aiLogic->_board->_dataAboutPawnPromotion._pawnColor, _aiLogic->_board->_dataAboutPawnPromotion._pawnX, _aiLogic->_board->_dataAboutPawnPromotion._pawnY);
                        }
                        else if(choice == 2){
                            _aiLogic->_board->addRook(_aiLogic->_board->_dataAboutPawnPromotion._pawnColor, _aiLogic->_board->_dataAboutPawnPromotion._pawnX, _aiLogic->_board->_dataAboutPawnPromotion._pawnY);
                        }
                        else if (choice == 3){
                            _aiLogic->_board->addBishop(_aiLogic->_board->_dataAboutPawnPromotion._pawnColor, _aiLogic->_board->_dataAboutPawnPromotion._pawnX, _aiLogic->_board->_dataAboutPawnPromotion._pawnY);
                        }
                        else if (choice == 4) {
                            _aiLogic->_board->addKnight(_aiLogic->_board->_dataAboutPawnPromotion._pawnColor, _aiLogic->_board->_dataAboutPawnPromotion._pawnX, _aiLogic->_board->_dataAboutPawnPromotion._pawnY);
                        }
                        _aiLogic->_board->_dataAboutPawnPromotion._isPawnPromotion = false;
                        _aiLogic->_board->_dataAboutPawnPromotion._pawnColor = 2;
                        _aiLogic->_board->_dataAboutPawnPromotion._pawnX = -1;
                        _aiLogic->_board->_dataAboutPawnPromotion._pawnY = -1;

                        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
                    }
                }


                if (!clickedOnPiece(mousePos)) {
                    clickedOnField(mousePos);
                } // Jeśli nie kliknięto na poprawne pole, to clickedOnField zlikwiduje zaznaczenie na pionku

                if(!_isGameStartSoundPlayed){
                    _gameSounds->PlayStartGameSound();
                    _isGameStartSoundPlayed = true;
                }

            } else if (event.mouseButton.button == sf::Mouse::Right) {
                // Przywracanie pionka na miejsce po kliknięciu prawym przyciskiem myszy
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
            // Koniec przesuwania pionka
            if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                stopDragging(mousePos);
            }
        }
    }
}


bool GameWithAiState::clickedOnPiece(const sf::Vector2f& mousePosition) {
    for (auto& piece : _aiLogic->_board->b_pieces) {
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
void GameWithAiState::clickedOnField(const sf::Vector2f& mousePosition) {
    if (selectedPiece != nullptr) {
        int snappedX = int(mousePosition.x / 75);
        int snappedY = int((mousePosition.y - 50) / 75);
        bool iscapture = false;
        if (selectedPiece->isValidMove(snappedX, snappedY) && !_aiLogic->_board->isKingInCheckAfterMove(selectedPiece, Coordinate(snappedX, snappedY))) {
            if (_aiLogic->_board->isEnemyPieceAt(snappedX, snappedY, selectedPiece->getColor())) {
                _aiLogic->_board->removePiece(snappedX, snappedY, _capturedPieces);
                iscapture = true;
            }

            selectedPiece->move(snappedX, snappedY);
            if(!_aiLogic->_board->_dataAboutPawnPromotion._isPawnPromotion){
                currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
            }
            if (_aiLogic->_board->isCheckmate(currentPlayerTurn)) {
                _gameSounds->PlayEndGameSound();
                _isGameOver = true;
                _isMovingAllowed = false;
                _gameOver->whoWins(currentPlayerTurn, checkmate);
            }
            else if (_aiLogic->_board->isStalemate(currentPlayerTurn)) {
                
            }
            else if(_aiLogic->_board->isKingInCheck(currentPlayerTurn)){
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

void GameWithAiState::startDragging(const sf::Vector2f& mousePosition) {
    for (auto& piece : _aiLogic->_board->b_pieces) {
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

void GameWithAiState::stopDragging(sf::Vector2f& mousePosition) {
    isDragging = false;

    int snappedX = int(mousePosition.x / 75);
    int snappedY = int((mousePosition.y - 50) / 75);
    
    bool isCapture = false; // Flaga do śledzenia, czy pionek został zbity

    if (draggedPiece->isValidMove(snappedX, snappedY) && 
        !_aiLogic->_board->isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY)) 
        && !_aiLogic->_board->_dataAboutPawnPromotion._isPawnPromotion) {

        if (_aiLogic->_board->isEnemyPieceAt(snappedX, snappedY, draggedPiece->getColor())) {
            std::cout << "Zbito" << std::endl;
            _aiLogic->_board->removePiece(snappedX, snappedY, _capturedPieces);
            isCapture = true; // Ustawienie flagi na true, gdy pionek został zbity
        }

        draggedPiece->move(snappedX, snappedY);
        if(!_aiLogic->_board->_dataAboutPawnPromotion._isPawnPromotion){
            currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
        }        

        if (_aiLogic->_board->isCheckmate(currentPlayerTurn)) {
            _gameSounds->PlayEndGameSound();
            _isGameOver = true;
            _isMovingAllowed = false;
            _gameOver->whoWins(currentPlayerTurn, checkmate);
        }
        else if (_aiLogic->_board->isStalemate(currentPlayerTurn)) {
            std::cout << "Pat" << std::endl;
        }
        else if(_aiLogic->_board->isKingInCheck(currentPlayerTurn)){
            _gameSounds->PlayCheckSound();
        }
        else if (!isCapture) { // Odtwórz dźwięk ruchu tylko wtedy, gdy pionek nie został zbity
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


void GameWithAiState::Update() {
    // Usuwanie pionka, który doszedł do końca planszy
    for(auto& piece : _aiLogic->_board->b_pieces){
        if(piece->getBoardPosition().x == -1 && piece->getBoardPosition().y == -1){
            std::cout << "Usunieto pionka" << std::endl;
            _aiLogic->_board->removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y, nullptr);
        }
    }

    if(currentPlayerTurn == BLACK){
        _aiLogic->aiMove(BLACK);
        currentPlayerTurn = WHITE;
    }
   

    _gameOver->Update();
}

void GameWithAiState::Draw() {
    sf::View view = sf::View(sf::FloatRect(0, 0, 800, 700));
    _data->window.setView(view);
    _data->window.clear(sf::Color( 58, 58, 58 ));
    
    _aiLogic->_board->drawBoard(_data->window, false);

    if (isDragging && draggedPiece && _isMovingAllowed) {
        _aiLogic->_board->showPossibleMoves(_data->window, draggedPiece);
        _aiLogic->_board->showPossibleCaptures(_data->window, draggedPiece);
        _aiLogic->_board->markPieceField(_data->window, draggedPiece);
        sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);

        sf::Vector2f temp;
        temp.x = mousePos.x - draggedPiece->getSprite().getGlobalBounds().width / 2;
        temp.y = mousePos.y - draggedPiece->getSprite().getGlobalBounds().height / 2;
        draggedPiece->move(temp);
    }
    else if(selectedPiece && _isMovingAllowed){
        _aiLogic->_board->showPossibleMoves(_data->window, selectedPiece);
        _aiLogic->_board->showPossibleCaptures(_data->window, selectedPiece);
        _aiLogic->_board->markPieceField(_data->window, selectedPiece);
    }

    

    _aiLogic->_board->showCheck(_data->window, currentPlayerTurn);
    _aiLogic->_board->drawPieces(_data->window, draggedPiece);
    _capturedPieces->Draw();
    _decorations->Draw();
    
    if(_aiLogic->_board->_dataAboutPawnPromotion._isPawnPromotion){
        _pawnPromotion->ChangePosition(_aiLogic->_board->_dataAboutPawnPromotion._pawnX, 50, _aiLogic->_board->_dataAboutPawnPromotion._pawnColor);
        _pawnPromotion->Draw();
    }

    if(_isGameOver) _gameOver->Draw();

    _data->window.display();
}



void GameWithAiState::ClearObjects() {
    _aiLogic->_board->deleteObjects();
    delete _decorations;
    delete _capturedPieces;
    delete _pawnPromotion;
    delete _gameSounds;
    delete _gameOver;
    delete _aiLogic;
    _data->soundManager.ClearSounds();
    _data->assetManager.clearAssets();
}