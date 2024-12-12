#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>


class AssetManager {
public:
    AssetManager() {}
    ~AssetManager() {}

    void LoadTexture(std::string name, std::string fileName);
    sf::Texture &GetTexture(std::string name);

    void LoadFont(std::string name, std::string fileName);
    sf::Font &GetFont(std::string name);

private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
    std::map<std::string, sf::RectangleShape> _rectangles;
    std::map<std::string, sf::Text> _text;
};