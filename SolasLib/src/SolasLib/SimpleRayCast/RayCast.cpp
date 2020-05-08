#include <SolasLib/SimpleRayCast/RayCast.hpp>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

bool rayCast(int srcX, int srcY, int destX, int destY, float range, float spanDifference, Light* light, int tileSize, std::vector<TileLightState>& tileArray, int floorGridWidth)
{
	//glm::vec2 currentRayDirection = glm::vec2(destX - srcX, destY - srcY);
	//
	//Check each square from the source tile to the destination tile until a blocking square is found; using the Bresenham line algorithm
	//
	bool isSteep = abs(destY - srcY) > abs(destX - srcX);
	if (isSteep)
	{
		int temp = srcX;
		srcX = srcY;
		srcY = temp;

		temp = destX;
		destX = destY;
		destY = temp;
	}
	//cout << "raycasting from " << srcX << ", " << srcY << " to " << destX << ", " << destY << endl;

	int deltaX = abs(destX - srcX);
	int deltaY = abs(destY - srcY);

	float error = deltaX * 0.5f;
	int y = srcY;

	int xstep = srcX > destX ? -1 : 1;
	int ystep = srcY > destY ? -1 : 1;

	for (int x = srcX; x != destX; x += xstep)
	{
		int i;
		int j;
		if (isSteep)
		{
			i = y;
			j = x;
		}
		else
		{
			i = x;
			j = y;
		}

		//Perform the action that was a parameter of this method.
		//Return false if we want to terminate the raycast early
		float distanceFromSrc = 0;
		if (isSteep) {
			distanceFromSrc = tileSize * glm::length(glm::vec2(i - srcY, j - srcX));
		}
		else {
			distanceFromSrc = tileSize * glm::length(glm::vec2(i - srcX, j - srcY));
		}
		if (distanceFromSrc < range)
		{
			int x = light->lightMapWidth / 2 + i - (int)(light->x / tileSize);
			int y = light->lightMapHeight / 2 + j - (int)(light->y / tileSize);
			int newLighting = (int)(255.0f * 
				(
					glm::min(1.0f, 0.0f + spanDifference) * (1.0f - (distanceFromSrc / range))
				)
			);

			int existingLighting = light->lightMap[x + y * light->lightMapWidth];
			if (newLighting > existingLighting)
			{
				light->lightMap[x + y * light->lightMapWidth] = newLighting;
				int lightingDelta = newLighting - existingLighting;

				tileArray[j * floorGridWidth + i].addLighting(255, 255, 255, lightingDelta);
			}
		}
		if (tileArray[j * floorGridWidth + i].isWall) {
			return false;
		}

		error -= deltaY;

		if (error < 0)
		{
			y += ystep;
			error += deltaX;
		}
	}
	return true;
}

float getSpanDifference(Light* light, int tileX, int tileY, int tileSize)
{
	//Get the offset from the tile to the centre of the raycaster
	//Note: The y coordinate is negated because the formula on the next line works with a coordinate system
	//in which the y axis points down, whereas with XNA, the y axis points up
	glm::vec2 voffset = glm::vec2((tileX * tileSize) - light->x, (tileY * tileSize) - light->y);

	//Calculate the angle between the direction and this offset
	float angle = glm::acos(
		glm::dot<2, float, glm::qualifier::highp>(voffset, light->direction) / (glm::length(voffset) * glm::length(light->direction))
	);

	//If the angle is small enough this tile is within the span of the light
	return (light->span - angle) / light->span;
}