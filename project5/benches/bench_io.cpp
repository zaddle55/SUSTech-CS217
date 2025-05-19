#include "../src/mcv.decl.h"
#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>
using namespace std;
using namespace mcv;

const string IMG_DIR = "./img/";

static void encode_1cNxN(benchmark::State &state) {
  size_t N = state.range(0);
  string fname = IMG_DIR + "1c" + to_string(N) + "x" + to_string(N) + ".bmp";
  for (auto _ : state) {
    state.PauseTiming();

    // create NxN 1 Channel image
    pMat<1> out(N, N);
    out.fill(color::WHITE);
    state.ResumeTiming();
    imwrite(fname, out);

    benchmark::ClobberMemory();
  }
  state.SetComplexityN(N);
}

BENCHMARK(encode_1cNxN)
    ->RangeMultiplier(2)
    ->Range(128, 8192)
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::oNSquared);

static void decode_1cNxN(benchmark::State &state) {
  size_t N = state.range(0);
  string fname = IMG_DIR + "1c" + to_string(N) + "x" + to_string(N) + ".bmp";
  for (auto _ : state) {

    auto pMat = imread<1>(fname);

    benchmark::ClobberMemory();
  }
  state.SetComplexityN(N);
}

BENCHMARK(decode_1cNxN)
    ->RangeMultiplier(2)
    ->Range(128, 8192)
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::oNSquared);

static void encode_Nc4096x4096(benchmark::State &state) {
  size_t N = state.range(0);
  string fname = IMG_DIR + to_string(N) + "c4096x4096" + ".bmp";
  for (auto _ : state) {
    state.PauseTiming();

    // create 4096x4096 N Channel image
    if (N == 1) {
      pMat<1> out(4096, 4096);
      out.fill(color::WHITE);
      state.ResumeTiming();
      imwrite(fname, out);
    } else if (N == 3){
      pMat<3> out(4096, 4096);
      out.fill(color::WHITE);
      state.ResumeTiming();
      imwrite(fname, out);
    } else {
      pMat<4> out(4096, 4096);
      out.fill(color::WHITE);
      state.ResumeTiming();
      imwrite(fname, out);
    }

    benchmark::ClobberMemory();
  }
  state.SetComplexityN(4096*4096 * N);
}

BENCHMARK(encode_Nc4096x4096)
    ->Arg(1)
    ->Arg(3)
    ->Arg(4)
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::o1);

static void decode_Nc4096x4096(benchmark::State &state) {
  size_t N = state.range(0);
  string fname = IMG_DIR + to_string(N) + "c4096x4096" + ".bmp";
  for (auto _ : state) {

    // create 4096x4096 N Channel image
    if (N == 1) {
      auto pMat = imread<1>(fname);
    } else if (N == 3) {
      auto pMat = imread<3>(fname);
    } else {
      auto pMat = imread<4>(fname);
    }

    benchmark::ClobberMemory();
  }
  state.SetComplexityN(N);
}

BENCHMARK(decode_Nc4096x4096)
    ->Arg(1)
    ->Arg(3)
    ->Arg(4)
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::o1);

static void encode_1cNxM(benchmark::State &state) {
  size_t N = state.range(0);
  size_t M = state.range(1);
  string fname = IMG_DIR + "1c" + to_string(N) + "x" + to_string(M) + ".bmp";
  for (auto _ : state) {
    state.PauseTiming();

    // create NxN 1 Channel image
    pMat<1> out(N, M);
    out.fill(color::WHITE);
    state.ResumeTiming();
    imwrite(fname, out);

    benchmark::ClobberMemory();
  }
  state.SetComplexityN(N * M);
}

BENCHMARK(encode_1cNxM)
    ->Args({240, 4860})
    ->Args({360, 3240})
    ->Args({480, 2430})
    ->Args({720, 1620})
    ->Args({810, 1440})
    ->Args({1080, 1080})
    ->Args({1440, 810})
    ->Args({1620, 720})
    ->Args({2430, 480})
    ->Args({3240, 360})
    ->Args({4860, 240})
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::oN);

static void decode_1cNxM(benchmark::State &state) {
  size_t N = state.range(0);
  size_t M = state.range(1);
  string fname = IMG_DIR + "1c" + to_string(N) + "x" + to_string(M) + ".bmp";
  for (auto _ : state) {
    auto in = imread<1>(fname);

    benchmark::ClobberMemory();
  }
  state.SetComplexityN(N * M);
}

BENCHMARK(decode_1cNxM)
    ->Args({240, 4860})
    ->Args({360, 3240})
    ->Args({480, 2430})
    ->Args({720, 1620})
    ->Args({810, 1440})
    ->Args({1080, 1080})
    ->Args({1440, 810})
    ->Args({1620, 720})
    ->Args({2430, 480})
    ->Args({3240, 360})
    ->Args({4860, 240})
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();