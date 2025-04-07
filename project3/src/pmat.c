#include "../inc/pmat.h"
#include <float.h>

MatErr Mat_alloc(Mat *mat, size_t rows, size_t cols)
{
  if (!mat)
    return MAT_NULL_POINTER;
  
  mat->rows = rows;
  mat->cols = cols;

  mat->bytes = (uint8_t *)malloc(rows * cols * sizeof(uint8_t));

  if (!mat->bytes)
  {
        WARNING("Failed to allocate %zu bytes for mat.", rows * cols * sizeof(uint8_t));
        return MAT_MEM_FAIL;
    }
    return MAT_OK;
}

MatErr Mat_realloc(Mat *mat, size_t rows, size_t cols)
{
    if (!mat || !mat->bytes) return MAT_NULL_POINTER;
    mat->bytes = (uint8_t *)realloc(mat->bytes, rows * cols * sizeof(uint8_t));
    if (!mat->bytes)
    {
        WARNING("Failed to reallocate %zu bytes for mat.", rows * cols * sizeof(uint8_t));
        return MAT_MEM_FAIL;
    }
    return MAT_OK;
}

void Mat_release(Mat *mat)
{
    if (!mat) return;
    if (mat->bytes)
    {
        // WARNING("%p", mat->bytes);
        free(mat->bytes);
        mat->bytes = NULL;
    }
}

MatErr Mat_copy(Mat *dst, const Mat *src)
{
    if (!dst || !src) return MAT_NULL_POINTER;
    Mat_release(dst);
    dst->rows = src->rows;
    dst->cols = src->cols;
    dst->bytes = src->bytes;
    return MAT_OK;
}

MatErr Mat_deepcopy(Mat *dst, const Mat *src)
{
    if (!dst || !src) return MAT_NULL_POINTER;
    Mat_release(dst);
    dst->rows = src->rows;
    dst->cols = src->cols;
    memcpy(dst->bytes, src->bytes, src->rows * src->cols);
    return MAT_OK;
}

MatErr Mat_swap(Mat *m1, Mat *m2)
{
    if (!m1 || !m2) return MAT_NULL_POINTER;
    SWAP(m1->rows, m2->rows);
    SWAP(m1->cols, m2->cols);
    uint8_t* swap = m1->bytes;
    m1->bytes = m2->bytes;
    m2->bytes = swap;
    return MAT_OK;
}

MatErr Mat_confuse_avg(Mat* res, const Mat* lhs, const Mat* rhs)
{
    if (!res || !lhs || !rhs)
    {
        // TODO
        return MAT_NULL_POINTER;
    }
    if(lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        WARNING("Uncompatiable shapes of mat: between (%zu, %zu) and (%zu, %zu)", 
        lhs->rows, lhs->cols, rhs->rows, rhs->cols);
        return MAT_SHAPE_DISMATCH;
    }
    size_t psize = lhs->rows * lhs->cols;
    // #ifdef __SSE__
    // #else
    
    // #endif
    for (size_t i = 0; i < psize; i++)
    {
        uint16_t sum = ((uint16_t)lhs->bytes[i] + 
                        (uint16_t)(rhs->bytes[i])) / 2;
        res->bytes[i] = (uint8_t)sum;
    }
    return MAT_OK;
}

MatErr Mat_confuse_weight(Mat* res, const Mat* lhs, const Mat* rhs, 
                        const double lw, const double rw)
{
    if (!res || !lhs || !rhs)
    {
        // TODO
        return MAT_NULL_POINTER;
    }
    if(lhs->rows != rhs->rows || lhs->cols != rhs->cols)
    {
        WARNING("Uncompatiable shapes of mat: between (%zu, %zu) and (%zu, %zu)", 
        lhs->rows, lhs->cols, rhs->rows, rhs->cols);
        return MAT_SHAPE_DISMATCH;
    }
    size_t psize = lhs->rows * lhs->cols;
    double _lw = lw / (lw + rw), _rw = rw / (lw + rw);
    // #ifdef __SSE__
    // #else
    
    // #endif
    for (size_t i = 0; i < psize; i++)
    {
        res->bytes[i] = lhs->bytes[i] * _lw + (rhs->bytes[i]) * _rw;
    }
    return MAT_OK;
}

MatErr Mat_adds(Mat *res, const Mat *lhs, const int32_t rhs)
{
     if (!res || !lhs || !rhs)
    {
        // TODO
        return MAT_NULL_POINTER;
    }
    bool overflow = false;
    size_t psize = lhs->rows * lhs->cols;
    for (size_t i = 0; i < psize; i++)
    {
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
    NOTION("Overflow may occur with the rhs as %d, please make sure the pixel of result is within range of (0, 255)", rhs);
    return MAT_OK;
}

double bilinearInterpolation(const Mat* channel, double x, double y)
{
    int x1 = (int)x;
    int y1 = (int)y;
    int x2 = x1 + 1;
    int y2 = y1 + 1;
    
    // 确保坐标在图像范围内
    if (x1 < 0 || y1 < 0 || x2 >= channel->cols || y2 >= channel->rows) {
        return 0;  // 超出边界返回0值
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
            double translateX, double translateY)
{
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

AffineMat AffineMat_inv(const AffineMat *m)
{
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

int log2_uint32(uint32_t n)
{
  int k = 0;
  while ((1U << k) < n)
    ++k;
  return k;
}

void fft_change(double complex *a, size_t n, int k) {
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

void fft1d(double complex *data, size_t n, bool inverse) {
  // 计算k = log2(n)
  int k = log2_uint32(n);

  // 预计算旋转因子
  double complex *omega =
      (double complex *)malloc((n >> 1) * sizeof(double complex));
  for (size_t i = 0; i < (n >> 1); ++i) {
    double angle = 2.0 * M_PI / n * i * (inverse ? 1.0 : -1.0);
    omega[i] = cos(angle) + I * sin(angle);
  }

  // 位反转排序
  fft_change(data, n, k);

//   int half_n = n >> 1;

  // 蝴蝶运算
  for (size_t i = 1; i <= k; ++i) {
    uint32_t omega_step = 1U << (k - i);
    uint32_t step_size = 1U << i;
    uint32_t half_step = step_size >> 1;

    for (uint32_t j = 0; j < n; j += step_size) {
      for (uint32_t l = 0, omega_idx = 0; l < half_step;
           ++l, omega_idx += omega_step) {
        double complex t = omega[omega_idx] * data[j + l + half_step];
        data[j + l + half_step] = data[j + l] - t;
        data[j + l] = data[j + l] + t;
      }
    }
  }

  // 如果是逆变换，需要除以n
  if (inverse) {
    for (int i = 0; i < n; i++) {
      data[i] /= n;
    }
  }

  free(omega);
}

void fft2d(FFTMat *mat, bool inverse)
{
  size_t rows = mat->rows;
  size_t cols = mat->cols;

  // 临时缓冲区
  double complex *temp = (double complex *)malloc(sizeof(double complex) *
                                                  (rows > cols ? rows : cols));

  // 对每一行进行FFT
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      temp[j] = mat->cdata[i * mat->cols + j];
    }
    fft1d(temp, cols, inverse);
  }

  // 对每一列进行FFT
  for (size_t j = 0; j < cols; j++) {
    for (size_t i = 0; i < rows; i++) {
      temp[i] = mat->cdata[i * mat->cols + j];
    }

    fft1d(temp, rows, inverse);
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
      temp.cdata[newI * cols + newJ] =
            mat->cdata[i * cols + rows];
    //   setComplexValue(&temp, newI, newJ, getComplexValue(mat, i, j));
    }
  }

  memcpy(&mat->cdata, &temp.cdata, rows * cols);
  // 将临时矩阵拷贝回原矩阵
//   for (int i = 0; i < rows; i++) {
//     for (int j = 0; j < cols; j++) {
//     //   setComplexValue(mat, i, j, getComplexValue(&temp, i, j));
//     }
//   }

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
      u8mat->bytes[i * cols + j] = (uint8_t)roundf64((mag / maxMag) * 255);
    }
  }
}
