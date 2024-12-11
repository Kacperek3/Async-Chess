#include "MenuState.h"


MenuState::MenuState(GameDataRef data) : _data(data) {
    
}

void MenuState::Init(){
    _data->assetManager.LoadTexture("Background", "../assets/Designer.png");
    _data->assetManager.LoadFont("Poppins-Thin", "../assets/fonts/Poppins-Thin.ttf");

    backgroundSprite.setTexture(_data->assetManager.GetTexture("Background"));
    font = _data->assetManager.GetFont("Poppins-Thin");
}

/*
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
} */

void MenuState::HandleInput() {
    sf::Event event;
    while (_data->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _data->window.close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);

            /*
            // Sprawdzanie, czy kliknięto w opcję "Play with AI"
            if (playWithAiText.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play with AI selected\n";
            }

            // Sprawdzanie, czy kliknięto w opcję "Play with 2 Players"
            if (playWith2Text.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play with 2 Players selected\n";
                //window->setSize(sf::Vector2u(800, 600));
                // gsm->pushState(std::make_shared<GameWith2State>(&gsm, &window));
            }

            // Sprawdzanie, czy kliknięto w opcję "Play by LAN"
            if (playByLanText.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play by LAN selected\n";
            }

            // Sprawdzanie, czy kliknięto w opcję "Exit"
            if (exitText.getGlobalBounds().contains(mousePos)) {
                std::cout << "Play by LAN selected\n";

                //_data->window.close();
                return;
            }*/

        }
    }
}

void MenuState::Update() {
    sf::Vector2f mousePos = _data->inputManager.GetMousePosition(_data->window);
    
    /*
    // Sprawdzaj, czy myszka najeżdża na opcję i zmieniaj kolor tekstu
    checkMouseHover(mousePos, playWithAiText, playWithAiRect);
    checkMouseHover(mousePos, playWith2Text, playWith2Rect);
    checkMouseHover(mousePos, playByLanText, playByLanRect);
    checkMouseHover(mousePos, exitText, exitRect);*/
}

void MenuState::Draw() {
    _data->window.clear();
    _data->window.draw(backgroundSprite);

    _data->window.display();
}
