#include "Game.h"

Game::Game(GameStateManager* gsm, sf::RenderWindow* window)
: gsm(gsm), window(window), board(window),currentPlayerTurn(WHITE) {
    // Inicjalizacja innych elementów, jeśli potrzeba
}

void Game::handleInput() {
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    // Przełącz widoczność koordynatów po naciśnięciu 'R'
                    showCoordinates = !showCoordinates;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

                    for (auto& piece : board.b_pieces) {
                        if (piece->getSprite().getGlobalBounds().contains(mousePosition) && piece->getColor() == currentPlayerTurn) {
                            isDragging = true;
                            draggedPiece = dynamic_cast<Piece*>(piece);
                            dragOffset = mousePosition - draggedPiece->getPosition();
                            break;
                        }
                    }
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                    isDragging = false;
                    
                    sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                    

                    // Zaokrąglenie pozycji myszy do najbliższego pola
                    float snappedX = int(mousePosition.x / 75);
                    float snappedY = int(mousePosition.y / 75);
                    
                    std::cout << board.isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY)) << std::endl;


                    if (draggedPiece->isValidMove(snappedX, snappedY) && !board.isKingInCheckAfterMove(draggedPiece, Coordinate(snappedX, snappedY))) {
                        if(board.isEnemyPieceAt(snappedX, snappedY, draggedPiece->getColor())){
                            std::cout << "Zbito" << std::endl;
                            board.removePiece(snappedX, snappedY);
                        }

                        draggedPiece->move(snappedX, snappedY);
                        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
                        

                        
                        if(board.isCheckmate(currentPlayerTurn)){
                            std::cout << "Szach" << std::endl;
                        }
                        if(board.isStalemate(currentPlayerTurn)){
                            std::cout << "Pat" << std::endl;
                        }
                    }
                    else{
                        draggedPiece->simulateMove(draggedPiece->getBoardPosition().x, draggedPiece->getBoardPosition().y);
                    }


                    draggedPiece = nullptr;
                    dragOffset = sf::Vector2f(0, 0);
                }
                break;

            default:
                break;
            
            
        }
    }
}

void Game::update() {

    // usuwanie pionka, który doszedł do końca planszy
    for(auto& piece : board.b_pieces){
        if(piece->getPosition().x == -1 && piece->getPosition().y == -1){
            board.removePiece(piece->getBoardPosition().x, piece->getBoardPosition().y);
        }
    }
    
}

void Game::render() {
    window->clear();

    // Rysowanie planszy i elementów gry
    board.drawBoard(*window, showCoordinates);
    
    if(isDragging && draggedPiece != nullptr){
        board.showPossibleMoves(*window, draggedPiece);
        board.showPossibleCaptures(*window, draggedPiece);
        board.markPieceField(*window, draggedPiece);



        // przesuwanie pionka, który jest aktualnie przeciągany, musi być rysowany na wierzchu dlatego jest tutaj
        sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        draggedPiece->move(mousePosition - dragOffset);
    }
    board.drawPieces(*window, draggedPiece);
    window->display();
}


