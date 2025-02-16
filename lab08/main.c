#ifdef WITH_AVX2_
  #include <immintrin.h>
#endif

#ifdef WITH_NEON_
  #include <arm_neon.h>
#endif

#ifdef _OPENMP
  #include <omp.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef float data_t;

#define PRINT_VEC(arr, n) \
  for (int i = 0; i < (n); i++) \
  { \
    printf("%f ", arr[i]); \
  } \
  printf("\n");

clock_t st, ed;
#define TIME_START \
  st = clock();
#define TIME_END(str) \
  ed = clock(); \
  printf("%s Time: %f\n", str, (double)(ed - st) / CLOCKS_PER_SEC);

// pure C function
int vec_add_(const data_t *a, const data_t *b, data_t *c,int n)
{
  if(a == NULL || b == NULL || c == NULL || n <= 0)
  {
    return 0;
  }

  for (size_t i = 0; i < n; i++)
  {
    c[i] = a[i] + b[i];
  }
  return 1;
}

int vec_add_simd_(const data_t *a, const data_t *b, data_t *c, int n)
{
    if(a == NULL || b == NULL || c == NULL || n <= 0)
    {
      return 0;
    }
  
    #ifdef WITH_AVX2_
      __m128 ra = _mm_setzero_ps();
      __m128 rb = _mm_setzero_ps();
      __m128 rc = _mm_setzero_ps();
  
      for (int i = 0; i < n / 4; i++)
      {
        ra = _mm_load_ps(a + i * 4);
        rb = _mm_load_ps(b + i * 4);
        rc = _mm_add_ps(ra, rb);
        _mm_store_ps(c + i * 4, rc);
      }
      
      // handle the remaining elements
      for (int i = n - n % 4; i < n; i++)
      {
        c[i] = a[i] + b[i];
      }
  
      return 1;
  
    #elif defined(WITH_NEON_)
      float32x4_t ra = vdupq_n_f32(0.0f);
      float32x4_t rb = vdupq_n_f32(0.0f);
      float32x4_t rc = vdupq_n_f32(0.0f);
  
      for (int i = 0; i < n / 4; i++)
      {
        ra = vld1q_f32(a + i * 4);
        rb = vld1q_f32(b + i * 4);
        rc = vaddq_f32(ra, rb);
        vst1q_f32(c + i * 4, rc);
      }
  
      // handle the remaining elements
      for (int i = n - n % 4; i < n; i++)
      {
        c[i] = a[i] + b[i];
      }
  
      return 1;
    #else
      printf("No SIMD support\n");
      return 0;
    #endif
}

int vec_add_simd_omp_(const data_t *a, const data_t *b, data_t *c, int n)
{
    if(a == NULL || b == NULL || c == NULL || n <= 0)
    {
      return 0;
    }

    #ifndef _OPENMP
      printf("No OpenMP support\n");
      return 0;
    #endif
  
    #ifdef WITH_AVX2_
      __m128 ra = _mm_setzero_ps();
      __m128 rb = _mm_setzero_ps();
      __m128 rc = _mm_setzero_ps();
  
      #pragma omp parallel for
      for (int i = 0; i < n / 4; i++)
      {
        ra = _mm_load_ps(a + i * 4);
        rb = _mm_load_ps(b + i * 4);
        rc = _mm_add_ps(ra, rb);
        _mm_store_ps(c + i * 4, rc);
      }
      
      // handle the remaining elements
      for (int i = n - n % 4; i < n; i++)
      {
        c[i] = a[i] + b[i];
      }
  
      return 1;
  
    #elif defined(WITH_NEON_)
      float32x4_t ra = vdupq_n_f32(0.0f);
      float32x4_t rb = vdupq_n_f32(0.0f);
      float32x4_t rc = vdupq_n_f32(0.0f);
  
      #pragma omp parallel for
      for (int i = 0; i < n / 4; i++)
      {
        ra = vld1q_f32(a + i * 4);
        rb = vld1q_f32(b + i * 4);
        rc = vaddq_f32(ra, rb);
        vst1q_f32(c + i * 4, rc);
      }
  
      // handle the remaining elements
      for (int i = n - n % 4; i < n; i++)
      {
        c[i] = a[i] + b[i];
      }
  
      return 1;
    #else
      printf("No SIMD support\n");
      return 0;
    #endif
}

data_t *rand_vec(size_t n)
{
  data_t *arr = (data_t *)malloc(n * sizeof(data_t));
  for (int i = 0; i < n; i++)
  {
    arr[i] = rand() % 100 - 50 + rand() / (RAND_MAX + 1.0);
  }
  return arr;
}

int main()
{
    int n = 10;
    srand(time(NULL));
    data_t *a = rand_vec(n);
    data_t *b = rand_vec(n);
    data_t *c = (data_t *)malloc(n * sizeof(data_t));
    data_t *c_simd = (data_t *)malloc(n * sizeof(data_t));
    data_t *c_simd_omp = (data_t *)malloc(n * sizeof(data_t));
    
    printf("--- Before vec_add ---\n");
    PRINT_VEC(a, n);
    PRINT_VEC(b, n);
    vec_add_(a, b, c, n);
    vec_add_simd_(a, b, c_simd, n);
    vec_add_simd_omp_(a, b, c_simd_omp, n);
    printf("\n");
    printf("--- After vec_add plain ---\n");
    PRINT_VEC(c, n);
    printf("--- After vec_add with SIMD ---\n");
    PRINT_VEC(c_simd, n);
    printf("--- After vec_add with SIMD and OpenMP ---\n");
    PRINT_VEC(c_simd_omp, n);
    printf("\n");
    
    free(a);
    free(b);
    free(c);

    // start timing on 1M vector adding, no output vec
    n = 1024 * 1024 * 8;
    a = rand_vec(n);
    b = rand_vec(n);
    c = (data_t *)malloc(n * sizeof(data_t));

    TIME_START;
    vec_add_(a, b, c, n);
    TIME_END("pure C vector adding");

    TIME_START;
    vec_add_simd_(a, b, c, n);
    TIME_END("SIMD vector adding");

    TIME_START;
    vec_add_simd_omp_(a, b, c, n);
    TIME_END("SIMD and OpenMP vector adding");
    
    return 0;
}