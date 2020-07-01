#pragma once
#include <SolasLib/core/Light.hpp>

enum class CastingAlgorithm 
{
	BOUND_RAY_CAST,
	SIMPLE_RAY_CAST
};

class LightCaster
{
public:
	virtual void update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) abstract;
};