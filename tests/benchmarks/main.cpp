#include "addLight.cpp"
#include "updateLight.cpp"
#include "startup.cpp"
#include <benchmark/benchmark.h>

const auto RUNNING_TIME_SECONDS = 10;

BENCHMARK(STARTUP)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(ADD_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(BROADEN_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(MOVE_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(ROTATE_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);

BENCHMARK_MAIN();
