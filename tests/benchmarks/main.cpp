#include "addLight.cpp"
#include "updateLight.cpp"
#include <benchmark/benchmark.h>

BENCHMARK(ADD_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK(MOVE_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK_MAIN();
