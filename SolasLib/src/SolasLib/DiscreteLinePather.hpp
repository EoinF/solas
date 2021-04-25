#pragma once
#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>

class DiscreteLinePather {
  private:
	bool isSteep;
	int deltaX;
	int deltaY;
	float error;
	int currentX, currentY;
	int destX, destY;

	int xstep;
	int ystep;

  public:
	bool isFinished;
	DiscreteLinePather(int srcTileX, int srcTileY, int destX, int destY);
	glm::ivec2 nextTile();
};
