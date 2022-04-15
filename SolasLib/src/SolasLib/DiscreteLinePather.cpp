#include "DiscreteLinePather.hpp"

DiscreteLinePather::DiscreteLinePather(int srcTileX, int srcTileY, int destX, int destY) {
	//
	// Check each square from the source tile to the destination tile until a blocking square is
	// found; using the Bresenham line algorithm
	//
	isSteep = abs(destY - srcTileY) > abs(destX - srcTileX);
	if (isSteep) {
		int temp = srcTileX;
		srcTileX = srcTileY;
		srcTileY = temp;

		temp = destX;
		destX = destY;
		destY = temp;
	}

	deltaX = abs(destX - srcTileX);
	deltaY = abs(destY - srcTileY);

	error = deltaX * 0.5f;

	xstep = srcTileX > destX ? -1 : 1;
	ystep = srcTileY > destY ? -1 : 1;

	currentX = srcTileX;
	currentY = srcTileY;
	this->destX = destX;
	this->destY = destY;

	isFinished = currentX == destX;
}

glm::ivec2 DiscreteLinePather::nextTile() {
	if (currentX == destX) {
		throw std::runtime_error("Pathing is already finished. Check with 'isFinished'");
	} else {
		error -= deltaY;

		if (error < 0) {
			currentY += ystep;
			error += deltaX;
		}
	}

	glm::ivec2 nextTileLocation(currentX, currentY);
	currentX += xstep;
	nextTileLocation.x = currentX;
	if (isSteep) {
		nextTileLocation.x = currentY;
		nextTileLocation.y = currentX;
	}

	isFinished = currentX == destX;

	return nextTileLocation;
}
