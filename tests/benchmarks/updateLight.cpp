#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace updateVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
const int RANGE_NORMAL = 8 * TILE_SIZE;
const int RANGE_LONG = 128 * TILE_SIZE;
} // namespace updateVars

static void MOVE_LIGHT(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::RANGE_NORMAL);
	auto &light = *lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLightPosition(lightId, light.x + 1, light.y + 1);
		lightmapManager.update();
	}
}

static void MOVE_LIGHT_LONG_RANGE(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::RANGE_LONG);
	auto &light = *lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLightPosition(lightId, light.x + 1, light.y + 1);
		lightmapManager.update();
	}
}

static void ROTATE_LIGHT(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::RANGE_NORMAL);
	auto &light = *lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLightDirection(lightId,
											 glm::vec2(-light.direction.y, light.direction.x));
		lightmapManager.update();
	}
}

static void ROTATE_LIGHT_LONG_RANGE(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::RANGE_LONG);
	auto &light = *lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLightDirection(lightId,
											 glm::vec2(-light.direction.y, light.direction.x));
		lightmapManager.update();
	}
}

static void BROADEN_LIGHT(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::RANGE_NORMAL);
	auto &light = *lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLightSpan(lightId, light.span + 0.3f);
		lightmapManager.update();
	}
}

static void BROADEN_LIGHT_LONG_RANGE(benchmark::State &state) {
	LightmapManager lightmapManager(updateVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
									updateVars::CHUNK_SIZE);
	auto lightId = lightmapManager.addLight(30, 30, glm::pi<float>(), updateVars::RANGE_LONG);
	auto &light = *lightmapManager.getLightsMap()[lightId];
	for (auto _ : state) {
		lightmapManager.updateLightSpan(lightId, light.span + 0.3f);
		lightmapManager.update();
	}
}
