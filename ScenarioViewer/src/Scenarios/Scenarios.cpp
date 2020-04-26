#include "Scenarios.h"

void loadScenario(int scenarioNumber, SetGridTypeFunction setGridType, AddLightFunction addLight, int numTilesX, int numTilesY, int tileSize)
{
    switch (scenarioNumber)
    {
        case 1:
            scenario1(setGridType, addLight, numTilesX, numTilesY, tileSize);
            break;
        case 2:
            scenario2(setGridType, addLight, numTilesX, numTilesY, tileSize);
            break;
    }
}

void scenario1(SetGridTypeFunction setGridType, AddLightFunction addLight, int numTilesX, int numTilesY, int tileSize)
{
    for (int j = numTilesY / 3; j < 2 * numTilesY / 3; j++) {
        setGridType(numTilesX / 3, j, true, 125, 125, 0);
    }

    int skip = 0;
    for (int j = numTilesY / 4; j < 3 * numTilesY / 4; j++) {
        skip++;
        if (skip > 10) {
            skip = 0;
            continue;
        }
        setGridType(3 * numTilesX / 4, j, true, 0, 255, 0);
    }


    int numLightsX = 5;
    int numLightsY = 4;
    float mapWidth = (float)(numTilesX * tileSize);
    float mapHeight = (float)(numTilesY * tileSize);
    for (int i = 0; i < numLightsX; ++i) {
        for (int j = 0; j < numLightsY; ++j) {
            addLight((i + 0.5f) * (mapWidth / numLightsX), (j + 0.5f) * (mapHeight / numLightsY), glm::normalize(glm::vec2(0.4f, -1.0f)), 0.8f, 150.0f);
        }
    }
}

void scenario2(SetGridTypeFunction setGridType, AddLightFunction addLight, int numTilesX, int numTilesY, int tileSize)
{
    int startX = numTilesX / 3;
    int startY = numTilesY / 3;
    int endX = std::min(startX + 50, numTilesX-1);
    int endY = std::min(startY + 50, numTilesY-1);

    for (int i = startX; i < endX; i++) 
    {
        setGridType(i, startY, true, 255, 255, 255);
        setGridType(i, endY, true, 255, 255, 255);
    }
    for (int j = startY; j <= endY; j++) {
        setGridType(startX, j, true, 255, 255, 255);
        setGridType(endX, j, true, 255, 255, 255);
    }

    setGridType((startX + endX) / 2, endY, false, 255, 0, 0);
    setGridType(1 + (startX + endX) / 2, endY, false, 255, 0, 0);

    addLight(tileSize * (float)(endX + startX) / 2, tileSize * (float)(endY + startY) / 2, glm::normalize(glm::vec2(0.4f, -1.0f)), 0.8f, 300.0f);
}
