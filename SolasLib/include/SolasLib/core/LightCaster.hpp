#pragma once

#include <vector>
#include <map>
#include <set>
#include <SolasLib/core/Light.hpp>
#include <SolasLib/core/ChunkMap.hpp>

enum class CastingAlgorithm { BOUND_RAY_CAST, SIMPLE_RAY_CAST };

typedef int light_id_t;

class LightCaster {
  public:
	virtual std::set<int> getAffectedLights(std::int64_t tileX, std::int64_t tileY,
											const ChunkMap &chunkMap) = 0;
	virtual void removeLight(int lightId, Light &light, ChunkMap &chunkMap) = 0;
	virtual void update(int lightId, Light &light, ChunkMap &chunkMap) = 0;
};
