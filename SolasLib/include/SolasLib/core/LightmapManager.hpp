#pragma once

#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <SolasLib/core/TileLightState.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <SolasLib/core/Light.hpp>

class LightmapManager
{
protected:
	std::vector<TileLightState> tileArray;
	int tileSize;
	std::map<int, Light *> lightsMap;
	int nextLightId;
	int floorGridWidth;
	int floorGridHeight;
	bool isPaused;
	LightCaster * lightCaster;

public:
	LightmapManager(int width, int height, int tileSize, CastingAlgorithm type);

	std::vector<TileLightState> &getTileArray()
	{
		return this->tileArray;
	}

	TileLightState *getTileState(int x, int y)
	{
		return &this->tileArray[y * floorGridWidth + x];
	}

	void setTileSize(int tileSize)
	{
		this->tileSize = tileSize;
	}

	std::map<int, Light *> getLightsMap()
	{
		return this->lightsMap;
	}

	int addLight(float x, float y, float span, float range);
	int addLight(float x, float y, float span, float range, glm::vec2 direction);
	void updateLight(int lightId, float x, float y, float span, float range, glm::vec2 direction);
	void clearLights();
	void clearTileState();
	void removeLight(int lightId);
	void resetLighting();
	void update();
};
