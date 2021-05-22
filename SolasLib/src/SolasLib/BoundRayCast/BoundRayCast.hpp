#pragma once
#include <memory>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <SolasLib/core.hpp>
#include <SolasLib/core/LightCaster.hpp>
#include <SolasLib/core/ChunkMap.hpp>
#include "../DiscreteLinePather.hpp"
#include "../Rectangle.hpp"

struct BoundRayCastNode {
	explicit BoundRayCastNode(int x = 0, int y = 0, float distanceFromSrc = 0) {
		this->location.x = x;
		this->location.y = y;
		this->distanceFromSrc = distanceFromSrc;
	}
	float distanceFromSrc;
	std::vector<glm::vec2> directionsToNode;
	glm::ivec2 location;
	std::map<int, std::unique_ptr<BoundRayCastNode>>
		children; // Maps each tile index to the next dependent tile
};

typedef std::vector<std::pair<Rectangle, light_id_t>> RegionsToLightIds;

class BoundRayCast : public LightCaster {
  public:
	BoundRayCast(std::int64_t tileSize);
	std::map<light_id_t, std::unique_ptr<Light>> boundLightMap;
	RegionsToLightIds regionsToLightIds;

	std::set<light_id_t> getAffectedLights(std::int64_t tileX, std::int64_t tileY,
										   const ChunkMap &chunkMap) override;
	void removeLight(light_id_t lightId, Light &light, ChunkMap &chunkMap) override;
	void update(light_id_t lightId, Light &light, ChunkMap &chunkMap) override;

  protected:
	BoundRayCastNode dependencyTreeRoot;
	std::map<light_id_t, std::unique_ptr<int[]>> lightCastMap;
	void addNewLight(light_id_t lightId, Light &light, ChunkMap &chunkMap);
	void updateLight(light_id_t lightId, Light &light, ChunkMap &chunkMap);
	void clearLightMapping(int lightId, ChunkMap &chunkMap);
	void boundRayCast(std::int64_t i, std::int64_t j, std::int64_t tileSize);
	void applyLightDependencyPath(int lightId, Light &light, BoundRayCastNode &currentNode,
								  ChunkMap &chunkMap);
};
