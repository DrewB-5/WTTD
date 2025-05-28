#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct Point
{
    float x, y;
    bool smooth = false;
    bool hasSmooth = false;
};

struct Polygon
{
    std::vector<sf::Vector2f> points;
    bool smooth = false;
};

class Level
{
public:
    bool loadFromFile(const std::string &jsonPath, const sf::Vector2u &windowSize);

    void draw(sf::RenderWindow &window);
    void update(float dt); // New method for updating enemy movement

    const std::vector<Point> &getPath() const;
    const std::vector<sf::Vector2f> &getSmoothPath() const;

    bool loadEnemySprite(const std::string &filePath, const sf::Vector2u &windowSize);
    void updateEnemy(float dt);
    void drawEnemy(sf::RenderWindow &window);

private:
    void generateSmoothPath();
    sf::Vector2f catmullRom(const sf::Vector2f &p0, const sf::Vector2f &p1,
                            const sf::Vector2f &p2, const sf::Vector2f &p3, float t);
    std::vector<sf::Vector2f> generateSmoothPolygon(const std::vector<sf::Vector2f> &points);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<Point> path;
    std::vector<sf::Vector2f> smoothPath;

    std::vector<Polygon> waterPolygons;
    std::vector<Polygon> obstaclePolygons;

    sf::Texture enemyTexture;
    sf::Sprite enemySprite;

    float enemyTravelDist = 0.f;
    float enemySpeed = 150.f;
};
