#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SolasLib/core.hpp>
#include "Scenarios/Scenarios.h"

class GameObject
{
private:
    sf::Color colour;

public:
    float x;
    float y;

    GameObject(float x = 0, float y = 0, uint8_t r = 0, uint8_t g = 100, uint8_t b = 170)
    {
        this->x = x;
        this->y = y;
        this->colour.r = r;
        this->colour.g = g;
        this->colour.b = b;
    }

    sf::Color getColour()
    {
        return this->colour;
    }
    void setColour(uint8_t r, uint8_t g, uint8_t b)
    {
        this->colour.r = r;
        this->colour.g = g;
        this->colour.b = b;
    }
};

class Game
{
protected:
    LightmapManager *lightmapManager;
    std::vector<GameObject> gameObjects;
    int numTilesX, numTilesY;
    float currentTime;
    void startScenario(int index);
    int tileSize;
    sf::Image lightmapOverlayImage;
    sf::Uint8 *pixels;
    int heldLightId = 0;

public:
    std::map<int, sf::Shape *> sprites;
    std::vector<sf::Shape *> debugSprites;
    sf::Texture lightmapOverlay;
    int scenarioIndex;
    void nextScenario();
    void previousScenario();

    void addWall(int tileX, int tileY, uint8_t r, uint8_t g, uint8_t b);
    void addLight(float x, float y);
    int modeIndex;

    Game(int tileSize = 8, int numTilesX = 50, int numTilesY = 50);
    void update(sf::Vector2i mousePosition, glm::vec2 deltaPosition, float deltaSpan, bool isMouseClicked, float timeDelta);
};
