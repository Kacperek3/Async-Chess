#include "GameStateManager.h"
#include "MenuState.h"
#include "GameWith2State.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
    GameStateManager gsm;
    
    window.setFramerateLimit(150);
    
    gsm.pushState(new MenuState(&gsm, &window));

    while (window.isOpen()) {
        gsm.handleInput(); 
        gsm.closePoppedStates();
        gsm.update();       
        gsm.render();
        window.display();
    }
    gsm.closeAllStates();
    return 0;
}

// todo bicie w przelocie
// todo dodanie dzwieku do gry
// todo zapisywanie stanu gry
// todo wczytywanie stanu gry
// todo zegar
// todo zapisywanie ruchow
// todo zrobienie menu
// todo zrobienie AI
// todo zrobienie sieciowego trybu gry
// todo hall of fame
