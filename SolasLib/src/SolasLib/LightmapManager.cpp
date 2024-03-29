#include <SolasLib/core/LightmapManager.hpp>
#include "BoundRayCast/BoundRayCast.hpp"

LightmapManager::LightmapManager(std::int64_t tileSize, CastingAlgorithm type,
								 std::int64_t chunkSize)
	: chunkMap(chunkSize, tileSize) {
	this->isPaused = false;
	this->nextLightId = 0;

	switch (type) {
	case CastingAlgorithm::BOUND_RAY_CAST:
		this->lightCaster = std::make_unique<BoundRayCast>(tileSize);
		break;
	default:
		this->lightCaster = std::make_unique<BoundRayCast>(tileSize);
	}
}

void LightmapManager::allocateChunksForLight(std::int64_t x, std::int64_t y, float range) {
	std::int64_t divisor = chunkMap.chunkSize * chunkMap.tileSize;
	double rangeInTiles = 1 + glm::ceil(range / chunkMap.tileSize);
	double actualRange = rangeInTiles * chunkMap.tileSize;

	std::int64_t startChunkX = floor((x - actualRange) / divisor);
	std::int64_t startChunkY = floor((y - actualRange) / divisor);
	std::int64_t endChunkX = floor((x + actualRange) / divisor);
	std::int64_t endChunkY = floor((y + actualRange) / divisor);

	for (std::int64_t i = startChunkX; i <= endChunkX; i++) {
		for (std::int64_t j = startChunkY; j <= endChunkY; j++) {
			chunkMap.getOrAllocateChunk(i, j);
		}
	}
}

const TileLightState &LightmapManager::getTileState(std::int64_t x, std::int64_t y) {
	return chunkMap.getTileLightState(x, y);
}

int LightmapManager::addLight(std::int64_t x, std::int64_t y, float span, float range,
							  glm::vec2 direction, int brightness) {
	int lightId = nextLightId++;
	this->lightsMap.insert({lightId, std::make_unique<Light>(x, y, direction, span, range,
															 brightness, chunkMap.tileSize)});

	allocateChunksForLight(x, y, range);
	this->lightCaster->addLight(lightId, *this->lightsMap[lightId], chunkMap);

	return lightId;
}

void LightmapManager::updateLightPosition(light_id_t lightId, std::int64_t x, std::int64_t y) {
	this->lightsMap[lightId]->setPosition(x, y);
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, this->lightsMap[lightId]->range);
}

void LightmapManager::updateLightSpan(light_id_t lightId, float span) {
	this->lightsMap[lightId]->span = span;
	this->lightsMap[lightId]->shouldUpdate = true;
}

void LightmapManager::updateLightDirection(light_id_t lightId, glm::vec2 direction) {
	this->lightsMap[lightId]->direction = glm::normalize(direction);
	this->lightsMap[lightId]->shouldUpdate = true;
}

void LightmapManager::updateTile(int x, int y, bool isWall) {
	auto &tile = chunkMap.getTileLightState(x, y);
	tile.isWall = isWall;
	for (int lightId : lightCaster->getAffectedLights(x, y, chunkMap)) {
		lightsMap[lightId]->shouldUpdate = true;
	}
}

void LightmapManager::clearTileState() { chunkMap.clearTileState(); }

void LightmapManager::clearLights() {
	for (auto &lightIdPair : this->lightsMap) {
		this->lightCaster->removeLight(lightIdPair.first, *lightIdPair.second, chunkMap);
	}
	this->lightsMap.clear();
}

void LightmapManager::removeLight(int lightId) {
	this->lightCaster->removeLight(lightId, *lightsMap[lightId], chunkMap);
	lightsMap.erase(lightId);
}

void LightmapManager::update() {
	for (auto &idLightPair : this->lightsMap) {
		int lightId = idLightPair.first;
		Light &light = *idLightPair.second;

		if (light.shouldUpdate) {
			lightCaster->update(lightId, light, chunkMap);
			light.shouldUpdate = false;
		}
	}
}
