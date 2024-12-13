#include "GameWith2State.h"
#include "MenuState.h"

GameWith2State::GameWith2State(GameDataRef data): _data(data), _board(&data->window) {
    
}

void GameWith2State::Init(){
    currentPlayerTurn = WHITE;
    showCoordinates = false;
    isDragging = false;
    draggedPiece = nullptr;
    dragOffset = sf::Vector2f(0, 0);
}

GameWith2State::~GameWith2State() {
    
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
                if (event.key.code == sf::Keyboard::R) {
                    toggleCoordinates();
                }
                else if (event.key.code == sf::Keyboard::L) {
                    _data->stateManager.RemoveState();
                }
                else if(event.key.code == sf::Keyboard::Escape){
                    std::cout << "Escape" << std::endl;
                    _data->stateManager.AddState(StateRef(new MenuState(_data)), false);
                    return;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                    startDragging(mousePos);
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                    sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
                    stopDragging(mousePos);
                }
                break;

            default:    
                break;
        }
    }
}

void GameWith2State::startDragging(const sf::Vector2f& mousePosition) {
    for (auto& piece : _board.b_pieces) {
        if (piece->getSprite().getGlobalBounds().contains(mousePosition) && piece->getColor() == currentPlayerTurn) {
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

void GameWith2State::toggleCoordinates() {
    showCoordinates = !showCoordinates;
}

void GameWith2State::Update() {
    // Usuwanie pionka, który doszedł do końca planszy
    for(auto& piece : _board.b_pieces){
        if(piece->getPosition().x == -1 && piece->getPosition().y == -1){
            _board.removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y);
        }
    }
}

void GameWith2State::Draw() {

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    _data->window.setView(view);
    _data->window.clear(sf::Color(40, 20, 2));

    _board.drawBoard(_data->window, showCoordinates);
    
    if (isDragging && draggedPiece) {
        _board.showPossibleMoves(_data->window, draggedPiece);
        _board.showPossibleCaptures(_data->window, draggedPiece);
        _board.markPieceField(_data->window, draggedPiece);

        sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
        draggedPiece->move(mousePos - dragOffset);
    }
    _board.showCheck(_data->window, currentPlayerTurn);
    _board.drawPieces(_data->window, draggedPiece);
    
    _data->window.display();
}
