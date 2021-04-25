#include "RayCastUtils.hpp"

float getSpanDifference(Light &light, int tileX, int tileY, float tileSize) {
	// Get the offset from the tile to the centre of the raycaster
	// Note: The y coordinate is negated because the formula on the next line works with a
	// coordinate system in which the y axis points down, whereas with XNA, the y axis points up
	glm::vec2 voffset = glm::vec2((tileX * tileSize) - light.x, (tileY * tileSize) - light.y);

	// Calculate the angle between the direction and this offset
	float angle = glm::dot(light.direction, voffset);

	// If the angle is small enough this tile is within the span of the light
	return (light.span - abs(angle)) / light.span;
}
