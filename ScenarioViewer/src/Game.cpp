#include "Game.h"

using namespace std;

int NUM_MODES = 2;

Game::Game(int tileSize, int numTilesX, int numTilesY)
{
    this->tileSize = tileSize;
    this->numTilesX = numTilesX;
    this->numTilesY = numTilesY;
    this->floorGrid = new IGameObject * [numTilesX * numTilesY];
    this->lightmapManager = new LightmapManager(this->floorGrid, numTilesX, numTilesY, tileSize);
    this->floorGridVertices = sf::VertexArray(sf::PrimitiveType::Triangles, 0);

    for (int j = 0; j < numTilesY; j++)
    {
        for (int i = 0; i < numTilesX; i++)
        {
            float x = (float)(i * tileSize);
            float y = (float)(j * tileSize);
            this->gameObjects.push_back(GameObject(x, y));
            this->floorGrid[j * numTilesX + i] = &this->gameObjects.back();

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

void Game::update(sf::Vector2i mousePosition, float timeDelta)
{
    currentTime += timeDelta * 2.0f;

    for (int i = 0; i < this->numTilesX; ++i)
    {
        for (int j = 0; j < this->numTilesY; ++j)
        {
            GameObject gameObject = this->gameObjects[j * numTilesX + i];
            int startingIndex = 6 * (gameObject.x + gameObject.y * numTilesX);
            for (int offset = 0; offset < 6; ++offset)
            {
                this->floorGridVertices[startingIndex + offset].color = gameObject.getColour();
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
            this->gameObjects[y * numTilesX + x].isWall = isWall;
            this->gameObjects[y * numTilesX + x].setColour(r, g, b);
        }, [=](float x, float y, glm::vec2 direction, float span, float range)
        {
            this->lightmapManager->addLight(x, y, direction, span, range, tileSize);
        }, numTilesX, numTilesY, tileSize);
}
