#pragma once

#include <vector>
#include <map>
#include <set>
#include <SolasLib/core/Light.hpp>
#include <SolasLib/core/ChunkMap.hpp>

enum class CastingAlgorithm { BOUND_RAY_CAST, SHADOW_CAST };

typedef int light_id_t;

class LightCaster {
  public:
	LightCaster(std::int64_t tileSize) {}
	virtual std::set<int> getAffectedLights(std::int64_t tileX, std::int64_t tileY,
											const ChunkMap &chunkMap) = 0;
	virtual void addLight(light_id_t lightId, Light &light, ChunkMap &chunkMap) = 0;
	virtual void removeLight(light_id_t lightId, Light &light, ChunkMap &chunkMap) = 0;
	virtual void update(light_id_t lightId, Light &light, ChunkMap &chunkMap) = 0;

	virtual ~LightCaster(){};
};
