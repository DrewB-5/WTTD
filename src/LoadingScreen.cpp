#include "LoadingScreen.hpp"
#include <thread>
#include <iostream>

LoadingScreen::LoadingScreen(sf::RenderWindow &win, Level &lvl)
    : window(win), level(lvl), isLoading(true), finished(false), opacity(255), scale(1.f)
{
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Failed to load font!\n";
    }

    loadingText.setFont(font);
    loadingText.setString("Loading...");
    loadingText.setCharacterSize(60);
    loadingText.setFillColor(sf::Color(255, 255, 255, opacity));

    sf::FloatRect textBounds = loadingText.getLocalBounds();
    loadingText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    loadingText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
}

void LoadingScreen::startLoading(const std::string &levelPath)
{
    // Launch background thread to simulate or perform actual level loading
    loadingThread = std::thread([this, levelPath]()
                                {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Simulate loading time
        // Derive JSON path from image path
        std::string jsonPath = levelPath.substr(0, levelPath.find_last_of('.')) + ".json";
        level.loadFromFile(jsonPath, window.getSize());
        isLoading = false; });
}

void LoadingScreen::update()
{
    if (!isLoading && !finished)
    {
        if (opacity > 0)
        {
            opacity -= 10;
            if (opacity < 0)
                opacity = 0;
            scale -= 0.05f;
            if (scale < 0.5f)
                scale = 0.5f;

            loadingText.setFillColor(sf::Color(255, 255, 255, opacity));
            loadingText.setScale(scale, scale);
        }
        else
        {
            if (loadingThread.joinable())
                loadingThread.join();
            finished = true;
        }
    }
}

void LoadingScreen::draw()
{
    window.clear(sf::Color::Black);
    window.draw(loadingText);
}

bool LoadingScreen::isFinished() const
{
    return finished;
}