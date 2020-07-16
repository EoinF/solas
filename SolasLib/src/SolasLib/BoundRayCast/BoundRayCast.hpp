#pragma once
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <glm/glm.hpp>

struct BoundRayCastNode {
	BoundRayCastNode(int x = 0, int y = 0, int brightness = 0) {
		this->location.x = x;
		this->location.y = y;
		this->brightness = brightness;
	}
	std::vector<glm::vec2> directionsToNode;
	glm::ivec2 location;
	std::map<int, BoundRayCastNode*> children; // Maps each tile index to the next dependent tile
	int brightness;

	~BoundRayCastNode() {
		for (auto child : children) {
			delete child.second;
		}
	}
};

struct BoundLight {
	BoundLight() : BoundLight(0, 0, 1, 0.0f, glm::vec2(0, 0), 0) {};
	BoundLight(int srcX, int srcY, int halfCastingMapWidth, float span, glm::vec2 direction, int brightness) {
		this->srcX = srcX;
		this->srcY = srcY;
		this->halfCastingMapWidth = halfCastingMapWidth;
		this->span = span;
		this->direction = direction;
		this->brightness = brightness;
		this->castingMapWidth = halfCastingMapWidth * 2;

		this->dependencyTreeRoot.location.x = 0;
		this->dependencyTreeRoot.location.y = 0;
		this->dependencyTreeRoot.brightness = brightness;

		this->lightMap = new int[castingMapWidth * castingMapWidth]();
	}
	int * lightMap;
	int srcX, srcY;
	BoundRayCastNode dependencyTreeRoot;
	float span;
	glm::vec2 direction;
	int brightness;
	int halfCastingMapWidth;
	int castingMapWidth;

	~BoundLight() {
		delete[] lightMap;
	}
};

class BoundRayCast : public LightCaster {
	std::map<int, BoundLight*> boundLightMap;

	void removeLight(int lightId, Light* light, int tileSize, int chunkSize, ChunkMap& chunkMap) override;
	void update(int lightId, Light* light, int tileSize, int chunkSize, ChunkMap& chunkMap) override;
};