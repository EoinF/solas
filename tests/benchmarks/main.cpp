#include "addLight.cpp"
#include "updateLight.cpp"
#include "removeLight.cpp"
#include "startup.cpp"
#include <benchmark/benchmark.h>

const auto RUNNING_TIME_SECONDS = 5;

BENCHMARK(STARTUP)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);

BENCHMARK(ADD_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK(ADD_LIGHT_MID_RANGE)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK(ADD_LIGHT_LONG_RANGE)->Unit(benchmark::kMillisecond)->MinTime(5);

BENCHMARK(REMOVE_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK(REMOVE_LIGHT_LONG_RANGE)->Unit(benchmark::kMillisecond)->MinTime(5);

BENCHMARK(MOVE_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(MOVE_LIGHT_LONG_RANGE)->Unit(benchmark::kMillisecond)->MinTime(5);
BENCHMARK(BROADEN_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(BROADEN_LIGHT_LONG_RANGE)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(ROTATE_LIGHT)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);
BENCHMARK(ROTATE_LIGHT_LONG_RANGE)->Unit(benchmark::kMillisecond)->MinTime(RUNNING_TIME_SECONDS);

BENCHMARK_MAIN();
