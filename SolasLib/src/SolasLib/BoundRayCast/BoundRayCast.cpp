#include "BoundRayCast.hpp"
#include "../DiscreteLinePather.hpp"

void clearLightMapping(int lightId, BoundLight &boundLight, ChunkMap &chunkMap,
					   ChunkTileLightIdsMap &tilesToLightIdsMap);
void boundRayCast(BoundLight &boundLight, std::int64_t i, std::int64_t j);
void applyLightDependencyPath(int lightId, BoundLight &boundLight, BoundRayCastNode &currentNode,
							  ChunkMap &chunkMap, ChunkTileLightIdsMap &tilesToLightIdsMap);
void addLightDependency(int lightId, std::int64_t tileX, std::int64_t tileY, ChunkMap &chunkMap,
						ChunkTileLightIdsMap &tilesToLightIdsMap);

void BoundRayCast::update(int lightId, Light &light, ChunkMap &chunkMap) {
	std::int64_t srcTileX = light.x / chunkMap.tileSize;
	std::int64_t srcTileY = light.y / chunkMap.tileSize;

	BoundLight *boundLight;

	// Check if a mapping for this light exists
	if (boundLightMap.find(lightId) != boundLightMap.end()) {
		boundLight = boundLightMap[lightId].get();

		clearLightMapping(lightId, *boundLight, chunkMap, tilesToLightIdsMap);
		boundLight->srcX = srcTileX;
		boundLight->srcY = srcTileY;
		boundLight->direction = glm::normalize(light.direction);
		boundLight->span = light.span;
	} else {
		boundLight = new BoundLight(
			srcTileX, srcTileY, 1 + static_cast<int>(glm::ceil(light.range / chunkMap.tileSize)),
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
	applyLightDependencyPath(lightId, *boundLight, boundLight->dependencyTreeRoot, chunkMap,
							 tilesToLightIdsMap);

	light.shouldUpdate = false;
}

void BoundRayCast::removeLight(int lightId, Light &light, ChunkMap &chunkMap) {
	clearLightMapping(lightId, *boundLightMap[lightId], chunkMap, tilesToLightIdsMap);
	boundLightMap.erase(lightId);
}

const std::set<int> &BoundRayCast::getAffectedLights(std::int64_t tileX, std::int64_t tileY,
													 const ChunkMap &chunkMap) {
	double chunkSizeF = chunkMap.chunkSize;
	std::int64_t chunkX = floor(tileX / chunkSizeF);
	std::int64_t chunkY = floor(tileY / chunkSizeF);
	chunk_index_t chunkIndex = chunkMap.getChunkIndex(chunkX, chunkY);
	std::int64_t chunkTileX = tileX - chunkX * chunkMap.chunkSize;
	std::int64_t chunkTileY = tileY - chunkY * chunkMap.chunkSize;
	std::int64_t tileIndex = tileX + tileY * chunkMap.chunkSize;

	if (tilesToLightIdsMap.count(chunkIndex) > 0 &&
		tilesToLightIdsMap[chunkIndex].count(tileIndex) > 0) {
		return tilesToLightIdsMap[chunkIndex][tileIndex];
	}
	static std::set<int> emptySet;
	return emptySet;
}

void clearLightMapping(int lightId, BoundLight &boundLight, ChunkMap &chunkMap,
					   ChunkTileLightIdsMap &tilesToLightIdsMap) {
	for (std::int64_t x = 0; x < boundLight.castingMapWidth; x++) {
		for (std::int64_t y = 0; y < boundLight.castingMapWidth; y++) {
			std::int64_t tileX = boundLight.srcX + x - boundLight.halfCastingMapWidth;
			std::int64_t tileY = boundLight.srcY + y - boundLight.halfCastingMapWidth;

			double chunkSizeF = chunkMap.chunkSize;
			std::int64_t chunkX = floor(tileX / chunkSizeF);
			std::int64_t chunkY = floor(tileY / chunkSizeF);
			chunk_index_t chunkIndex = chunkMap.getChunkIndex(chunkX, chunkY);
			std::int64_t tileIndex = tileX + tileY * chunkMap.chunkSize;
			tilesToLightIdsMap[chunkIndex][tileIndex].erase(lightId);

			chunkMap.getTileLightStateUnsafe(tileX, tileY)
				.subtractLighting(boundLight.lightMap[x + y * boundLight.castingMapWidth]);
			boundLight.lightMap[x + y * boundLight.castingMapWidth] = 0;
		}
	}
}

void boundRayCast(BoundLight &boundLight, std::int64_t i, std::int64_t j) {
	BoundRayCastNode *currentNode = &boundLight.dependencyTreeRoot;
	DiscreteLinePather pather(0, 0, i, j);

	while (!pather.isFinished) {
		auto nextTile = pather.nextTile();
		double distance = glm::length(glm::vec2(nextTile.x, nextTile.y));

		if (distance >= boundLight.halfCastingMapWidth) {
			break;
		}
		std::int64_t tileX = boundLight.srcX + nextTile.x;
		std::int64_t tileY = boundLight.srcY + nextTile.y;

		std::int64_t tileIndex =
			(boundLight.halfCastingMapWidth + nextTile.x) +
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

		int brightness = (boundLight.brightness * (boundLight.halfCastingMapWidth - distance) /
						  boundLight.halfCastingMapWidth);
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
							  ChunkMap &chunkMap, ChunkTileLightIdsMap &tilesToLightIdsMap) {
	if (!isNodeReachable(currentNode, boundLight.direction, boundLight.span)) {
		return;
	}
	std::int64_t tileX = boundLight.srcX + currentNode.location.x;
	std::int64_t tileY = boundLight.srcY + currentNode.location.y;

	auto &tile = chunkMap.getTileLightStateUnsafe(tileX, tileY);

	addLightDependency(lightId, tileX, tileY, chunkMap, tilesToLightIdsMap);

	float angleToTile = glm::acos(
		glm::dot(boundLight.direction,
				 glm::normalize(glm::vec2(currentNode.location.x, currentNode.location.y))));

	int newLighting =
		static_cast<int>(currentNode.brightness *
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
		applyLightDependencyPath(lightId, boundLight, *node.second, chunkMap, tilesToLightIdsMap);
	}
}

void addLightDependency(int lightId, std::int64_t tileX, std::int64_t tileY, ChunkMap &chunkMap,
						ChunkTileLightIdsMap &tilesToLightIdsMap) {
	double chunkSizeF = chunkMap.chunkSize;
	std::int64_t chunkX = floor(tileX / chunkSizeF);
	std::int64_t chunkY = floor(tileY / chunkSizeF);
	chunk_index_t chunkIndex = chunkMap.getChunkIndex(chunkX, chunkY);
	std::int64_t tileIndex = tileX + tileY * chunkMap.chunkSize;

	if (tilesToLightIdsMap.count(chunkIndex) == 0) {
		tilesToLightIdsMap.insert({chunkIndex, std::map<std::int64_t, std::set<light_id_t>>()});
	}
	if (tilesToLightIdsMap[chunkIndex].count(tileIndex) == 0) {
		tilesToLightIdsMap[chunkIndex].insert({tileIndex, std::set<light_id_t>()});
	}

	tilesToLightIdsMap[chunkIndex][tileIndex].insert(lightId);
}
