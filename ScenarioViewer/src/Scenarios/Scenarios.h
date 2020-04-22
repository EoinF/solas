#pragma once
#include <SFML/Graphics.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <functional>

using SetGridTypeFunction = std::function<void(int x, int y, bool isWall, float r, float g, float b)>;
using AddLightFunction = std::function<void(float x, float y, glm::vec2 direction, float span, float range)>;

const int NUM_SCENARIOS = 2;

void loadScenario(int scenarioNumber, SetGridTypeFunction setGridType, AddLightFunction addLight, int numTilesX, int numTilesY, int tileSize);
void scenario1(SetGridTypeFunction setGridType, AddLightFunction addLight, int numTilesX, int numTilesY, int tileSize);
void scenario2(SetGridTypeFunction setGridType, AddLightFunction addLight, int numTilesX, int numTilesY, int tileSize);
