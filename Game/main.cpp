#include "Game.h"
#include "GameStateManager.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
    GameStateManager gsm;

    
    gsm.pushState(new Game(&gsm, &window));

    while (window.isOpen()) {
        gsm.handleInput(); 
        gsm.update();       
        window.clear();
        gsm.render();
        window.display();
    }

    return 0;
}

// todo roszada
// todo pat
// todo bicie w przelocie



// todo zapisywanie stanu gry
// todo wczytywanie stanu gry
// todo zegar
// todo zapisywanie ruchow
// todo zrobienie menu
// todo zrobienie AI
// todo zrobienie sieciowego trybu gry
// todo hall of fame
