#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 4
#define TEST_DURATION 1  // 睡眠时间(秒)
#define NUM_ITERATIONS 5 // 实验重复次数
#define _POSIX_C_SOURCE 200809L

// 用于存储线程测试结果的结构体
typedef struct {
    double clock_duration;
    double monotonic_duration;
    double process_cputime_duration;
} ThreadResult;

// 测试函数 - 执行一些CPU密集型计算和一些睡眠
void perform_work() {
    // CPU密集型工作
    volatile double sum = 0.0;
    for (int i = 0; i < 100; i++) {
        sum += i * 0.1;
    }
    
    // 睡眠一段时间模拟I/O等待
    usleep(TEST_DURATION * 1000000);
}

// 单线程测试
void single_thread_test() {
    printf("\n=== 单线程测试 ===\n");
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        printf("\n迭代 %d:\n", iter + 1);
        
        // 使用clock()测量
        clock_t start_clock = clock();
        perform_work();
        clock_t end_clock = clock();
        double clock_duration = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
        
        // 使用clock_gettime() CLOCK_MONOTONIC测量
        struct timespec start_mono, end_mono;
        clock_gettime(CLOCK_MONOTONIC, &start_mono);
        perform_work();
        clock_gettime(CLOCK_MONOTONIC, &end_mono);
        double monotonic_duration = (end_mono.tv_sec - start_mono.tv_sec) +
                                    (end_mono.tv_nsec - start_mono.tv_nsec) / 1e9;
        
        // 使用clock_gettime() CLOCK_PROCESS_CPUTIME_ID测量
        struct timespec start_cpu, end_cpu;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
        perform_work();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
        double cpu_duration = (end_cpu.tv_sec - start_cpu.tv_sec) +
                              (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;
        
        printf("clock(): %.6f秒\n", clock_duration);
        printf("CLOCK_MONOTONIC: %.6f秒\n", monotonic_duration);
        printf("CLOCK_PROCESS_CPUTIME_ID: %.6f秒\n", cpu_duration);
    }
}

// 多线程测试函数
void* thread_test(void* arg) {
    ThreadResult* result = (ThreadResult*)arg;
    
    // 使用clock()测量
    clock_t start_clock = clock();
    perform_work();
    clock_t end_clock = clock();
    result->clock_duration = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
    
    // 使用clock_gettime() CLOCK_MONOTONIC测量
    struct timespec start_mono, end_mono;
    clock_gettime(CLOCK_MONOTONIC, &start_mono);
    perform_work();
    clock_gettime(CLOCK_MONOTONIC, &end_mono);
    result->monotonic_duration = (end_mono.tv_sec - start_mono.tv_sec) +
                                (end_mono.tv_nsec - start_mono.tv_nsec) / 1e9;
    
    // 使用clock_gettime() CLOCK_PROCESS_CPUTIME_ID测量
    struct timespec start_cpu, end_cpu;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
    perform_work();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    result->process_cputime_duration = (end_cpu.tv_sec - start_cpu.tv_sec) +
                                      (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;
    
    return NULL;
}

// 多线程测试
void multi_thread_test() {
    printf("\n=== 多线程测试 (%d线程) ===\n", NUM_THREADS);
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        printf("\n迭代 %d:\n", iter + 1);
        
        pthread_t threads[NUM_THREADS];
        ThreadResult thread_results[NUM_THREADS];
        
        // 使用process-wide clock()测量总体时间
        clock_t start_process_clock = clock();
        
        // 使用CLOCK_MONOTONIC测量总体时间
        struct timespec start_process_mono, end_process_mono;
        clock_gettime(CLOCK_MONOTONIC, &start_process_mono);
        
        // 创建线程
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_create(&threads[i], NULL, thread_test, &thread_results[i]);
        }
        
        // 等待所有线程完成
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
        
        // 测量process-wide结束时间
        clock_t end_process_clock = clock();
        clock_gettime(CLOCK_MONOTONIC, &end_process_mono);
        
        // 计算process-wide持续时间
        double process_clock_duration = (double)(end_process_clock - start_process_clock) / CLOCKS_PER_SEC;
        double process_mono_duration = (end_process_mono.tv_sec - start_process_mono.tv_sec) +
                                       (end_process_mono.tv_nsec - start_process_mono.tv_nsec) / 1e9;
        
        // 计算各方法的平均值
        double avg_clock = 0.0, avg_monotonic = 0.0, avg_cputime = 0.0;
        for (int i = 0; i < NUM_THREADS; i++) {
            avg_clock += thread_results[i].clock_duration;
            avg_monotonic += thread_results[i].monotonic_duration;
            avg_cputime += thread_results[i].process_cputime_duration;
        }
        avg_clock /= NUM_THREADS;
        avg_monotonic /= NUM_THREADS;
        avg_cputime /= NUM_THREADS;
        
        printf("进程级 clock(): %.6f秒\n", process_clock_duration);
        printf("进程级 CLOCK_MONOTONIC: %.6f秒\n", process_mono_duration);
        printf("线程平均 clock(): %.6f秒\n", avg_clock);
        printf("线程平均 CLOCK_MONOTONIC: %.6f秒\n", avg_monotonic);
        printf("线程平均 CLOCK_PROCESS_CPUTIME_ID: %.6f秒\n", avg_cputime);
        
        // 显示各线程的详细结果
        printf("\n各线程详细结果:\n");
        for (int i = 0; i < NUM_THREADS; i++) {
            printf("线程 %d:\n", i);
            printf("  clock(): %.6f秒\n", thread_results[i].clock_duration);
            printf("  CLOCK_MONOTONIC: %.6f秒\n", thread_results[i].monotonic_duration);
            printf("  CLOCK_PROCESS_CPUTIME_ID: %.6f秒\n", thread_results[i].process_cputime_duration);
        }
    }
}

int main() {
    printf("比较clock()和clock_gettime()的精度和行为差异\n");
    printf("测试持续时间: %d秒 x %d次迭代\n", TEST_DURATION, NUM_ITERATIONS);
    
    // 单线程测试
    single_thread_test();
    
    // 多线程测试
    multi_thread_test();
    
    return 0;
}