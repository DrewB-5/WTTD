#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(const std::string &texturePath, float relScale, float relX, float relY, sf::RenderWindow &window);

    void update(const sf::Vector2f &mousePos);
    void draw(sf::RenderWindow &window) const;
    bool isClicked(const sf::Vector2f &mousePos) const;
    void setHoverColor(const sf::Color &color);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Color normalColor = sf::Color::White;
    sf::Color hoverColor = sf::Color(200, 200, 200);

    float relScale;
    float relX;
    float relY;
};
