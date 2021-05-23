#include "addLight.cpp"
#include "updateLight.cpp"
#include "removeLight.cpp"
#include "startup.cpp"
#include <functional>
#include <benchmark/benchmark.h>

const auto RUNNING_TIME_SECONDS = 5;

// BENCHMARK_CAPTURE(STARTUP, "BoundRayCast", CastingAlgorithm::BOUND_RAY_CAST)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(ADD_LIGHT, "BoundRayCast - 16 tile radius", CastingAlgorithm::BOUND_RAY_CAST,
// 16)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(ADD_LIGHT, "BoundRayCast - 128 tile radius", CastingAlgorithm::BOUND_RAY_CAST,
// 				  128)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(REMOVE_LIGHT, "BoundRayCast - 16 tile radius",
// CastingAlgorithm::BOUND_RAY_CAST, 				  16)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(REMOVE_LIGHT, "BoundRayCast - 128 tile radius",
// CastingAlgorithm::BOUND_RAY_CAST, 				  128)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(MOVE_LIGHT, "BoundRayCast - 16 tile radius", CastingAlgorithm::BOUND_RAY_CAST,
// 16)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);
// BENCHMARK_CAPTURE(MOVE_LIGHT, "BoundRayCast - 128 tile radius", CastingAlgorithm::BOUND_RAY_CAST,
// 				  128)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(BROADEN_LIGHT, "BoundRayCast - 16 tile radius",
// CastingAlgorithm::BOUND_RAY_CAST, 				  16)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);
// BENCHMARK_CAPTURE(BROADEN_LIGHT, "BoundRayCast - 128 tile radius",
// CastingAlgorithm::BOUND_RAY_CAST, 				  128)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

// BENCHMARK_CAPTURE(ROTATE_LIGHT, "BoundRayCast - 16 tile radius",
// CastingAlgorithm::BOUND_RAY_CAST, 				  16)

// BENCHMARK_CAPTURE(ROTATE_LIGHT, "BoundRayCast - 128 tile radius",
// CastingAlgorithm::BOUND_RAY_CAST, 				  128)
// 	->Unit(benchmark::kMillisecond)
// 	->MinTime(RUNNING_TIME_SECONDS);

std::map<std::string, CastingAlgorithm> nameToAlgorithmMap = {
	{"BoundRayCast", CastingAlgorithm::BOUND_RAY_CAST}};

std::map<std::string, std::function<void(benchmark::State &state, CastingAlgorithm algorithm,
										 std::int64_t lightRange)>>
	mainBenchmarks = {{"Add Light", ADD_LIGHT},
					  {"Remove Light", REMOVE_LIGHT},
					  {"Move Light", MOVE_LIGHT},
					  {"Broaden Light", BROADEN_LIGHT},
					  {"Rotate Light", ROTATE_LIGHT}};

int main(int argc, char **argv) {
	std::string testName;
	for (auto &algorithm : nameToAlgorithmMap) {
		testName = "Startup: " + algorithm.first;
		benchmark::RegisterBenchmark(testName.c_str(), STARTUP, CastingAlgorithm::BOUND_RAY_CAST)
			->Unit(benchmark::kMillisecond)
			->MinTime(RUNNING_TIME_SECONDS);

		for (auto benchmark : mainBenchmarks) {
			for (auto &lightRange : {8LL, 128LL}) {
				testName = benchmark.first + ": " + algorithm.first + " - " +
						   std::to_string(lightRange) + " tile radius";
				benchmark::RegisterBenchmark(testName.c_str(), benchmark.second, algorithm.second,
											 lightRange)
					->Unit(benchmark::kMillisecond)
					->MinTime(RUNNING_TIME_SECONDS);
				;
			}
		}
	}
	benchmark::Initialize(&argc, argv);
	benchmark::RunSpecifiedBenchmarks();
}
