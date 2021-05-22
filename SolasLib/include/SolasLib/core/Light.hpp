#pragma once
#include <memory>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

const int TILE_BUFFER = 3;

class Light {
  public:
	std::int64_t x, y;
	std::int64_t srcTileX, srcTileY;
	float range;
	std::int64_t rangeInTiles;
	glm::vec2 direction;
	float span;
	bool shouldUpdate;
	int brightness;

	Light(std::int64_t x, std::int64_t y, glm::vec2 direction, float span, float range,
		  int brightness, int tileSize) {

		this->tileSize = tileSize;
		setPosition(x, y);
		this->direction = direction;
		this->span = span;
		this->range = range;
		this->brightness = brightness;
		this->shouldUpdate = true;

		this->rangeInTiles = static_cast<int>(range / tileSize);
	}

	void setPosition(std::int64_t x, std::int64_t y) {
		this->x = x;
		this->y = y;
		this->srcTileX = x / tileSize;
		this->srcTileY = y / tileSize;
	}

  private:
	std::int64_t tileSize;
};
