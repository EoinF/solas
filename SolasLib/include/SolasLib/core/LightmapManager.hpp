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

class LightmapManager {
  private:
	void allocateChunksForLight(std::int64_t x, std::int64_t y, float range);

  protected:
	std::map<light_id_t, std::unique_ptr<Light>> lightsMap;
	int nextLightId;
	bool isPaused;
	std::unique_ptr<LightCaster> lightCaster;

  public:
	ChunkMap chunkMap;
	LightmapManager(std::int64_t tileSize, CastingAlgorithm type, std::int64_t chunkSize);

	const TileLightState &getTileState(std::int64_t x, std::int64_t y);

	std::map<light_id_t, std::unique_ptr<Light>> &getLightsMap() { return this->lightsMap; }

	light_id_t addLight(std::int64_t x, std::int64_t y, float span, float range,
						glm::vec2 direction = glm::vec2(1, 0), int brightness = 100);
	void updateLightPosition(light_id_t lightId, std::int64_t x, std::int64_t y);
	void updateLightSpan(light_id_t lightId, float span);
	void updateLightDirection(light_id_t lightId, glm::vec2 direction);
	void updateTile(int tileX, int tileY, bool isWall);
	void clearLights();
	void clearTileState();
	void removeLight(light_id_t lightId);
	void resetLighting();
	void update();
};
