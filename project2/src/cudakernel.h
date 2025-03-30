
#ifndef CUDA_KERNEL_H_
#define CUDA_KERNEL_H_

int dotproduct_cuda_i32(int *a, int *b, long long *result, unsigned long long size);
int dotproduct_cuda_f64(double *a, double *b, long double *result, unsigned long long size);


#ifdef EXPORTS
extern "C" 
#endif

#endif