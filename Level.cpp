#include "Level.hpp"
#include <iostream>

bool Level::loadFromFile(const std::string &path, const sf::Vector2u &windowSize)
{
    if (!backgroundTexture.loadFromFile(path))
    {
        std::cerr << "Failed to load level background: " << path << "\n";
        return false;
    }

    backgroundSprite.setTexture(backgroundTexture);

    backgroundSprite.setScale(
        float(windowSize.x) / backgroundTexture.getSize().x,
        float(windowSize.y) / backgroundTexture.getSize().y);

    return true;
}

void Level::draw(sf::RenderWindow &window)
{
    window.draw(backgroundSprite);
}
