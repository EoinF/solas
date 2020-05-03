#include <SolasLib/core/LightmapManager.hpp>
#include <SolasLib/SimpleRayCast/RayCast.hpp>
#include "stdio.h"
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

using namespace std;

int LightmapManager::addLight(float x, float y, glm::vec2 direction, float span, float range, int tileSize)
{
	int lightId = nextLightId++;
	this->lightsMap.insert(pair<int, Light*>(lightId, new Light(x, y, direction, span, range, tileSize)));
	this->lightsMap[lightId]->shouldUpdate = true;
	return lightId;
}

void LightmapManager::updateLight(int lightId, float x, float y, glm::vec2 direction, float span, float range)
{
	this->lightsMap[lightId]->x = x;
	this->lightsMap[lightId]->y = y;
	this->lightsMap[lightId]->direction = direction;
	this->lightsMap[lightId]->span = span;
	this->lightsMap[lightId]->range = range;
	this->lightsMap[lightId]->shouldUpdate = true;
}

void LightmapManager::clearLights()
{
	for (auto idLightPair : this->lightsMap)
	{
		this->removeLight(idLightPair.first, idLightPair.second);
		delete idLightPair.second;
	}
	this->lightsMap.clear();
}


void LightmapManager::removeLight(int lightId, Light* light)
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

			if (x >= 0 && y >= 0 && x < floorGridWidth && y < floorGridHeight) {
				tileArray[y * floorGridWidth + x].subtractLighting(255, 255, 255, brightness);
			}
			light->lightMap[i + j * light->lightMapWidth] = 0;
		}
	}
}

void LightmapManager::resetLighting()
{
	for (auto& lightIdPair : this->lightsMap)
	{
		for (int i = 0; i < lightIdPair.second->lightMapWidth; i++)
		{
			for (int j = 0; j < lightIdPair.second->lightMapHeight; j++)
			{
				lightIdPair.second->lightMap[i + j * lightIdPair.second->lightMapHeight] = 0;
			}
		}
	}
}

void LightmapManager::update()
{
	float tileSizeF = (float)tileSize;
	float spanDifference;
	for (auto idLightPair : this->lightsMap)
	{
		int lightId = idLightPair.first;
		Light* light = idLightPair.second;
		if (light->shouldUpdate) {
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

					int x = -light->lightMapWidth / 2 + i + (int)(light->x / tileSizeF);
					int y = -light->lightMapHeight / 2 + j + (int)(light->y / tileSizeF);

					if (x >= 0 && y >= 0 && x < floorGridWidth && y < floorGridHeight) {
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
	}
}