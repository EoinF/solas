#include "BoundRayCast.hpp"

bool isNodeReachable(BoundRayCastNode &node, glm::vec2 direction, float span, float range);

const int MAX_RANGE = 128;

BoundRayCast::BoundRayCast(std::int64_t tileSize) : LightCaster(tileSize) {
	dependencyTreeRoot.directionToNode.x = 1;
	// Precalculate the ray paths to each perimeter tile
	for (int i = -MAX_RANGE; i < MAX_RANGE + 1; i++) {
		int j = -MAX_RANGE;
		boundRayCast(i, j, tileSize);
		j = MAX_RANGE;
		boundRayCast(i, j, tileSize);
	}
	for (int j = -MAX_RANGE + 1; j < MAX_RANGE; j++) {
		int i = -MAX_RANGE;
		boundRayCast(i, j, tileSize);
		i = +MAX_RANGE;
		boundRayCast(i, j, tileSize);
	}
}

void BoundRayCast::update(int lightId, Light &light, ChunkMap &chunkMap) {
	updateLight(lightId, light, chunkMap);

	// Apply the dependency tree
	applyLightDependencyPath(lightId, light, dependencyTreeRoot, chunkMap);

	light.shouldUpdate = false;
}

void BoundRayCast::removeLight(int lightId, Light &light, ChunkMap &chunkMap) {
	for (auto iter = regionsToLightIds.begin(); iter != regionsToLightIds.end(); iter++) {
		if ((*iter).second == lightId) {
			regionsToLightIds.erase(iter);
			break;
		}
	}
	clearLightMapping(lightId, chunkMap);
	boundLightMap.erase(lightId);
}

std::set<int> BoundRayCast::getAffectedLights(std::int64_t tileX, std::int64_t tileY,
											  const ChunkMap &chunkMap) {
	std::set<int> emptySet;

	for (auto regionLightPair : regionsToLightIds) {
		if (regionLightPair.first.containsPoint(tileX, tileY)) {
			emptySet.insert(regionLightPair.second);
		}
	}
	return emptySet;
}

void BoundRayCast::addLight(light_id_t lightId, Light &light, ChunkMap &chunkMap) {
	boundLightMap.insert({lightId, std::make_unique<Light>(light)});
	std::int64_t castingWidth = light.rangeInTiles * 2;

	this->lightCastMap[lightId] = std::make_unique<int[]>(castingWidth * castingWidth);

	Rectangle castingRegion(light.srcTileX - light.rangeInTiles,
							light.srcTileY - light.rangeInTiles, castingWidth, castingWidth);

	regionsToLightIds.push_back({castingRegion, lightId});
}

void BoundRayCast::updateLight(light_id_t lightId, Light &light, ChunkMap &chunkMap) {
	Light *existingLight = boundLightMap[lightId].get();
	clearLightMapping(lightId, chunkMap);
	existingLight->srcTileX = light.srcTileX;
	existingLight->srcTileY = light.srcTileY;
	existingLight->direction = glm::normalize(light.direction);
	existingLight->span = light.span;
	for (auto regionLightPair : regionsToLightIds) {
		if (regionLightPair.second == lightId) {
			regionLightPair.first.x = existingLight->srcTileX - existingLight->rangeInTiles;
			regionLightPair.first.y = existingLight->srcTileY - existingLight->rangeInTiles;
			break;
		}
	}
}

void BoundRayCast::clearLightMapping(int lightId, ChunkMap &chunkMap) {
	Light &boundLight = *boundLightMap[lightId];
	for (std::int64_t x = 0; x < boundLight.rangeInTiles * 2; x++) {
		for (std::int64_t y = 0; y < boundLight.rangeInTiles * 2; y++) {
			std::int64_t tileX = boundLight.srcTileX + x - boundLight.rangeInTiles;
			std::int64_t tileY = boundLight.srcTileY + y - boundLight.rangeInTiles;

			chunkMap.getTileLightStateUnsafe(tileX, tileY)
				.subtractLighting(lightCastMap[lightId][x + y * boundLight.rangeInTiles * 2]);
			lightCastMap[lightId][x + y * boundLight.rangeInTiles * 2] = 0;
		}
	}
}

void BoundRayCast::boundRayCast(std::int64_t i, std::int64_t j, std::int64_t tileSize) {
	BoundRayCastNode *currentNode = &dependencyTreeRoot;
	DiscreteLinePather pather(0, 0, i, j);
	if (i < 0 && j == 0) {
		printf("raycasting to %d, %d\n", i, j);
	}

	while (!pather.isFinished) {
		auto nextTile = pather.nextTile();
		float distance = glm::length(glm::vec2(nextTile.x, nextTile.y));

		if (distance >= MAX_RANGE) {
			break;
		}

		std::int64_t tileIndex =
			(MAX_RANGE + nextTile.x) + (MAX_RANGE + nextTile.y) * MAX_RANGE * 2;

		// If this node doesn't exist create it
		if (currentNode->children.find(tileIndex) == currentNode->children.end()) {
			BoundRayCastNode *newNode =
				new BoundRayCastNode(nextTile.x, nextTile.y, (distance * tileSize) - tileSize / 2);
			currentNode->children.insert({tileIndex, std::unique_ptr<BoundRayCastNode>(newNode)});
			currentNode = newNode;
		} else { // otherwise use the existing node
			currentNode = currentNode->children[tileIndex].get();
		}

		glm::vec2 rayDirection = glm::vec2(i, j);
		if (i < 0 && j == 0) {
			printf("%d,%d = %f,%f\n", currentNode->location.x, currentNode->location.y,
				   currentNode->directionToNode.x, currentNode->directionToNode.y);
		}
		if (glm::length(currentNode->directionToNode) == 0) {
			currentNode->directionToNode = glm::normalize(rayDirection);
			currentNode->span = 0;
			if (i < 0 && j == 0) {
				printf("setting to %d, %d\n", i, j);
			}
		} else {
			auto angleBetween =
				glm::acos(glm::dot(glm::normalize(rayDirection), currentNode->directionToNode));
			if (currentNode->span == 0) {
				currentNode->span = angleBetween * 2;
				currentNode->directionToNode =
					glm::normalize((currentNode->directionToNode + rayDirection) / 2.0f);
			} else if (angleBetween > currentNode->span) {
				float ratio = currentNode->span / angleBetween;
				currentNode->span = (angleBetween + currentNode->span) / 2;
				currentNode->directionToNode = glm::normalize(
					(currentNode->directionToNode * (1 + ratio) + rayDirection * (2 - ratio)) /
					2.0f);
			}
		}
	}
}

bool isNodeReachable(BoundRayCastNode &node, glm::vec2 direction, float lightSpan, float range) {
	if (node.distanceFromSrc > range) {
		return false;
	}
	// Direction vectors are all normalized so no need to divide by their lengths
	float angle = glm::acos(glm::dot(direction, node.directionToNode));
	if (angle <= (node.span + lightSpan) / 2.0f) {
		return true;
	}
	return false;
}

void BoundRayCast::applyLightDependencyPath(int lightId, Light &light,
											BoundRayCastNode &currentNode, ChunkMap &chunkMap) {
	if (!isNodeReachable(currentNode, light.direction, light.span, light.range)) {
		return;
	}
	std::int64_t tileX = light.srcTileX + currentNode.location.x;
	std::int64_t tileY = light.srcTileY + currentNode.location.y;

	auto &tile = chunkMap.getTileLightStateUnsafe(tileX, tileY);

	float angleToTile = glm::acos(
		glm::dot(light.direction,
				 glm::normalize(glm::vec2(currentNode.location.x, currentNode.location.y))));

	int brightness = (light.brightness * (light.range - currentNode.distanceFromSrc) / light.range);

	int newLighting = static_cast<int>(brightness * glm::min(1.0f, 2.0f * ((0.1f + light.span))));

	int lightMapArrayIndex =
		((light.rangeInTiles + currentNode.location.x) +
		 (light.rangeInTiles + currentNode.location.y) * light.rangeInTiles * 2);
	int existingLighting = lightCastMap[lightId][lightMapArrayIndex];

	if (newLighting > existingLighting) {
		lightCastMap[lightId][lightMapArrayIndex] = newLighting;
		int lightingDelta = newLighting - existingLighting;

		tile.addLighting(lightingDelta);
	}

	if (tile.isWall) {
		return;
	}

	for (auto &node : currentNode.children) {
		applyLightDependencyPath(lightId, light, *node.second, chunkMap);
	}
}
