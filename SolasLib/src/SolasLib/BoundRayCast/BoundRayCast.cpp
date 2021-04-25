#include "BoundRayCast.hpp"
#include "../DiscreteLinePather.hpp"

void clearLightMapping(int lightId, BoundLight &boundLight, int tileSize, int chunkSize,
					   ChunkMap &chunkMap, ChunkTileLightIdsMap &tilesToLightIdsMap);
void boundRayCast(BoundLight &boundLight, int i, int j);
void applyLightDependencyPath(int lightId, BoundLight &boundLight, BoundRayCastNode &currentNode,
							  int chunkSize, ChunkMap &chunkMap,
							  ChunkTileLightIdsMap &tilesToLightIdsMap);
void addLightDependency(int lightId, int tileX, int tileY, int chunkSize,
						ChunkTileLightIdsMap &tilesToLightIdsMap);

void BoundRayCast::update(int lightId, Light &light, int tileSize, int chunkSize,
						  ChunkMap &chunkMap) {
	float tileSizeF = (float)tileSize;
	int srcTileX = (int)(light.x / tileSizeF);
	int srcTileY = (int)(light.y / tileSizeF);

	BoundLight *boundLight;

	// Check if a mapping for this light exists
	if (boundLightMap.find(lightId) != boundLightMap.end()) {
		boundLight = boundLightMap[lightId].get();

		clearLightMapping(lightId, *boundLight, tileSize, chunkSize, chunkMap, tilesToLightIdsMap);
		boundLight->srcX = srcTileX;
		boundLight->srcY = srcTileY;
		boundLight->direction = glm::normalize(light.direction);
		boundLight->span = light.span;
	} else {
		boundLight = new BoundLight(srcTileX, srcTileY, 1 + (int)glm::ceil(light.range / tileSizeF),
									light.span, light.direction, light.brightness);
		boundLightMap.insert({lightId, std::unique_ptr<BoundLight>(boundLight)});
		BoundRayCastNode &currentNode = boundLight->dependencyTreeRoot;

		// Precalculate the ray paths to each perimeter tile
		for (int i = -boundLight->halfCastingMapWidth; i < boundLight->halfCastingMapWidth + 1;
			 i++) {
			int j = -boundLight->halfCastingMapWidth;
			boundRayCast(*boundLight, i, j);
			j = +boundLight->halfCastingMapWidth;
			boundRayCast(*boundLight, i, j);
		}
		for (int j = -boundLight->halfCastingMapWidth + 1; j < boundLight->halfCastingMapWidth;
			 j++) {
			int i = -boundLight->halfCastingMapWidth;
			boundRayCast(*boundLight, i, j);
			i = +boundLight->halfCastingMapWidth;
			boundRayCast(*boundLight, i, j);
		}
	}

	// Apply the dependency tree
	applyLightDependencyPath(lightId, *boundLight, boundLight->dependencyTreeRoot, chunkSize,
							 chunkMap, tilesToLightIdsMap);
	light.shouldUpdate = false;
}

void BoundRayCast::removeLight(int lightId, Light &light, int tileSize, int chunkSize,
							   ChunkMap &chunkMap) {
	clearLightMapping(lightId, *boundLightMap[lightId], tileSize, chunkSize, chunkMap,
					  tilesToLightIdsMap);
	boundLightMap.erase(lightId);
}

const std::set<int> &BoundRayCast::getAffectedLights(int tileX, int tileY, int tileSize,
													 int chunkSize) {
	float chunkSizeF = (float)chunkSize;
	int chunkX = (int)floorf(tileX / chunkSizeF);
	int chunkY = (int)floorf(tileY / chunkSizeF);
	int chunkIndex = getChunkIndex(chunkX, chunkY);
	int chunkTileX = tileX - chunkX * chunkSize;
	int chunkTileY = tileY - chunkY * chunkSize;
	int tileIndex = tileX + tileY * chunkSize;

	if (tilesToLightIdsMap.count(chunkIndex) > 0 &&
		tilesToLightIdsMap[chunkIndex].count(tileIndex) > 0) {
		return tilesToLightIdsMap[chunkIndex][tileIndex];
	}
	static std::set<int> emptySet;
	return emptySet;
}

void clearLightMapping(int lightId, BoundLight &boundLight, int tileSize, int chunkSize,
					   ChunkMap &chunkMap, ChunkTileLightIdsMap &tilesToLightIdsMap) {
	for (int x = 0; x < boundLight.castingMapWidth; x++) {
		for (int y = 0; y < boundLight.castingMapWidth; y++) {
			int tileX = boundLight.srcX + x - boundLight.halfCastingMapWidth;
			int tileY = boundLight.srcY + y - boundLight.halfCastingMapWidth;

			float chunkSizeF = (float)chunkSize;
			int chunkX = (int)floorf(tileX / chunkSizeF);
			int chunkY = (int)floorf(tileY / chunkSizeF);
			int chunkIndex = getChunkIndex(chunkX, chunkY);
			int chunkTileX = tileX - chunkX * chunkSize;
			int chunkTileY = tileY - chunkY * chunkSize;
			int tileIndex = tileX + tileY * chunkSize;
			tilesToLightIdsMap[chunkIndex][tileIndex].erase(lightId);

			getTileFast(tileX, tileY, chunkSize, chunkMap)
				.subtractLighting(boundLight.lightMap[x + y * boundLight.castingMapWidth]);
			boundLight.lightMap[x + y * boundLight.castingMapWidth] = 0;
		}
	}
}

void boundRayCast(BoundLight &boundLight, int i, int j) {
	BoundRayCastNode *currentNode = &boundLight.dependencyTreeRoot;
	DiscreteLinePather pather(0, 0, i, j);

	while (!pather.isFinished) {
		auto nextTile = pather.nextTile();
		float distance = glm::length(glm::vec2(nextTile.x, nextTile.y));

		if (distance >= boundLight.halfCastingMapWidth) {
			break;
		}
		int tileX = boundLight.srcX + nextTile.x;
		int tileY = boundLight.srcY + nextTile.y;

		int tileIndex = (boundLight.halfCastingMapWidth + nextTile.x) +
						(boundLight.halfCastingMapWidth + nextTile.y) * boundLight.castingMapWidth;

		// If this node doesn't exist create it
		if (currentNode->children.find(tileIndex) == currentNode->children.end()) {
			BoundRayCastNode *newNode = new BoundRayCastNode(nextTile.x, nextTile.y, 0);
			currentNode->children.insert({tileIndex, std::unique_ptr<BoundRayCastNode>(newNode)});
			currentNode = newNode;
		} else { // otherwise use the existing node
			currentNode = currentNode->children[tileIndex].get();
		}

		glm::vec2 rayDirection = glm::vec2(i, j);
		currentNode->directionsToNode.push_back(glm::normalize(rayDirection));

		int brightness =
			(int)(boundLight.brightness * ((float)boundLight.halfCastingMapWidth - distance) /
				  (float)boundLight.halfCastingMapWidth);
		if (currentNode->brightness < brightness) {
			currentNode->brightness = brightness;
		}
	}
}

bool isNodeReachable(BoundRayCastNode &node, glm::vec2 direction, float span) {
	if (node.directionsToNode.size() > 0) {
		for (auto &directionToNode : node.directionsToNode) {
			// Direction vectors are all normalized so no need to divide by their lengths
			float angle = glm::acos(glm::dot(direction, directionToNode));
			if (angle <= span / 2.0f) {
				return true;
			}
		}
		return false;
	}
	return true;
}

void applyLightDependencyPath(int lightId, BoundLight &boundLight, BoundRayCastNode &currentNode,
							  int chunkSize, ChunkMap &chunkMap,
							  ChunkTileLightIdsMap &tilesToLightIdsMap) {
	if (!isNodeReachable(currentNode, boundLight.direction, boundLight.span)) {
		return;
	}
	int tileX = boundLight.srcX + currentNode.location.x;
	int tileY = boundLight.srcY + currentNode.location.y;

	auto &tile = getTileFast(tileX, tileY, chunkSize, chunkMap);

	addLightDependency(lightId, tileX, tileY, chunkSize, tilesToLightIdsMap);

	float angleToTile = glm::acos(
		glm::dot(boundLight.direction,
				 glm::normalize(glm::vec2(currentNode.location.x, currentNode.location.y))));

	int newLighting = (int)(currentNode.brightness *
							glm::min(1.0f, 2.0f * ((0.1f + boundLight.span) - angleToTile * 2)));

	int lightMapArrayIndex =
		((boundLight.halfCastingMapWidth + currentNode.location.x) +
		 (boundLight.halfCastingMapWidth + currentNode.location.y) * boundLight.castingMapWidth);
	int existingLighting = boundLight.lightMap[lightMapArrayIndex];
	if (newLighting > existingLighting) {
		boundLight.lightMap[lightMapArrayIndex] = newLighting;
		int lightingDelta = newLighting - existingLighting;

		tile.addLighting(lightingDelta);
	}

	if (tile.isWall) {
		return;
	}

	for (auto &node : currentNode.children) {
		applyLightDependencyPath(lightId, boundLight, *node.second, chunkSize, chunkMap,
								 tilesToLightIdsMap);
	}
}

void addLightDependency(int lightId, int tileX, int tileY, int chunkSize,
						ChunkTileLightIdsMap &tilesToLightIdsMap) {
	float chunkSizeF = (float)chunkSize;
	int chunkX = (int)floorf(tileX / chunkSizeF);
	int chunkY = (int)floorf(tileY / chunkSizeF);
	int chunkIndex = getChunkIndex(chunkX, chunkY);
	int chunkTileX = tileX - chunkX * chunkSize;
	int chunkTileY = tileY - chunkY * chunkSize;
	int tileIndex = tileX + tileY * chunkSize;

	if (tilesToLightIdsMap.count(chunkIndex) == 0) {
		tilesToLightIdsMap.insert({chunkIndex, std::map<int, std::set<int>>()});
	}
	if (tilesToLightIdsMap[chunkIndex].count(tileIndex) == 0) {
		tilesToLightIdsMap[chunkIndex].insert({tileIndex, std::set<int>()});
	}

	tilesToLightIdsMap[chunkIndex][tileIndex].insert(lightId);
}
