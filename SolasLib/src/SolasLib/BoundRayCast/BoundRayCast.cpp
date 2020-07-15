#include "BoundRayCast.hpp"
#include "../DiscreteLinePather.hpp"
#include<string>

void clearLightMapping(BoundLight* boundLight, int tileSize, int chunkSize, ChunkMap& chunkMap);
void boundRayCast(BoundLight* boundLight, int i, int j);
void applyLightDependencyPath(BoundLight* boundLight, BoundRayCastNode* currentNode, int chunkSize, ChunkMap& chunkMap);

void BoundRayCast::update(int lightId, Light* light, int tileSize, int chunkSize, ChunkMap& chunkMap)
{
	float tileSizeF = (float)tileSize;
	int srcTileX = (int)(light->x / tileSizeF);
	int srcTileY = (int)(light->y / tileSizeF);

	BoundLight* boundLight;

	// Check if a mapping for this light exists
	if (boundLightMap.find(lightId) != boundLightMap.end())
	{
		boundLight = boundLightMap[lightId];

		clearLightMapping(boundLight, tileSize, chunkSize, chunkMap);
		boundLight->srcX = srcTileX;
		boundLight->srcY = srcTileY;
	}
	else
	{
		boundLight = new BoundLight(srcTileX, srcTileY, 1 + (int)glm::ceil(light->range / tileSizeF), light->span, light->direction, light->brightness);
		BoundRayCastNode* currentNode = &boundLight->dependencyTreeRoot;

		// Precalculate the ray paths to each perimeter tile
		for (int i = -boundLight->halfCastingMapWidth; i < boundLight->halfCastingMapWidth; i++)
		{
			int j = -boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j);
			j = +boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j);
		}
		for (int j = -boundLight->halfCastingMapWidth + 1; j < boundLight->halfCastingMapWidth - 1; j++)
		{
			int i = -boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j);
			i = +boundLight->halfCastingMapWidth;
			boundRayCast(boundLight, i, j);
		}
		boundLightMap.insert({ lightId, boundLight });
	}

	// Apply the dependency tree
	applyLightDependencyPath(boundLight, &boundLight->dependencyTreeRoot, chunkSize, chunkMap);
	light->shouldUpdate = false;
}

void BoundRayCast::removeLight(int lightId, Light* light, int tileSize, int chunkSize, ChunkMap& chunkMap)
{
	BoundLight* boundLight = boundLightMap[lightId];
	clearLightMapping(boundLight, tileSize, chunkSize, chunkMap);
	delete boundLightMap[lightId];
	boundLightMap.erase(lightId);
}

void clearLightMapping(BoundLight* boundLight, int tileSize, int chunkSize, ChunkMap& chunkMap)
{
	for (int x = 0; x < boundLight->castingMapWidth; x++)
	{
		for (int y = 0; y < boundLight->castingMapWidth; y++)
		{
			int tileX = boundLight->srcX + x - boundLight->halfCastingMapWidth;
			int tileY = boundLight->srcY + y - boundLight->halfCastingMapWidth;

			getChunkFast(tileX, tileY, chunkSize, chunkMap)->subtractLighting(boundLight->lightMap[x + y * boundLight->castingMapWidth]);
			boundLight->lightMap[x + y * boundLight->castingMapWidth] = 0;
		}
	}
}

void boundRayCast(BoundLight * boundLight, int i, int j)
{
	glm::vec2 rayDirection = glm::vec2(i, j);
	float dotProduct = glm::dot(boundLight->direction, rayDirection);
	float angle = glm::acos(dotProduct / (glm::length(boundLight->direction) * glm::length(rayDirection)));
	if (angle > boundLight->span / 2.0f)
	{
		return;
	}
	BoundRayCastNode* currentNode = &boundLight->dependencyTreeRoot;
	DiscreteLinePather pather(0, 0, i, j);

	while (!pather.isFinished) {
		auto nextTile = pather.nextTile();
		float distance = glm::length(glm::vec2(nextTile.x, nextTile.y));
		
		if (distance >= boundLight->halfCastingMapWidth)
		{
			break;
		}

		int tileX = boundLight->srcX + nextTile.x;
		int tileY = boundLight->srcY + nextTile.y;

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

		int brightness = (int)(boundLight->brightness * ((float)boundLight->halfCastingMapWidth - distance) / (float)boundLight->halfCastingMapWidth);
		if (currentNode->brightness < brightness)
		{
			currentNode->brightness = brightness;
		}
	}
}

void applyLightDependencyPath(BoundLight * boundLight, BoundRayCastNode * currentNode, int chunkSize, ChunkMap& chunkMap)
{
	int tileX = boundLight->srcX + currentNode->location.x;
	int tileY = boundLight->srcY + currentNode->location.y;

	auto chunk = getChunkFast(tileX, tileY, chunkSize, chunkMap);
	auto chunk2 = getChunkFast(tileX, tileY, chunkSize, chunkMap);

	int lightMapArrayIndex = ((boundLight->halfCastingMapWidth + currentNode->location.x) + (boundLight->halfCastingMapWidth + currentNode->location.y) * boundLight->castingMapWidth);
	int newLighting = currentNode->brightness;

	int existingLighting = boundLight->lightMap[lightMapArrayIndex];
	if (newLighting > existingLighting)
	{
		boundLight->lightMap[lightMapArrayIndex] = newLighting;
		int lightingDelta = newLighting - existingLighting;

		chunk->addLighting(lightingDelta);
		//std::cout << "chunk1:" << std::to_string(chunk->brightness()) << std::endl;
		//std::cout << "chunk2:" << std::to_string(chunk2->brightness()) << std::endl;
		//auto chunk3 = getChunkFast(tileX, tileY, chunkSize, chunkMap);
		//std::cout << "chunk3:" << std::to_string(chunk3->brightness()) << std::endl;
	}

	if (chunk->isWall)
	{
		return;
	}

	for (auto node : currentNode->children) {
		applyLightDependencyPath(boundLight, node.second, chunkSize, chunkMap);
	}
}