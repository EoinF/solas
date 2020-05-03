#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

const int TILE_BUFFER = 3;

class Light
{
public:
	float x, y;
	float range;
	glm::vec2 direction;
	float span;
	bool shouldUpdate;
	int* lightMap;
	int lightMapWidth;
	int lightMapHeight;
	Light(float x, float y, glm::vec2 direction, float span, float range, int tileSize)
	{
		this->x = x;
		this->y = y;
		this->direction = direction;
		this->span = span;
		this->range = range;
		this->shouldUpdate = true;

		int radiusInTiles = (int)(range / (float)tileSize);
		this->lightMapWidth = TILE_BUFFER * 2 + radiusInTiles * 2;
		this->lightMapHeight = TILE_BUFFER * 2 + radiusInTiles * 2;

		this->lightMap = new int[lightMapWidth * lightMapHeight]();
	}
};
