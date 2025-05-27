#pragma once
#include <SFML/Graphics.hpp>

class LoadingScreen
{
public:
    LoadingScreen(sf::RenderWindow &window);
    void run();

private:
    sf::RenderWindow &window;
    sf::Font font;
    sf::Text loadingText;
};
