#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Level
{
public:
    bool loadFromFile(const std::string &path, const sf::Vector2u &windowSize);
    void draw(sf::RenderWindow &window);

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};
