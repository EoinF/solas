#pragma once

#include <vector>
#include <map>
#include <set>
#include <SolasLib/core/Light.hpp>

enum class CastingAlgorithm { BOUND_RAY_CAST, SIMPLE_RAY_CAST };

typedef int light_id_t;
typedef std::int64_t chunk_index_t;
typedef std::map<chunk_index_t, std::vector<TileLightState>> ChunkMap;

class LightCaster {
  public:
	virtual const std::set<int> &getAffectedLights(std::int64_t tileX, std::int64_t tileY,
												   std::int64_t tileSize, std::int64_t chunkSize,
												   std::int64_t maxChunks) = 0;
	virtual void removeLight(int lightId, Light &light, std::int64_t tileSize,
							 std::int64_t chunkSize, std::int64_t maxChunks,
							 ChunkMap &chunkMap) = 0;
	virtual void update(int lightId, Light &light, std::int64_t tileSize, std::int64_t chunkSize,
						std::int64_t maxChunks, ChunkMap &chunkMap) = 0;
};
