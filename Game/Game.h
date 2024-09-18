#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <math.h>
#include <iostream>

class Game {
public:
    Game();  // Konstruktor
    void run();  // Funkcja do uruchomienia głównej pętli gry

private:
    void processEvents();  // Obsługa zdarzeń
    void update();         // Aktualizacja stanu gry
    void render();         // Renderowanie gry

    sf::RenderWindow window;  // Główne okno SFML
    Board board;              // Obiekt planszy szachowej

    int currentPlayerTurn;  // Aktualny gracz
    sf::Vector2f dragOffset;  // Różnica między pozycją myszy a pionka, gdy zaczyna się przeciąganie
    bool isDragging = false;   // Flaga mówiąca, czy aktualnie przeciągasz pionek
    Piece* draggedPiece = nullptr; // Wskaźnik na pionek, który jest aktualnie przeciągany
};

