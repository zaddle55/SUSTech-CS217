__global__ void dotProd_kernel_i32(int *a, int *b, int *c, int n) {
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

__global__ void dotProd_kernel_f32(float *a, float *b, float *c, int n) {
    __shared__ float cache[256];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int cacheIndex = threadIdx.x;

    float tmp = 0;
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