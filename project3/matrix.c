#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <immintrin.h> // AVX intrinsics

#include "matrix.h"

#define OPT_THRESHOLD 1024 * 1024 // threshold for using OpenMP
#define BLOCK_SIZE 16      // block size for tiling
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void __pack_matrix_a(int m, int k, const float *src, int src_stride_, float *dst)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            dst[i * k + j] = src[i * src_stride_ + j];
        }
    }
}

static void __pack_matrix_b(int k, int n, const float *src, int src_stride_, float *dst)
{
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dst[j * k + i] = src[i * src_stride_ + j];
        }
    }
}

/**
 * @brief Multiply two matrices using the micro-kernel approach
 * @param k the number of shared dimensions
 * @param a the pointer to the matrix A
 * @param b the pointer to the matrix B
 * @param c the pointer to the matrix C (result)
 * @param c_stride_ the stride of the matrix C
 *
 */
static void __micro_kernel_4x4(int k, const float *a, const float *b, float *c, int c_stride_)
{
    // create 4x4 block to store the result
    __m128 c00 = _mm_setzero_ps();
    __m128 c10 = _mm_setzero_ps();
    __m128 c20 = _mm_setzero_ps();
    __m128 c30 = _mm_setzero_ps();

    for (int p = 0; p < k; p++)
    {
        __m128 b0 = _mm_load_ps(b + p * 4); // load 4 elements from the p-th column of B

        __m128 a0 = _mm_set1_ps(a[p]);         // broadcast the p-th element of A
        __m128 a1 = _mm_set1_ps(a[p + k]);     // broadcast the (p+4)-th element of A
        __m128 a2 = _mm_set1_ps(a[p + 2 * k]); // broadcast the (p+8)-th element of A
        __m128 a3 = _mm_set1_ps(a[p + 3 * k]); // broadcast the (p+12)-th element of A

        c00 = _mm_add_ps(c00, _mm_mul_ps(a0, b0));
        c10 = _mm_add_ps(c10, _mm_mul_ps(a1, b0));
        c20 = _mm_add_ps(c20, _mm_mul_ps(a2, b0));
        c30 = _mm_add_ps(c30, _mm_mul_ps(a3, b0));
    }

    _mm_store_ps(c, c00);
    _mm_store_ps(c + c_stride_, c10);
    _mm_store_ps(c + 2 * c_stride_, c20);
    _mm_store_ps(c + 3 * c_stride_, c30);
}

Mat *initMat(int m, int n)
{
    Mat *mat = (Mat *)malloc(sizeof(Mat));
    mat->m = m;
    mat->n = n;
    mat->data = (float *)malloc(m * n * sizeof(float *));

    return mat;
}

Mat *randMat(int m, int n)
{
    Mat *mat = initMat(m, n);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            srand(time(NULL));
            mat->data[i * n + j] = (float)rand() / RAND_MAX;
        }
    }

    return mat;
}

int cpyMat(Mat *dest, const Mat *src)
{
    if (dest->m != src->m || dest->n != src->n)
    {
        printf("Warning: matrix dimensions do not match\n");
        return 0;
    }

    int n = src->n, m = src->m;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dest->data[i * n + j] = src->data[i * n + j];
        }
    }

    return 1;
}

void freeMat(Mat *mat)
{
    free(mat->data);
    free(mat);
}

int readMat(Mat *mat)
{
    for (int i = 0; i < mat->m; i++)
    {
        for (int j = 0; j < mat->n; j++)
        {
            if (scanf("%f", &mat->data[i * mat->n + j]) != 1)
            {
                printf("Invalid input at (%d, %d)\n", i, j);
                return 0;
            }
        }
    }

    printf("Matrix read successfully\n");
    return 1;
}

const float peekMat(const Mat *mat, int i, int j)
{
    return mat->data[i * mat->n + j];
}

int setMat(Mat *mat, int i, int j, float value)
{
    if (i < 0 || i >= mat->m || j < 0 || j >= mat->n)
    {
        printf("Invalid index (%d, %d)\n", i, j);
        return 0;
    }

    mat->data[i * mat->n + j] = value;
    return 1;
}

void printMat(const Mat *mat)
{
    for (int i = 0; i < mat->m; i++)
    {
        for (int j = 0; j < mat->n; j++)
        {
            printf("%f ", mat->data[i * mat->n + j]);
        }

        printf("\n");
    }
}

int mulMatPlain(Mat *result, const Mat *mat1, const Mat *mat2)
{
    // check mat1 and mat2 matrix dimensions
    if (mat1->n != mat2->m)
    {
        printf("Invalid matrix dimensions\n");
        return 0;
    }

    // check result matrix dimensions
    if (result->m != mat1->m || result->n != mat2->n)
    {
        freeMat(result);
        result = initMat(mat1->m, mat2->n);
    }

    for (int i = 0; i < mat1->m; i++)
    {
        for (int j = 0; j < mat2->n; j++)
        {
            result->data[i * mat2->n + j] = 0;

            for (int k = 0; k < mat1->n; k++)
            {
                result->data[i * mat2->n + j] += mat1->data[i * mat1->n + k] * mat2->data[k * mat2->n + j];
            }
        }
    }

    return 1;
}

// use OpenMP to parallelize the matrix multiplication when the matrix size is smaller than OPT_THRESHOLD
int __mul_mat_opm(float *result, const float *mat1, const float *mat2, int m, int n, int k)
{
    if (!result || !mat1 || !mat2) {
        printf("Null pointer input!\n");
        return 0;
    }

    // Allocate memory for packed matrices
    float *packed_a = (float*)aligned_alloc(32, m * k * sizeof(float));
    float *packed_b = (float*)aligned_alloc(32, k * n * sizeof(float));

    if (!packed_a || !packed_b) {
        free(packed_a);
        free(packed_b);
        printf("Memory allocation failed!\n");
        return 0;
    }

    // Pack matrices A and B
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < m; i += BLOCK_SIZE) {
                for (int j = 0; j < k; j += BLOCK_SIZE) {
                    int mb = MIN(BLOCK_SIZE, m - i);
                    int kb = MIN(BLOCK_SIZE, k - j);
                    __pack_matrix_a(mb, kb, 
                                &mat1[i * k + j], k,
                                packed_a + i * k + j);
                }
            }
        }
        
        #pragma omp section
        {
            for (int i = 0; i < k; i += BLOCK_SIZE) {
                for (int j = 0; j < n; j += BLOCK_SIZE) {
                    int kb = MIN(BLOCK_SIZE, k - i);
                    int nb = MIN(BLOCK_SIZE, n - j);
                    __pack_matrix_b(kb, nb,
                                &mat2[i * n + j], n,
                                packed_b + j * k + i);
                }
            }
        }
    }

    // Main computation with blocking
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i += 4) {
        for (int j = 0; j < n; j += 4) {
            if (i + 4 <= m && j + 4 <= n) {
                __micro_kernel_4x4(k,
                               packed_a + i * k,
                               packed_b + j * k,
                               &result[i * n + j],
                               n);
            } else {
                // Handle edge cases without SIMD
                for (int ii = i; ii < MIN(i + 4, m); ii++) {
                    for (int jj = j; jj < MIN(j + 4, n); jj++) {
                        float sum = 0.0f;
                        for (int p = 0; p < k; p++) {
                            sum += mat1[ii * k + p] * mat2[p * n + jj];
                        }
                        result[ii * n + jj] = sum;
                    }
                }
            }
        }
    }

    free(packed_a);
    free(packed_b);
    return 1;
}

// use CUDA to parallelize the matrix multiplication when the matrix size is larger than OPT_THRESHOLD
int __mul_mat_cuda(float *result, const float *mat1, const float *mat2, int m, int n, int k);

int cmpMat(const Mat *mat1, const Mat *mat2)
{
    if ( mat1->m != mat2->m || mat1->n != mat2->n)
    {
        printf("Matrix dimensions do not match\n");
        return -1;
    }

    int n = mat1->n, m = mat1->m;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mat1->data[i * n + j] != mat2->data[i * n + j])
            {
                return mat1->data[i * n + j] - mat2->data[i * n + j];
            }
        }
    }

    return 0;
}

int mulMat(Mat *result, const Mat *mat1, const Mat *mat2)
{

    if (!mat1 || !mat2 || !result )
    {
        printf("Null pointer input!\n");
        return 0;
    }

    // check mat1 and mat2 matrix dimensions
    if (mat1->n != mat2->m)
    {
        printf("Invalid matrix dimensions\n");
        return 0;
    }

    // check result matrix dimensions
    if (result->m != mat1->m || result->n != mat2->n)
    {
        freeMat(result);
        result = initMat(mat1->m, mat2->n);
    }

    if (mat1->m * mat2->n <= OPT_THRESHOLD)
    {
        return __mul_mat_opm(result->data, 
                            mat1->data, mat2->data, 
                            mat1->m, mat2->n, 
                            mat1->n);
    }
    else
    {
        return __mul_mat_opm(result->data, 
                            mat1->data, mat2->data, 
                            mat1->m, mat2->n, 
                            mat1->n);
        // __mul_mat_cuda(result->data, mat1->data, mat2->data, mat1->m, mat2->n, mat1->n);
    }

    return 1;
}

// int main()
// {
//     int m, n;
//     printf( "Enter the number of rows and columns for the first matrix: " );
//     scanf( "%d %d", &m, &n );

//     Mat* mat1 = initMat( m, n );
//     printf( "Enter the first matrix:\n" );
//     readMat( mat1 );

//     printf( "Enter the number of rows and columns for the second matrix: " );
//     scanf( "%d %d", &m, &n );

//     Mat* mat2 = initMat( m, n );
//     printf( "Enter the second matrix:\n" );
//     readMat( mat2 );

//     Mat* result = initMat( mat1->m, mat2->n );
//     if ( mulMat( result, mat1, mat2 ) )
//     {
//         printf( "The result of the multiplication is:\n" );
//         printMat( result );
//     }

//     freeMat( mat1 );
//     freeMat( mat2 );
//     freeMat( result );

//     return 0;
// }