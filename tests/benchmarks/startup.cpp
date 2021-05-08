#include <SolasLib/core.hpp>
#include <glm/glm.hpp>
#include <benchmark/benchmark.h>

namespace startupVars {
const int CHUNK_SIZE = 100;
const int TILE_SIZE = 8;
} // namespace startupVars

static void STARTUP(benchmark::State &state) {
	for (auto _ : state) {
		new LightmapManager(startupVars::TILE_SIZE, CastingAlgorithm::BOUND_RAY_CAST,
							startupVars::CHUNK_SIZE);
	}
}
