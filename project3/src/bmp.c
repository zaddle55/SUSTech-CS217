#include "../inc/bmp.h"
#include <math.h>

int BMPImage_decode(BMPImage *bimg, IStream *istream) {
  if (!bimg || !istream) {
    WARNING("The arguments of input can't be null.");
    return 1;
  }

  BFHeader bfheader;
  uint64_t bfheader_bytes =
      IStream_readBytes(istream, (byte *)&bfheader, sizeof(BFHeader));
  if (bfheader_bytes != sizeof(BFHeader)) {
    WARNING("Failed to parse the Bitmap File Header: expect %zu bytes but got "
            "%zu bytes",
            sizeof(BFHeader), bfheader_bytes);
    return 1;
  }

  word bi_size;
  uint64_t ret = IStream_readWord(istream, &bi_size);
  if (ret != 4) {
    WARNING("Failed to parse the Bitmap Info Size: expect 4 bytes but got %zu "
            "bytes",
            ret);
    return 1;
  } else if (bi_size != STD_BIHEADER_LEN) {
    WARNING("Not a standard Bitmap Info Size: expect %u bytes but got %u bytes",
            STD_BIHEADER_LEN, bi_size);
    return 1;
  }

  BIHeader biheader;
  biheader.bi_size = bi_size;
  uint64_t biheader_bytes =
      IStream_readBytes(istream, (byte *)&biheader + sizeof(bi_size),
                        sizeof(BIHeader) - sizeof(bi_size));

  if (biheader_bytes != (sizeof(BIHeader) - sizeof(bi_size))) {
    WARNING("Failed to parse the Bitmap Info Header: expected %zu bytes but "
            "got %zu bytes",
            sizeof(BIHeader), biheader_bytes + sizeof(bi_size));
    return 1;
  }

  if (biheader.bi_compress != STD_COMPRESS_TYPE) {
    WARNING("Not a standard compress type: only no-compressed type(0) is "
            "supported.");
    return 1;
  }

  if (biheader.bi_bitcnt != STD_BIT_COUNT) {
    WARNING("Not a stardard bit count: expected %u-bit but got %u-bit.",
            STD_BIT_COUNT, biheader.bi_bitcnt);
    return 1;
  }

  bimg->bfHeader = bfheader;
  bimg->biHeader = biheader;

  bool topDown = (biheader.bi_height & 0x80000000) != 0;

  word prows = topDown ? (biheader.bi_height & 0x7FFFFFFF) : biheader.bi_height;
  word pcols = biheader.bi_width;
  bimg->psize = pcols * prows;

  Mat r = Mat(prows, pcols);
  Mat g = Mat(prows, pcols);
  Mat b = Mat(prows, pcols);

  Mat_alloc(&r, prows, pcols);
  Mat_alloc(&g, prows, pcols);
  Mat_alloc(&b, prows, pcols);

  word padding = (LINE_ALIGN - (pcols * 3) % LINE_ALIGN) % LINE_ALIGN;

  for (dword row = 0; row < prows; row++) {
    // relative row index in mat
    dword _row = topDown ? row : (prows - 1 - row);

    for (dword col = 0; col < pcols; col++) {
      byte pixel[3];
      if (IStream_readBytes(istream, pixel, 3) != 3) {
        WARNING(
            "Failed to read pixel bytes at [%lu,%lu]: expect at least 3 bytes.",
            _row, col);
        return 1;
      }

      // packed index in mat
      dword idx = _row * pcols + col;

      // RRGGBB order
      b.bytes[idx] = pixel[0]; // b channel
      g.bytes[idx] = pixel[1]; // g channel
      r.bytes[idx] = pixel[2]; // r channel
    }

    // skip padding bytes
    if (padding > 0) {
      byte padBytes[3] = {0};
      if (IStream_readBytes(istream, padBytes, padding) != padding) {
        WARNING("Incorrect padding at line %lu", _row);
        return 1;
      }
    }
  }

  Mat_copy(&bimg->r, &r);
  Mat_copy(&bimg->g, &g);
  Mat_copy(&bimg->b, &b);
  bimg->a.bytes = NULL;

  return 0;
}

int BMPImage_encode(BMPImage *bimg, OStream *ostream) {
  if (!bimg || !ostream) {
    WARNING("The arguments of input can't be null.");
    return 1;
  }
  OStream_writeBytes(ostream, (byte *)&bimg->bfHeader, sizeof(BFHeader));
  OStream_writeBytes(ostream, (byte *)&bimg->biHeader, sizeof(BIHeader));
  bool topDown = (bimg->biHeader.bi_height & 0x80000000) != 0;

  word prows = topDown ? (bimg->biHeader.bi_height & 0x7FFFFFFF)
                       : bimg->biHeader.bi_height;
  word pcols = bimg->biHeader.bi_width;
  hword bytes_per_pixel = bimg->biHeader.bi_bitcnt >> 3;
  dword padding =
      (LINE_ALIGN - (pcols * bytes_per_pixel) % LINE_ALIGN) % LINE_ALIGN;

  for (dword row = 0; row < prows; row++) {
    // relative row index in mat
    dword _row = topDown ? row : (prows - 1 - row);

    for (dword col = 0; col < pcols; col++) {
      byte pixel[bytes_per_pixel];
      memset(pixel, 0, bytes_per_pixel);
      // packed index in mat
      dword idx = _row * pcols + col;

      // RRGGBB order
      switch (bytes_per_pixel) {
      case 4:
        pixel[3] = bimg->a.bytes[idx];
      case 3:
        pixel[2] = bimg->r.bytes[idx];
      case 2:
        pixel[1] = bimg->g.bytes[idx];
      case 1:
        pixel[0] = bimg->b.bytes[idx];
      default:
        break;
      }
      if (OStream_writeBytes(ostream, pixel, bytes_per_pixel) !=
          bytes_per_pixel) {
        WARNING("Failed to write pixel bytes at [%lu,%lu]: expect at least %u "
                "bytes.",
                row, col, bytes_per_pixel);
        return 1;
      }
    }

    // skip padding bytes
    if (padding > 0) {
      byte padBytes[padding];
      memset(padBytes, 0, padding);
      if (OStream_writeBytes(ostream, padBytes, padding) != padding) {
        WARNING("Incorrect padding at line %lu", _row);
        return 1;
      }
    }
  }

  return 0;
}

void BMPImage_dbg(const BMPImage *bimg, FILE *output) {
  if (!bimg || !output) {
    WARNING("The arguments of input can't be null.");
    return;
  }

  // 输出BFHeader信息
  fprintf(output, "=== BMP File Header ===\n");
  fprintf(output, "Type: 0x%04X\n", bimg->bfHeader.bf_type);
  fprintf(output, "Size: %u bytes\n", bimg->bfHeader.bf_size);
  fprintf(output, "Reserved: 0x%08X\n", bimg->bfHeader.bf_reserve);
  fprintf(output, "Offset: %u bytes\n", bimg->bfHeader.bf_offset);

  // 输出BIHeader信息
  fprintf(output, "\n=== BMP Info Header ===\n");
  fprintf(output, "Size: %u bytes\n", bimg->biHeader.bi_size);
  fprintf(output, "Width: %u pixels\n", bimg->biHeader.bi_width);
  fprintf(output, "Height: %u pixels ", bimg->biHeader.bi_height);
  // 检查高度最高位，判断存储方向
  if (bimg->biHeader.bi_height & 0x80000000) {
    fprintf(output, "(top-down)\n");
  } else {
    fprintf(output, "(bottom-up)\n");
  }
  fprintf(output, "Planes: %u\n", bimg->biHeader.bi_planes);
  fprintf(output, "Bit Count: %u bits\n", bimg->biHeader.bi_bitcnt);
  fprintf(output, "Compression: %u\n", bimg->biHeader.bi_compress);
  fprintf(output, "Image Size: %u bytes\n", bimg->biHeader.bi_size_image);
  fprintf(output, "X Resolution: %u pixels/meter\n",
          bimg->biHeader.pixels_per_meter_h);
  fprintf(output, "Y Resolution: %u pixels/meter\n",
          bimg->biHeader.pixels_per_meter_v);
  fprintf(output, "Colors Used: %u\n", bimg->biHeader.bi_clr_used);
  fprintf(output, "Important Colors: %u\n", bimg->biHeader.bi_clr_important);

  // 输出图像维度信息
  fprintf(output, "\n=== Image Data ===\n");
  fprintf(output, "Dimensions: %zu x %zu pixels\n", bimg->r.rows, bimg->r.cols);

  // 输出RGB数据样本
  fprintf(output, "\n=== RGB Sample (top-left corner) ===\n");
  const size_t sample_rows = 5;
  const size_t sample_cols = 5;

  for (size_t i = 0; i < sample_rows && i < bimg->r.rows; i++) {
    for (size_t j = 0; j < sample_cols && j < bimg->r.cols; j++) {
      size_t idx = i * bimg->r.cols + j;
      fprintf(output, "Pixel[%zu,%zu]: R=%3u G=%3u B=%3u\n", i, j,
              bimg->r.bytes[idx], bimg->g.bytes[idx], bimg->b.bytes[idx]);
    }
  }

  // 输出统计信息
  fprintf(output, "\n=== Image Statistics ===\n");

  // 计算每个通道的平均值
  uint32_t r_sum = 0, g_sum = 0, b_sum = 0;
  size_t total_pixels = bimg->r.rows * bimg->r.cols;

  for (size_t i = 0; i < total_pixels; i++) {
    r_sum += bimg->r.bytes[i];
    g_sum += bimg->g.bytes[i];
    b_sum += bimg->b.bytes[i];
  }

  fprintf(output, "Averages: R=%.2f G=%.2f B=%.2f\n",
          (float)r_sum / total_pixels, (float)g_sum / total_pixels,
          (float)b_sum / total_pixels);
}

void BMPImage_release(BMPImage *bimg) {
  if (!bimg)
    return;
  Mat_release(&bimg->r);
  Mat_release(&bimg->g);
  Mat_release(&bimg->b);
  Mat_release(&bimg->a);
}

int __BMPImage_cvtclr_rgb2bgr(BMPImage *bimg) {
  if (!bimg)
    return 0;
  return Mat_swap(&bimg->r, &bimg->b);
}

#define __BMPImage_cvtclr_bgr2rgb(__bimg) __BMPImage_cvtclr_rgb2bgr(__bimg)

int __BMPImage_cvtclr_rgb2hsv(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    rgb(prev, bimg->r.bytes[i], bimg->g.bytes[i], bimg->b.bytes[i]);
    color next = cvtclr_rgb2hsv(prev);
    bimg->r.bytes[i] = next.hsv[2];
    bimg->g.bytes[i] = next.hsv[1];
    bimg->b.bytes[i] = next.hsv[0];
  }

  return 0;
}

int __BMPImage_cvtclr_hsv2rgb(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    hsv(prev, bimg->r.bytes[i], bimg->g.bytes[i], bimg->b.bytes[i]);
    color next = cvtclr_hsv2rgb(prev);
    bimg->r.bytes[i] = next.rgb[2];
    bimg->g.bytes[i] = next.rgb[1];
    bimg->b.bytes[i] = next.rgb[0];
  }

  return 0;
}

int __BMPImage_cvtclr_rgb2gray(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    rgb(prev, bimg->r.bytes[i], bimg->g.bytes[i], bimg->b.bytes[i]);
    color next = cvtclr_rgb2gray(prev);
    bimg->r.bytes[i] = next.gray[2];
    bimg->g.bytes[i] = next.gray[1];
    bimg->b.bytes[i] = next.gray[0];
  }

  return 0;
}

int __BMPImage_cvtclr_hsv2gray(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    hsv(prev, bimg->r.bytes[i], bimg->g.bytes[i], bimg->b.bytes[i]);
    color next = cvtclr_hsv2gray(prev);
    bimg->r.bytes[i] = next.gray[2];
    bimg->g.bytes[i] = next.gray[1];
    bimg->b.bytes[i] = next.gray[0];
  }

  return 0;
}

int __BMPImage_cvtclr_hsv2bgr(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    hsv(prev, bimg->r.bytes[i], bimg->g.bytes[i], bimg->b.bytes[i]);
    color next = cvtclr_hsv2rgb(prev);
    /* diff from hsv2rgb */
    /* BBGGRR */
    bimg->r.bytes[i] = next.rgb[0];
    bimg->g.bytes[i] = next.rgb[1];
    bimg->b.bytes[i] = next.rgb[2];
  }

  return 0;
}

int __BMPImage_cvtclr_bgr2hsv(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    /* diff from rgb2hsv */
    rgb(prev, bimg->b.bytes[i], bimg->g.bytes[i], bimg->r.bytes[i]);
    color next = cvtclr_rgb2hsv(prev);
    bimg->r.bytes[i] = next.hsv[2];
    bimg->g.bytes[i] = next.hsv[1];
    bimg->b.bytes[i] = next.hsv[0];
  }

  return 0;
}

int __BMPImage_cvtclr_bgr2gray(BMPImage *bimg) {
  if (!bimg)
    return 1;

  for (size_t i = 0; i < bimg->psize; i++) {
    color prev;
    /* diff from rgb2gray */
    rgb(prev, bimg->b.bytes[i], bimg->g.bytes[i], bimg->r.bytes[i]);
    color next = cvtclr_rgb2gray(prev);
    bimg->r.bytes[i] = next.gray[2];
    bimg->g.bytes[i] = next.gray[1];
    bimg->b.bytes[i] = next.gray[0];
  }

  return 0;
}

int BMPImage_cvtclr(BMPImage *bimg, const char *mode) {
  if (!mode)
    return 1;
  if (strcmp(mode, "rgb2hsv") == 0) {
    return __BMPImage_cvtclr_rgb2hsv(bimg);
  } else if (strcmp(mode, "rgb2bgr") == 0) {
    return __BMPImage_cvtclr_rgb2bgr(bimg);
  } else if (strcmp(mode, "rgb2gray") == 0) {
    return __BMPImage_cvtclr_rgb2gray(bimg);
  } else if (strcmp(mode, "hsv2rgb") == 0) {
    return __BMPImage_cvtclr_hsv2rgb(bimg);
  } else if (strcmp(mode, "hsv2gray") == 0) {
    return __BMPImage_cvtclr_hsv2gray(bimg);
  } else if (strcmp(mode, "hsv2bgr") == 0) {
    return __BMPImage_cvtclr_hsv2bgr(bimg);
  } else if (strcmp(mode, "bgr2hsv") == 0) {
    return __BMPImage_cvtclr_bgr2hsv(bimg);
  } else if (strcmp(mode, "bgr2rgb") == 0) {
    return __BMPImage_cvtclr_bgr2rgb(bimg);
  } else if (strcmp(mode, "bgr2gray") == 0) {
    return __BMPImage_cvtclr_bgr2gray(bimg);
  } else {
    WARNING("Such mode '%s' is not supported", mode);
    return 1;
  }
}

int __BMPImage_affine_transform(BMPImage *dst, const BMPImage *src,
                                const AffineMat *mat, word newWidth,
                                word newHeight) {
  if (!dst || !src || !mat) {
    WARNING("Arguments of input can't be empty");
    return 1;
  }
  word srcWidth = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word srcHeight = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                           : src->biHeader.bi_height;
  if (newWidth == 0) {
    newWidth = srcWidth;
  }
  if (newHeight == 0) {
    newHeight = srcHeight;
  }

  // 创建目标图像
  BFHeader bfheader = STD_BFHeader(newWidth, newHeight, (STD_BIT_COUNT / 8));
  BIHeader biheader = STD_BIHeader(newWidth, newHeight, (STD_BIT_COUNT / 8),
                                   src->biHeader.pixels_per_meter_h,
                                   src->biHeader.pixels_per_meter_v);

  // 初始化目标图像的通道
  Mat r = Mat(newHeight, newWidth);
  Mat g = Mat(newHeight, newWidth);
  Mat b = Mat(newHeight, newWidth);
  int err = Mat_alloc(&r, newHeight, newWidth) ||
            Mat_alloc(&g, newHeight, newWidth) ||
            Mat_alloc(&b, newHeight, newWidth);
  if (err) {
    WARNING("Failed to allocate memory for the image channels");
    return 1;
  }

  // 计算逆变换矩阵（用于逆向映射）
  AffineMat inv = AffineMat_inv(mat);

  // 对每个目标像素应用逆变换
  for (size_t y = 0; y < newHeight; y++) {
    for (size_t x = 0; x < newWidth; x++) {
      // 应用逆向映射找到源图像中的对应点
      double srcX = inv.a * x + inv.b * y + inv.c;
      double srcY = inv.d * x + inv.e * y + inv.f;

      // 检查源坐标是否在原图像范围内
      if (srcX >= 0 && srcX < srcWidth - 1 && srcY >= 0 &&
          srcY < srcHeight - 1) {

        // 使用双线性插值获取颜色值
        r.bytes[y * newWidth + x] =
            (uint8_t)bilinearInterpolation(&src->r, srcX, srcY);
        g.bytes[y * newWidth + x] =
            (uint8_t)bilinearInterpolation(&src->g, srcX, srcY);
        b.bytes[y * newWidth + x] =
            (uint8_t)bilinearInterpolation(&src->b, srcX, srcY);

        // 无线性插值
        // r.bytes[y * newWidth + x] =
        //     (uint8_t)src->r.bytes[(size_t)round(srcY * srcWidth + srcX)];
        // g.bytes[y * newWidth + x] =
        //     (uint8_t)src->g.bytes[(size_t)round(srcY * srcWidth + srcX)];
        // b.bytes[y * newWidth + x] =
        //     (uint8_t)src->b.bytes[(size_t)round(srcY * srcWidth + srcX)];

      } else {
        // 设置默认背景色
        r.bytes[y * newWidth + x] = 0;
        g.bytes[y * newWidth + x] = 0;
        b.bytes[y * newWidth + x] = 0;
      }
    }
  }

  // 更新图像
  dst->bfHeader = bfheader;
  dst->biHeader = biheader;
  dst->psize = newHeight * newWidth;
  Mat_copy(&dst->r, &r);
  Mat_copy(&dst->g, &g);
  Mat_copy(&dst->b, &b);
  dst->a.bytes = NULL;

  return 0;
}

int BMPImage_rotate(BMPImage *dst, const BMPImage *src, double angle) {
  if (!dst || !src) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  double angleRad = angle * M_PI / 180.0;
  double cosAngle = cos(angleRad);
  double sinAngle = sin(angleRad);

  word width = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                        : src->biHeader.bi_height;

  word newWidth = (word)(width * ABS(cosAngle) + height * ABS(sinAngle));
  word newHeight = (word)(width * ABS(sinAngle) + height * ABS(cosAngle));

  double centerX = width / 2.0;
  double centerY = height / 2.0;
  double newCenterX = newWidth / 2.0;
  double newCenterY = newHeight / 2.0;

  AffineMat mat = AffineMat_create(
      angle, 1.0, 1.0, newCenterX - centerX * cosAngle + centerY * sinAngle,
      newCenterY - centerX * sinAngle - centerY * cosAngle);

  return __BMPImage_affine_transform(dst, src, &mat, newWidth, newHeight);
}

int BMPImage_scale(BMPImage *dst, const BMPImage *src, double scaleX,
                   double scaleY) {
  if (!dst || !src) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  if (scaleX <= 0) {
    WARNING("The scale factor on X axis can't be a non-positive number %lf",
            scaleX);
    return 1;
  }
  if (scaleY <= 0) {
    WARNING("The scale factor on Y axis can't be a non-positive number %lf",
            scaleY);
    return 1;
  }
  word width = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                        : src->biHeader.bi_height;

  word newWidth = (word)(width * scaleX);
  word newHeight = (word)(height * scaleY);

  AffineMat mat = AffineMat_create(0.0, scaleX, scaleY, 0.0, 0.0);

  return __BMPImage_affine_transform(dst, src, &mat, newWidth, newHeight);
}

int BMPImage_translate(BMPImage *dst, const BMPImage *src, int deltaX,
                       int deltaY) {
  if (!dst || !src) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  word width = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                        : src->biHeader.bi_height;

  word newWidth = width + ABS(deltaX);
  word newHeight = height + ABS(deltaY);

  AffineMat mat = AffineMat_create(0.0, 1.0, 1.0, deltaX, deltaY);
  return __BMPImage_affine_transform(dst, src, &mat, newWidth, newHeight);
}

int BMPImage_flip(BMPImage *dst, const BMPImage *src, bool flipX, bool flipY) {
  if (!dst || !src) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  word width = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                        : src->biHeader.bi_height;

  AffineMat mat;

  if (flipX && flipY) {
    mat = AffineMat_create(0.0, -1.0, -1.0, width, height);
  } else if (flipX) {
    mat = AffineMat_create(0.0, -1.0, 1.0, width, 0);
  } else if (flipY) {
    mat = AffineMat_create(0.0, 1.0, -1.0, 0, height);
  } else {
    mat = AffineMat_create(0.0, 1.0, 1.0, 0, 0);
  }

  return __BMPImage_affine_transform(dst, src, &mat, width, height);
}

int BMPImage_fft_transform(BMPImage *dst, const BMPImage *src, bool shift) {
  FFTMat rmat, gmat, bmat;
  word width = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                        : src->biHeader.bi_height;
  word newWidth = 1, newHeight = 1;
  while (newWidth < width)
    newWidth <<= 1;
  while (newHeight < height)
    newHeight <<= 1;
  int ret = FFTMat_alloc(&rmat, newHeight, newWidth) ||
            FFTMat_alloc(&gmat, newHeight, newWidth) ||
            FFTMat_alloc(&bmat, newHeight, newWidth);
  if (ret)
    return 1;

  for (size_t i = 0; i < newHeight; ++i) {
    for (size_t j = 0; j < newWidth; ++j) {
      if (i < height && j < width) {
        rmat.cdata[i * newWidth + j] = src->r.bytes[i * width + j] + 0.0 * I;
        gmat.cdata[i * newWidth + j] = src->g.bytes[i * width + j]  0.0 * I;
        bmat.cdata[i * newWidth + j] = src->b.bytes[i * width + j] + 0.0 * I;
      } else {
        rmat.cdata[i * newWidth + j] = 0.0 + 0.0 * I;
        gmat.cdata[i * newWidth + j] = 0.0 + 0.0 * I;
        bmat.cdata[i * newWidth + j] = 0.0 + 0.0 * I;
      }
    }
  }
  fft2d(&rmat, false);
  fft2d(&gmat, false);
  fft2d(&bmat, false);
  if (shift) {
    fft_shift(&rmat);
    fft_shift(&gmat);
    fft_shift(&bmat);
  }

  Mat_alloc(&dst->r, newHeight, newWidth);
  Mat_alloc(&dst->g, newHeight, newWidth);
  Mat_alloc(&dst->b, newHeight, newWidth);
  fft_magnitude(&dst->r, &rmat);
  fft_magnitude(&dst->g, &gmat);
  fft_magnitude(&dst->b, &bmat);

  BFHeader bfheader = STD_BFHeader(newWidth, newHeight, (STD_BIT_COUNT / 8));
  BIHeader biheader = STD_BIHeader(newWidth, newHeight, (STD_BIT_COUNT / 8),
                                   src->biHeader.pixels_per_meter_h,
                                   src->biHeader.pixels_per_meter_v);
  dst->bfHeader = bfheader;
  dst->biHeader = biheader;
  dst->psize = newWidth * newHeight;
  FFTMat_release(&rmat);
  FFTMat_release(&gmat);
  FFTMat_release(&bmat);
  return 0;
}

char *BMP_asciiart(BMPImage *src) {
  if (!src) {
    WARNING("Arguments of input can't be empty!");
    return NULL;
  }
  word width = src->biHeader.bi_width;
  bool topDown = (src->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src->biHeader.bi_height & 0x7FFFFFFF)
                        : src->biHeader.bi_height;
  double scaleX = (double)ART_MAX_LINE_BUFFER / width;
  if (scaleX > 1.0)
    scaleX = 1.0;
  double scaleY = scaleX / 2.5;
  word newWidth = scaleX * width;
  word newHeight = scaleY * height;

  char *buf = (char *)malloc((newHeight * (newWidth + 1) + 1) * sizeof(char));
  if (!buf)
    return NULL;

  BMPImage scaledImg = BMPImage_0();
  BMPImage_scale(&scaledImg, src, scaleX, scaleY);
  BMPImage_cvtclr(&scaledImg, "rgb2gray");

  size_t i = 0;
  for (size_t j = 0; j < newHeight; j++) {
    for (size_t k = 0; k < newWidth; k++) {
      size_t binval = scaledImg.r.bytes[j * newWidth + k] / 14;

      buf[i++] = LEVELS[binval];
    }
    buf[i++] = '\n';
  }
  buf[i] = '\0';
  BMPImage_release(&scaledImg);
  return buf;
}

int __BMPImage_confuse_avg(BMPImage *dst, const BMPImage *lhs,
                           const BMPImage *rhs) {

  if (!dst || !lhs || !rhs) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  int ret = Mat_confuse_avg(&dst->r, &lhs->r, &rhs->r) ||
            Mat_confuse_avg(&dst->g, &lhs->g, &rhs->g) ||
            Mat_confuse_avg(&dst->b, &lhs->b, &rhs->b);
  if (ret != 0)
    return ret;
  else
    return 0;
}

int __BMPImage_confuse_weight(BMPImage *dst, const BMPImage *lhs,
                              const BMPImage *rhs, Confuse_args weights) {

  if (!dst || !lhs || !rhs) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  double lw = 0.5, rw = 0.5;
  if (weights.valid) {
    lw = weights.w1;
    rw = weights.w2;
  }
  int ret = Mat_confuse_weight(&dst->r, &lhs->r, &rhs->r, lw, rw) ||
            Mat_confuse_weight(&dst->g, &lhs->g, &rhs->g, lw, rw) ||
            Mat_confuse_weight(&dst->b, &lhs->b, &rhs->b, lw, rw);
  if (ret != 0)
    return ret;
  else
    return 0;
}

int __BMPImage_confuse_max(BMPImage *dst, const BMPImage *lhs,
                           const BMPImage *rhs) {

  if (!dst || !lhs || !rhs) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  int ret = Mat_confuse_max(&dst->r, &lhs->r, &rhs->r) ||
            Mat_confuse_max(&dst->g, &lhs->g, &rhs->g) ||
            Mat_confuse_max(&dst->b, &lhs->b, &rhs->b);
  if (ret != 0)
    return ret;
  else
    return 0;
}

int BMPImage_confuse(BMPImage *dst, const BMPImage *src1, const BMPImage *src2,
                     OpRule rule, Confuse_args args) {

  if (!dst || !src1 || !src2) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  word width = src1->biHeader.bi_width;
  bool topDown = (src1->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (src1->biHeader.bi_height & 0x7FFFFFFF)
                        : src1->biHeader.bi_height;
  BMPImage_4((*dst), width, height, src1->biHeader.pixels_per_meter_h,
             src1->biHeader.pixels_per_meter_v);

  switch (rule) {
  case OP_RULE_AVG:
    return __BMPImage_confuse_avg(dst, src1, src2);
  case OP_RULE_WEIGHT:
    return __BMPImage_confuse_weight(dst, src1, src2, args);
  case OP_RULE_MAX:
    return __BMPImage_confuse_max(dst, src1, src2);
  default:
    WARNING("Unrecognized confusion mode!");
    return 1;
  }
}

int BMPImage_conv(BMPImage *res, const BMPImage *basis, const BMPImage *kernel,
                  size_t stride_x, size_t stride_y, bool is_padding) {
  if (!res || !basis || !kernel) {
    WARNING("Arguments of input can't be empty!");
    return MAT_NULL_POINTER;
  }

  word width = basis->biHeader.bi_width;
  bool topDown = (basis->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (basis->biHeader.bi_height & 0x7FFFFFFF)
                        : basis->biHeader.bi_height;

  word newHeight, newWidth;

  if (is_padding) {
    // With padding, output size remains the same as input (when stride=1)
    newHeight = (height - 1) / stride_y + 1;
    newWidth = (width - 1) / stride_x + 1;
  } else {
    // Without padding, output size shrinks
    newHeight = (height - kernel->r.rows) / stride_y + 1;
    newWidth = (width - kernel->r.cols) / stride_x + 1;
  }

  BMPImage_4((*res), newWidth, newHeight, basis->biHeader.pixels_per_meter_h,
             basis->biHeader.pixels_per_meter_v);
  Mat_conv2d(&res->r, &basis->r, &kernel->r, stride_x, stride_y, is_padding);
  Mat_conv2d(&res->g, &basis->g, &kernel->g, stride_x, stride_y, is_padding);
  Mat_conv2d(&res->b, &basis->b, &kernel->b, stride_x, stride_y, is_padding);

  return 0;
}

int BMPImage_line(BMPImage *bimg, Line l, color clr) {
  if (!bimg || !bimg->r.bytes || !bimg->g.bytes || !bimg->b.bytes) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }

  word width = bimg->biHeader.bi_width;
  bool topDown = (bimg->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (bimg->biHeader.bi_height & 0x7FFFFFFF)
                        : bimg->biHeader.bi_height;

  if (l.stroke < 0.5)
    l.stroke = 0.5;

  int dx = abs(l.x1 - l.x0), sx = l.x0 < l.x1 ? 1 : -1;
  int dy = abs(l.y1 - l.y0), sy = l.y0 < l.y1 ? 1 : -1;
  int err = dx - dy;

  double ed = dx + dy == 0 ? 1.0 : sqrtf((double)dx * dx + (double)dy * dy);
  double wd = (l.stroke + 1.0) / 2.0; // 半线宽

#define BLEND_PIXEL(px, py)                                                    \
  do {                                                                         \
    if ((px) >= 0 && (px) < width && (py) >= 0 && (py) < height) {             \
      size_t idx = (py)*width + (px);                                          \
      bimg->r.bytes[idx] = clr.rgb[2];                                         \
      bimg->g.bytes[idx] = clr.rgb[1];                                         \
      bimg->b.bytes[idx] = clr.rgb[0];                                         \
    }                                                                          \
  } while (0)
  for (;;) {
    BLEND_PIXEL(l.x0, l.y0);

    int e2 = err, x2 = l.x0;

    // X步进
    if (2 * e2 >= -dx) {
      e2 += dy;
      for (int y2 = l.y0; e2 < ed * wd && (l.y1 != y2 || dx > dy); e2 += dx) {
        y2 += sy;
        BLEND_PIXEL(l.x0, y2);
      }

      if (l.x0 == l.x1)
        break;
      e2 = err;
      err -= dy;
      l.x0 += sx;
    }

    // Y步进
    if (2 * e2 <= dy) {
      for (e2 = dx - e2; e2 < ed * wd && (l.x1 != x2 || dx < dy); e2 += dy) {
        x2 += sx;
        BLEND_PIXEL(x2, l.y0);
      }

      if (l.y0 == l.y1)
        break;
      err += dx;
      l.y0 += sy;
    }
  }

#undef BLEND_PIXEL
  return 0;
}

int BMPImage_rect(BMPImage *bimg, Rect rect, color clr) {
  if (!bimg || !bimg->r.bytes || !bimg->g.bytes || !bimg->b.bytes) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }

  word width = bimg->biHeader.bi_width;
  bool topDown = (bimg->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (bimg->biHeader.bi_height & 0x7FFFFFFF)
                        : bimg->biHeader.bi_height;

  int sx = rect.dx > 0 ? rect.x : rect.x + rect.dx;
  int sy = rect.dy > 0 ? rect.y : rect.y + rect.dy;
  int ex = sx + abs(rect.dx), ey = sy + abs(rect.dy);

  if (sx < 0) {
    sx = 0;
  } else if (sx >= width) {
    sx = width - 1;
  }
  if (sy < 0) {
    sy = 0;
  } else if (sy >= height) {
    sy = height - 1;
  }
  if (ex < 0) {
    ex = 0;
  } else if (ex >= width) {
    ex = width - 1;
  }
  if (ey < 0) {
    ey = 0;
  } else if (ey >= height) {
    ey = height - 1;
  }

  Mat_fillr(&bimg->r, sx, sy, ex, ey, clr.rgb[2]);
  Mat_fillr(&bimg->g, sx, sy, ex, ey, clr.rgb[1]);
  Mat_fillr(&bimg->b, sx, sy, ex, ey, clr.rgb[0]);

  return 0;
}

int BMPImage_circle(BMPImage *bimg, Circle c, color clr) {

  if (!bimg || !bimg->r.bytes || !bimg->g.bytes || !bimg->b.bytes) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }

  word width = bimg->biHeader.bi_width;
  bool topDown = (bimg->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (bimg->biHeader.bi_height & 0x7FFFFFFF)
                        : bimg->biHeader.bi_height;
  int r = c.radius, x = r, y = 0, err = 0, i;
#define BLEND_PIXEL(px, py, alpha)                                             \
  do {                                                                         \
    if ((px) >= 0 && (px) < width && (py) >= 0 && (py) < height) {             \
      size_t idx = (py)*width + (px);                                          \
      bimg->r.bytes[idx] =                                                     \
          (uint8_t)((double)bimg->r.bytes[idx] * (1.0f - (alpha)) +            \
                    (double)clr.rgb[2] * (alpha));                             \
      bimg->g.bytes[idx] =                                                     \
          (uint8_t)((double)bimg->g.bytes[idx] * (1.0f - (alpha)) +            \
                    (double)clr.rgb[1] * (alpha));                             \
      bimg->b.bytes[idx] =                                                     \
          (uint8_t)((double)bimg->b.bytes[idx] * (1.0f - (alpha)) +            \
                    (double)clr.rgb[0] * (alpha));                             \
    }                                                                          \
  } while (0)

  for (r = 2 * r + 1; x > 0; err += ++y * 2 - 1) {
    if (err + 2 * x + 1 < r)
      err += ++x * 2 - 1;
    for (; err > 0; err -= --x * 2 + 1) {
      double alpha = (double)err / r;
      BLEND_PIXEL(c.rx + x, c.ry + y, 1.0 - alpha);
      BLEND_PIXEL(c.rx - y, c.ry + x, 1.0 - alpha);
      BLEND_PIXEL(c.rx - x, c.ry - y, 1.0 - alpha);
      BLEND_PIXEL(c.rx + y, c.ry - x, 1.0 - alpha);
    }
    for (i = x; i > 0; i--) {
      BLEND_PIXEL(c.rx + i, c.ry + y, 1.0);
      BLEND_PIXEL(c.rx - y, c.ry + i, 1.0);
      BLEND_PIXEL(c.rx - i, c.ry - y, 1.0);
      BLEND_PIXEL(c.rx + y, c.ry - i, 1.0);
    }
  }
  BLEND_PIXEL(c.rx, c.ry, 1.0);

  return 0;
}

int BMPImage_noise_pepper(BMPImage *bimg, double prob) {
  if (!bimg || !bimg->r.bytes || !bimg->g.bytes || !bimg->b.bytes) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }
  if (prob < 0 || prob > 1) {
    WARNING("Probablity %lf is not within range [0, 1].", prob);
    return 1;
  }

  for (size_t i = 0; i < bimg->psize; i++) {
    if (rand() < prob * RAND_MAX) {
      if (rand() < RAND_MAX >> 1) {
        bimg->r.bytes[i] = bimg->g.bytes[i] = bimg->b.bytes[i] = 0;
      } else {
        bimg->r.bytes[i] = bimg->g.bytes[i] = bimg->b.bytes[i] = 255;
      }
    }
  }

  return 0;
}

int BMPImage_clip(BMPImage *bimg, Rect rect) {
  if (!bimg || !bimg->r.bytes || !bimg->g.bytes || !bimg->b.bytes) {
    WARNING("Arguments of input can't be empty!");
    return 1;
  }

  word width = bimg->biHeader.bi_width;
  bool topDown = (bimg->biHeader.bi_height & 0x80000000) != 0;
  word height = topDown ? (bimg->biHeader.bi_height & 0x7FFFFFFF)
                        : bimg->biHeader.bi_height;

  int sx = rect.dx > 0 ? rect.x : rect.x + rect.dx;
  int sy = rect.dy > 0 ? rect.y : rect.y + rect.dy;
  int ex = sx + abs(rect.dx), ey = sy + abs(rect.dy);

  if (sx < 0) {
    sx = 0;
  } else if (sx >= width) {
    sx = width - 1;
  }
  if (sy < 0) {
    sy = 0;
  } else if (sy >= height) {
    sy = height - 1;
  }
  if (ex < 0) {
    ex = 0;
  } else if (ex >= width) {
    ex = width - 1;
  }
  if (ey < 0) {
    ey = 0;
  } else if (ey >= height) {
    ey = height - 1;
  }

  word newWidth = ex - sx;
  word newHeight = ey - sy;

  Mat_clip(&bimg->r, sx, sy, ex, ey);
  Mat_clip(&bimg->g, sx, sy, ex, ey);
  Mat_clip(&bimg->b, sx, sy, ex, ey);

  bimg->bfHeader = STD_BFHeader(newWidth, newHeight, STD_BIT_COUNT / 8);
  bimg->biHeader = STD_BIHeader(newWidth, newHeight, (STD_BIT_COUNT / 8),
                                bimg->biHeader.pixels_per_meter_h,
                                bimg->biHeader.pixels_per_meter_v);
  bimg->psize = newWidth * newHeight;
  return 0;
}
