#include "Game.h"

using namespace std;

int NUM_MODES = 2;

Game::Game(int tileSize, int numTilesX, int numTilesY)
{
    this->tileSize = tileSize;
    this->numTilesX = numTilesX;
    this->numTilesY = numTilesY;
    this->lightmapManager = new LightmapManager(numTilesX, numTilesY, tileSize);

    this->scenarioIndex = 0;
    this->startScenario(this->scenarioIndex);
};

void Game::update(sf::Vector2i mousePosition, float timeDelta)
{

}

void Game::addWall(int tileX, int tileY, uint8_t r, uint8_t g, uint8_t b) {
    float x = (float)(tileX * tileSize);
    float y = (float)(tileY * tileSize);
    GameObject gameObject = GameObject(x, y, r, g, b);
    this->gameObjects.push_back(gameObject);
    sf::Shape* newSprite = new sf::RectangleShape(sf::Vector2f((float)tileSize, (float)tileSize));
    newSprite->setFillColor(gameObject.getColour());
    newSprite->setPosition(x, y);
    this->sprites.insert(pair<int, sf::Shape*>(tileY * numTilesX + tileX, newSprite));
}

void Game::nextScenario() {
    this->scenarioIndex = (this->scenarioIndex + 1) % NUM_SCENARIOS;
    this->startScenario(this->scenarioIndex);
}

void Game::previousScenario() {
    this->scenarioIndex = (this->scenarioIndex + NUM_SCENARIOS - 1) % NUM_SCENARIOS;
    this->startScenario(this->scenarioIndex);
}

void Game::startScenario(int index) {
    //this->lightmapManager->clearLights();
    loadScenario(
        index + 1,
        [=](int tileX, int tileY, bool isWall, uint8_t r, uint8_t g, uint8_t b)
        {
            this->lightmapManager->getTileState(tileX, tileY)->isWall = isWall;
            this->addWall(tileX, tileY, r, g, b);
        }, [=](float x, float y, glm::vec2 direction, float span, float range)
        {
            this->lightmapManager->addLight(x, y, direction, span, range, tileSize);
        }, numTilesX, numTilesY, tileSize);
}
