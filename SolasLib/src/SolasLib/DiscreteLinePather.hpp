#pragma once

struct TileLocation
{
	TileLocation(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};


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
	TileLocation nextTile();
};