#pragma once
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <glm/glm.hpp>

struct BoundRayCastNode {
	BoundRayCastNode(int x = 0, int y = 0) {
		this->location.x = x;
		this->location.y = y;
	}
	glm::ivec2 location;
	std::map<int, BoundRayCastNode*> children; // Maps each tile index to the next dependent tile

	~BoundRayCastNode() {
		for (auto child : children) {
			delete child.second;
		}
	}
};

struct BoundLight {
	BoundLight() : BoundLight(0, 0, 1) {};
	BoundLight(int srcX, int srcY, int halfCastingMapWidth) {
		this->srcX = srcX;
		this->srcY = srcY;
		this->halfCastingMapWidth = halfCastingMapWidth;
		this->castingMapWidth = halfCastingMapWidth * 2;

		this->dependencyTreeRoot.location.x = 0;
		this->dependencyTreeRoot.location.y = 0;

		this->lightMap = new int[castingMapWidth * castingMapWidth]();
	}
	int * lightMap;
	int srcX, srcY;
	BoundRayCastNode dependencyTreeRoot;
	int halfCastingMapWidth;
	int castingMapWidth;

	~BoundLight() {
		delete[] lightMap;
	}
};

class BoundRayCast : public LightCaster {
	std::map<int, BoundLight*> boundLightMap;

	void removeLight(int lightId, Light* light, int tileSize, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) override;
	void update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray) override;
};