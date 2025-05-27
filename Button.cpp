#include "Button.hpp"
#include <iostream>

Button::Button(const std::string &texturePath, float relScale, float relX, float relY, sf::RenderWindow &window)
    : relScale(relScale), relX(relX), relY(relY)
{
    if (!texture.loadFromFile(texturePath))
    {
        std::cerr << "Failed to load button texture: " << texturePath << "\n";
    }
    sprite.setTexture(texture);

    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

    float scaleFactorX = (relScale * window.getSize().x) / texture.getSize().x;
    float scaleFactorY = (relScale * window.getSize().y) / texture.getSize().y;
    float scale = (scaleFactorX < scaleFactorY) ? scaleFactorX : scaleFactorY;
    sprite.setScale(scale, scale);

    float posX = relX * window.getSize().x;
    float posY = relY * window.getSize().y;
    sprite.setPosition(posX, posY);
}

void Button::update(const sf::Vector2f &mousePos)
{
    if (sprite.getGlobalBounds().contains(mousePos))
        sprite.setColor(hoverColor);
    else
        sprite.setColor(normalColor);
}

void Button::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

bool Button::isClicked(const sf::Vector2f &mousePos) const
{
    return sprite.getGlobalBounds().contains(mousePos);
}

void Button::setHoverColor(const sf::Color &color)
{
    hoverColor = color;
}
