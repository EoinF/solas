#pragma once
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <glm/glm.hpp>

struct BoundLight {
	int srcX, srcY;
	int* castingMap; // Used to precalculate the effect of the light
	int halfCastingMapWidth;
	int castingMapWidth;
};

class BoundRayCast : public LightCaster {
	std::map<int, BoundLight> boundLightMap;

	void update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) override;
};