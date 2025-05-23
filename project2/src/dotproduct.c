#include <bits/types/FILE.h>
#include <time.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#ifdef __EP4__
#include "cudakernel.h"
#endif

#include <stddef.h>

#define CACHE_SIZE 8

#ifdef __x86_64__
#include <immintrin.h>
#define SSE_
#define AVX_
#elif defined(__aarch64__) || defined(__arm__)
#include <arm_neon.h>
#define NEON_
#endif

#define _POSIX_C_SOURCE 200809L

#if defined(__EP2__)
#define CPU_TIME_BEGIN \
    double start = omp_get_wtime(); \

#define CPU_TIME_END \
    double end = omp_get_wtime(); \
    printf("%.9f\n", end - start);
#else
#define CPU_TIME_BEGIN \
    struct timespec start, end; \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); \

#define CPU_TIME_END \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); \
    double elapsed = (end.tv_sec - start.tv_sec) + \
                     (end.tv_nsec - start.tv_nsec) / 1e9; \
    printf("%.9f\n", elapsed);
#endif

typedef signed char i8;
typedef short i16;
typedef int i32;
typedef float f32;
typedef double f64;
typedef long long scalar_i;
typedef long double scalar_f;

#define vec_create(__tp, __name, __size) \
    (__tp *)malloc(sizeof(__tp) * __size); \
    if (!__name) { \
        fprintf(stderr, "Failed to allocate memory for " #__name "\n"); \
        exit(EXIT_FAILURE); \
    } \
    memset(__name, 0, sizeof(__tp) * __size); \

#define vec_randi(__name, __size, __start, __end) \
    for (size_t i = 0; i < __size; i++) { \
        __name[i] = (rand() % (__end - __start + 1) + __start); \
    } \

#define vec_randf(__name, __size, __start, __end) \
    for (size_t i = 0; i < __size; i++) { \
        __name[i] = ((double)rand() / RAND_MAX) * (__end - __start) + __start; \
    } \

#define vec_free(__name) \
    if (__name == NULL) { \
        fprintf(stderr, "Vector pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    free(__name); \
    __name = NULL; \

#define vec_fromFileStream(__name, __fp, __size) \
    if (__fp == NULL) { \
        fprintf(stderr, "File pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    if (__name == NULL) { \
        fprintf(stderr, "Vector pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    _Generic((__name), \
    i8 *: vec_fromFileStream_i8, \
    i16 *: vec_fromFileStream_i16, \
    i32 *: vec_fromFileStream_i32, \
    f32 *: vec_fromFileStream_f32, \
    f64 *: vec_fromFileStream_f64 \
)(__fp, __name, __size)

#define dotprod_plain(__a, __b, __result, __size) \
    if (__a == NULL || __b == NULL) { \
        fprintf(stderr, "Vector pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    if (__result == NULL) { \
        fprintf(stderr, "Result pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    _Generic((__a), \
    i8 *: dotproduct_plain_i8, \
    i16 *: dotproduct_plain_i16, \
    i32 *: dotproduct_plain_i32, \
    f32 *: dotproduct_plain_f32, \
    f64 *: dotproduct_plain_f64 \
)(__a, __b, __result, __size)

#define dotprod_unwind(__a, __b, __result, __size) \
    if (__a == NULL || __b == NULL) { \
        fprintf(stderr, "Vector pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    if (__result == NULL) { \
        fprintf(stderr, "Result pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    _Generic((__a), \
    i8 *: dotproduct_unwind_i8, \
    i16 *: dotproduct_unwind_i16, \
    i32 *: dotproduct_unwind_i32, \
    f32 *: dotproduct_unwind_f32, \
    f64 *: dotproduct_unwind_f64 \
)(__a, __b, __result, __size)

#define dotprod_parallel(__a, __b, __result, __size) \
    if (__a == NULL || __b == NULL) { \
        fprintf(stderr, "Vector pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    if (__result == NULL) { \
        fprintf(stderr, "Result pointer is NULL\n"); \
        exit(EXIT_FAILURE); \
    } \
    _Generic((__a), \
    i8 *: dotproduct_parallel_i8, \
    i16 *: dotproduct_parallel_i16, \
    i32 *: dotproduct_parallel_i32, \
    f32 *: dotproduct_parallel_f32, \
    f64 *: dotproduct_parallel_f64 \
)(__a, __b, __result, __size)

void vec_fromFileStream_i8(FILE *fp, i8 *vec, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (fscanf(fp, "%hhd", &vec[i]) != 1) {
            fprintf(stderr, "Failed to read int8 type from filestream.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void vec_fromFileStream_i16(FILE *fp, i16 *vec, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (fscanf(fp, "%hd", &vec[i]) != 1) {
            fprintf(stderr, "Failed to read int16 type from filestream.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void vec_fromFileStream_i32(FILE *fp, i32 *vec, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (fscanf(fp, "%d", &vec[i]) != 1) {
            fprintf(stderr, "Failed to read int32 type from filestream.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void vec_fromFileStream_f32(FILE *fp, f32 *vec, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (fscanf(fp, "%f", &vec[i]) != 1) {
            fprintf(stderr, "Failed to read float32 type from filestream.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void vec_fromFileStream_f64(FILE *fp, f64 *vec, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (fscanf(fp, "%lf", &vec[i]) != 1) {
            fprintf(stderr, "Failed to read float64 type from filestream.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void dotproduct_plain_i8(i8 *a, i8 *b, scalar_i *result, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_plain_i16(i16 *a, i16 *b, scalar_i *result, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *result += a[i] * b[i];
    }
}
void dotproduct_plain_i32(i32 *a, i32 *b, scalar_i *result, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *result += a[i] * b[i];
    }
}
void dotproduct_plain_f32(f32 *a, f32 *b, scalar_f *result, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *result += a[i] * b[i];
    }
}
void dotproduct_plain_f64(f64 *a, f64 *b, scalar_f *result, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_unwind_i8(i8 *a, i8 *b, scalar_i *result, size_t size) {
    for (int i = 0; i < size; i += CACHE_SIZE) {
        *result += a[i] * b[i];
        *result += a[i + 1] * b[i + 1];
        *result += a[i + 2] * b[i + 2];
        *result += a[i + 3] * b[i + 3];
        *result += a[i + 4] * b[i + 4];
        *result += a[i + 5] * b[i + 5];
        *result += a[i + 6] * b[i + 6];
        *result += a[i + 7] * b[i + 7];
    }
    for (int i = size - size % CACHE_SIZE; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_unwind_i16(i16 *a, i16 *b, scalar_i *result, size_t size) {
    for (int i = 0; i < size; i += CACHE_SIZE) {
        *result += a[i] * b[i];
        *result += a[i + 1] * b[i + 1];
        *result += a[i + 2] * b[i + 2];
        *result += a[i + 3] * b[i + 3];
        *result += a[i + 4] * b[i + 4];
        *result += a[i + 5] * b[i + 5];
        *result += a[i + 6] * b[i + 6];
        *result += a[i + 7] * b[i + 7];
    }
    for (int i = size - size % CACHE_SIZE; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_unwind_i32(i32 *a, i32 *b, scalar_i *result, size_t size) {
    for (int i = 0; i < size; i += CACHE_SIZE) {
        *result += a[i] * b[i];
        *result += a[i + 1] * b[i + 1];
        *result += a[i + 2] * b[i + 2];
        *result += a[i + 3] * b[i + 3];
        *result += a[i + 4] * b[i + 4];
        *result += a[i + 5] * b[i + 5];
        *result += a[i + 6] * b[i + 6];
        *result += a[i + 7] * b[i + 7];
    }
    for (int i = size - size % CACHE_SIZE; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_unwind_f32(f32 *a, f32 *b, scalar_f *result, size_t size) {
    for (int i = 0; i < size; i += CACHE_SIZE) {
        *result += a[i] * b[i];
        *result += a[i + 1] * b[i + 1];
        *result += a[i + 2] * b[i + 2];
        *result += a[i + 3] * b[i + 3];
        *result += a[i + 4] * b[i + 4];
        *result += a[i + 5] * b[i + 5];
        *result += a[i + 6] * b[i + 6];
        *result += a[i + 7] * b[i + 7];
    }
    for (int i = size - size % CACHE_SIZE; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_unwind_f64(f64 *a, f64 *b, scalar_f *result, size_t size) {
    for (int i = 0; i < size; i += CACHE_SIZE) {
        *result += a[i] * b[i];
        *result += a[i + 1] * b[i + 1];
        *result += a[i + 2] * b[i + 2];
        *result += a[i + 3] * b[i + 3];
        *result += a[i + 4] * b[i + 4];
        *result += a[i + 5] * b[i + 5];
        *result += a[i + 6] * b[i + 6];
        *result += a[i + 7] * b[i + 7];
    }
    for (int i = size - size % CACHE_SIZE; i < size; i++) {
        *result += a[i] * b[i];
    }
}

void dotproduct_parallel_i8(i8 *a, i8 *b, scalar_i *result, size_t size) {
    scalar_i temp = 0;
    #pragma omp parallel for reduction(+:temp)
    for (size_t i = 0; i < size; i++) {
        temp += a[i] * b[i];
    }
    *result += temp;
}

void dotproduct_parallel_i16(i16 *a, i16 *b, scalar_i *result, size_t size) {
    scalar_i temp = 0;
    #pragma omp parallel for reduction(+:temp)
    for (size_t i = 0; i < size; i++) {
        temp += a[i] * b[i];
    }
    *result += temp;
}

void dotproduct_parallel_i32(i32 *a, i32 *b, scalar_i *result, size_t size) {
    scalar_i temp = 0;
    #pragma omp parallel for reduction(+:temp)
    for (size_t i = 0; i < size; i++) {
        temp += a[i] * b[i];
    }
    *result += temp;
}

void dotproduct_parallel_f32(f32 *a, f32 *b, scalar_f *result, size_t size) {
    scalar_f temp = 0;
    #pragma omp parallel for reduction(+:temp)
    for (size_t i = 0; i < size; i++) {
        temp += a[i] * b[i];
    }
    *result += temp;
}

void dotproduct_parallel_f64(f64 *a, f64 *b, scalar_f *result, size_t size) {
    scalar_f temp = 0;
    #pragma omp parallel for reduction(+:temp)
    for (size_t i = 0; i < size; i++) {
        temp += a[i] * b[i];
    }
    *result += temp;
}

void dotproduct_simdi(register i32 *a, register i32 *b, register scalar_i *result, register size_t size) {

    #ifdef SSE_
        register __m128i lo = _mm_setzero_si128();
        register __m128i hi = _mm_setzero_si128();

        register __m128i* pa = (__m128i*)a;
        register __m128i* pb = (__m128i*)b;
        
        // Process 4 elements at a time (SSE registers are 128-bit = 4 x int32)
        register size_t i = 0;
        for (; i + 3 <= size; i += 4) {
            // Load 4 int32 values from each array
            register __m128i va = _mm_lddqu_si128(pa + (i >> 2ULL));
            register __m128i vb = _mm_lddqu_si128(pb + (i >> 2ULL));
            
            // Multiply with extended precision: 32-bit * 32-bit -> 64-bit
            // _mm_mul_epi32 only multiplies odd indices
            
            // Multiply odd indices (1, 3)
            register __m128i mul_odd = _mm_mul_epi32(va, vb);
            
            // Shift right to get even indices (0, 2) in position
            va = _mm_srli_epi64(va, 32);
            vb = _mm_srli_epi64(vb, 32);
            
            // Multiply even indices (0, 2)
            register __m128i mul_even = _mm_mul_epi32(va, vb);

            lo = _mm_add_epi64(lo, mul_odd);
            hi = _mm_add_epi64(hi, mul_even);
        }

        lo = _mm_add_epi64(lo, hi);

        // Extract results from the __m128i
        *result += _mm_extract_epi64(lo, 0) + _mm_extract_epi64(lo, 1);
        
        // Handle remaining elements
        for (; i < size; i++) {
            *result += (int64_t)a[i] * b[i];
        }
    #elif defined(NEON_)
        int64x2_t sum_0 = vdupq_n_s64(0);
        int64x2_t sum_1 = vdupq_n_s64(0);
        
        // Process 4 elements at a time (2 elements per loop iteration per vector)
        size_t i = 0;
        for (; i + 3 < length; i += 4) {
            // Load 2 int32 values into each vector
            int32x2_t va_0 = vld1_s32(&a[i]);
            int32x2_t vb_0 = vld1_s32(&b[i]);
            int32x2_t va_1 = vld1_s32(&a[i+2]);
            int32x2_t vb_1 = vld1_s32(&b[i+2]);
            
            // Multiply with extended precision: 32-bit * 32-bit -> 64-bit
            int64x2_t prod_0 = vmull_s32(va_0, vb_0);
            int64x2_t prod_1 = vmull_s32(va_1, vb_1);
            
            // Accumulate the products
            sum_0 = vaddq_s64(sum_0, prod_0);
            sum_1 = vaddq_s64(sum_1, prod_1);
        }
        
        // Combine the partial sums
        int64x2_t sum = vaddq_s64(sum_0, sum_1);
        
        // Extract and sum the two 64-bit values
        *result = vgetq_lane_s64(sum, 0) + vgetq_lane_s64(sum, 1);
        
        for (; i < length; i++) {
            *result += (int64_t)a[i] * b[i];
        }    
    #else
        printf("No SIMD support available. Falling back to plain dot product.\n");
        // Fallback to plain dot product if SIMD is not available
        for (size_t i = 0; i < size; i++) {
            *result += a[i] * b[i];
        }
    #endif
    }

void dotproduct_simdf(register f64* a, register f64* b, register scalar_f* result, register size_t size) {
    #ifdef AVX_
        register __m128d lo = _mm_setzero_pd();
        register __m128d hi = _mm_setzero_pd();
        
        // Process 2 elements at a time (SSE registers are 128-bit = 2 x float64)
        register size_t i = 0;
        for (; i + 1 <= size; i += 2) {
            // Load 2 float64 values from each array
            register __m128d va = _mm_loadu_pd(&a[i]);
            register __m128d vb = _mm_loadu_pd(&b[i]);
            
            // Multiply with extended precision: 64-bit * 64-bit -> 128-bit
            register __m128d mul = _mm_mul_pd(va, vb);
            
            lo = _mm_add_pd(lo, mul);
        }

        lo = _mm_add_pd(lo, hi);

        // Extract results from the register __m128d
        *result += _mm_cvtsd_f64(lo) + _mm_cvtsd_f64(_mm_unpackhi_pd(lo, lo));
        
        // Handle remaining elements
        for (; i < size; i++) {
            *result += a[i] * b[i];
        }
    #elif defined(NEON_)
        float64x2_t sum = vdupq_n_f64(0);
        
        // Process 2 elements at a time (2 elements per loop iteration per vector)
        size_t i = 0;
        for (; i + 1 < length; i += 2) {
            // Load 2 float64 values into each vector
            float64x2_t va = vld1q_f64(&a[i]);
            float64x2_t vb = vld1q_f64(&b[i]);
            
            // Multiply with extended precision: 64-bit * 64-bit -> 128-bit
            float64x2_t prod = vmulq_f64(va, vb);
            
            // Accumulate the products
            sum = vaddq_f64(sum, prod);
        }
        
        // Extract and sum the two 64-bit values
        *result = vgetq_lane_f64(sum, 0) + vgetq_lane_f64(sum, 1);
        
        for (; i < length; i++) {
            *result += a[i] * b[i];
        }
    #else
        printf("No SIMD support available. Falling back to plain dot product.\n");
        // Fallback to plain dot product if SIMD is not available
        for (size_t i = 0; i < size; i++) {
            *result += a[i] * b[i];
        }
    #endif
}

int main(int argc, char *argv[])
{
    srand(time(NULL)); 
    #ifdef __EP1__
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <size> <data_type> \n", argv[0]);
        return EXIT_FAILURE;
    }
    size_t size = atoll(argv[1]);
    char* data_t = argv[2];
    if (strcmp(data_t, "char") == 0) {
        i8 *a = vec_create(i8, a, size);
        i8 *b = vec_create(i8, b, size);
        scalar_i result = 0;

        vec_randi(a, size, -2, 1);
        vec_randi(b, size, -2, 1);
        CPU_TIME_BEGIN
        dotprod_plain(a, b, &result, size);
        CPU_TIME_END

        // printf("Dot product result: %lld\n", result);
        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "short") == 0) {
        i16 *a = vec_create(i16, a, size);
        i16 *b = vec_create(i16, b, size);
        scalar_i result = 0;

        vec_randi(a, size, -2, 1);
        vec_randi(b, size, -2, 1);
        CPU_TIME_BEGIN
        dotprod_plain(a, b, &result, size);
        CPU_TIME_END

        // printf("Dot product result: %lld\n", result);
        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "int") == 0) {
        i32 *a = vec_create(i32, a, size);
        i32 *b = vec_create(i32, b, size);
        scalar_i result = 0;

        vec_randi(a, size, -2, 1);
        vec_randi(b, size, -2, 1);
        CPU_TIME_BEGIN
        dotprod_plain(a, b, &result, size);
        CPU_TIME_END

        // printf("Dot product result: %lld\n", result);
        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "float") == 0) {
        f32 *a = vec_create(f32, a, size);
        f32 *b = vec_create(f32, b, size);
        scalar_f result = 0;

        vec_randf(a, size, -2.0f, 1.0f);
        vec_randf(b, size, -2.0f, 1.0f);
        CPU_TIME_BEGIN
        dotprod_plain(a, b, &result, size);
        CPU_TIME_END

        // printf("Dot product result: %lf\n", result);
        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "double") == 0) {
        f64 *a = vec_create(f64, a, size);
        f64 *b = vec_create(f64, b, size);
        scalar_f result = 0;

        vec_randf(a, size, -2.0f , 1.0f);
        vec_randf(b, size , -2.0f , 1.f);

        CPU_TIME_BEGIN
        dotprod_plain(a, b, &result, size);
        CPU_TIME_END

        vec_free(a);
        vec_free(b);
    } else {
         fprintf(stderr,"Unsupported data type: %s\n", data_t);
            return EXIT_FAILURE;
    }
    #endif

    #ifdef __EP2__
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <size> <data_type> <thread_numt>\n", argv[0]);
    }
    size_t size = atoll(argv[1]);
    char* data_t = argv[2];
    int thread_num = atoi(argv[3]);
    omp_set_num_threads(thread_num);
    if (strcmp(data_t, "char") == 0) {
        i8 *a = vec_create(i8, a, size);
        i8 *b = vec_create(i8, b, size);
        scalar_i result = 0;

        vec_randi(a, size, -2, 1);
        vec_randi(b, size, -2, 1);
        CPU_TIME_BEGIN
        dotprod_parallel(a, b, &result, size);
        CPU_TIME_END

        // printf("Dot product result: %lld\n", result);
        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "short") == 0) {
        i16 *a = vec_create(i16, a, size);
        i16 *b = vec_create(i16, b, size);
        scalar_i result = 0;

        vec_randi(a, size, -2, 1);
        vec_randi(b, size, -2, 1);
        CPU_TIME_BEGIN
        dotprod_parallel(a, b, &result, size);
        CPU_TIME_END

        // printf("Dot product result: %lld\n", result);
        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "int") == 0) {
        i32 *a = vec_create(i32, a, size);
        i32 *b = vec_create(i32, b, size);
        scalar_i result = 0;

        vec_randi(a, size , -2 , 1 );
        vec_randi(b , size , -2 , 1 );
        
        CPU_TIME_BEGIN
        dotprod_parallel(a , b , &result , size );
        CPU_TIME_END

       // printf("Dot product result: %lld\n", result);
       vec_free(a);
       vec_free(b);

    } else if (strcmp(data_t , "float") == 0) {
       f32 *a = vec_create(f32 , a , size );
       f32 *b = vec_create(f32 , b , size );
       scalar_f result = 0;

       vec_randf(a , size , -2.0f , 1.0f );
       vec_randf(b , size , -2.0f , 1.0f );

       CPU_TIME_BEGIN
       dotprod_parallel(a , b , &result , size );
       CPU_TIME_END
    } else if (strcmp(data_t , "double") == 0) {
       f64 *a = vec_create(f64 , a , size );
       f64 *b = vec_create(f64 , b , size );
       scalar_f result = 0;

       vec_randf(a , size , -2.0f , 1.0f );
       vec_randf(b , size , -2.0f , 1.0f );

       CPU_TIME_BEGIN
       dotprod_parallel(a , b , &result , size );
       CPU_TIME_END
    } else {
        fprintf(stderr,"Unsupported data type: %s\n", data_t);
        return EXIT_FAILURE;
    }
    #endif

    #ifdef __EP4__
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <size> <data_type> \n", argv[0]);
        return EXIT_FAILURE;
    }
    size_t size = atoll(argv[1]);
    char* data_t = argv[2];
    if (strcmp(data_t, "int") == 0) {
        i32 *a = vec_create(i32, a, size);
        i32 *b = vec_create(i32, b, size);
        scalar_i result = 0;

        vec_randi(a, size, -2, 1);
        vec_randi(b, size, -2, 1);

        {
            printf(" === Plain === \n");
            CPU_TIME_BEGIN
            dotprod_plain(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %lld\n", result);
        }
        
        result = 0;

        {
            printf(" === Unroll === \n");
            CPU_TIME_BEGIN
            dotprod_unwind(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %lld\n", result);
        }

        result = 0;

        {
            printf(" === SIMD === \n");
            CPU_TIME_BEGIN
            dotproduct_simdi(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %lld\n", result);
        }

        result = 0;

        {
            printf(" === CUDA Parallel === \n");
            CPU_TIME_BEGIN
            dotproduct_cuda_i32(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %lld\n", result);
        }

        vec_free(a);
        vec_free(b);
    } else if (strcmp(data_t, "double") == 0) {
        f64 *a = vec_create(f64, a, size);
        f64 *b = vec_create(f64, b, size);
        scalar_f result = 0;

        vec_randf(a, size, -2.0f , 1.0f );
        vec_randf(b , size , -2.0f , 1.0f );

        {
            printf(" === Plain === \n");
            CPU_TIME_BEGIN
            dotprod_plain(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %Lf\n", result);
        }

        result = 0;

        {
            printf(" === Unroll === \n");
            CPU_TIME_BEGIN
            dotprod_unwind(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %Lf\n", result);
        }

        result = 0;

        {
            printf(" === SIMD === \n");
            CPU_TIME_BEGIN
            dotproduct_simdf(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %Lf\n", result);
        }

        result = 0;

        {
            printf(" === CUDA Parallel === \n");
            CPU_TIME_BEGIN
            dotproduct_cuda_f64(a, b, &result, size);
            CPU_TIME_END
            printf("Dot product result: %Lf\n", result);
        }

        vec_free(a);
        vec_free(b);
        
    } else {
         fprintf(stderr,"Unsupported data type: %s\n", data_t);
            return EXIT_FAILURE;
    }
    #endif

    return EXIT_SUCCESS;
}
