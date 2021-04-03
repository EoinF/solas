#include "Game.h"

int NUM_MODES = 2;

Game::Game(int tileSize, int numTilesX, int numTilesY)
{
    this->tileSize = tileSize;
    this->numTilesX = numTilesX;
    this->numTilesY = numTilesY;
    this->pixels = new sf::Uint8[numTilesX * numTilesY * 4];
    this->lightmapManager = new LightmapManager(tileSize, CastingAlgorithm::BOUND_RAY_CAST, numTilesY);
    lightmapOverlay.create(numTilesX, numTilesY);
    lightmapOverlay.setSmooth(true);

    placementPosition = glm::vec2(200, 200);

    this->scenarioIndex = 2;

    this->startScenario(this->scenarioIndex);
};

void Game::update(sf::Vector2i mousePosition, glm::vec2 deltaPosition, float deltaSpan, bool isMouseClicked, float timeDelta)
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

    updatePlacement(isMouseClicked, mousePosition, deltaPosition, deltaSpan);
    lightmapManager->update();

    int index = 0;
    for (int j = 0; j < numTilesY; j++)
    {
        for (int i = 0; i < numTilesX; i++)
        {
            auto tile = lightmapManager->getTileState(i, j);
            pixels[index] = tile->brightness();
            pixels[index + 1] = tile->brightness();
            pixels[index + 2] = tile->brightness();
            pixels[index + 3] = 255 - glm::min(255, tile->brightness());

            index += 4;
        }
    }
    lightmapOverlayImage.create(numTilesX, numTilesY, pixels);
    lightmapOverlay.update(lightmapOverlayImage);
}

void Game::setPlacementMode(PlacementMode newMode)
{
    std::cout << "setting placement mode" << std::endl;
    if (newMode == PlacementMode::LIGHT)
    {
        this->heldLightId = this->lightmapManager->addLight(placementPosition.x, placementPosition.y, glm::pi<float>(), 200, glm::vec2(1, 0), 255);
    }
    else
    {
        std::cout << "removing light" << std::endl;
        this->lightmapManager->removeLight(heldLightId);
        std::cout << "removed light" << std::endl;
    }
    placementMode = newMode;
}

void Game::updatePlacement(bool isMouseClicked, sf::Vector2i mousePosition, glm::vec2 deltaPosition, float deltaSpan)
{
    placementPosition += deltaPosition;
    int tileX = floorf(placementPosition.x / tileSize);
    int tileY = floorf(placementPosition.y / tileSize);
    placementSprite->setPosition(tileX * tileSize, tileY * tileSize);
    auto heldLight = this->lightmapManager->getLightsMap()[heldLightId];
    if (placementMode == PlacementMode::LIGHT)
    {
        float updatedSpan = heldLight->span + deltaSpan;
        if (updatedSpan > 2.3f * glm::pi<float>())
        {
            updatedSpan = 0.0f;
        }

        this->lightmapManager->updateLight(
            heldLightId,
            placementPosition.x,
            placementPosition.y,
            updatedSpan,
            glm::vec2(mousePosition.x - heldLight->x, mousePosition.y - heldLight->y));
        if (isMouseClicked)
        {
            std::cout << "placed light at: " << tileX << "," << tileY << std::endl;
            this->heldLightId = this->lightmapManager->addLight(tileX * tileSize, tileY * tileSize, heldLight->span, heldLight->range, heldLight->direction, heldLight->brightness);
            this->addLight(tileX * tileSize, tileY * tileSize);
        }
    }
    else
    {
        if (isMouseClicked)
        {
            bool wasWall = this->lightmapManager->getTileState(tileX, tileY)->isWall;
            if (!wasWall)
            {
                addWall(tileX, tileY, 125, 255, 125);
            }
            else
            {
                int index = tileY * numTilesX + tileX;
                delete this->sprites[index];
                this->sprites.erase(index);
            }
            this->lightmapManager->getTileState(tileX, tileY)->isWall = !wasWall;
        }
    }
}

void Game::addWall(int tileX, int tileY, uint8_t r, uint8_t g, uint8_t b)
{
    float x = (float)(tileX * tileSize);
    float y = (float)(tileY * tileSize);
    GameObject gameObject = GameObject(x, y, r, g, b);
    this->gameObjects.push_back(gameObject);
    sf::Shape *newSprite = new sf::RectangleShape(sf::Vector2f((float)tileSize, (float)tileSize));
    newSprite->setFillColor(gameObject.getColour());
    newSprite->setPosition(x, y);
    this->sprites.insert(std::pair<int, sf::Shape *>(tileY * numTilesX + tileX, newSprite));
}

void Game::addLight(float x, float y)
{
    GameObject gameObject = GameObject(x, y, 125, 255, 125);
    this->gameObjects.push_back(gameObject);
    sf::Shape *newSprite = new sf::CircleShape(3.0f);
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
    for (auto debugSprite : this->debugSprites)
    {
        delete debugSprite;
    }
    this->debugSprites.clear();
    for (auto sprite : this->sprites)
    {
        delete sprite.second;
    }
    this->sprites.clear();
    loadScenario(
        index + 1,
        [=](int tileX, int tileY, bool isWall, uint8_t r, uint8_t g, uint8_t b) {
            this->lightmapManager->getTileState(tileX, tileY)->isWall = isWall;
            this->addWall(tileX, tileY, r, g, b);
        },
        [=](float x, float y, float span, float range, glm::vec2 direction) {
            this->lightmapManager->addLight(x, y, span, range, direction, 255);
            this->addLight(x, y);
        },
        numTilesX, numTilesY, tileSize);

    placementSprite = new sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
    placementSprite->setFillColor(sf::Color(255, 125, 125));
    placementSprite->setPosition(placementPosition.x, placementPosition.y);
    this->debugSprites.push_back(placementSprite);
    if (placementMode == PlacementMode::LIGHT)
    {
        this->heldLightId = this->lightmapManager->addLight(placementPosition.x, placementPosition.y, glm::pi<float>(), 200, glm::vec2(1, 0), 255);
    }
    this->lightmapManager->update();
}
