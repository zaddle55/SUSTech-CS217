#include "../inc/pmat.h"
#include <float.h>

MatErr Mat_alloc(Mat *mat, size_t rows, size_t cols) {
  if (!mat)
    return MAT_NULL_POINTER;

  mat->rows = rows;
  mat->cols = cols;

  mat->bytes = (uint8_t *)malloc(rows * cols * sizeof(uint8_t));

  if (!mat->bytes) {
    WARNING("Failed to allocate %zu bytes for mat.",
            rows * cols * sizeof(uint8_t));
    return MAT_MEM_FAIL;
  }
  return MAT_OK;
}

MatErr Mat_realloc(Mat *mat, size_t rows, size_t cols) {
  if (!mat || !mat->bytes)
    return MAT_NULL_POINTER;
  
  mat->bytes =
      (uint8_t *)realloc(mat->bytes, rows * cols * sizeof(uint8_t));
  if (!mat->bytes) {
    WARNING("Failed to reallocate %zu bytes for mat.",
            rows * cols * sizeof(uint8_t));
    return MAT_MEM_FAIL;
  }
  mat->rows = rows;
  mat->cols = cols;
  return MAT_OK;
}

void Mat_release(Mat *mat) {
  if (!mat)
    return;
  if (mat->bytes) {
    // WARNING("%p", mat->bytes);
    free(mat->bytes);
    mat->bytes = NULL;
  }
}

MatErr Mat_copy(Mat *dst, const Mat *src) {
  if (!dst || !src)
    return MAT_NULL_POINTER;
  Mat_release(dst);
  dst->rows = src->rows;
  dst->cols = src->cols;
  dst->bytes = src->bytes;
  return MAT_OK;
}

MatErr Mat_deepcopy(Mat *dst, const Mat *src) {
  if (!dst || !src)
    return MAT_NULL_POINTER;
  dst->rows = src->rows;
  dst->cols = src->cols;
  memcpy(dst->bytes, src->bytes, src->rows * src->cols);
  return MAT_OK;
}

MatErr Mat_swap(Mat *m1, Mat *m2) {
  if (!m1 || !m2)
    return MAT_NULL_POINTER;
  SWAP(m1->rows, m2->rows);
  SWAP(m1->cols, m2->cols);
  uint8_t *swap = m1->bytes;
  m1->bytes = m2->bytes;
  m2->bytes = swap;
  return MAT_OK;
}

MatErr Mat_fill(Mat *mat, const uint8_t value) {
  if (!mat)
    return MAT_NULL_POINTER;
  size_t psize = mat->rows * mat->cols;
  memset(mat->bytes, value, psize);
  return MAT_OK;
}

MatErr Mat_fillr(Mat *mat, size_t sx, size_t sy, size_t ex, size_t ey,
                 const uint8_t value) {
  if (!mat)
    return MAT_NULL_POINTER;
  if (sx >= mat->cols || sy >= mat->rows || ex >= mat->cols || ey >= mat->rows)
    return MAT_OUT_OF_BOUND;
  for (size_t i = sy; i < ey; i++) {
    for (size_t j = sx; j < ex; j++) {
      mat->bytes[i * mat->cols + j] = value;
    }
  }
  return MAT_OK;
}

MatErr Mat_pad(Mat *base, uint8_t padVal, size_t padL, size_t padT, size_t padR,
               size_t padD) {
  if (!base)
    return MAT_NULL_POINTER;
  size_t rows = base->rows;
  size_t cols = base->cols;
  size_t newRows = base->rows + padT + padD;
  size_t newCols = base->cols + padL + padR;

  Mat temp;
  Mat_alloc(&temp, rows, cols);
  Mat_deepcopy(&temp, base);

  if (Mat_realloc(base, newRows, newCols) != MAT_OK) {
    WARNING("Fail to reallocate memory for padding matrix");
    Mat_release(&temp);
    return MAT_MEM_FAIL;
  }

  memset(base->bytes, padVal, newRows * newCols);
  for (size_t i = 0; i < rows; i++) {
    size_t offset = (i + padT) * newCols + padL;
    memcpy(base->bytes + offset, temp.bytes + i * cols, cols);
  }

  Mat_release(&temp);
  return MAT_OK;
}

MatErr Mat_clip(Mat *base, size_t sx, size_t sy, size_t ex, size_t ey) {
  if (!base)
    return MAT_NULL_POINTER;
  if (sx >= base->cols || sy >= base->rows || ex >= base->cols || ey >= base->rows)
    return MAT_OUT_OF_BOUND;
  size_t rows = base->rows;
  size_t cols = base->cols;
  size_t newRows = ey - sy;
  size_t newCols = ex - sx;

  Mat temp;
  Mat_alloc(&temp, rows, cols);
  Mat_deepcopy(&temp, base);

  if (Mat_realloc(base, newRows, newCols) != MAT_OK) {
    WARNING("Fail to reallocate memory for padding matrix");
    Mat_release(&temp);
    return MAT_MEM_FAIL;
  }

  for (size_t i = 0; i < newRows; i++) {
    size_t offset = (i + sy) * cols + sx;
    memcpy(base->bytes + i * newCols, temp.bytes + offset, newCols);
  }

  Mat_release(&temp);
  return MAT_OK;
}

MatErr Mat_confuse_avg(Mat *res, const Mat *lhs, const Mat *rhs) {
  if (!res || !lhs || !rhs) {
    WARNING("Arguments of input can't be empty!");
    return MAT_NULL_POINTER;
  }
  if (lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
    WARNING("Uncompatiable shapes of mat: between (%zu, %zu) and (%zu, %zu)",
            lhs->rows, lhs->cols, rhs->rows, rhs->cols);
    return MAT_SHAPE_DISMATCH;
  }
  size_t psize = lhs->rows * lhs->cols;
  for (size_t i = 0; i < psize; i++) {
    uint8_t half_lhs = lhs->bytes[i] >> 1;
    uint8_t half_rhs = rhs->bytes[i] >> 1;
    res->bytes[i] = half_lhs + half_rhs;
  }
  return MAT_OK;
}

MatErr Mat_confuse_weight(Mat *res, const Mat *lhs, const Mat *rhs,
                          const double lw, const double rw) {
  if (!res || !lhs || !rhs) {
    WARNING("Arguments of input can't be empty!");
    return MAT_NULL_POINTER;
  }
  if (lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
    WARNING("Uncompatiable shapes of mat: between (%zu, %zu) and (%zu, %zu)",
            lhs->rows, lhs->cols, rhs->rows, rhs->cols);
    return MAT_SHAPE_DISMATCH;
  }
  size_t psize = lhs->rows * lhs->cols;
  double frac_l = lw / (lw + rw), frac_r = rw / (lw + rw);
  // #ifdef __SSE__
  // #else

  // #endif
  for (size_t i = 0; i < psize; i++) {
    res->bytes[i] = (uint8_t)(lhs->bytes[i] * frac_l + rhs->bytes[i] * frac_r);
  }
  return MAT_OK;
}

MatErr Mat_confuse_max(Mat *res, const Mat *lhs, const Mat *rhs) {
  if (!res || !lhs || !rhs) {
    // TODO
    return MAT_NULL_POINTER;
  }
  if (lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
    WARNING("Uncompatiable shapes of mat: between (%zu, %zu) and (%zu, %zu)",
            lhs->rows, lhs->cols, rhs->rows, rhs->cols);
    return MAT_SHAPE_DISMATCH;
  }
  size_t psize = lhs->rows * lhs->cols;
  // #ifdef __SSE__
  // #else

  // #endif
  for (size_t i = 0; i < psize; i++) {
    if (lhs->bytes[i] > rhs->bytes[i])
      res->bytes[i] = lhs->bytes[i];
    else
      res->bytes[i] = rhs->bytes[i];
  }
  return MAT_OK;
}

MatErr Mat_adds(Mat *res, const Mat *lhs, const int32_t rhs) {
  if (!res || !lhs || !rhs) {
    // TODO
    return MAT_NULL_POINTER;
  }
  bool overflow = false;
  size_t psize = lhs->rows * lhs->cols;
  for (size_t i = 0; i < psize; i++) {
    int32_t sum = (int32_t)lhs->bytes[i] + rhs;
    if (sum < 0) {
      sum = 0;
      overflow = true;
    } else if (sum > 255) {
      sum = 255;
      overflow = true;
    }
    res->bytes[i] = sum;
  }
  if (overflow)
    NOTION("Overflow may occur with the rhs as %d, please make sure the pixel of "
          "result is within range of [0, 255]", rhs);
  return MAT_OK;
}

double bilinearInterpolation(const Mat *channel, double x, double y) {
  int x1 = (int)x;
  int y1 = (int)y;
  int x2 = x1 + 1;
  int y2 = y1 + 1;

  // 确保坐标在图像范围内
  if (x1 < 0 || y1 < 0 || x2 >= channel->cols || y2 >= channel->rows) {
    return 0; // 超出边界返回0值
  }

  double dx = x - x1;
  double dy = y - y1;

  // 进行双线性插值
  double value = (1 - dx) * (1 - dy) * channel->bytes[y1 * channel->cols + x1] +
                 dx * (1 - dy) * channel->bytes[y1 * channel->cols + x2] +
                 (1 - dx) * dy * channel->bytes[y2 * channel->cols + x1] +
                 dx * dy * channel->bytes[y2 * channel->cols + x2];

  // NOTION("value: %lf", value);

  return value;
}

AffineMat AffineMat_create(double angle, double scaleX, double scaleY,
                           double translateX, double translateY) {
  AffineMat mat;

  // 应用旋转和缩放
  double angleRad = angle * M_PI / 180.0;
  mat.a = scaleX * cos(angleRad);
  mat.b = -scaleY * sin(angleRad);
  mat.d = scaleX * sin(angleRad);
  mat.e = scaleY * cos(angleRad);

  // 应用平移
  mat.c = translateX;
  mat.f = translateY;

  return mat;
}

AffineMat AffineMat_inv(const AffineMat *m) {
  AffineMat inv;

  double det = m->a * m->e - m->b * m->d;
  if (fabs(det) < FLT_EPSILON) {
    WARNING("Affine matrix is singular, cannot compute inverse.");
    return *m; // 返回原矩阵，表示错误
  }

  inv.a = m->e / det;
  inv.b = -m->b / det;
  inv.c = (m->b * m->f - m->e * m->c) / det;

  inv.d = -m->d / det;
  inv.e = m->a / det;
  inv.f = (m->d * m->c - m->a * m->f) / det;

  return inv;
}

MatErr FFTMat_alloc(FFTMat *mat, size_t rows, size_t cols) {
  if (!mat)
    return MAT_NULL_POINTER;

  mat->rows = rows;
  mat->cols = cols;

  mat->cdata = (double complex *)malloc(rows * cols * sizeof(double complex));
  if (!mat->cdata)
    return MAT_MEM_FAIL;
  mat->ddata = (double *)malloc(rows * cols * sizeof(double));
  if (!mat->ddata) {
    free(mat->cdata);
    return MAT_MEM_FAIL;
  }
  return MAT_OK;
}

void FFTMat_release(FFTMat *mat) {
  if (!mat)
    return;
  if (mat->cdata)
    free(mat->cdata);
  if (mat->ddata)
    free(mat->ddata);
}

int log2_uint32(uint32_t n) {
  int k = 0;
  while ((1U << k) <= n)
    ++k;
  return k - 1;
}

void fft_change(double complex *a, uint32_t n, int k) {
  for (uint32_t i = 0; i < n; ++i) {
    // 使用Stanford Bit Twiddling Hacks中的位反转技巧
    uint32_t t = i;
    t = ((t >> 1) & 0x55555555) | ((t & 0x55555555) << 1);
    t = ((t >> 2) & 0x33333333) | ((t & 0x33333333) << 2);
    t = ((t >> 4) & 0x0F0F0F0F) | ((t & 0x0F0F0F0F) << 4);
    t = ((t >> 8) & 0x00FF00FF) | ((t & 0x00FF00FF) << 8);
    t = (t >> 16) | (t << 16);
    // 调整位宽，只反转k位
    t >>= (32 - k);

    // 只在需要时交换，避免重复交换
    if (i < t) {
      double complex temp = a[i];
      a[i] = a[t];
      a[t] = temp;
    }
  }
}

unsigned int bit_reverse(unsigned int x, int log2n) {
  int n = 0;
  for (int i = 0; i < log2n; i++) {
    n <<= 1;
    n |= (x & 1);
    x >>= 1;
  }
  return n;
}

void fft1d(double complex *data, size_t n, bool inverse) {
  // 计算k = log2(n)
  int k = log2_uint32(n);

  // 位反转排序
  fft_change(data, n, k);
  // for (int i = 0; i < n; i++) {
  //   int j = bit_reverse(i, k);
  //   if (i < j) {
  //     complex double temp = data[i];
  //     data[i] = data[j];
  //     data[j] = temp;
  //   }
  // }

  //   int half_n = n >> 1;

  // 蝴蝶运算
  for (size_t len = 2; len <= n; len <<= 1) {
    double angle = 2 * M_PI / len * (inverse ? -1 : 1);
    complex double wn = cos(angle) + I * sin(angle);

    for (size_t i = 0; i < n; i += len) {
      complex double w = 1.0 + 0.0 * I;
      for (size_t j = 0; j < len / 2; j++) {
        complex double u = data[i + j];
        complex double t = w * data[i + j + len / 2];

        data[i + j] = u + t;
        data[i + j + len / 2] = u - t;

        w *= wn;
      }
    }
  }

  // 如果是逆变换，需要除以n
  if (inverse) {
    for (size_t i = 0; i < n; i++) {
      data[i] /= n;
    }
  }
}

void fft2d(FFTMat *mat, bool inverse) {
  size_t rows = mat->rows;
  size_t cols = mat->cols;

  if ((mat->rows & (mat->rows - 1)) != 0) {
    WARNING("The value of rows should be the power of 2, but got %zu",
            mat->rows);
    return;
  } 
  if ((mat->rows & (mat->rows - 1)) != 0) {
    WARNING("The value of cols should be the power of 2, but got %zu",
            mat->cols);
    return;
  }
  // 临时缓冲区
  double complex *temp = (double complex *)malloc(sizeof(double complex) *
                                                  (rows > cols ? rows : cols));

  // 对每一行进行FFT
  for (size_t i = 0; i < rows; i++) {
    fft1d(mat->cdata + i * cols, cols, inverse);
  }
  // 对每一列进行FFT
  for (size_t j = 0; j < cols; j++) {
    for (size_t i = 0; i < rows; i++) {
      temp[i] = mat->cdata[i * mat->cols + j];
    }
    fft1d(temp, rows, inverse);

    for (size_t i = 0; i < rows; i++) {
      mat->cdata[i * mat->cols + j] = temp[i];
    }
  }
  
  free(temp);
}

void fft_shift(FFTMat *mat) {
  size_t rows = mat->rows;
  size_t cols = mat->cols;
  size_t halfRows = rows / 2;
  size_t halfCols = cols / 2;

  FFTMat temp;
  FFTMat_alloc(&temp, rows, cols);

  // 交换四个象限
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      size_t newI = (i + halfRows) % rows;
      size_t newJ = (j + halfCols) % cols;
      temp.cdata[newI * cols + newJ] = mat->cdata[i * cols + j];
      //   setComplexValue(&temp, newI, newJ, getComplexValue(mat, i, j));
    }
  }

  // memcpy(mat->cdata, temp.cdata, rows * cols);
  // 将临时矩阵拷贝回原矩阵
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        //   setComplexValue(mat, i, j, getComplexValue(&temp, i, j));
        mat->cdata[i * cols + j] = temp.cdata[i * cols + j];
      }
    }

  FFTMat_release(&temp);
}

void fft_magnitude(Mat *u8mat, FFTMat *fmat) {
  if (fmat->rows != u8mat->rows || fmat->cols != u8mat->cols)
    return;
  size_t rows = fmat->rows;
  size_t cols = fmat->cols;

  // 找出最大的幅度值用于归一化
  double maxMag = 0.0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      double complex value = fmat->cdata[i * cols + j];
      double mag = log(1.0 + cabs(value));
      // double mag = cabs(value);
      fmat->ddata[i * cols + j] = mag;
      if (mag > maxMag) {
        maxMag = mag;
      }
    }
  }

  // 归一化并保存幅度值
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      double mag = fmat->ddata[i * cols + j];
      // 归一化到[0, 255]范围
      u8mat->bytes[i * cols + j] =
          (uint8_t)(((mag) / (maxMag)) * 255);
      // u8mat->bytes[i * cols + j] =
      //     (uint8_t)roundf((mag > 255.0)? 255.0 : mag);
    }
  }
}

MatErr Mat_conv2d(Mat *res, const Mat *basis, const Mat *kernel, size_t stride_x,
                  size_t stride_y, bool is_padding) {
  // Parameter validation
  if (!res || !basis || !kernel)
    return MAT_NULL_POINTER;
  if (!basis->bytes || !kernel->bytes)
    return MAT_NULL_POINTER;
  if (kernel->rows % 2 == 0 || kernel->cols % 2 == 0) {
    WARNING("The size of kernel is not odd: (%zu, %zu)", kernel->rows,
            kernel->cols);
    return MAT_SHAPE_DISMATCH;
  }

  size_t k_rows = kernel->rows;
  size_t k_cols = kernel->cols;
  size_t k_center_y = k_rows / 2;
  size_t k_center_x = k_cols / 2;

  size_t output_rows, output_cols;

  if (is_padding) {
    // With padding, output size remains the same as input (when stride=1)
    output_rows = (basis->rows - 1) / stride_y + 1;
    output_cols = (basis->cols - 1) / stride_x + 1;
  } else {
    // Without padding, output size shrinks
    output_rows = (basis->rows - k_rows) / stride_y + 1;
    output_cols = (basis->cols - k_cols) / stride_x + 1;
  }

  // Allocate or validate output matrix
  if (!res->bytes) {
    res->bytes = (uint8_t *)calloc(output_rows * output_cols, sizeof(uint8_t));
    if (!res->bytes)
      return MAT_MEM_FAIL;
  }
  res->rows = output_rows;
  res->cols = output_cols;

  // Clear the output matrix
  memset(res->bytes, 0, output_rows * output_cols);

  // Standard convolution implementation
  for (size_t out_y = 0; out_y < output_rows; out_y++) {
    for (size_t out_x = 0; out_x < output_cols; out_x++) {
      uint32_t sum = 0; // Use larger type to prevent overflow

      // Calculate corresponding input position
      size_t in_y_start = out_y * stride_y;
      size_t in_x_start = out_x * stride_x;

      // Convolve the kernel with the input region
      for (size_t k_y = 0; k_y < k_rows; k_y++) {
        for (size_t k_x = 0; k_x < k_cols; k_x++) {
          // Calculate input position with kernel offset
          int64_t in_y = in_y_start + k_y - (is_padding ? k_center_y : 0);
          int64_t in_x = in_x_start + k_x - (is_padding ? k_center_x : 0);

          // Skip if outside input bounds (zero padding effect)
          if (in_y < 0 || in_y >= (int64_t)basis->rows || in_x < 0 ||
              in_x >= (int64_t)basis->cols) {
            continue;
          }

          uint8_t pixel = basis->bytes[in_y * basis->cols + in_x];
          uint8_t kernel_val = kernel->bytes[k_y * kernel->cols + k_x];
          sum += pixel * kernel_val;
        }
      }
      double clamp_sum = sum / (k_cols * k_rows * 255.0);

      // Clamp value to uint8_t range
      res->bytes[out_y * output_cols + out_x] =
          (clamp_sum > 255) ? 255 : (uint8_t)clamp_sum;
    }
  }

  return MAT_OK;
}
