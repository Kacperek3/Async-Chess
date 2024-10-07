#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class SidePanel
{
    /* data */
public:
    SidePanel();
    void draw(sf::RenderWindow& window);
private:
    //sf::RectangleShape sidePanel; //wystarczy że to odkomentuje i będzie błąd
};

