#include "gtest/gtest.h"
#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;

static void ADD_LIGHT(benchmark::State& state) {
	LightmapManager lightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, CHUNK_SIZE);
	for (auto _ : state) {
		lightmapManager.addLight(30, 30, glm::pi<float>(), TILE_SIZE * 8);
		lightmapManager.update();
	}
}

static void MOVE_LIGHT(benchmark::State& state) {
	LightmapManager lightmapManager(TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST, CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), TILE_SIZE * 8);
	auto light = lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLight(lightId,
			light->x+1, light->y, light->span, light->direction);
		lightmapManager.update();
	}
}

BENCHMARK(ADD_LIGHT)->Unit(benchmark::kMillisecond);
BENCHMARK(MOVE_LIGHT)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();
