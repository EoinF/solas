#include "BoundRayCast.hpp"
#include "../DiscreteLinePather.hpp";

void boundRayCast(BoundLight* boundLight, int i, int j, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray);
void applyLightDependencyPath(BoundLight* boundLight, BoundRayCastNode* currentNode, std::vector<TileLightState>& tileArray, int floorGridWidth, int floorGridHeight);

void BoundRayCast::update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray)
{
	float tileSizeF = (float)tileSize;
	int srcTileX = (int)(light->x / tileSizeF);
	int srcTileY = (int)(light->y / tileSizeF);

	BoundLight* boundLight;

	// Check if a mapping for this light exists
	if (boundLightMap.find(lightId) != boundLightMap.end())
	{
		boundLight = boundLightMap[lightId];
	}
	else
	{
		boundLight = new BoundLight(srcTileX, srcTileY, 1 + (int)glm::ceil(light->range / tileSizeF), light->span);
		int startX = boundLight->srcX - boundLight->halfCastingMapWidth;
		int startY = boundLight->srcY - boundLight->halfCastingMapWidth;
		BoundRayCastNode* currentNode = &boundLight->dependencyTreeRoot;

		// Precalculate the ray paths to each perimeter tile
		for (int i = -boundLight->halfCastingMapWidth; i < boundLight->halfCastingMapWidth; i++)
		{
			int j = -boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j, floorGridWidth, floorGridHeight, tileArray);
			j = +boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j, floorGridWidth, floorGridHeight, tileArray);
		}
		for (int j = -boundLight->halfCastingMapWidth + 1; j < boundLight->halfCastingMapWidth - 1; j++)
		{
			int i = -boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j, floorGridWidth, floorGridHeight, tileArray);
			i = +boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j, floorGridWidth, floorGridHeight, tileArray);
		}
		boundLightMap.insert({ lightId, boundLight });
	}

	// Apply the dependency tree
	applyLightDependencyPath(boundLight, &boundLight->dependencyTreeRoot, tileArray, floorGridWidth, floorGridHeight);

	light->shouldUpdate = false;
}

void BoundRayCast::removeLight(int lightId, Light* light, int tileSize, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray)
{
	BoundLight* boundLight = boundLightMap[lightId];

	int startX = glm::max(boundLight->halfCastingMapWidth - boundLight->srcX, 0);
	int startY = glm::max(boundLight->halfCastingMapWidth - boundLight->srcY, 0);

	int endX = glm::min(boundLight->halfCastingMapWidth + floorGridWidth - boundLight->srcX, boundLight->castingMapWidth);
	int endY = glm::min(boundLight->halfCastingMapWidth + floorGridHeight - boundLight->srcY, boundLight->castingMapWidth);

	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			int tileX = boundLight->srcX + x - boundLight->halfCastingMapWidth;
			int tileY = boundLight->srcY + y - boundLight->halfCastingMapWidth;
			
			tileArray[tileX + tileY * floorGridWidth].subtractLighting(boundLight->lightMap[x + y * boundLight->castingMapWidth]);
		}
	}
	delete boundLightMap[lightId];
	boundLightMap.erase(lightId);
}

void boundRayCast(BoundLight * boundLight, int i, int j, int floorGridWidth, int floorGridHeight, std::vector<TileLightState> & tileArray)
{
	float angle = std::atan2f(j, i);
	if (2 * abs(angle) > boundLight->span)
	{
		return;
	}
	BoundRayCastNode* currentNode = &boundLight->dependencyTreeRoot;
	DiscreteLinePather pather(0, 0, i, j);

	while (!pather.isFinished) {
		auto nextTile = pather.nextTile();
		float distance = glm::length(glm::vec2(nextTile.x, nextTile.y));

		int tileX = boundLight->srcX + nextTile.x;
		int tileY = boundLight->srcY + nextTile.y;

		if (distance >= boundLight->halfCastingMapWidth
			|| tileX >= floorGridWidth
			|| tileY >= floorGridHeight
			|| tileX < 0
			|| tileY < 0)
		{
			break;
		}
		int tileIndex = (boundLight->halfCastingMapWidth + nextTile.x) + (boundLight->halfCastingMapWidth + nextTile.y) * boundLight->castingMapWidth;

		// If this node doesn't exist create it
		if (currentNode->children.find(tileIndex) == currentNode->children.end()) {
			auto newNode = new BoundRayCastNode(nextTile.x, nextTile.y);
			currentNode->children.insert({
				tileIndex,
				newNode
			});
			currentNode = newNode;
		}
		else // otherwise use the existing node
		{
			currentNode = currentNode->children[tileIndex];
		}

		int brightness = (int)(255.0f * ((float)boundLight->halfCastingMapWidth - distance) / (float)boundLight->halfCastingMapWidth);
		if (currentNode->brightness < brightness)
		{
			currentNode->brightness = brightness;
		}
	}
}

void applyLightDependencyPath(BoundLight * boundLight, BoundRayCastNode * currentNode, std::vector<TileLightState>& tileArray, int floorGridWidth, int floorGridHeight)
{
	int tileArrayIndex = ((boundLight->srcX + currentNode->location.x) + (boundLight->srcY + currentNode->location.y) * floorGridWidth);
	int lightMapArrayIndex = ((boundLight->halfCastingMapWidth + currentNode->location.x) + (boundLight->halfCastingMapWidth + currentNode->location.y) * boundLight->castingMapWidth);
	int newLighting = currentNode->brightness;

	int existingLighting = boundLight->lightMap[lightMapArrayIndex];
	if (newLighting > existingLighting)
	{
		boundLight->lightMap[lightMapArrayIndex] = newLighting;
		int lightingDelta = newLighting - existingLighting;

		tileArray[tileArrayIndex].addLighting(lightingDelta);
	}

	if (tileArray[tileArrayIndex].isWall)
	{
		return;
	}

	for (auto node : currentNode->children) {
		applyLightDependencyPath(boundLight, node.second, tileArray, floorGridWidth, floorGridHeight);
	}
}