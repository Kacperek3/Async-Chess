#include "GameWith2State.h"

GameWith2State::GameWith2State(GameStateManager* gsm, sf::RenderWindow* window)
    : gsm(gsm), window(window), board(window), currentPlayerTurn(WHITE) {
    window->setFramerateLimit(200);
}

void GameWith2State::handleInput() {
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    toggleCoordinates();
                }
                else if(event.key.code == sf::Keyboard::Escape){
                    gsm->popState();
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                    startDragging(mousePosition);
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                    sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                    stopDragging(mousePosition);
                }
                break;

            default:
                break;
        }
    }
}

void GameWith2State::startDragging(const sf::Vector2f& mousePosition) {
    for (auto& piece : board.b_pieces) {
        if (piece->getSprite().getGlobalBounds().contains(mousePosition) && piece->getColor() == currentPlayerTurn) {
            isDragging = true;
            draggedPiece = dynamic_cast<Piece*>(piece);
            dragOffset = mousePosition - draggedPiece->getPosition();
            break;
        }
    }
}

void GameWith2State::stopDragging(const sf::Vector2f& mousePosition) {
    isDragging = false;

    float snappedX = int(mousePosition.x / 75);
    float snappedY = int(mousePosition.y / 75);
    
    if (draggedPiece->isValidMove(snappedX, snappedY) && 
        !board.isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY))) {

        if (board.isEnemyPieceAt(snappedX, snappedY, draggedPiece->getColor())) {
            std::cout << "Zbito" << std::endl;
            board.removePiece(snappedX, snappedY);
        }

        draggedPiece->move(snappedX, snappedY);
        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;

        if (board.isCheckmate(currentPlayerTurn)) {
            std::cout << "Szach" << std::endl;
        }
        if (board.isStalemate(currentPlayerTurn)) {
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

void GameWith2State::update() {
    // Usuwanie pionka, który doszedł do końca planszy
    for(auto& piece : board.b_pieces){
        if(piece->getPosition().x == -1 && piece->getPosition().y == -1){
            board.removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y);
        }
    }
}

void GameWith2State::render() {
    window->clear();

    board.drawBoard(*window, showCoordinates);

    if (isDragging && draggedPiece) {
        board.showPossibleMoves(*window, draggedPiece);
        board.showPossibleCaptures(*window, draggedPiece);
        board.markPieceField(*window, draggedPiece);

        sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        draggedPiece->move(mousePosition - dragOffset);
    }
    board.showCheck(*window, currentPlayerTurn);
    board.drawPieces(*window, draggedPiece);
    window->display();
}