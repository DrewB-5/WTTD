#include <SFML/Graphics.hpp>
#include <iostream>
#include "src/Button.hpp"
#include "src/LoadingScreen.hpp"
#include "src/Level.hpp"

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "What The Tower Defense 1.0.4", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/menu/background.png"))
    {
        std::cerr << "Failed to load background image!\n";
        return 1;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / backgroundTexture.getSize().x,
        float(window.getSize().y) / backgroundTexture.getSize().y);

    // Menu overlay and background
    sf::RectangleShape darkOverlay(sf::Vector2f(window.getSize()));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile("assets/menu/menu_background.png"))
    {
        std::cerr << "Failed to load menu background!\n";
        return 1;
    }
    sf::Sprite menuSprite(menuTexture);
    float menuScale = 1.5f;
    menuSprite.setScale(menuScale, menuScale);
    menuSprite.setPosition(
        (window.getSize().x - menuTexture.getSize().x * menuScale) / 2.f,
        (window.getSize().y - menuTexture.getSize().y * menuScale) / 2.f);

    // Buttons
    Button playButton("assets/menu/play_button.png", 0.2f, 0.5f, 0.85f, window);
    Button exitButton("assets/menu/exit_button.png", 0.1f, 0.045f, 0.075f, window);
    Button settingsButton("assets/menu/settings_button.png", 0.085f, 0.045f, 0.17f, window);
    Button placeholderButton("assets/menu/placeholder_button.png", 0.15f, 0.45f, 0.55f, window);

    bool isMenuOpen = false;
    bool loadingTriggered = false;
    bool gameStarted = false;

    Level level;
    std::unique_ptr<LoadingScreen> loader;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                if (!gameStarted)
                    isMenuOpen = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
                event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
            {
                std::cout << "Ctrl+Q detected, exiting...\n";
                return -1;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                if (!isMenuOpen)
                {
                    if (playButton.isClicked(mousePosF))
                    {
                        std::cout << "Play button clicked!\n";
                        isMenuOpen = true;
                    }
                    else if (settingsButton.isClicked(mousePosF))
                    {
                        std::cout << "Settings clicked!\n";
                    }
                    else if (exitButton.isClicked(mousePosF))
                    {
                        std::cout << "Exit clicked!\n";
                        window.close();
                    }
                }
                else if (!loadingTriggered && placeholderButton.isClicked(mousePosF))
                {
                    std::cout << "<placeholder> button clicked! Starting loading screen...\n";
                    loader = std::make_unique<LoadingScreen>(window, level);
                    loader->startLoading("data/levels/dev_test/dev_test.png");
                    loadingTriggered = true;
                }
            }
        }

        // Update
        sf::Vector2f mouseF(sf::Mouse::getPosition(window));
        if (!isMenuOpen)
        {
            playButton.update(mouseF);
            exitButton.update(mouseF);
            settingsButton.update(mouseF);
        }
        else if (!loadingTriggered)
        {
            placeholderButton.update(mouseF);
        }

        if (loadingTriggered && loader)
        {
            loader->update();
            if (loader->isFinished())
            {
                loadingTriggered = false;
                gameStarted = true;
                loader.reset();
            }
        }

        // Draw
        window.clear();

        if (gameStarted)
        {
            level.draw(window);
        }
        else if (loadingTriggered && loader)
        {
            loader->draw();
        }
        else
        {
            window.draw(background);
            playButton.draw(window);
            settingsButton.draw(window);
            exitButton.draw(window);

            if (isMenuOpen)
            {
                window.draw(darkOverlay);
                window.draw(menuSprite);
                placeholderButton.draw(window);
            }
        }

        window.display();
    }

    return 0;
}
