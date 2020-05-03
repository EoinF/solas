#pragma once

#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <SolasLib/SimpleRayCast/RayCast.hpp>

using namespace std;

class LightmapManager
{
protected:
	vector<TileLightState> tileArray;
	int tileSize;
	map<int, Light*> lightsMap;
	int nextLightId;
	int floorGridWidth;
	int floorGridHeight;
	bool isPaused;
public:
	LightmapManager(int width, int height, int tileSize)
	{
		this->isPaused = false;
		this->nextLightId = 0;
		this->tileSize = tileSize;

		//tileArray.resize(width * height);
		cout << "creating tile array" << endl;
		for (int i = 0; i < width * height; i++) {
			tileArray.push_back(TileLightState());
		}

		this->floorGridWidth = width;
		this->floorGridHeight = height;
	}

	vector<TileLightState>& getTileArray() {
		return this->tileArray;
	}

	TileLightState* getTileState(int x, int y) {
		return &this->tileArray[y * floorGridWidth + x];
	}

	void setTileSize(int tileSize) {
		this->tileSize = tileSize;
	}

	map<int, Light*> getLightsMap()
	{
		return this->lightsMap;
	}

	int addLight(float x, float y, glm::vec2 direction, float span, float range, int tileSize);
	void updateLight(int lightId, float x, float y, glm::vec2 direction, float span, float range);
	void clearLights();
	void removeLight(int lightId, Light* light);
	void resetLighting();
	void update();
};
