#include "BoundRayCast.hpp"

void BoundRayCast::update(int tileSize, Light* light, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray)
{
	float tileSizeF = (float)tileSize;
	float spanDifference;
	int srcTileX = (int)(light->x / tileSizeF);
	int srcTileY = (int)(light->y / tileSizeF);

	int startX = (int)((light->x - light->range) / tileSizeF);
	int startY = (int)((light->y - light->range) / tileSizeF);
	int endX = (int)((light->x + light->range) / tileSizeF);
	int endY = (int)((light->y + light->range) / tileSizeF);

	endX = glm::min(endX, floorGridWidth - 1);
	endY = glm::min(endY, floorGridHeight - 1);
	startX = glm::max(startX, 0);
	startY = glm::max(startY, 0);

	int newLighting = 255;

	for (int i = startX; i <= endX; i++)
	{
		for (int j = startY; j <= endY; j++)
		{
			/*int x = light->lightMapWidth / 2 + i - (int)(light->x / tileSize);
			int y = light->lightMapHeight / 2 + j - (int)(light->y / tileSize);
			light->lightMap[x + y * light->lightMapWidth] = newLighting;*/
			tileArray[j * floorGridWidth + i].addLighting(255, 255, 255, newLighting);
		}
	}
}