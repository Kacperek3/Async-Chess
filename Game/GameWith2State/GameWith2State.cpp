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
    _clockWidget->Init();
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
                    startDragging(mousePos);


                    _clockWidget->StartButtonPressed();
                    _isClockTimeSet = _clockWidget->getIsClockTimeSet();
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
               _clockWidget->inputTime(event);
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
        !_board.isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY)) && _isClockTimeSet) {
        if (_board.isEnemyPieceAt(snappedX, snappedY, draggedPiece->getColor())) {
            std::cout << "Zbito" << std::endl;
            _board.removePiece(snappedX, snappedY, _capturedPieces);
        }

        draggedPiece->move(snappedX, snappedY);
        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
        _board.rotatePieces();
        _clockWidget->togglePlayerTime();
        _clockWidget->rotatePositionClocks();

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
            _board.removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y, nullptr);
        }
    }

    _clockWidget->Update();
}

void GameWith2State::Draw() {

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    _data->window.setView(view);
    _data->window.clear(sf::Color( 58, 58, 58 ));
    
    _board.drawBoard(_data->window, false);

    if (isDragging && draggedPiece) {
        if(_isClockTimeSet){
            _board.showPossibleMoves(_data->window, draggedPiece);
            _board.showPossibleCaptures(_data->window, draggedPiece);
            _board.markPieceField(_data->window, draggedPiece);
        }
        sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
        draggedPiece->move(mousePos - dragOffset);
    }
    _board.showCheck(_data->window, currentPlayerTurn);
    _board.drawPieces(_data->window, draggedPiece);
    _capturedPieces->Draw();
   
    _clockWidget->Draw();

    _data->window.display();
}



void GameWith2State::ClearObjects() {
    _board.deleteObjects();
    delete _clockWidget;
    delete _capturedPieces;
    _data->assetManager.clearAssets();
    
    //delete _backgroud_to_textField1;
}
