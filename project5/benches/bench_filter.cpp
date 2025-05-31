#include "../src/mcv.decl.h"
#include <benchmark/benchmark.h>
#include <string>
using namespace std;
using namespace mcv;

const string IMG_DIR = "./img/";

static void boxfilter_NxM(benchmark::State &state) {
  size_t N = state.range(0);
  size_t M = state.range(1);
  string fname =
      IMG_DIR + "sample_" + to_string(N) + "x" + to_string(M) + ".jpg";
  pMat img = imread<3>(fname);
  for (auto _ : state) {
    
    // apply box filter
    auto filtered = conv2d(img, getBoxFilter<3>());
    benchmark::ClobberMemory();
  }
  state.SetComplexityN(N);
  state.counters["width"] = N;
  state.counters["height"] = M;
}

static void boxfilter_KxK(benchmark::State &state) {
  size_t K = state.range(0);
  string fname = IMG_DIR + "sample_600x400.jpg";
  pMat img = imread<3>(fname);
  for (auto _ : state) {
    
    // apply box filter
    if (K == 3) {
      auto filtered = conv2d(img, getBoxFilter<3>());
    } else if (K == 5) {
      auto filtered = conv2d(img, getBoxFilter<5>());
    } else if (K == 7) {
      auto filtered = conv2d(img, getBoxFilter<7>());
    } else if (K == 9) {
      auto filtered = conv2d(img, getBoxFilter<9>());
    } else if (K == 11) {
      auto filtered = conv2d(img, getBoxFilter<11>());
    } else {
      throw std::invalid_argument("Unsupported kernel size: " + to_string(K));
    }
  }
  state.SetComplexityN(K);
  state.counters["kernel_size"] = K;
}

BENCHMARK(boxfilter_NxM)
    ->Args({300, 200})
    ->Args({600, 400})
    ->Args({900, 600})
    ->Args({1200, 800})
    ->Args({1800, 1200})
    ->Args({2400, 1600})
    ->Args({3000, 2000})
    ->Args({3600, 2400})
    ->Args({6000, 4000})
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNSquared);

BENCHMARK(boxfilter_KxK)
    ->Args({3})
    ->Args({5})
    ->Args({7})
    ->Args({9})
    ->Args({11})
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNSquared);

BENCHMARK_MAIN();