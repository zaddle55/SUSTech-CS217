#include <stdio.h>
#include <cuda_runtime.h>

__global__ void dotProd_kernel_i32(int *a, int *b, long long *c, unsigned long long size) {
    __shared__ int cache[256];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int cacheIndex = threadIdx.x;

    int tmp = 0;
    while (tid < n) {
        tmp += a[tid] * b[tid];
        tid += blockDim.x * gridDim.x;
    }

    cache[cacheIndex] = tmp;

    __syncthreads();

    // do reduction in shared mem
    int i = blockDim.x >> 1;
    while (i != 0) {
        if (cacheIndex < i) {
            cache[cacheIndex] += cache[cacheIndex + i];
        }
        __syncthreads();
        i >>= 1;
    }

    if (cacheIndex == 0) {
        c[blockIdx.x] = cache[0];
    }
}

__global__ void dotProd_kernel_f64(double *a, double *b, long double *c, unsigned long long n) {
    __shared__ double cache[256];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int cacheIndex = threadIdx.x;

    double tmp = 0;
    while (tid < n) {
        tmp += a[tid] * b[tid];
        tid += blockDim.x * gridDim.x;
    }

    cache[cacheIndex] = tmp;

    __syncthreads();

    // do reduction in shared mem
    int i = blockDim.x >> 1;
    while (i != 0) {
        if (cacheIndex < i) {
            cache[cacheIndex] += cache[cacheIndex + i];
        }
        __syncthreads();
        i >>= 1;
    }

    if (cacheIndex == 0) {
        c[blockIdx.x] = cache[0];
    }
}

int dotproduct_cuda_i32(int *a, int *b, long long *result, unsigned long long size) {
    int *d_a, *d_b;
    long long *d_c, *partial_results;
    
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    if (blocksPerGrid > 65535) blocksPerGrid = 65535;
    
    cudaMalloc((void**)&d_a, size * sizeof(int));
    cudaMalloc((void**)&d_b, size * sizeof(int));
    cudaMalloc((void**)&d_c, blocksPerGrid * sizeof(long long));
    
    cudaMemcpy(d_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    
    partial_results = (long long*)malloc(blocksPerGrid * sizeof(long long));
    
    dotProd_kernel_i32<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, size);

    cudaDeviceSynchronize();
    cudaMemcpy(partial_results, d_c, blocksPerGrid * sizeof(long long), cudaMemcpyDeviceToHost);
    
    // reduce the partial results on the host
    *result = 0;
    for (int i = 0; i < blocksPerGrid; i++) {
        *result += partial_results[i];
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(partial_results);
    
    return 0;
}

int dotproduct_cuda_f64(double *a, double *b, long double *result, unsigned long long size) {
    double *d_a, *d_b;
    long double *d_c, *partial_results;
    
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    if (blocksPerGrid > 65535) blocksPerGrid = 65535;
    
    cudaMalloc((void**)&d_a, size * sizeof(double));
    cudaMalloc((void**)&d_b, size * sizeof(double));
    cudaMalloc((void**)&d_c, blocksPerGrid * sizeof(long double));
    
    cudaMemcpy(d_a, a, size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size * sizeof(double), cudaMemcpyHostToDevice);
    
    partial_results = (long double*)malloc(blocksPerGrid * sizeof(long double));
    
    dotProd_kernel_f64<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, size);

    cudaDeviceSynchronize();
    cudaMemcpy(partial_results, d_c, blocksPerGrid * sizeof(long double), cudaMemcpyDeviceToHost);
    
    // reduce the partial results on the host
    *result = 0;
    for (int i = 0; i < blocksPerGrid; i++) {
        *result += partial_results[i];
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(partial_results);
    
    return 0;
}

int main(int argc, char *argv[]) {
    // Example usage
    int a[256], b[256];
    long long result_i32;
    long double result_f64;

    for (int i = 0; i < 256; i++) {
        a[i] = i;
        b[i] = i;
    }

    dotproduct_cuda_i32(a, b, &result_i32, 256);
    printf("Dot product (int): %lld\n", result_i32);

    double a_f64[256], b_f64[256];
    for (int i = 0; i < 256; i++) {
        a_f64[i] = (double)i;
        b_f64[i] = (double)i;
    }

    dotproduct_cuda_f64(a_f64, b_f64, &result_f64, 256);
    printf("Dot product (double): %Lf\n", result_f64);

    return 0;
}