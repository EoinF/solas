#include "Game.h"

int NUM_MODES = 2;

Game::Game(int tileSize, int numTilesX, int numTilesY)
{
    this->tileSize = tileSize;
    this->numTilesX = numTilesX;
    this->numTilesY = numTilesY;
    this->pixels = new sf::Uint8[numTilesX * numTilesY * 4];
    this->lightmapManager = new LightmapManager(numTilesX, numTilesY, tileSize, CastingAlgorithm::BOUND_RAY_CAST);
    lightmapOverlay.create(numTilesX, numTilesY);
    lightmapOverlay.setSmooth(false);

    this->scenarioIndex = 2;

    this->startScenario(this->scenarioIndex);
};

void Game::update(sf::Vector2i mousePosition, float timeDelta)
{
    currentTime += timeDelta * 2.0f;
    //for (auto idLightPair : this->lightmapManager->getLightsMap())
    //{
    //    int id = idLightPair.first;
    //    Light *light = idLightPair.second;

    //    glm::vec2 direction(sinf(currentTime + id), cosf(currentTime + id));
    //    //glm::vec2 direction(mousePosition.x - light->x, mousePosition.y - light->y);

    //    this->lightmapManager->updateLight(
    //        id,
    //        light->x, light->y,
    //        direction,
    //        light->span,
    //        light->range);
    //}
    lightmapManager->update();

    auto tiles = lightmapManager->getTileArray();
    int index = 0;
    for (auto &tile : tiles)
    {
        /*if (tile.r() > 0) {
            pixels[index] = 255;
            pixels[index + 1] = 255;
            pixels[index + 2] = 255;
            pixels[index + 3] = 255;
        }
        else */
        {
            pixels[index] = tile.brightness();
            pixels[index + 1] = tile.brightness();
            pixels[index + 2] = tile.brightness();
            pixels[index + 3] = 255 - tile.brightness();
        }
        index += 4;
    }
    lightmapOverlayImage.create(numTilesX, numTilesY, pixels);
    lightmapOverlay.update(lightmapOverlayImage);
}

void Game::addWall(int tileX, int tileY, uint8_t r, uint8_t g, uint8_t b)
{
    float x = (float)(tileX * tileSize);
    float y = (float)(tileY * tileSize);
    GameObject gameObject = GameObject(x, y, r, g, b);
    this->gameObjects.push_back(gameObject);
    sf::Shape* newSprite = new sf::RectangleShape(sf::Vector2f((float)tileSize, (float)tileSize));
    newSprite->setFillColor(gameObject.getColour());
    newSprite->setPosition(x, y);
    this->sprites.insert(std::pair<int, sf::Shape*>(tileY * numTilesX + tileX, newSprite));
}

void Game::addLight(float x, float y)
{
    GameObject gameObject = GameObject(x, y, 125, 255, 125);
    this->gameObjects.push_back(gameObject);
    sf::Shape* newSprite = new sf::CircleShape(3.0f);
    newSprite->setFillColor(gameObject.getColour());
    newSprite->setPosition(x, y);
    this->debugSprites.push_back(newSprite);
}

void Game::nextScenario()
{
    this->scenarioIndex = (this->scenarioIndex + 1) % NUM_SCENARIOS;
    this->startScenario(this->scenarioIndex);
}

void Game::previousScenario()
{
    this->scenarioIndex = (this->scenarioIndex + NUM_SCENARIOS - 1) % NUM_SCENARIOS;
    this->startScenario(this->scenarioIndex);
}

void Game::startScenario(int index)
{
    this->lightmapManager->clearLights();
    this->lightmapManager->clearTileState();
    this->gameObjects.clear();
    this->debugSprites.clear();
    this->sprites.clear();
    loadScenario(
        index + 1,
        [=](int tileX, int tileY, bool isWall, uint8_t r, uint8_t g, uint8_t b) {
            this->lightmapManager->getTileState(tileX, tileY)->isWall = isWall;
            this->addWall(tileX, tileY, r, g, b);
        },
        [=](float x, float y, glm::vec2 direction, float span, float range) {
            this->lightmapManager->addLight(x, y, direction, span, range);
            this->addLight(x, y);
        },
        numTilesX, numTilesY, tileSize);
}
