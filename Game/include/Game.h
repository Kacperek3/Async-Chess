#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <math.h>
#include <iostream>
#include "GameState.h"
#include "GameStateManager.h"

class Game : public GameState {
public:
    Game(GameStateManager* gsm, sf::RenderWindow* window);  // Konstruktor

private:
    void handleInput() override;  // Obsługa zdarzeń
    void update() override;         // Aktualizacja stanu gry
    void render() override;         // Renderowanie gry


    GameStateManager *gsm;
    sf::RenderWindow *window;  // Główne okno SFML


    Board board;              // Obiekt planszy szachowej

    int currentPlayerTurn;  // Aktualny gracz
    sf::Vector2f dragOffset;  // Różnica między pozycją myszy a pionka, gdy zaczyna się przeciąganie
    bool isDragging = false;   // Flaga mówiąca, czy aktualnie przeciągasz pionek
    Piece* draggedPiece = nullptr; // Wskaźnik na pionek, który jest aktualnie przeciągany
    bool showCoordinates = false;  //  Zmienna kontrolująca widoczność koordynatów
};
