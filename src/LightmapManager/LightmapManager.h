#pragma once
#include <stdio.h>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

using namespace std;

const int TILE_BUFFER = 3;

class IGameObject
{
public:
	float x = 0;
	float y = 0;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	void addLighting(float r, float g, float b) {
		this->r += r;
		this->g += g;
		this->b += b;
	}
	void subtractLighting(float r, float g, float b) {
		this->r -= r;
		this->g -= g;
		this->b -= b;
	}
	void setLighting(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	bool isWall = false;
};
struct Colour {
	float r;
	float g;
	float b;
	Colour() {
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
};

class Light
{
public:
	float x, y;
	float range;
	glm::vec2 direction;
	float span;
	bool shouldUpdate;
	float* lightMap;
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
		
		this->lightMap = new float[lightMapWidth * lightMapHeight]();
	}
};


// Hint: T must derive from IGameObject
template <typename T>
class LightmapManager
{
protected:
	vector <T> staticObjects;
	vector <T> dynamicObjects;
	T** floorGrid;
	int tileSize;
	map<int, Light*> lightsMap;
	int nextLightId;
	int floorGridWidth;
	int floorGridHeight;
	bool isPaused;
public:
	LightmapManager(T** floorGrid, int width, int height, int tileSize)
	{
		this->isPaused = false;
		this->nextLightId = 0;
		this->tileSize = tileSize;
		this->floorGrid = floorGrid;
		this->floorGridWidth = width;
		this->floorGridHeight = height;
	}

	void setTileSize(int tileSize) {
		this->tileSize = tileSize;
	}

	void setStaticObjects(vector<T> staticObjects)
	{
		this->staticObjects = staticObjects;
		for (auto& staticObject: staticObjects) {
			staticObject.setLighting(0.0);
		}
	}

	int addLight(float x, float y, glm::vec2 direction, float span, float range, int tileSize)
	{
		int lightId = nextLightId++;
		this->lightsMap.insert(pair<int, Light*>(lightId, new Light(x, y, direction, span, range, tileSize)));
		this->lightsMap[lightId]->shouldUpdate = true;
		return lightId;
	}

	void updateLight(int lightId, float x, float y, glm::vec2 direction, float span, float range)
	{
		this->lightsMap[lightId]->x = x;
		this->lightsMap[lightId]->y = y;
		this->lightsMap[lightId]->direction = direction;
		this->lightsMap[lightId]->span = span;
		this->lightsMap[lightId]->range = range;
		this->lightsMap[lightId]->shouldUpdate = true;
	}

	void clearLights()
	{
		for (auto idLightPair : this->lightsMap)
		{
			this->removeLight(idLightPair.first, idLightPair.second);
			delete idLightPair.second;
		}
		this->lightsMap.clear();
	}

	void unpause()
	{
		this->isPaused = false;
	}

	void pause()
	{
		this->isPaused = true;
	}

	void removeLight(int lightId, Light* light)
	{
		float tileSizeF = (float)tileSize;
		int srcTileX = (int)(light->x / tileSizeF);
		int srcTileY = (int)(light->y / tileSizeF);

		int startX = (int)(-0.5 + (light->x - light->range) / tileSizeF);
		int startY = (int)(-0.5 + (light->y - light->range) / tileSizeF);
		int endX = (int)(0.5 + (light->x + light->range) / tileSizeF);
		int endY = (int)(0.5 + (light->y + light->range) / tileSizeF);

		endX = min(endX, floorGridWidth - 1);
		endY = min(endY, floorGridHeight - 1);
		startX = max(startX, 0);
		startY = max(startY, 0);

		//
		// Reset the lightcast mask for this light
		//
		for (int i = 0; i < light->lightMapWidth; i++)
		{
			for (int j = 0; j < light->lightMapHeight; j++)
			{
				float brightness = light->lightMap[i + j * light->lightMapWidth];

				int x = i + (int)(light->x / tileSizeF) - light->lightMapWidth / 2;
				int y = j + (int)(light->y / tileSizeF) - light->lightMapHeight / 2;

				if (x >= 0 && y >= 0 && x < floorGridWidth && y < floorGridHeight) {
					floorGrid[y][x].subtractLighting(brightness, brightness, brightness);
				}
				light->lightMap[i + j * light->lightMapWidth] = 0.0f;
			}
		}
	}

	map<int, Light*> getLightsMap()
	{
		return this->lightsMap;
	}

	void resetLighting()
	{
		for (auto& lightIdPair : this->lightsMap) 
		{
			for (int i = 0; i < lightIdPair.second->lightMapWidth; i++)
			{
				for (int j = 0; j < lightIdPair.second->lightMapHeight; j++)
				{
					lightIdPair.second->lightMap[i + j * lightIdPair.second->lightMapHeight] = 0.0f;
				}
			}
		}
	}

	void update()
	{
		if (isPaused)
		{
			return;
		}
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

				endX = min(endX, floorGridWidth - 1);
				endY = min(endY, floorGridHeight - 1);
				startX = max(startX, 0);
				startY = max(startY, 0);

				//
				// Reset the lightcast mask for this light
				//
				for (int i = 0; i < light->lightMapWidth; i++)
				{
					for (int j = 0; j < light->lightMapHeight; j++)
					{
						float brightness = light->lightMap[i + j * light->lightMapWidth];

						int x = -light->lightMapWidth / 2 + i + (int)(light->x / tileSizeF);
						int y = -light->lightMapHeight / 2 + j + (int)(light->y / tileSizeF);

						if (x >= 0 && y >= 0 && x < floorGridWidth && y < floorGridHeight) {
							floorGrid[y][x].subtractLighting(brightness, brightness, brightness);
						}
						light->lightMap[i + j * light->lightMapWidth] = 0.0f;
					}
				}
				//
				//Use the raycast algorithm on the outer edges of the ray source
				//
				for (int i = startX; i <= endX; i++)
				{
					spanDifference = getSpanDifference(light, i, startY);
					if (spanDifference > 0)
					{
						rayCast(srcTileX, srcTileY, i, startY, light->range, spanDifference, light);
					}
					spanDifference = getSpanDifference(light, i, endY);
					if (spanDifference > 0)
					{
						rayCast(srcTileX, srcTileY, i, endY, light->range, spanDifference, light);
					}
				}

				for (int j = startY + 1; j < endY; j++)
				{
					spanDifference = getSpanDifference(light, startX, j);
					if (spanDifference > 0)
					{
						rayCast(srcTileX, srcTileY, startX, j, light->range, spanDifference, light);
					}
					spanDifference = getSpanDifference(light, endX, j);
					if (spanDifference > 0)
					{
						rayCast(srcTileX, srcTileY, endX, j, light->range, spanDifference, light);
					}
				}
				light->shouldUpdate = false;
			}
		}
	}

	bool rayCast(int srcX, int srcY, int destX, int destY, float range, float spanDifference, Light* light)
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
				float newLighting =
					min(1.0f, 0.2f + spanDifference)
					* (1.0f - (distanceFromSrc / range));

				float existingLighting = light->lightMap[x + y * light->lightMapWidth];
				if (newLighting > existingLighting)
				{
					light->lightMap[x + y * light->lightMapWidth] = newLighting;
					float lightingDelta = newLighting - existingLighting;
					floorGrid[j][i].addLighting(lightingDelta, lightingDelta, lightingDelta);
				}
			}
			if (floorGrid[j][i].isWall) {
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

	float getSpanDifference(Light* light, int tileX, int tileY)
	{
		//Get the offset from the tile to the centre of the raycaster
		//Note: The y coordinate is negated because the formula on the next line works with a coordinate system
		//in which the y axis points down, whereas with XNA, the y axis points up
		glm::vec2 voffset = glm::vec2((tileX * tileSize) - light->x, light->y - (tileY * tileSize));

		//Calculate the angle between the direction and this offset
		float angle = glm::acos(
			glm::dot<2, float, glm::qualifier::highp>(voffset, light->direction) / (glm::length(voffset) * glm::length(light->direction))
		);

		//If the angle is small enough this tile is within the span of the light
		return (light->span - angle) / light->span;
	}
};
