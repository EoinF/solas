#pragma once
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <glm/glm.hpp>

class SimpleRayCast : public LightCaster {
	void removeLight(int lightId, Light* light, int tileSize, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) override;
	void update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) override;
};