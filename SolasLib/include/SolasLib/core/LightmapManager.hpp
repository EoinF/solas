#pragma once

#include <stdio.h>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <memory>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <SolasLib/core/TileLightState.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <SolasLib/core/Light.hpp>

chunk_index_t _getChunkIndex(std::int64_t x, std::int64_t y, std::int64_t maxChunks);

TileLightState &getTileFast(std::int64_t tileX, std::int64_t tileY, std::int64_t chunkSize,
							std::int64_t maxChunks, ChunkMap &chunkMap);

class LightmapManager {
  private:
	void allocateChunksForLight(std::int64_t x, std::int64_t y, float range);
	void allocateChunk(chunk_index_t chunkIndex);
	std::vector<TileLightState> &getOrAllocateChunk(chunk_index_t chunkIndex);

  protected:
	ChunkMap chunkMap;
	std::int64_t tileSize;
	std::map<light_id_t, std::unique_ptr<Light>> lightsMap;
	std::int64_t chunkSize;
	std::int64_t maxChunks;
	int nextLightId;
	bool isPaused;
	std::unique_ptr<LightCaster> lightCaster;
	TileLightState &_getTileState(std::int64_t x, std::int64_t y);

  public:
	LightmapManager(std::int64_t tileSize, CastingAlgorithm type, std::int64_t chunkSize);

	const TileLightState &getTileState(std::int64_t x, std::int64_t y);

	void setTileSize(int tileSize) { this->tileSize = tileSize; }

	std::map<light_id_t, std::unique_ptr<Light>> &getLightsMap() { return this->lightsMap; }

	light_id_t addLight(std::int64_t x, std::int64_t y, float span, float range,
						glm::vec2 direction = glm::vec2(1, 0), int brightness = 100);
	void updateLight(light_id_t lightId, std::int64_t x, std::int64_t y, float span,
					 glm::vec2 direction = glm::vec2(1, 0));
	void updateTile(int tileX, int tileY, bool isWall);
	void clearLights();
	void clearTileState();
	void removeLight(light_id_t lightId);
	void resetLighting();
	void update();
};
