#include "Game.h"

Game::Game() 
: window(sf::VideoMode(800, 800), "Szachy"), board(), currentPlayerTurn(WHITE) {
    // Inicjalizacja innych elementów, jeśli potrzeba
}

void Game::run() {
    // Główna pętla gry
    while (window.isOpen()) {
        processEvents();  // Przetwarzanie zdarzeń
        update();         // Aktualizacja stanu gry
        render();         // Renderowanie gry
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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
                    
                    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    // Zaokrąglenie pozycji myszy do najbliższego pola
                    float snappedX = std::round(mousePosition.x / 82);
                    float snappedY = std::round(mousePosition.y / 89);
                    

                    if (draggedPiece->isValidMove(snappedX, snappedY)) {
                        draggedPiece->move(snappedX, snappedY);
                        currentPlayerTurn = (currentPlayerTurn == WHITE) ? BLACK : WHITE;
                        
                        for(Coordinate c : draggedPiece->getPossibleMoves()){
                            std::cout <<c.x << " " << c.y << std::endl;
                        }
                        std::cout << std::endl;
                        // zmien turę po dozwolonym ruchu
                        
                    }
                    else{
                        draggedPiece->move(draggedPiece->getBoardPosition().x, draggedPiece->getBoardPosition().y);
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
    
    // Logika gry np. sprawdzanie ruchów, zmiana tur, AI itp.
    if (isDragging && draggedPiece != nullptr) {
        // Pobieranie aktualnej pozycji myszy
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Aktualizacja pozycji pionka
        draggedPiece->move(mousePosition - dragOffset);
    }
    
}

void Game::render() {
    window.clear();

    // Rysowanie planszy i elementów gry
    board.draw(window);

    window.display();
}
