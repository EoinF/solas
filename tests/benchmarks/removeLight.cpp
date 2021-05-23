#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace removeVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace removeVars

static void REMOVE_LIGHT(benchmark::State &state, CastingAlgorithm algorithm,
						 std::int64_t lightRange) {
	LightmapManager lightmapManager(removeVars::TILE_SIZE, algorithm, removeVars::CHUNK_SIZE);
	for (auto _ : state) {
		state.PauseTiming();
		auto lightId =
			lightmapManager.addLight(30, 30, glm::pi<float>(), removeVars::TILE_SIZE * lightRange);
		lightmapManager.update();
		state.ResumeTiming();
		lightmapManager.removeLight(lightId);
		lightmapManager.update();
	}
}
