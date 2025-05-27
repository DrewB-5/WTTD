#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
#include "Level.hpp"

class LoadingScreen
{
public:
    LoadingScreen(sf::RenderWindow &win, Level &lvl);

    void startLoading(const std::string &levelPath);
    void update();
    void draw();
    bool isFinished() const;

private:
    sf::RenderWindow &window;
    Level &level;

    sf::Font font;
    sf::Text loadingText;

    std::thread loadingThread;
    bool isLoading;
    bool finished;
    int opacity;
    float scale;
    sf::Uint8 levelOpacity = 0;
};
