#include <SolasLib/core/LightmapManager.hpp>
#include "BoundRayCast/BoundRayCast.hpp"
#include "SimpleRayCast/SimpleRayCast.hpp"
#include "stdio.h"
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>


long getChunkIndex(int x, int y)
{
	return (x + MAX_CHUNKS_X / 2) + (y * MAX_CHUNKS_X + 1);
}

TileLightState * getChunkFast(int tileX, int tileY, int chunkSize, ChunkMap & chunkMap)
{
	float chunkSizeF = (float)chunkSize;
	int chunkX = floorf(tileX / chunkSizeF);
	int chunkY = floorf(tileY / chunkSizeF);
	int chunkIndex = getChunkIndex(chunkX, chunkY);
	int chunkTileX = tileX - chunkX * chunkSize;
	int chunkTileY = tileY - chunkY * chunkSize;

	return &chunkMap[chunkIndex][chunkTileX + chunkTileY * chunkSize];
}

void LightmapManager::allocateChunksForLight(float x, float y, float range)
{
	float divisor = (float)(chunkSize * tileSize);
	float rangeInTiles = 1 + glm::ceil(range / (float)tileSize);
	float actualRange = rangeInTiles * tileSize;

	int startChunkX = (int)floorf((x - actualRange) / divisor);
	int startChunkY = (int)floorf((y - actualRange) / divisor);
	int endChunkX = (int)floorf((x + actualRange) / divisor);
	int endChunkY = (int)floorf((y + actualRange) / divisor);

	for (int i = startChunkX; i <= endChunkX; i++)
	{
		for (int j = startChunkY; j <= endChunkY; j++)
		{
			getOrAllocateChunk(getChunkIndex(i, j));
		}
	}
}

LightmapManager::LightmapManager(int tileSize, CastingAlgorithm type, int chunkSize)
{
	this->isPaused = false;
	this->nextLightId = 0;
	this->tileSize = tileSize;
	this->chunkSize = chunkSize;

	allocateChunk(getChunkIndex(0, 0));

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

void LightmapManager::allocateChunk(int chunkIndex)
{
	std::vector<TileLightState> tileArray;

	//tileArray.resize(chunkSize * chunkSize);
	for (int i = 0; i < chunkSize * chunkSize; i++)
	{
		tileArray.push_back(TileLightState());
	}

	this->chunkMap.insert({ chunkIndex, tileArray });
}

std::vector<TileLightState> & LightmapManager::getOrAllocateChunk(int chunkIndex)
{
	if (this->chunkMap.find(chunkIndex) == this->chunkMap.end())
	{
		allocateChunk(chunkIndex);
	}
	return this->chunkMap[chunkIndex];
}

TileLightState* LightmapManager::getTileState(int x, int y)
{
	int chunkX = floorf(x / (float)chunkSize);
	int chunkY = floorf(y / (float)chunkSize);
	int chunkIndex = getChunkIndex(chunkX, chunkY);
	int chunkTileX = x - chunkSize * chunkX;
	int chunkTileY = y - chunkSize * chunkY;
	return &(LightmapManager::getOrAllocateChunk(chunkIndex)[chunkTileX + chunkTileY * chunkSize]);
}

int LightmapManager::addLight(float x, float y, float span, float range)
{
	return addLight(x, y, span, range, glm::vec2(1, 0));
}

int LightmapManager::addLight(float x, float y, float span, float range, glm::vec2 direction)
{
	int lightId = nextLightId++;
	this->lightsMap.insert(std::pair<int, Light*>(lightId, new Light(x, y, direction, span, range, tileSize)));
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, range);

	return lightId;
}

void LightmapManager::updateLight(int lightId, float x, float y, float span, float range, glm::vec2 direction)
{
	this->lightsMap[lightId]->x = x;
	this->lightsMap[lightId]->y = y;
	this->lightsMap[lightId]->direction = direction;
	this->lightsMap[lightId]->span = span;
	this->lightsMap[lightId]->range = range;
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, range);
}

void LightmapManager::clearTileState()
{
	auto& tileArray = this->getTileArray();
	for (int i = 0; i < this->chunkSize * this->chunkSize; i++)
	{
		tileArray[i].isWall = false;
	}
}

void LightmapManager::clearLights()
{
	for (auto& lightIdPair : this->lightsMap)
	{
		this->lightCaster->removeLight(lightIdPair.first, lightIdPair.second, tileSize, chunkSize, chunkMap);
		delete lightIdPair.second;
	}
	this->lightsMap.clear();
}

void LightmapManager::removeLight(int lightId)
{
	Light* light = lightsMap[lightId];
	this->lightCaster->removeLight(lightId, light, tileSize, chunkSize, chunkMap);
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
			lightCaster->update(lightId, light, tileSize, chunkSize, chunkMap);
			light->shouldUpdate = false;
		}
	}
}