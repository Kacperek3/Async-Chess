#include "MenuState.h"


MenuState::MenuState(GameStateManager* gsm, sf::RenderWindow* window) : gsm(gsm), window(window) {
    if (!font.loadFromFile("../assets/fonts/Poppins-Thin.ttf")) {
        std::cerr << "loading font\n";
    }

    if (!backgroundTexture.loadFromFile("../assets/Designer.png")) {
        std::cerr << "Error loading background image\n";
    }


    // Przypisz teksturę do sprite'a
    backgroundSprite.setTexture(backgroundTexture);

    setupText(playWithAiText, playWithAiRect, "Play with AI", 100);
    setupText(playWith2Text, playWith2Rect, "Play with 2 Players", 250);
    setupText(playByLanText, playByLanRect, "Play by LAN", 400);
    setupText(exitText, exitRect, "Exit", 550);
}

// Funkcja pomocnicza do ustawienia tekstu
void MenuState::setupText(sf::Text& text, sf::RectangleShape& rect, const std::string& str, float yPos) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Black);
    text.setPosition(100, yPos);

    rect.setSize(sf::Vector2f(text.getGlobalBounds().width + 20, text.getGlobalBounds().height + 40));
    rect.setPosition(text.getPosition().x - 10, text.getPosition().y - 10);
    rect.setFillColor(normalColor);
    rect.setOutlineColor(normalColor);
    rect.setOutlineThickness(2);       
}

// Sprawdzanie najechania na opcję i zmiana koloru tekstu
void MenuState::checkMouseHover(const sf::Vector2f& mousePos, sf::Text& text, sf::RectangleShape& rect) {
    if (text.getGlobalBounds().contains(mousePos)) {
        text.setFillColor(hoverColor);
        text.setFillColor(sf::Color::Black);
    }
}

void MenuState::handleInput() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

            // Sprawdzanie, czy kliknięto w opcję "Play with AI"
            if (playWithAiText.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play with AI selected\n";
            }

            // Sprawdzanie, czy kliknięto w opcję "Play with 2 Players"
            if (playWith2Text.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play with 2 Players selected\n";
                window->setSize(sf::Vector2u(800, 600));
                gsm->pushState(new GameWith2State(gsm, window));
            }

            // Sprawdzanie, czy kliknięto w opcję "Play by LAN"
            if (playByLanText.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play by LAN selected\n";
            }

            // Sprawdzanie, czy kliknięto w opcję "Exit"
            if (exitText.getGlobalBounds().contains(mousePos)) {
                window->close();
                return;
            }
        }
    }
}

void MenuState::update() {
    sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    // Sprawdzaj, czy myszka najeżdża na opcję i zmieniaj kolor tekstu
    checkMouseHover(mousePos, playWithAiText, playWithAiRect);
    checkMouseHover(mousePos, playWith2Text, playWith2Rect);
    checkMouseHover(mousePos, playByLanText, playByLanRect);
    checkMouseHover(mousePos, exitText, exitRect);
}

void MenuState::render() {
    window->clear();
    window->draw(backgroundSprite);

    // Rysowanie opcji ushenu
    window->draw(playWithAiRect);
    window->draw(playWith2Rect);
    window->draw(playByLanRect);
    window->draw(exitRect);

    window->draw(playWithAiText);
    window->draw(playWith2Text);
    window->draw(playByLanText);
    window->draw(exitText);
}
