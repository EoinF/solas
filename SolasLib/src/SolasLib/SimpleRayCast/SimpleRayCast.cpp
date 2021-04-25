#include "SimpleRayCast.hpp"
#include "../DiscreteLinePather.hpp"
#include "../RayCastUtils.hpp"

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY, float tileSize,
				Light &light, int chunkSize, ChunkMap &chunkMap);

void SimpleRayCast::update(int tileSize, Light &light, int lightId, int chunkSize,
						   ChunkMap &chunkMap) {
	float tileSizeF = (float)tileSize;
	int srcTileX = (int)(light.x / tileSizeF);
	int srcTileY = (int)(light.y / tileSizeF);

	int startX = (int)glm::floor((light.x - light.range) / tileSizeF);
	int startY = (int)glm::floor((light.y - light.range) / tileSizeF);
	int endX = (int)glm::ceil((light.x + light.range) / tileSizeF);
	int endY = (int)glm::ceil((light.y + light.range) / tileSizeF);

	//
	// Reset the lightcast mask for this light
	//
	for (int i = 0; i < light.lightMapWidth; i++) {
		for (int j = 0; j < light.lightMapHeight; j++) {
			int brightness = light.lightMap[i + j * light.lightMapWidth];

			int x = -light.lightMapWidth / 2 + i + (int)(light.x / tileSizeF);
			int y = -light.lightMapHeight / 2 + j + (int)(light.y / tileSizeF);
			int chunkIndex = getChunkIndex(x, y);
			chunkMap[chunkIndex][y * chunkSize + x].subtractLighting(brightness);

			light.lightMap[i + j * light.lightMapWidth] = 0;
		}
	}

	//
	// Use the raycast algorithm on the outer edges of the ray source
	//
	for (int i = startX; i <= endX; i++) {
		rayCastOne(srcTileX, srcTileY, i, startY, tileSizeF, light, chunkSize, chunkMap);
		rayCastOne(srcTileX, srcTileY, i, endY, tileSizeF, light, chunkSize, chunkMap);
	}

	for (int j = startY + 1; j < endY; j++) {
		rayCastOne(srcTileX, srcTileY, startX, j, tileSizeF, light, chunkSize, chunkMap);
		rayCastOne(srcTileX, srcTileY, endX, j, tileSizeF, light, chunkSize, chunkMap);
	}
	light.shouldUpdate = false;
}

void SimpleRayCast::removeLight(int lightId, Light &light, int tileSize, int chunkSize,
								ChunkMap &chunkMap) {
	float tileSizeF = (float)tileSize;
	int srcTileX = (int)(light.x / tileSizeF);
	int srcTileY = (int)(light.y / tileSizeF);

	int startX = (int)(-0.5 + (light.x - light.range) / tileSizeF);
	int startY = (int)(-0.5 + (light.y - light.range) / tileSizeF);
	int endX = (int)(0.5 + (light.x + light.range) / tileSizeF);
	int endY = (int)(0.5 + (light.y + light.range) / tileSizeF);

	//
	// Reset the lightcast mask for this light
	//
	for (int i = 0; i < light.lightMapWidth; i++) {
		for (int j = 0; j < light.lightMapHeight; j++) {
			int brightness = light.lightMap[i + j * light.lightMapWidth];

			int x = i + (int)(light.x / tileSizeF) - light.lightMapWidth / 2;
			int y = j + (int)(light.y / tileSizeF) - light.lightMapHeight / 2;

			int chunkIndex = getChunkIndex(x, y);
			chunkMap[chunkIndex][y * chunkSize + x].subtractLighting(brightness);

			light.lightMap[i + j * light.lightMapWidth] = 0;
		}
	}
}

const std::set<int> &SimpleRayCast::getAffectedLights(int tileX, int tileY, int tileSize,
													  int chunkSize) {
	// TODO update lights based on tile
	return {};
}

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY, float tileSize,
				Light &light, int chunkSize, ChunkMap &chunkMap) {
	float spanDifference = getSpanDifference(light, toTileX, toTileY, tileSize);
	if (spanDifference > 0) {
		DiscreteLinePather pather(fromTileX, fromTileY, toTileX, toTileY);
		while (!pather.isFinished) {
			auto nextTile = pather.nextTile();
			int chunkIndex = getChunkIndex(nextTile.x, nextTile.y);

			float distanceFromSrc = glm::length(
				glm::vec2(nextTile.x * tileSize - light.x, nextTile.y * tileSize - light.y));

			if (distanceFromSrc < light.range) {
				int x = light.lightMapWidth / 2 + nextTile.x - (int)(light.x / tileSize);
				int y = light.lightMapHeight / 2 + nextTile.y - (int)(light.y / tileSize);
				int newLighting = (int)(255.0f * (glm::min(1.0f, 0.0f + spanDifference) *
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
