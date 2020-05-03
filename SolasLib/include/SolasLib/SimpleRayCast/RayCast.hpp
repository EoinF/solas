#pragma once

#include <SolasLib/core/Light.hpp>
#include <SolasLib/core/TileLightState.hpp>
#include <stdio.h>
#include <vector>

using namespace std;

float getSpanDifference(Light* light, int tileX, int tileY, int tileSize);
bool rayCast(int srcX, int srcY, int destX, int destY, float range, float spanDifference, Light* light, int tileSize, vector<TileLightState>& tileArray, int floorGridWidth);
