#include "SimpleRayCast.hpp"
#include "../DiscreteLinePather.hpp"
#include "../RayCastUtils.hpp"

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY, Light &light,
				ChunkMap &chunkMap);

void SimpleRayCast::update(int lightId, Light &light, ChunkMap &chunkMap) {
	float tileSizeF = static_cast<float>(chunkMap.tileSize);
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
			chunkMap.getTileLightStateUnsafe(x, y).subtractLighting(brightness);

			light.lightMap[i + j * light.lightMapWidth] = 0;
		}
	}

	//
	// Use the raycast algorithm on the outer edges of the ray source
	//
	for (int i = startX; i <= endX; i++) {
		rayCastOne(srcTileX, srcTileY, i, startY, light, chunkMap);
		rayCastOne(srcTileX, srcTileY, i, endY, light, chunkMap);
	}

	for (int j = startY + 1; j < endY; j++) {
		rayCastOne(srcTileX, srcTileY, startX, j, light, chunkMap);
		rayCastOne(srcTileX, srcTileY, endX, j, light, chunkMap);
	}
	light.shouldUpdate = false;
}

void SimpleRayCast::removeLight(int lightId, Light &light, ChunkMap &chunkMap) {
	float tileSizeF = static_cast<float>(chunkMap.tileSize);
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

			chunkMap.getTileLightStateUnsafe(x, y).subtractLighting(brightness);

			light.lightMap[i + j * light.lightMapWidth] = 0;
		}
	}
}

const std::set<int> &SimpleRayCast::getAffectedLights(std::int64_t tileX, std::int64_t tileY,
													  const ChunkMap &chunkMap) {
	// TODO update lights based on tile
	return {};
}

void rayCastOne(int fromTileX, int fromTileY, int toTileX, int toTileY, Light &light,
				ChunkMap &chunkMap) {
	float spanDifference = getSpanDifference(light, toTileX, toTileY, chunkMap.tileSize);
	if (spanDifference > 0) {
		DiscreteLinePather pather(fromTileX, fromTileY, toTileX, toTileY);
		while (!pather.isFinished) {
			auto nextTile = pather.nextTile();

			float distanceFromSrc =
				glm::length(glm::vec2(nextTile.x * chunkMap.tileSize - light.x,
									  nextTile.y * chunkMap.tileSize - light.y));

			if (distanceFromSrc < light.range) {
				int x = light.lightMapWidth / 2 + nextTile.x -
						static_cast<int>(light.x / chunkMap.tileSize);
				int y = light.lightMapHeight / 2 + nextTile.y -
						static_cast<int>(light.y / chunkMap.tileSize);
				int newLighting =
					static_cast<int>(255.0f * (glm::min(1.0f, 0.0f + spanDifference) *
											   (1.0f - (distanceFromSrc / light.range))));

				int existingLighting = light.lightMap[x + y * light.lightMapWidth];
				if (newLighting > existingLighting) {
					light.lightMap[x + y * light.lightMapWidth] = newLighting;
					int lightingDelta = newLighting - existingLighting;
					chunkMap.getTileLightStateUnsafe(nextTile.x, nextTile.y)
						.addLighting(lightingDelta);
				}
			}
			if (chunkMap.getTileLightStateUnsafe(nextTile.x, nextTile.y).isWall) {
				return;
			}
		}
	}
}
