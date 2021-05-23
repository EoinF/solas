#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace addVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace addVars

static void ADD_LIGHT(benchmark::State &state, CastingAlgorithm algorithm,
					  std::int64_t lightRange) {
	LightmapManager lightmapManager(addVars::TILE_SIZE, algorithm, addVars::CHUNK_SIZE);
	for (auto _ : state) {
		lightmapManager.addLight(30, 30, glm::pi<float>(), addVars::TILE_SIZE * lightRange);
		lightmapManager.update();
	}
}