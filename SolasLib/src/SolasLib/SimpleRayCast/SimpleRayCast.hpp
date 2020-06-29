#pragma once
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <glm/glm.hpp>

class SimpleRayCast : public LightCaster {
	void update(int tileSize, Light* light, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) override;
};