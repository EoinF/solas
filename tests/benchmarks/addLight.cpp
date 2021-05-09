#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace addVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace addVars

static void ADD_LIGHT(benchmark::State &state) {
	LightmapManager lightmapManager(addVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									addVars::CHUNK_SIZE);
	for (auto _ : state) {
		lightmapManager.addLight(30, 30, glm::pi<float>(), addVars::TILE_SIZE * 8);
		lightmapManager.update();
	}
}

static void ADD_LIGHT_MID_RANGE(benchmark::State &state) {
	LightmapManager lightmapManager(addVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									addVars::CHUNK_SIZE);
	for (auto _ : state) {
		lightmapManager.addLight(30, 30, glm::pi<float>(), addVars::TILE_SIZE * 64);
		lightmapManager.update();
	}
}

static void ADD_LIGHT_LONG_RANGE(benchmark::State &state) {
	LightmapManager lightmapManager(addVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									addVars::CHUNK_SIZE);
	for (auto _ : state) {
		lightmapManager.addLight(30, 30, glm::pi<float>(), addVars::TILE_SIZE * 128);
		lightmapManager.update();
	}
}
