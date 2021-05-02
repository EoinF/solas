#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace updateVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace updateVars

static void MOVE_LIGHT(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::TILE_SIZE * 8);
	auto light = lightmapManager.getLightsMap()[lightId].get();
	for (auto _ : state) {
		lightmapManager.updateLightPosition(lightId, light->x + 1, light->y);
		lightmapManager.update();
	}
}
