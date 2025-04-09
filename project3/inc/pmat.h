#ifndef PMAT_H_
#define PMAT_H_

#include "error.h"
#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rect_ {
  int x;
  int y;
  int dx;
  int dy;
} Rect;

typedef struct Line_ {
  int x0;
  int y0;
  int x1;
  int y1;
  double stroke;
} Line;

typedef struct Circle_ {
  double radius;
  int rx;
  int ry;
} Circle;

typedef uint8_t (*binopu8)(const uint8_t lhs, const uint8_t rhs);
#define _Tp uint8_t
_Tp addu8(const _Tp lhs, const _Tp rhs);
_Tp subu8(const _Tp lhs, const _Tp rhs);
_Tp mulu8(const _Tp lhs, const _Tp rhs);
_Tp divu8(const _Tp lhs, const _Tp rhs);
#undef _Tp

typedef enum {
  MAT_OK = 0,
  MAT_NULL_POINTER,
  MAT_SHAPE_DISMATCH,
  MAT_MEM_FAIL,
  MAT_OUT_OF_BOUND
} MatErr;

typedef struct Matu8_2d_ {
  size_t rows;
  size_t cols;
  uint8_t *bytes;
  // const int CHANNELS = 1;
} Matu8_2d, Mat;

#define SWAP(__x, __y)                                                         \
  (__x) ^= (__y);                                                              \
  (__y) ^= (__x);                                                              \
  (__x) ^= (__y);
#define Mat(rows, cols)                                                        \
  (Matu8_2d) { rows, cols, NULL }

MatErr Mat_alloc(Mat *mat, size_t rows, size_t cols);
MatErr Mat_realloc(Mat *mat, size_t rows, size_t cols);
void Mat_release(Mat *mat);
MatErr Mat_copy(Mat *dst, const Mat *src);
MatErr Mat_deepcopy(Mat *dst, const Mat *src);
MatErr Mat_swap(Mat *m1, Mat *m2);
MatErr Mat_confuse_avg(Mat *res, const Mat *lhs, const Mat *rhs);
MatErr Mat_confuse_weight(Mat *res, const Mat *lhs, const Mat *rhs,
                          const double lw, const double rw);
MatErr Mat_pad(Mat *base, uint8_t padVal, size_t padL, size_t padT, size_t padR,
               size_t padD);
MatErr Mat_fill(Mat* mat, const uint8_t value);
MatErr Mat_fillr(Mat* mat, size_t sx, size_t sy, size_t ex, size_t ey, const uint8_t value);
MatErr Mat_clip(Mat *base, size_t sx, size_t sy, size_t ex, size_t ey);
MatErr Mat_adds(Mat *res, const Mat *lhs, const int32_t rhs);
double bilinearInterpolation(const Mat *channel, double x, double y);
MatErr Mat_conv2d(Mat *res, const Mat *basis, const Mat *kernel,
                  size_t stride_x, size_t stride_y, bool is_padding);
// MatErr Mat_muls(Mat* res, const Mat* lhs, const uint8_t rhs);
// MatErr Mat_divs(Mat* res, const Mat* lhs, const uint8_t rhs);
// MatErr Mat_add(Mat* res, Mat* lhs, Mat* rhs);
// MatErr Mat_sub(Mat* res, Mat* lhs, Mat* rhs);

// MatErr Mat_mul(Mat* res, Mat* lhs, Mat* rhs);
// MatErr Mat_conv2d(Mat* res, Mat* basis, Mat* kernel);

// typedef struct _2 {
//     size_t rows;
//     size_t cols;
//     double* dwords;
//     // const int CHANNELS = 1;
// } Matf642d1c, Matf;

typedef struct Matf64_2x3_ {
  double a, b, c; // x' = ax + by + c
  double d, e, f; // y' = dx + ey + f
} Matf64_2x3, AffineMat;
AffineMat AffineMat_create(double angle, double scaleX, double scaleY,
                           double translateX, double translateY);
AffineMat AffineMat_inv(const AffineMat *m);

/* for clangd */
#ifndef complex
#define complex _Complex
#endif

typedef struct Matf64_cplx_2d_ {
  size_t rows;
  size_t cols;
  double *ddata;
  double complex *cdata;
} Matf64_cplx_2d, FFTMat;

MatErr FFTMat_alloc(FFTMat *mat, size_t rows, size_t cols);
void FFTMat_release(FFTMat *mat);

void fft1d(double complex *data, size_t n, bool inverse);
void fft2d(FFTMat *mat, bool inverse);
void fft_shift(FFTMat *mat);
void fft_magnitude(Mat *u8mat, FFTMat *fmat);

typedef double Matf_3x3[9];
typedef double Matf_5x5[25];

static Matf_3x3 LAPLACIAN_D3 = {0, -1, 0, -1, 4, -4, 0, -1, 0};
static Matf_3x3 GAUSSIAN_D3 = {1, 2, 1, 2, 4, 2, 1, 2, 1};
static Matf_3x3 GAUSSIAN_F3 = {};

#endif
