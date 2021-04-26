#include "SimpleRayCast.hpp"
#include "../DiscreteLinePather.hpp"
#include "../RayCastUtils.hpp"

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY, float tileSize,
				Light &light, int chunkSize, std::int64_t maxChunks, ChunkMap &chunkMap);

void SimpleRayCast::update(int lightId, Light &light, std::int64_t tileSize, std::int64_t chunkSize,
						   std::int64_t maxChunks, ChunkMap &chunkMap) {
	float tileSizeF = static_cast<float>(tileSize);
	int srcTileX = static_cast<int>(light.x / tileSizeF);
	int srcTileY = static_cast<int>(light.y / tileSizeF);

	int startX = static_cast<int>(glm::floor((light.x - light.range) / tileSizeF));
	int startY = static_cast<int>(glm::floor((light.y - light.range) / tileSizeF));
	int endX = static_cast<int>(glm::ceil((light.x + light.range) / tileSizeF));
	int endY = static_cast<int>(glm::ceil((light.y + light.range) / tileSizeF));

	//
	// Reset the lightcast mask for this light
	//
	for (int i = 0; i < light.lightMapWidth; i++) {
		for (int j = 0; j < light.lightMapHeight; j++) {
			int brightness = light.lightMap[i + j * light.lightMapWidth];

			int x = -light.lightMapWidth / 2 + i + static_cast<int>(light.x / tileSizeF);
			int y = -light.lightMapHeight / 2 + j + static_cast<int>(light.y / tileSizeF);
			chunk_index_t chunkIndex = _getChunkIndex(x, y, maxChunks);
			chunkMap[chunkIndex][y * chunkSize + x].subtractLighting(brightness);

			light.lightMap[i + j * light.lightMapWidth] = 0;
		}
	}

	//
	// Use the raycast algorithm on the outer edges of the ray source
	//
	for (int i = startX; i <= endX; i++) {
		rayCastOne(srcTileX, srcTileY, i, startY, tileSizeF, light, chunkSize, maxChunks, chunkMap);
		rayCastOne(srcTileX, srcTileY, i, endY, tileSizeF, light, chunkSize, maxChunks, chunkMap);
	}

	for (int j = startY + 1; j < endY; j++) {
		rayCastOne(srcTileX, srcTileY, startX, j, tileSizeF, light, chunkSize, maxChunks, chunkMap);
		rayCastOne(srcTileX, srcTileY, endX, j, tileSizeF, light, chunkSize, maxChunks, chunkMap);
	}
	light.shouldUpdate = false;
}

void SimpleRayCast::removeLight(int lightId, Light &light, std::int64_t tileSize,
								std::int64_t chunkSize, std::int64_t maxChunks,
								ChunkMap &chunkMap) {
	float tileSizeF = static_cast<float>(tileSize);
	int srcTileX = static_cast<int>(light.x / tileSizeF);
	int srcTileY = static_cast<int>(light.y / tileSizeF);

	int startX = static_cast<int>(-0.5 + (light.x - light.range) / tileSizeF);
	int startY = static_cast<int>(-0.5 + (light.y - light.range) / tileSizeF);
	int endX = static_cast<int>(0.5 + (light.x + light.range) / tileSizeF);
	int endY = static_cast<int>(0.5 + (light.y + light.range) / tileSizeF);

	//
	// Reset the lightcast mask for this light
	//
	for (int i = 0; i < light.lightMapWidth; i++) {
		for (int j = 0; j < light.lightMapHeight; j++) {
			int brightness = light.lightMap[i + j * light.lightMapWidth];

			int x = i + static_cast<int>(light.x / tileSizeF) - light.lightMapWidth / 2;
			int y = j + static_cast<int>(light.y / tileSizeF) - light.lightMapHeight / 2;

			chunk_index_t chunkIndex = _getChunkIndex(x, y, maxChunks);
			chunkMap[chunkIndex][y * chunkSize + x].subtractLighting(brightness);

			light.lightMap[i + j * light.lightMapWidth] = 0;
		}
	}
}

const std::set<int> &SimpleRayCast::getAffectedLights(std::int64_t tileX, std::int64_t tileY,
													  std::int64_t tileSize, std::int64_t chunkSize,
													  std::int64_t maxChunks) {
	// TODO update lights based on tile
	return {};
}

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY, float tileSize,
				Light &light, int chunkSize, std::int64_t maxChunks, ChunkMap &chunkMap) {
	float spanDifference = getSpanDifference(light, toTileX, toTileY, tileSize);
	if (spanDifference > 0) {
		DiscreteLinePather pather(fromTileX, fromTileY, toTileX, toTileY);
		while (!pather.isFinished) {
			auto nextTile = pather.nextTile();
			chunk_index_t chunkIndex = _getChunkIndex(nextTile.x, nextTile.y, maxChunks);

			float distanceFromSrc = glm::length(
				glm::vec2(nextTile.x * tileSize - light.x, nextTile.y * tileSize - light.y));

			if (distanceFromSrc < light.range) {
				int x = light.lightMapWidth / 2 + nextTile.x - static_cast<int>(light.x / tileSize);
				int y =
					light.lightMapHeight / 2 + nextTile.y - static_cast<int>(light.y / tileSize);
				int newLighting =
					static_cast<int>(255.0f * (glm::min(1.0f, 0.0f + spanDifference) *
											   (1.0f - (distanceFromSrc / light.range))));

				int existingLighting = light.lightMap[x + y * light.lightMapWidth];
				if (newLighting > existingLighting) {
					light.lightMap[x + y * light.lightMapWidth] = newLighting;
					int lightingDelta = newLighting - existingLighting;
					chunkMap[chunkIndex][nextTile.y * chunkSize + nextTile.x].addLighting(
						lightingDelta);
				}
			}
			if (chunkMap[chunkIndex][nextTile.y * chunkSize + nextTile.x].isWall) {
				return;
			}
		}
	}
}
