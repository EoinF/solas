#pragma once

#include <SolasLib/core/Light.hpp>
#include <SolasLib/core/IGameObject.hpp>

float getSpanDifference(Light* light, int tileX, int tileY, int tileSize);
bool rayCast(int srcX, int srcY, int destX, int destY, float range, float spanDifference, Light* light, int tileSize, IGameObject** floorGrid, int floorGridWidth);
