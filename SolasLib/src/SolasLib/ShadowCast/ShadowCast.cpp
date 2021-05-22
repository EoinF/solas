/*
 Adapted from: http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html#shadowcode
*/

/// <param name="blocksLight">A function that accepts the X and Y coordinates of a tile and
/// determines whether the given tile blocks the passage of light. The function must be able to
/// accept coordinates that are out of bounds.
/// </param>
/// <param name="setVisible">A function that sets a tile to be visible, given its X and Y
/// coordinates. The function must ignore coordinates that are out of bounds.
/// </param>
/// <param name="getDistance">A function that takes the X and Y coordinate of a point where X >= 0,
/// Y >= 0, and X >= Y, and returns the distance from the point to the origin.
/// </param>
#include "ShadowCast.hpp"

void compute(int octant, std::int64_t srcTileX, std::int64_t srcTileY, std::int64_t x, Slope top,
			 Slope bottom, std::int64_t rangeInTiles, ChunkMap &chunkMap, Light &light,
			 ShadowCastLightMap &lightMap, light_id_t lightId);

void clearLightMapping(int lightId, Light &light, ShadowCastLightMap &lightMap, ChunkMap &chunkMap);
void applyLighting(light_id_t lightId, Light &light, ShadowCastLightMap &lightMap,
				   ChunkMap &chunkMap);

void ShadowCast::update(int lightId, Light &light, ChunkMap &chunkMap) {
	std::int64_t srcTileX = light.x / chunkMap.tileSize;
	std::int64_t srcTileY = light.y / chunkMap.tileSize;
	std::int64_t rangeInTiles = 1 + static_cast<int>(glm::ceil(light.range / chunkMap.tileSize));

	if (lightMap.find(lightId) == lightMap.end()) {
		printf("creating lightmap entry for %d\n", lightId);
		lightMap[lightId] = std::make_unique<int[]>((rangeInTiles * 2) * (rangeInTiles * 2));
	} else {
		clearLightMapping(lightId, light, lightMap, chunkMap);
	}

	for (int octant = 0; octant < 8; octant++) {
		compute(octant, srcTileY, srcTileY, 1, Slope(1, 1), Slope(0, 1), rangeInTiles, chunkMap,
				light, lightMap, lightId);
	}
	lightMap[lightId][rangeInTiles + (rangeInTiles * rangeInTiles * 2)] = light.brightness;

	applyLighting(lightId, light, lightMap, chunkMap);
}

void ShadowCast::removeLight(int lightId, Light &light, ChunkMap &chunkMap) {
	clearLightMapping(lightId, light, lightMap, chunkMap);
	printf("erasing light %d\n", lightId);
	lightMap.erase(lightId);
}

std::set<int> ShadowCast::getAffectedLights(std::int64_t tileX, std::int64_t tileY,
											const ChunkMap &chunkMap) {
	return {};
}

void compute(int octant, std::int64_t srcTileX, std::int64_t srcTileY, std::int64_t x, Slope top,
			 Slope bottom, std::int64_t rangeInTiles, ChunkMap &chunkMap, Light &light,
			 ShadowCastLightMap &lightMap, light_id_t lightId) {
	for (; x <= rangeInTiles; x++) // rangeLimit < 0 || x <= rangeLimit
	{
		// compute the Y coordinates where the top vector leaves the column (on the right) and where
		// the bottom vector enters the column (on the left). this equals (x+0.5)*top+0.5 and
		// (x-0.5)*bottom+0.5 respectively, which can be computed like (x+0.5)*top+0.5 =
		// (2(x+0.5)*top+1)/2 = ((2x+1)*top+1)/2 to avoid floating point math
		int topY = (top.x == 1) ? x
								: ((x * 2 + 1) * top.y + top.x - 1) /
									  (top.x * 2); // the rounding is a bit tricky, though
		int bottomY = bottom.y == 0 ? 0 : ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);

		int wasOpaque = -1; // 0:false, 1:true, -1:not applicable
		for (int y = topY; y >= bottomY; y--) {
			int tx = srcTileX, ty = srcTileY;
			switch (octant) // translate local coordinates to map coordinates
			{
			case 0:
				tx += x;
				ty -= y;
				break;
			case 1:
				tx += y;
				ty -= x;
				break;
			case 2:
				tx -= y;
				ty -= x;
				break;
			case 3:
				tx -= x;
				ty -= y;
				break;
			case 4:
				tx -= x;
				ty += y;
				break;
			case 5:
				tx -= y;
				ty += x;
				break;
			case 6:
				tx += y;
				ty += x;
				break;
			case 7:
				tx += x;
				ty += y;
				break;
			}

			auto tileState = chunkMap.getTileLightState(tx, ty);

			float distanceToTile = glm::distance(glm::vec2(srcTileX, srcTileY), glm::vec2(tx, ty));
			bool inRange = rangeInTiles < 0 || distanceToTile <= rangeInTiles;
			if (inRange) {
				int brightness = light.brightness / 4;
				std::int64_t tileIndex = (tx + rangeInTiles - srcTileX) +
										 (ty + rangeInTiles - srcTileY) * rangeInTiles * 2;
				// (light.brightness * (rangeInTiles - distanceToTile) / rangeInTiles);
				lightMap[lightId][tileIndex] = brightness;
			}
			// NOTE: use the next line instead if you want the algorithm to be symmetrical
			// if(inRange && (y != topY || top.Y*x >= top.X*y) && (y != bottomY || bottom.Y*x <=
			// bottom.X*y)) SetVisible(tx, ty);

			bool isOpaque = !inRange || tileState.isWall;
			if (x != rangeInTiles) {
				if (isOpaque) {
					if (wasOpaque == 0) // if we found a transition from clear to opaque, this
										// sector is done in this column, so
					{ // adjust the bottom vector upwards and continue processing it in the next
					  // column.
						Slope newBottom(
							y * 2 + 1,
							x * 2 -
								1); // (x*2-1, y*2+1) is a vector to the top-left of the opaque tile
						if (!inRange || y == bottomY) {
							bottom = newBottom;
							break;
						} // don't recurse unless we have to
						else {
							compute(octant, srcTileX, srcTileY, x + 1, top, newBottom, rangeInTiles,
									chunkMap, light, lightMap, lightId);
						}
					}
					wasOpaque = 1;
				} else // adjust top vector downwards and continue if we found a transition from
					   // opaque to clear
				{ // (x*2+1, y*2+1) is the top-right corner of the clear tile (i.e. the bottom-right
				  // of the opaque tile)
					if (wasOpaque > 0) {
						top = Slope(y * 2 + 1, x * 2 + 1);
					}
					wasOpaque = 0;
				}
			}
		}

		if (wasOpaque != 0) {
			break; // if the column ended in a clear tile, continue processing the current sector
		}
	}
}

void applyLighting(light_id_t lightId, Light &light, ShadowCastLightMap &lightMap,
				   ChunkMap &chunkMap) {
	std::int64_t srcTileX = light.x / chunkMap.tileSize;
	std::int64_t srcTileY = light.y / chunkMap.tileSize;
	std::int64_t rangeInTiles = 1 + static_cast<int>(glm::ceil(light.range / chunkMap.tileSize));
	std::int64_t lightMapWidth = rangeInTiles * 2;
	for (std::int64_t x = 0; x < lightMapWidth; x++) {
		for (std::int64_t y = 0; y < lightMapWidth; y++) {
			std::int64_t tileX = srcTileX + x - rangeInTiles;
			std::int64_t tileY = srcTileY + y - rangeInTiles;

			chunkMap.getTileLightStateUnsafe(tileX, tileY)
				.addLighting(lightMap[lightId][x + y * lightMapWidth]);
		}
	}
}

void clearLightMapping(int lightId, Light &light, ShadowCastLightMap &lightMap,
					   ChunkMap &chunkMap) {
	std::int64_t srcTileX = light.x / chunkMap.tileSize;
	std::int64_t srcTileY = light.y / chunkMap.tileSize;

	std::int64_t rangeInTiles = 1 + static_cast<int>(glm::ceil(light.range / chunkMap.tileSize));
	std::int64_t lightMapWidth = rangeInTiles * 2;
	for (std::int64_t x = 0; x < lightMapWidth; x++) {
		for (std::int64_t y = 0; y < lightMapWidth; y++) {
			std::int64_t tileX = srcTileX + x - rangeInTiles;
			std::int64_t tileY = srcTileY + y - rangeInTiles;

			chunkMap.getTileLightStateUnsafe(tileX, tileY)
				.subtractLighting(lightMap[lightId][x + y * lightMapWidth]);
			lightMap[lightId][x + y * lightMapWidth] = 0;
		}
	}
}