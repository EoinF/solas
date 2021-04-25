#pragma once

#include <vector>
#include <map>
#include <set>
#include <SolasLib/core/Light.hpp>

enum class CastingAlgorithm { BOUND_RAY_CAST, SIMPLE_RAY_CAST };

typedef std::map<int, std::vector<TileLightState>> ChunkMap;

class LightCaster {
  public:
	virtual const std::set<int> &getAffectedLights(int tileX, int tileY, int tileSize,
												   int chunkSize) = 0;
	virtual void removeLight(int lightId, Light &light, int tileSize, int chunkSize,
							 ChunkMap &chunkMap) = 0;
	virtual void update(int lightId, Light &light, int tileSize, int chunkSize,
						ChunkMap &chunkMap) = 0;
};
