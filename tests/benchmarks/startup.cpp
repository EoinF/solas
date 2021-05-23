#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace startupVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace startupVars

static void STARTUP(benchmark::State &state, CastingAlgorithm algorithm) {
	for (auto _ : state) {
		new LightmapManager(startupVars::TILE_SIZE, algorithm, startupVars::CHUNK_SIZE);
	}
}
