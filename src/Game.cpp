#include "Game.h"

using namespace std;

int NUM_MODES = 2;

Game::Game(int tileSize, int numTilesX, int numTilesY)
{
    this->tileSize = tileSize;
    this->numTilesX = numTilesX;
    this->numTilesY = numTilesY;
    this->floorGrid = new GameObject * [numTilesY];
    this->lightmapManager = new LightmapManager(this->floorGrid, numTilesX, numTilesY, tileSize);
    this->floorGridVertices = sf::VertexArray(sf::PrimitiveType::Triangles, 0);

    for (int j = 0; j < numTilesY; j++)
    {
        this->floorGrid[j] = new GameObject[numTilesX];
        for (int i = 0; i < numTilesX; i++)
        {
            float x = (float)(i * tileSize);
            float y = (float)(j * tileSize);
            this->floorGrid[j][i].x = x;
            this->floorGrid[j][i].y = y;

            sf::Color baseColour = i % 2 == 0 ? sf::Color::Yellow : sf::Color::Red;

            //
            // Create 2 triangles per tile
            //
            floorGridVertices.append(sf::Vertex(sf::Vector2f(x, y), baseColour));
            floorGridVertices.append(sf::Vertex(sf::Vector2f(x + tileSize, y), baseColour));
            floorGridVertices.append(sf::Vertex(sf::Vector2f(x, y + tileSize), baseColour));

            floorGridVertices.append(sf::Vertex(sf::Vector2f(x, y + tileSize), baseColour));
            floorGridVertices.append(sf::Vertex(sf::Vector2f(x + tileSize, y), baseColour));
            floorGridVertices.append(sf::Vertex(sf::Vector2f(x + tileSize, y + tileSize), baseColour));
        }
    }

    this->scenarioIndex = 0;
    this->startScenario(this->scenarioIndex);
};

void Game::debug()
{
    map<int, Light*> lightsMap = this->lightmapManager->getLightsMap();

    for (auto spritePtr : this->debugSprites) {
        delete spritePtr;
    }
    this->debugSprites.clear();

    for (auto idLightPair: lightsMap)
    {
        Light light = *idLightPair.second;
        sf::CircleShape* originSprite = new sf::CircleShape(2);
        originSprite->setOrigin(2, 2);
        originSprite->setPosition(light.x, light.y);
        originSprite->setFillColor(sf::Color::Magenta);
        sf::RectangleShape* edge1Sprite = new sf::RectangleShape(sf::Vector2f(light.range, 1));

        edge1Sprite->setRotation(glm::degrees(std::atan2(-light.direction.y, light.direction.x)));
        edge1Sprite->setPosition(light.x, light.y);
        edge1Sprite->setFillColor(sf::Color::Green);

        /*sf::RectangleShape* edge2Sprite = new sf::RectangleShape(sf::Vector2f(1, light->range));
        edge2Sprite->setRotation(light->edge2);
        edge2Sprite->setPosition(light->x, light->y);
        edge2Sprite->setFillColor(sf::Color::Red);*/

        this->debugSprites.push_back(originSprite);
        this->debugSprites.push_back(edge1Sprite);
        //this->debugSprites.push_back(edge2Sprite);
    }
}

int updates = 0;
void Game::update(sf::Vector2i mousePosition, float timeDelta)
{
    currentTime += timeDelta * 2.0f;
    for (auto idLightPair : this->lightmapManager->getLightsMap())
    {
        int id = idLightPair.first;
        Light* light = idLightPair.second;
        this->lightmapManager->updateLight(
            id,
            light->x, light->y,
            //glm::vec2(mousePosition.x - light->x, light->y - mousePosition.y),
            glm::vec2(sinf(currentTime + id), cosf(currentTime + id)),
            light->span,
            light->range
        );
        debug();
    }

    this->lightmapManager->update();
    
    for (int i = 0; i < this->numTilesX; ++i)
    {
        for (int j = 0; j < this->numTilesY; ++j)
        {
            GameObject* gameObject = &this->floorGrid[j][i];
            int startingIndex = 6 * (i + j * numTilesX);
            for (int offset = 0; offset < 6; ++offset)
            {
                this->floorGridVertices[startingIndex + offset].color = gameObject->getColour();
            }
        }
    }
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
    this->lightmapManager->clearLights();
    loadScenario(
        index + 1,
        [=](int x, int y, bool isWall, float r, float g, float b)
        {
            this->floorGrid[y][x].isWall = isWall;
            this->floorGrid[y][x].setColour(r, g, b);
        }, [=](float x, float y, glm::vec2 direction, float span, float range)
        {
            this->lightmapManager->addLight(x, y, direction, span, range, tileSize);
        }, numTilesX, numTilesY, tileSize);
}

void Game::nextMode() {
    this->modeIndex = (this->modeIndex + 1) % NUM_MODES;
    this->startMode(this->modeIndex);
}

void Game::previousMode() {
    this->modeIndex = (this->modeIndex + NUM_MODES - 1) % NUM_MODES;
    this->startMode(this->modeIndex);
}

void Game::resetLighting(float brightness)
{
    for (int i = 0; i < this->numTilesX; ++i)
    {
        for (int j = 0; j < this->numTilesY; ++j)
        {
            GameObject* gameObject = &this->floorGrid[j][i];
            gameObject->setLighting(brightness, brightness, brightness);
        }
    }
    this->lightmapManager->resetLighting();
}

void Game::startMode(int index) 
{
    switch(index) 
    {
        case 0:
            this->lightmapManager->unpause();
            this->resetLighting(0.0);
            break;
        case 1:
            this->lightmapManager->pause();
            this->resetLighting(1.0);
            break;
    }
}
