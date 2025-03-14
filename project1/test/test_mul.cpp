#include "math.h"
#include <functional>
#include <string>
#include <benchmark/benchmark.h>
#include <random>
#include <chrono>
#include <iostream>
using namespace std;


static random_device rd;
static mt19937 rng{rd()};
static uniform_int_distribution<> digit_distrib('0', '9');

static void generate_random_digits(string &s) {
    for (char &c : s)
        c = static_cast<char>(digit_distrib(rng));
    if (rng() & 1)
        s[0] = '-';
}

static void parse_speed(benchmark::State &state) {
    
    string num_str(state.range(0), '0');
    for (auto _ : state) {
        state.PauseTiming();
        generate_random_digits(num_str);
        benchmark::DoNotOptimize(num_str);
        state.ResumeTiming();
        Exn num = atoExn(num_str.c_str(),  state.range(0) + 10);
        if (num == NULL) {
            fprintf(stderr, "Failed to convert string to extended number\n");
            return;
        }
        Exn_release(&num);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(parse_speed)
    ->RangeMultiplier(10)
    ->Range(10, 1000000)
    ->Unit(benchmark::kMicrosecond);

static void mul_plain_speed(benchmark::State &state) {
    int size = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Exn num1 = Exn_rand(size);
        Exn num2 = Exn_rand(size);
        int len = num1->length + num2->length;
        int sign = num1->sign * num2->sign;
        state.ResumeTiming();
        Exn result = __Exn_mul_pl(num1, num2, len, sign);
        state.PauseTiming();
        if (result == NULL) {
            fprintf(stderr, "Failed to multiply extended numbers\n");
            return;
        }
        Exn_release(&num1);
        Exn_release(&num2);
        Exn_release(&result);

        benchmark::ClobberMemory();
    }
    state.SetComplexityN(size);
}

BENCHMARK(mul_plain_speed)
    ->RangeMultiplier(8)
    ->Range(8, 1<<16)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNSquared);

static void mul_karatsuba_speed(benchmark::State &state) {
    int size = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Exn num1 = Exn_rand(size);
        Exn num2 = Exn_rand(size);
        int len = num1->length + num2->length;
        int sign = num1->sign * num2->sign;
        state.ResumeTiming();
        Exn result = __Exn_mul_karatsuba(num1, num2, len, sign);
        state.PauseTiming();
        if (result == NULL) {
            fprintf(stderr, "Failed to multiply extended numbers\n");
            return;
        }
        Exn_release(&num1);
        Exn_release(&num2);
        Exn_release(&result);

        benchmark::ClobberMemory();
    }
    state.SetComplexityN(size);
}

BENCHMARK(mul_karatsuba_speed)
    ->RangeMultiplier(8)
    ->Range(8, 1<<16)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNLogN);

static void mul_fft_speed(benchmark::State &state) {
    int size = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Exn num1 = Exn_rand(size);
        Exn num2 = Exn_rand(size);
        int len = num1->length + num2->length;
        int sign = num1->sign * num2->sign;
        state.ResumeTiming();
        Exn result = __Exn_mul_fft(num1, num2, len, sign);
        state.PauseTiming();
        if (result == NULL) {
            fprintf(stderr, "Failed to multiply extended numbers\n");
            return;
        }
        Exn_release(&num1);
        Exn_release(&num2);
        Exn_release(&result);

        benchmark::ClobberMemory();
    }
    state.SetComplexityN(size);
}

BENCHMARK(mul_fft_speed)
    ->RangeMultiplier(8)
    ->Range(8, 1<<16)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNLogN);

BENCHMARK_MAIN();