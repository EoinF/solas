#pragma once
#include <stdio.h>
#include <iostream>
#include <set>
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

class ShadowCast : public LightCaster {
	std::set<int> getAffectedLights(std::int64_t tileX, std::int64_t tileY,
									const ChunkMap &chunkMap) override;
	void removeLight(int lightId, Light &light, ChunkMap &chunkMap) override;
	void update(int lightId, Light &light, ChunkMap &chunkMap) override;
};
