#pragma once

#include <stdio.h>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <SolasLib/core/TileLightState.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <SolasLib/core/Light.hpp>

// The max number of chunks in the x direction
const long MAX_CHUNKS_X = 2048;

long getChunkIndex(int x, int y);
TileLightState &getTileFast(int tileX, int tileY, int chunkSize, ChunkMap &chunkMap);

class LightmapManager
{
private:
	void allocateChunksForLight(float x, float y, float range);
	void allocateChunk(int chunkIndex);
	std::vector<TileLightState> &getOrAllocateChunk(int chunkIndex);

protected:
	ChunkMap chunkMap;
	int tileSize;
	std::map<int, std::unique_ptr<Light>> lightsMap;
	int chunkSize;
	int nextLightId;
	bool isPaused;
	std::unique_ptr<LightCaster> lightCaster;
	TileLightState &_getTileState(int x, int y);

public:
	LightmapManager(int tileSize, CastingAlgorithm type, int chunkSize);

	std::vector<TileLightState> &getTileArray()
	{
		return this->chunkMap[getChunkIndex(0, 0)];
	}

	const TileLightState &getTileState(int x, int y);

	void setTileSize(int tileSize)
	{
		this->tileSize = tileSize;
	}

	std::map<int, std::unique_ptr<Light>> &getLightsMap()
	{
		return this->lightsMap;
	}

	int addLight(float x, float y, float span, float range, glm::vec2 direction = glm::vec2(1, 0), int brightness = 100);
	void updateLight(int lightId, float x, float y, float span, glm::vec2 direction = glm::vec2(1, 0));
	void updateTile(int tileX, int tileY, bool isWall);
	void clearLights();
	void clearTileState();
	void removeLight(int lightId);
	void resetLighting();
	void update();
};
