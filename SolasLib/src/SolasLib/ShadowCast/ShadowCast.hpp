#pragma once
#include <stdio.h>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>

struct Slope // represents the slope Y/X as a rational number
{
  public:
	Slope(int y, int x) {
		this->x = x;
		this->y = y;
	}
	int x, y;
};

typedef std::map<light_id_t, std::unique_ptr<int[]>> ShadowCastLightMap;

class ShadowCast : public LightCaster {
	ShadowCastLightMap lightMap;
	std::set<light_id_t> getAffectedLights(std::int64_t tileX, std::int64_t tileY,
										   const ChunkMap &chunkMap) override;
	void removeLight(light_id_t lightId, Light &light, ChunkMap &chunkMap) override;
	void update(light_id_t lightId, Light &light, ChunkMap &chunkMap) override;
};
