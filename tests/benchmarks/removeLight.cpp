#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace removeVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace removeVars

static void REMOVE_LIGHT(benchmark::State &state) {
	LightmapManager lightmapManager(removeVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									removeVars::CHUNK_SIZE);
	for (auto _ : state) {
		state.PauseTiming();
		auto lightId =
			lightmapManager.addLight(30, 30, glm::pi<float>(), removeVars::TILE_SIZE * 8);
		lightmapManager.update();
		state.ResumeTiming();
		lightmapManager.removeLight(lightId);
		lightmapManager.update();
	}
}

static void REMOVE_LIGHT_LONG_RANGE(benchmark::State &state) {
	LightmapManager lightmapManager(removeVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									removeVars::CHUNK_SIZE);
	for (auto _ : state) {
		state.PauseTiming();
		auto lightId =
			lightmapManager.addLight(30, 30, glm::pi<float>(), removeVars::TILE_SIZE * 128);
		lightmapManager.update();
		state.ResumeTiming();
		lightmapManager.removeLight(lightId);
		lightmapManager.update();
	}
}
