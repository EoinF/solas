#pragma once
#include <SolasLib/core/Light.hpp>
#include <SolasLib/core/LightmapManager.hpp>

enum class CastingAlgorithm 
{
	BOUND_RAY_CAST,
	SIMPLE_RAY_CAST
};

typedef std::map<int, std::vector<TileLightState>> ChunkMap;

class LightCaster
{
public:
	virtual void removeLight(int lightId, Light* light, int tileSize, int chunkSize, ChunkMap& chunkMap) abstract;
	virtual void update(int lightId, Light* light, int tileSize, int chunkSize, ChunkMap& chunkMap) abstract;
};