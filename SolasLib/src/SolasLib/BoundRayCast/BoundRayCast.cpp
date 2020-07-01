#include "BoundRayCast.hpp"

void BoundRayCast::update(int tileSize, Light* light, int lightId, int floorGridWidth, int floorGridHeight, std::vector<TileLightState>& tileArray)
{
	float tileSizeF = (float)tileSize;
	float spanDifference;
	int srcTileX = (int)(light->x / tileSizeF);
	int srcTileY = (int)(light->y / tileSizeF);

	int newLighting = 255;
	BoundLight * boundLight;

	// Check if a mapping for this light exists
	if (boundLightMap.find(lightId) != boundLightMap.end()) 
	{
		boundLight = &boundLightMap[lightId];
	}
	else 
	{
		boundLight = new BoundLight();
		boundLight->srcX = (int)(light->x / tileSizeF);
		boundLight->srcY = (int)(light->y / tileSizeF);
		boundLight->halfCastingMapWidth = (int)(light->range / tileSizeF);
		boundLight->castingMapWidth = 2 * boundLight->halfCastingMapWidth;
		boundLight->castingMap = new int[boundLight->castingMapWidth * boundLight->castingMapWidth];

		for (int i = 0; i < boundLight->castingMapWidth; i++) {
			for (int j = 0; j < boundLight->castingMapWidth; j++) {
				float distance = glm::length(glm::vec2((float)boundLight->halfCastingMapWidth - i, (float)boundLight->halfCastingMapWidth - j));

				if (distance < boundLight->halfCastingMapWidth) {
					boundLight->castingMap[i + j * boundLight->castingMapWidth] = (int)distance;
				}
				else {
					boundLight->castingMap[i + j * boundLight->castingMapWidth] = 0;
				}
			}
		}
		boundLightMap[lightId] = *boundLight;
	}

	int startX = boundLight->srcX - boundLight->halfCastingMapWidth;
	int startY = boundLight->srcY - boundLight->halfCastingMapWidth;
	int endX = startX + boundLight->castingMapWidth;
	int endY = startY + boundLight->castingMapWidth;

	endX = glm::min(endX, floorGridWidth - 1);
	endY = glm::min(endY, floorGridHeight - 1);
	startX = glm::max(startX, 0);
	startY = glm::max(startY, 0);

	// Apply the mapping
	for (int i = startX; i <= endX; i++)
	{
		for (int j = startY; j <= endY; j++)
		{
			/*int x = light->lightMapWidth / 2 + i - (int)(light->x / tileSize);
			int y = light->lightMapHeight / 2 + j - (int)(light->y / tileSize);
			light->lightMap[x + y * light->lightMapWidth] = newLighting;*/
			tileArray[j * floorGridWidth + i].addLighting(255, 255, 255, newLighting);
		}
	}
}