#pragma once

enum class CastingAlgorithm 
{
	BOUND_RAY_CAST,
	SIMPLE_RAY_CAST
};

class LightCaster
{
public:
	virtual void update(int tileSize, Light* light, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) abstract;
};