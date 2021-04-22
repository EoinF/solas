#pragma once
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <glm/glm.hpp>

class SimpleRayCast : public LightCaster
{
	const std::set<int> &getAffectedLights(int tileX, int tileY, int tileSize, int chunkSize) override;
	void removeLight(int lightId, Light &light, int tileSize, int chunkSize, ChunkMap &chunkMap) override;
	void update(int tileSize, Light &light, int lightId, int chunkSize, ChunkMap &chunkMap) override;
};