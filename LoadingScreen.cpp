#include "LoadingScreen.hpp"
#include <thread>

LoadingScreen::LoadingScreen(sf::RenderWindow &win) : window(win)
{
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    loadingText.setFont(font);
    loadingText.setString("Loading...");
    loadingText.setCharacterSize(60);
    loadingText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = loadingText.getLocalBounds();
    loadingText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    loadingText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
}

void LoadingScreen::run()
{
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 2)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(loadingText);
        window.display();
    }
}
