#pragma once
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "LightmapManager/LightmapManager.h"
#include "Scenarios/Scenarios.h"

using namespace std;

class GameObject : public IGameObject
{
private:
    sf::Color colour;
public:
    float baseR;
    float baseG;
    float baseB;

    GameObject(float x = 0, float y = 0)
    {
        this->x = x;
        this->y = y;
        this->baseR = 255;
        this->baseG = 100;
        this->baseB = 170;
        this->setLighting(0.0f, 0.0f, 0.0f);
    }

    sf::Color getColour() {
        colour.r = (uint8_t)(min(r, 1.0f) * baseR);
        colour.g = (uint8_t)(min(g, 1.0f) * baseG);
        colour.b = (uint8_t)(min(b, 1.0f) * baseB);
        return this->colour;
    }
    void setColour(float r, float g, float b) {
        this->baseR = r;
        this->baseG = g;
        this->baseB = b;
    }
};

class Game
{
protected:
    LightmapManager<GameObject>* lightmapManager;
    vector<GameObject> gameObjects;
    GameObject** floorGrid;
    int numTilesX, numTilesY;
    float currentTime;
    void startScenario(int index);
    void startMode(int index);
    int tileSize;
    void resetLighting(float brightness);

public:
    map<GameObject*, sf::Shape*> sprites;
    vector<sf::Shape*> debugSprites;
    sf::VertexArray floorGridVertices;
    int scenarioIndex;
    void nextScenario();
    void previousScenario();
    int modeIndex;
    void nextMode();
    void previousMode();

    Game(int tileSize = 8, int numTilesX = 50, int numTilesY = 50);
    void debug();
    void update(sf::Vector2i mousePosition, float timeDelta);
};
