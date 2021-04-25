#include <SolasLib/core/LightmapManager.hpp>
#include "BoundRayCast/BoundRayCast.hpp"
#include "SimpleRayCast/SimpleRayCast.hpp"

std::int64_t getChunkIndex(int x, int y) { return (x + MAX_CHUNKS_X / 2) + (y * MAX_CHUNKS_X + 1); }

TileLightState &getTileFast(int tileX, int tileY, int chunkSize, ChunkMap &chunkMap) {
	float chunkSizeF = static_cast<float>(chunkSize);
	int chunkX = static_cast<int>(floorf(tileX / chunkSizeF));
	int chunkY = static_cast<int>(floorf(tileY / chunkSizeF));
	int chunkIndex = getChunkIndex(chunkX, chunkY);
	int chunkTileX = tileX - chunkX * chunkSize;
	int chunkTileY = tileY - chunkY * chunkSize;

	return chunkMap[chunkIndex][chunkTileX + chunkTileY * chunkSize];
}

void LightmapManager::allocateChunksForLight(float x, float y, float range) {
	int divisor = chunkSize * tileSize;
	float rangeInTiles = 1 + glm::ceil(range / static_cast<float>(tileSize));
	float actualRange = rangeInTiles * tileSize;

	int startChunkX = static_cast<int>(floorf((x - actualRange) / divisor));
	int startChunkY = static_cast<int>(floorf((y - actualRange) / divisor));
	int endChunkX = static_cast<int>(floorf((x + actualRange) / divisor));
	int endChunkY = static_cast<int>(floorf((y + actualRange) / divisor));

	for (int i = startChunkX; i <= endChunkX; i++) {
		for (int j = startChunkY; j <= endChunkY; j++) {
			getOrAllocateChunk(getChunkIndex(i, j));
		}
	}
}

LightmapManager::LightmapManager(int tileSize, CastingAlgorithm type, int chunkSize) {
	this->isPaused = false;
	this->nextLightId = 0;
	this->tileSize = tileSize;
	this->chunkSize = chunkSize;

	allocateChunk(getChunkIndex(0, 0));

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

void LightmapManager::allocateChunk(int chunkIndex) {
	std::vector<TileLightState> tileArray;

	// tileArray.resize(chunkSize * chunkSize);
	for (int i = 0; i < chunkSize * chunkSize; i++) {
		tileArray.push_back(TileLightState());
	}

	this->chunkMap.insert({chunkIndex, tileArray});
}

std::vector<TileLightState> &LightmapManager::getOrAllocateChunk(int chunkIndex) {
	if (this->chunkMap.find(chunkIndex) == this->chunkMap.end()) {
		allocateChunk(chunkIndex);
	}
	return this->chunkMap[chunkIndex];
}

TileLightState &LightmapManager::_getTileState(int x, int y) {
	int chunkX = floorf(x / static_cast<float>(chunkSize));
	int chunkY = floorf(y / static_cast<float>(chunkSize));
	int chunkIndex = getChunkIndex(chunkX, chunkY);
	int chunkTileX = x - chunkSize * chunkX;
	int chunkTileY = y - chunkSize * chunkY;
	return LightmapManager::getOrAllocateChunk(chunkIndex)[chunkTileX + chunkTileY * chunkSize];
}

const TileLightState &LightmapManager::getTileState(int x, int y) { return _getTileState(x, y); }

int LightmapManager::addLight(float x, float y, float span, float range, glm::vec2 direction,
							  int brightness) {
	int lightId = nextLightId++;
	this->lightsMap.insert(
		{lightId, std::make_unique<Light>(x, y, direction, span, range, brightness, tileSize)});
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, range);

	return lightId;
}

void LightmapManager::updateLight(int lightId, float x, float y, float span, glm::vec2 direction) {
	this->lightsMap[lightId]->x = x;
	this->lightsMap[lightId]->y = y;
	this->lightsMap[lightId]->direction = direction;
	this->lightsMap[lightId]->span = span;
	this->lightsMap[lightId]->shouldUpdate = true;

	allocateChunksForLight(x, y, this->lightsMap[lightId]->range);
}

void LightmapManager::updateTile(int x, int y, bool isWall) {
	auto &tile = _getTileState(x, y);
	tile.isWall = isWall;
	auto tileKey = glm::vec2(x, y);
	auto &affectedLights = lightCaster->getAffectedLights(x, y, tileSize, chunkSize);
	for (int lightId : affectedLights) {
		lightsMap[lightId]->shouldUpdate = true;
	}
}

void LightmapManager::clearTileState() {
	auto &tileArray = this->getTileArray();
	for (auto &chunk : this->chunkMap) {
		for (int i = 0; i < chunkSize * chunkSize; i++) {
			chunk.second[i].isWall = false;
		}
	}
}

void LightmapManager::clearLights() {
	for (auto &lightIdPair : this->lightsMap) {
		this->lightCaster->removeLight(lightIdPair.first, *lightIdPair.second, tileSize, chunkSize,
									   chunkMap);
	}
	this->lightsMap.clear();
}

void LightmapManager::removeLight(int lightId) {
	this->lightCaster->removeLight(lightId, *lightsMap[lightId], tileSize, chunkSize, chunkMap);
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
			lightCaster->update(lightId, light, tileSize, chunkSize, chunkMap);
			light.shouldUpdate = false;
		}
	}
}
