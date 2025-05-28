#include "Level.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool Level::loadFromFile(const std::string &jsonPath, const sf::Vector2u &windowSize)
{
    std::ifstream file(jsonPath);
    if (!file)
    {
        std::cerr << "Failed to open JSON file: " << jsonPath << "\n";
        return false;
    }

    json j;
    file >> j;

    std::string imagePath;
    if (j.contains("image"))
        imagePath = j["image"].get<std::string>();
    else
    {
        std::cerr << "No 'image' field in JSON file: " << jsonPath << "\n";
        return false;
    }

    if (!backgroundTexture.loadFromFile(imagePath))
    {
        std::cerr << "Failed to load level background: " << imagePath << "\n";
        return false;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        float(windowSize.x) / backgroundTexture.getSize().x,
        float(windowSize.y) / backgroundTexture.getSize().y);

    // Load path points
    path.clear();
    for (const auto &point : j["path"])
    {
        Point pt;
        pt.x = point["x"].get<float>();
        pt.y = point["y"].get<float>();
        if (point.contains("smooth"))
        {
            pt.smooth = point["smooth"].get<bool>();
            pt.hasSmooth = true;
        }
        path.push_back(pt);
    }
    generateSmoothPath();

    float scaleX = float(windowSize.x) / backgroundTexture.getSize().x;
    float scaleY = float(windowSize.y) / backgroundTexture.getSize().y;

    // Helper to load polygons (water or obstacle)
    auto loadPolygons = [&](const json &jArray, std::vector<Polygon> &polygons)
    {
        polygons.clear();

        for (const auto &region : jArray)
        {
            if (!region.contains("points"))
                continue;

            Polygon poly;
            const auto &pts = region["points"];
            for (const auto &p : pts)
            {
                float x = p["x"].get<float>() * scaleX;
                float y = p["y"].get<float>() * scaleY;
                poly.points.emplace_back(x, y);
            }

            if (region.contains("smooth"))
                poly.smooth = region["smooth"].get<bool>();
            else
                poly.smooth = false;

            polygons.push_back(poly);
        }
    };

    // Load water polygons
    waterPolygons.clear();
    if (j.contains("water"))
        loadPolygons(j["water"], waterPolygons);

    // Load obstacle polygons
    obstaclePolygons.clear();
    if (j.contains("obstacle"))
        loadPolygons(j["obstacle"], obstaclePolygons);

    return true;
}

void Level::draw(sf::RenderWindow &window)
{
    window.draw(backgroundSprite);

    auto drawFilledPolygons = [&](const std::vector<Polygon> &polygons, const sf::Color &color)
    {
        for (const auto &poly : polygons)
        {
            std::vector<sf::Vector2f> pointsToDraw;

            if (poly.smooth && poly.points.size() >= 3)
            {
                pointsToDraw = generateSmoothPolygon(poly.points);
            }
            else
            {
                pointsToDraw = poly.points;
            }

            if (pointsToDraw.size() >= 3)
            {
                sf::ConvexShape shape;
                shape.setPointCount(pointsToDraw.size());
                for (size_t i = 0; i < pointsToDraw.size(); i++)
                    shape.setPoint(i, pointsToDraw[i]);
                shape.setFillColor(color);
                window.draw(shape);
            }
        }
    };

    // Draw filled water polygons in semi-transparent blue
    drawFilledPolygons(waterPolygons, sf::Color(0, 0, 255, 100));

    // Draw filled obstacle polygons in semi-transparent red
    drawFilledPolygons(obstaclePolygons, sf::Color(255, 0, 0, 150));

    // Draw the smooth path as red line
    if (!smoothPath.empty())
    {
        sf::VertexArray line(sf::LineStrip, smoothPath.size());
        for (size_t i = 0; i < smoothPath.size(); i++)
        {
            line[i].position = smoothPath[i];
            line[i].color = sf::Color::Red;
        }
        window.draw(line);
    }
}

const std::vector<Point> &Level::getPath() const
{
    return path;
}

const std::vector<sf::Vector2f> &Level::getSmoothPath() const
{
    return smoothPath;
}

void Level::generateSmoothPath()
{
    smoothPath.clear();
    if (path.size() < 2)
    {
        for (const auto &p : path)
            smoothPath.emplace_back(p.x, p.y);
        return;
    }

    std::vector<sf::Vector2f> pts;
    pts.emplace_back(path[0].x, path[0].y);
    for (const auto &p : path)
        pts.emplace_back(p.x, p.y);
    pts.emplace_back(path.back().x, path.back().y);

    const float step = 0.05f;

    for (size_t i = 1; i < pts.size() - 2; i++)
    {
        const auto &prev = pts[i];
        const auto &next = pts[i + 1];

        bool useSmooth;
        if (path[i - 1].hasSmooth)
        {
            useSmooth = path[i - 1].smooth;
        }
        else
        {
            useSmooth = (prev.x != next.x && prev.y != next.y);
        }

        if (useSmooth)
        {
            for (float t = 0; t < 1.0f; t += step)
                smoothPath.push_back(catmullRom(pts[i - 1], pts[i], pts[i + 1], pts[i + 2], t));
        }
        else
        {
            smoothPath.push_back(prev);
        }
    }

    smoothPath.push_back(pts[pts.size() - 2]);
}

sf::Vector2f Level::catmullRom(const sf::Vector2f &p0, const sf::Vector2f &p1,
                               const sf::Vector2f &p2, const sf::Vector2f &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    float x = 0.5f * ((2 * p1.x) +
                      (-p0.x + p2.x) * t +
                      (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 +
                      (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3);

    float y = 0.5f * ((2 * p1.y) +
                      (-p0.y + p2.y) * t +
                      (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 +
                      (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3);

    return {x, y};
}

std::vector<sf::Vector2f> Level::generateSmoothPolygon(const std::vector<sf::Vector2f> &points)
{
    std::vector<sf::Vector2f> smoothPoints;
    if (points.size() < 3)
        return points;

    const float step = 0.05f;
    size_t n = points.size();

    for (size_t i = 0; i < n; i++)
    {
        sf::Vector2f p0 = points[(i + n - 1) % n];
        sf::Vector2f p1 = points[i];
        sf::Vector2f p2 = points[(i + 1) % n];
        sf::Vector2f p3 = points[(i + 2) % n];

        for (float t = 0; t < 1.0f; t += step)
        {
            smoothPoints.push_back(catmullRom(p0, p1, p2, p3, t));
        }
    }
    return smoothPoints;
}

bool Level::loadEnemySprite(const std::string &filePath, const sf::Vector2u &windowSize)
{
    if (!enemyTexture.loadFromFile(filePath))
    {
        std::cerr << "Failed to load enemy sprite: " << filePath << "\n";
        return false;
    }
    enemySprite.setTexture(enemyTexture);

    float scaleFactor = 0.1f;
    enemySprite.setScale(scaleFactor, scaleFactor);

    enemyTravelDist = 0.f;
    return true;
}

void Level::updateEnemy(float dt)
{
    if (smoothPath.empty())
        return;

    enemyTravelDist += enemySpeed * dt;

    float pathLength = 0.f;
    for (size_t i = 1; i < smoothPath.size(); i++)
    {
        pathLength += std::hypot(
            smoothPath[i].x - smoothPath[i - 1].x,
            smoothPath[i].y - smoothPath[i - 1].y);
    }

    while (enemyTravelDist > pathLength)
        enemyTravelDist -= pathLength;

    float dist = 0.f;
    for (size_t i = 1; i < smoothPath.size(); i++)
    {
        float segLength = std::hypot(
            smoothPath[i].x - smoothPath[i - 1].x,
            smoothPath[i].y - smoothPath[i - 1].y);

        if (dist + segLength >= enemyTravelDist)
        {
            float remain = enemyTravelDist - dist;
            float ratio = remain / segLength;
            float x = smoothPath[i - 1].x + ratio * (smoothPath[i].x - smoothPath[i - 1].x);
            float y = smoothPath[i - 1].y + ratio * (smoothPath[i].y - smoothPath[i - 1].y);

            enemySprite.setPosition(x - enemySprite.getGlobalBounds().width / 2.f,
                                    y - enemySprite.getGlobalBounds().height / 2.f);
            break;
        }
        dist += segLength;
    }
}

void Level::drawEnemy(sf::RenderWindow &window)
{
    window.draw(enemySprite);
}
