#include <SolasLib/core/LightmapManager.hpp>
#include "BoundRayCast/BoundRayCast.hpp"
#include "SimpleRayCast/SimpleRayCast.hpp"

chunk_index_t _getChunkIndex(std::int64_t x, std::int64_t y, std::int64_t maxChunks) {
	return x + y * maxChunks;
}

TileLightState &getTileFast(std::int64_t tileX, std::int64_t tileY, std::int64_t chunkSize,
							std::int64_t maxChunks, ChunkMap &chunkMap) {
	double chunkSizeF = chunkSize;
	std::int64_t chunkX = floor(tileX / chunkSizeF);
	std::int64_t chunkY = floor(tileY / chunkSizeF);
	chunk_index_t chunkIndex = _getChunkIndex(chunkX, chunkY, maxChunks);
	std::int64_t chunkTileX = tileX - chunkX * chunkSize;
	std::int64_t chunkTileY = tileY - chunkY * chunkSize;

	return chunkMap[chunkIndex][chunkTileX + chunkTileY * chunkSize];
}

LightmapManager::LightmapManager(std::int64_t tileSize, CastingAlgorithm type,
								 std::int64_t chunkSize) {

	this->maxChunks = INT64_MAX / (chunkSize * chunkSize);
	this->isPaused = false;
	this->nextLightId = 0;
	this->tileSize = tileSize;
	this->chunkSize = chunkSize;

	allocateChunk(_getChunkIndex(0, 0, maxChunks));

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
	std::int64_t divisor = chunkSize * tileSize;
	double rangeInTiles = 1 + glm::ceil(range / tileSize);
	double actualRange = rangeInTiles * tileSize;

	std::int64_t startChunkX = floor((x - actualRange) / divisor);
	std::int64_t startChunkY = floor((y - actualRange) / divisor);
	std::int64_t endChunkX = floor((x + actualRange) / divisor);
	std::int64_t endChunkY = floor((y + actualRange) / divisor);

	for (std::int64_t i = startChunkX; i <= endChunkX; i++) {
		for (std::int64_t j = startChunkY; j <= endChunkY; j++) {
			getOrAllocateChunk(_getChunkIndex(i, j, maxChunks));
		}
	}
}

void LightmapManager::allocateChunk(chunk_index_t chunkIndex) {
	std::vector<TileLightState> tileArray;

	// tileArray.resize(chunkSize * chunkSize);
	for (int i = 0; i < chunkSize * chunkSize; i++) {
		tileArray.push_back(TileLightState());
	}

	this->chunkMap.insert({chunkIndex, tileArray});
}

std::vector<TileLightState> &LightmapManager::getOrAllocateChunk(chunk_index_t chunkIndex) {
	if (this->chunkMap.find(chunkIndex) == this->chunkMap.end()) {
		allocateChunk(chunkIndex);
	}
	return this->chunkMap[chunkIndex];
}

TileLightState &LightmapManager::_getTileState(std::int64_t x, std::int64_t y) {
	double chunkSizeF = chunkSize;
	std::int64_t chunkX = floor(x / chunkSizeF);
	std::int64_t chunkY = floor(y / chunkSizeF);
	std::int64_t chunkTileX = x - chunkSize * chunkX;
	std::int64_t chunkTileY = y - chunkSize * chunkY;
	return LightmapManager::getOrAllocateChunk(
		_getChunkIndex(chunkX, chunkY, maxChunks))[chunkTileX + chunkTileY * chunkSize];
}

const TileLightState &LightmapManager::getTileState(std::int64_t x, std::int64_t y) {
	return _getTileState(x, y);
}

int LightmapManager::addLight(std::int64_t x, std::int64_t y, float span, float range,
							  glm::vec2 direction, int brightness) {
	int lightId = nextLightId++;
	this->lightsMap.insert(
		{lightId, std::make_unique<Light>(x, y, direction, span, range, brightness, tileSize)});
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
	auto &tile = _getTileState(x, y);
	tile.isWall = isWall;
	auto tileKey = glm::vec2(x, y);
	auto &affectedLights = lightCaster->getAffectedLights(x, y, tileSize, chunkSize, maxChunks);
	for (int lightId : affectedLights) {
		lightsMap[lightId]->shouldUpdate = true;
	}
}

void LightmapManager::clearTileState() {
	for (auto &chunk : this->chunkMap) {
		for (int i = 0; i < chunkSize * chunkSize; i++) {
			chunk.second[i].isWall = false;
		}
	}
}

void LightmapManager::clearLights() {
	for (auto &lightIdPair : this->lightsMap) {
		this->lightCaster->removeLight(lightIdPair.first, *lightIdPair.second, tileSize, chunkSize,
									   maxChunks, chunkMap);
	}
	this->lightsMap.clear();
}

void LightmapManager::removeLight(int lightId) {
	this->lightCaster->removeLight(lightId, *lightsMap[lightId], tileSize, chunkSize, maxChunks,
								   chunkMap);
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
			lightCaster->update(lightId, light, tileSize, chunkSize, maxChunks, chunkMap);
			light.shouldUpdate = false;
		}
	}
}
