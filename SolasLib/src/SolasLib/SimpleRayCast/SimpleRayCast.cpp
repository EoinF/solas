#include "SimpleRayCast.hpp"
#include "../DiscreteLinePather.hpp"
#include "../RayCastUtils.hpp"

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY,
	float tileSize, Light* light,
	int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray);

void SimpleRayCast::update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray)
{
	float tileSizeF = (float)tileSize;
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
				tileArray[y * floorGridWidth + x].subtractLighting(brightness);
			}
			light->lightMap[i + j * light->lightMapWidth] = 0;
		}
	}

	//
	//Use the raycast algorithm on the outer edges of the ray source
	//
	for (int i = startX; i <= endX; i++)
	{
		rayCastOne(srcTileX, srcTileY, i, startY, tileSizeF, light, floorGridWidth, floorGridHeight, tileArray);
		rayCastOne(srcTileX, srcTileY, i, endY, tileSizeF, light, floorGridWidth, floorGridHeight, tileArray);
	}

	for (int j = startY + 1; j < endY; j++)
	{
		rayCastOne(srcTileX, srcTileY, startX, j, tileSizeF, light, floorGridWidth, floorGridHeight, tileArray);
		rayCastOne(srcTileX, srcTileY, endX, j, tileSizeF, light, floorGridWidth, floorGridHeight, tileArray);
	}
	light->shouldUpdate = false;
}

void SimpleRayCast::removeLight(int lightId, Light * light, int tileSize, int floorGridWidth, int floorGridHeight, std::vector<TileLightState> & tileArray)
{
	float tileSizeF = (float)tileSize;
	int srcTileX = (int)(light->x / tileSizeF);
	int srcTileY = (int)(light->y / tileSizeF);

	int startX = (int)(-0.5 + (light->x - light->range) / tileSizeF);
	int startY = (int)(-0.5 + (light->y - light->range) / tileSizeF);
	int endX = (int)(0.5 + (light->x + light->range) / tileSizeF);
	int endY = (int)(0.5 + (light->y + light->range) / tileSizeF);

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

			int x = i + (int)(light->x / tileSizeF) - light->lightMapWidth / 2;
			int y = j + (int)(light->y / tileSizeF) - light->lightMapHeight / 2;

			if (x >= 0 && y >= 0 && x < floorGridWidth && y < floorGridHeight)
			{
				tileArray[y * floorGridWidth + x].subtractLighting(brightness);
			}
			light->lightMap[i + j * light->lightMapWidth] = 0;
		}
	}
}

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY,
	float tileSize, Light* light,
	int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray)
{
	float spanDifference = getSpanDifference(light, toTileX, toTileY, tileSize);
	if (spanDifference > 0)
	{
		DiscreteLinePather pather(fromTileX, fromTileY, toTileX, toTileY);
		while (!pather.isFinished)
		{
			auto nextTile = pather.nextTile();

			float distanceFromSrc = glm::length(glm::vec2(nextTile.x * tileSize - light->x, nextTile.y * tileSize - light->y));

			if (distanceFromSrc < light->range)
			{
				int x = light->lightMapWidth / 2 + nextTile.x - (int)(light->x / tileSize);
				int y = light->lightMapHeight / 2 + nextTile.y - (int)(light->y / tileSize);
				int newLighting = (int)(255.0f *
					(
						glm::min(1.0f, 0.0f + spanDifference) * (1.0f - (distanceFromSrc / light->range))
					)
				);

				int existingLighting = light->lightMap[x + y * light->lightMapWidth];
				if (newLighting > existingLighting)
				{
					light->lightMap[x + y * light->lightMapWidth] = newLighting;
					int lightingDelta = newLighting - existingLighting;

					tileArray[nextTile.y * floorGridWidth + nextTile.x].addLighting(lightingDelta);
				}
			}
			if (tileArray[nextTile.y * floorGridWidth + nextTile.x].isWall)
			{
				return;
			}
		}
	}
}