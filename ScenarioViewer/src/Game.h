#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SolasLib/core.hpp>
#include "Scenarios/Scenarios.h"

using namespace std;

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

    sf::Color getColour() {
        return this->colour;
    }
    void setColour(uint8_t r, uint8_t g, uint8_t b) {
        this->colour.r = r;
        this->colour.g = g;
        this->colour.b = b;
    }
};

class Game
{
protected:
    LightmapManager* lightmapManager;
    vector<GameObject> gameObjects;
    int numTilesX, numTilesY;
    float currentTime;
    void startScenario(int index);
    int tileSize;

public:
    map<int, sf::Shape*> sprites;
    vector<sf::Shape*> debugSprites;
    int scenarioIndex;
    void nextScenario();
    void previousScenario();

    void addWall(int tileX, int tileY, uint8_t r, uint8_t g, uint8_t b);
    int modeIndex;

    Game(int tileSize = 8, int numTilesX = 50, int numTilesY = 50);
    void update(sf::Vector2i mousePosition, float timeDelta);
};
