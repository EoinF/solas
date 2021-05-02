#include <SolasLib/core/LightmapManager.hpp>
#include "BoundRayCast/BoundRayCast.hpp"
#include "SimpleRayCast/SimpleRayCast.hpp"

LightmapManager::LightmapManager(std::int64_t tileSize, CastingAlgorithm type,
								 std::int64_t chunkSize)
	: chunkMap(chunkSize, tileSize) {
	this->isPaused = false;
	this->nextLightId = 0;

	switch (type) {
	case CastingAlgorithm::BOUND_RAY_CAST:
		this->lightCaster = std::make_unique<BoundRayCast>();
		break;
	case CastingAlgorithm::SIMPLE_RAY_CAST:
		this->lightCaster = std::make_unique<SimpleRayCast>();
		break;
	default:
		this->lightCaster = std::make_unique<SimpleRayCast>();
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
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, range);

	return lightId;
}

void LightmapManager::updateLight(int lightId, std::int64_t x, std::int64_t y, float span,
								  glm::vec2 direction) {
	this->lightsMap[lightId]->x = x;
	this->lightsMap[lightId]->y = y;
	this->lightsMap[lightId]->direction = direction;
	this->lightsMap[lightId]->span = span;
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, this->lightsMap[lightId]->range);
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

void LightmapManager::resetLighting() {
	for (auto &lightIdPair : this->lightsMap) {
		for (int i = 0; i < lightIdPair.second->lightMapWidth; i++) {
			for (int j = 0; j < lightIdPair.second->lightMapHeight; j++) {
				lightIdPair.second->lightMap[i + j * lightIdPair.second->lightMapHeight] = 0;
			}
		}
	}
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
