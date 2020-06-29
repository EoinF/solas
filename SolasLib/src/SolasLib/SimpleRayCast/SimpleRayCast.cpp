#include "SimpleRayCast.hpp"

void SimpleRayCast::update(int tileSize, Light* light, int floorGridWidth, int floorGridHeight, std::vector<TileLightState> & tileArray)
{
	float tileSizeF = (float)tileSize;
	float spanDifference;
	int srcTileX = (int)(light->x / tileSizeF);
	int srcTileY = (int)(light->y / tileSizeF);

	int startX = (int)glm::floor((light->x - light->range) / tileSizeF);
	int startY = (int)glm::floor((light->y - light->range) / tileSizeF);
	int endX = (int)glm::ceil((light->x + light->range) / tileSizeF);
	int endY = (int)glm::ceil((light->y + light->range) / tileSizeF);

	endX = glm::min(endX, floorGridWidth - 1);
	endY = glm::min(endY, floorGridHeight - 1);
	startX = glm::max(startX, 0);
	startY = glm::max(startY, 0);

	//
	// Reset the lightcast mask for this light
	//
	for (int i = 0; i < light->lightMapWidth; i++)
	{
		for (int j = 0; j < light->lightMapHeight; j++)
		{
			int brightness = light->lightMap[i + j * light->lightMapWidth];

			int x = -light->lightMapWidth / 2 + i + (int)(light->x / tileSizeF);
			int y = -light->lightMapHeight / 2 + j + (int)(light->y / tileSizeF);

			if (x >= 0 && y >= 0 && x < floorGridWidth && y < floorGridHeight)
			{
				tileArray[y * floorGridWidth + x].subtractLighting(255, 255, 255, brightness);
			}
			light->lightMap[i + j * light->lightMapWidth] = 0;
		}
	}
	//
	//Use the raycast algorithm on the outer edges of the ray source
	//
	for (int i = startX; i <= endX; i++)
	{
		spanDifference = getSpanDifference(light, i, startY, tileSize);
		if (spanDifference > 0)
		{
			rayCast(srcTileX, srcTileY, i, startY, light->range, spanDifference, light, tileSize, tileArray, floorGridWidth);
		}
		spanDifference = getSpanDifference(light, i, endY, tileSize);
		if (spanDifference > 0)
		{
			rayCast(srcTileX, srcTileY, i, endY, light->range, spanDifference, light, tileSize, tileArray, floorGridWidth);
		}
	}

	for (int j = startY + 1; j < endY; j++)
	{
		spanDifference = getSpanDifference(light, startX, j, tileSize);
		if (spanDifference > 0)
		{
			rayCast(srcTileX, srcTileY, startX, j, light->range, spanDifference, light, tileSize, tileArray, floorGridWidth);
		}
		spanDifference = getSpanDifference(light, endX, j, tileSize);
		if (spanDifference > 0)
		{
			rayCast(srcTileX, srcTileY, endX, j, light->range, spanDifference, light, tileSize, tileArray, floorGridWidth);
		}
	}
	light->shouldUpdate = false;
}