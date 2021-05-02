#pragma once
#include <memory>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <SolasLib/core/ChunkMap.hpp>
#include "../DiscreteLinePather.hpp"

struct BoundRayCastNode {
	explicit BoundRayCastNode(int x = 0, int y = 0, int brightness = 0) {
		this->location.x = x;
		this->location.y = y;
		this->brightness = brightness;
	}
	std::vector<glm::vec2> directionsToNode;
	glm::ivec2 location;
	std::map<int, std::unique_ptr<BoundRayCastNode>>
		children; // Maps each tile index to the next dependent tile
	int brightness;
};

struct BoundLight {
	BoundLight() : BoundLight(0, 0, 1, 0.0f, glm::vec2(0, 0), 0) {}
	BoundLight(std::int64_t srcX, std::int64_t srcY, int halfCastingMapWidth, float span,
			   glm::vec2 direction, int brightness) {
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

		this->lightMap = std::make_unique<int[]>(castingMapWidth * castingMapWidth);
	}
	std::unique_ptr<int[]> lightMap;
	std::int64_t srcX, srcY;
	BoundRayCastNode dependencyTreeRoot;
	float span;
	glm::vec2 direction;
	int brightness;
	int halfCastingMapWidth;
	int castingMapWidth;
};

struct Rectangle {
	std::int64_t x, y, width, height;
	Rectangle(std::int64_t x, std::int64_t y, std::int64_t width, std::int64_t height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	bool containsPoint(std::int64_t pointX, std::int64_t pointY) {
		return pointX > this->x && pointX < this->x + this->width && pointY > this->y &&
			   pointY < this->y + height;
	}
};

typedef std::vector<std::pair<Rectangle, light_id_t>> RegionsToLightIds;

class BoundRayCast : public LightCaster {
  public:
	std::map<int, std::unique_ptr<BoundLight>> boundLightMap;
	RegionsToLightIds regionsToLightIds;

	std::set<light_id_t> getAffectedLights(std::int64_t tileX, std::int64_t tileY,
										   const ChunkMap &chunkMap) override;
	void removeLight(int lightId, Light &light, ChunkMap &chunkMap) override;
	void update(int lightId, Light &light, ChunkMap &chunkMap) override;
};
