#include <SolasLib/core/LightmapManager.hpp>
#include "BoundRayCast/BoundRayCast.hpp"
#include "SimpleRayCast/SimpleRayCast.hpp"
#include "stdio.h"
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

LightmapManager::LightmapManager(int width, int height, int tileSize, CastingAlgorithm type)
{
	this->isPaused = false;
	this->nextLightId = 0;
	this->tileSize = tileSize;

	//tileArray.resize(width * height);
	for (int i = 0; i < width * height; i++)
	{
		tileArray.push_back(TileLightState());
	}

	this->floorGridWidth = width;
	this->floorGridHeight = height;

	switch (type)
	{
	case CastingAlgorithm::BOUND_RAY_CAST:
		this->lightCaster = new BoundRayCast();
		break;
	case CastingAlgorithm::SIMPLE_RAY_CAST:
		this->lightCaster = new SimpleRayCast();
		break;
	default:
		this->lightCaster = new SimpleRayCast();
	}
}

int LightmapManager::addLight(float x, float y, glm::vec2 direction, float span, float range)
{
	int lightId = nextLightId++;
	this->lightsMap.insert(std::pair<int, Light*>(lightId, new Light(x, y, glm::normalize(direction), span, range, tileSize)));
	this->lightsMap[lightId]->shouldUpdate = true;
	return lightId;
}

void LightmapManager::updateLight(int lightId, float x, float y, glm::vec2 direction, float span, float range)
{
	this->lightsMap[lightId]->x = x;
	this->lightsMap[lightId]->y = y;
	this->lightsMap[lightId]->direction = glm::normalize(direction);
	this->lightsMap[lightId]->span = span;
	this->lightsMap[lightId]->range = range;
	this->lightsMap[lightId]->shouldUpdate = true;
}

void LightmapManager::clearTileState()
{
	for (int i = 0; i < this->floorGridWidth * this->floorGridHeight; i++)
	{
		this->tileArray[i].isWall = false;
	}
}

void LightmapManager::clearLights()
{
	for (auto& lightIdPair : this->lightsMap)
	{
		this->lightCaster->removeLight(lightIdPair.first, lightIdPair.second, tileSize, floorGridWidth, floorGridHeight, tileArray);
		delete lightIdPair.second;
	}
	this->lightsMap.clear();
}

void LightmapManager::removeLight(int lightId)
{
	Light* light = lightsMap[lightId];
	this->lightCaster->removeLight(lightId, light, tileSize, floorGridWidth, floorGridHeight, tileArray);
	delete light;
	lightsMap.erase(lightId);
}

void LightmapManager::resetLighting()
{
	for (auto& lightIdPair : this->lightsMap)
	{
		for (int i = 0; i < lightIdPair.second->lightMapWidth; i++)
		{
			for (int j = 0; j < lightIdPair.second->lightMapHeight; j++)
			{
				lightIdPair.second->lightMap[i + j * lightIdPair.second->lightMapHeight] = 0;
			}
		}
	}
}

void LightmapManager::update()
{
	for (auto idLightPair : this->lightsMap)
	{
		int lightId = idLightPair.first;
		Light* light = idLightPair.second;

		if (light->shouldUpdate)
		{
			lightCaster->update(tileSize, light, lightId, floorGridWidth, floorGridHeight, tileArray);
			light->shouldUpdate = false;
		}
	}
}