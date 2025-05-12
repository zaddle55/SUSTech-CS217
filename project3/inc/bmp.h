#ifndef BMP_H_
#define BMP_H_
#include "color.h"
#include "pmat.h"
#include "stream.h"

#define STD_BFHEADER_LEN 14
#define STD_BIHEADER_LEN 40
#define STD_IMAGE_TYPE 0x4d42
#define STD_RESERVE 0
#define STD_HEADER_LEN (STD_BFHEADER_LEN + STD_BIHEADER_LEN)
#define STD_BIT_COUNT 0x18
#define STD_PLANES 1
#define STD_COMPRESS_TYPE 0
#define STD_CLR_USED 0
#define STD_IMPORTANT_CLR 0
#define LINE_ALIGN 4

#define ART_MAX_LINE_BUFFER 150

#define CONFUSE_MODE
#define CONFUSE_ADDAVG "add"
#define CONFUSE_WEIGHTAVG "weight"
#define CONFUSE_MAX "max"
#define CONFUSE_GUASS "gauss"

#define ABS(__x) (((__x) >= 0) ? (__x) : -(__x))

typedef struct PACKED BitmapFileHeader_ {
  hword bf_type;
  word bf_size;
  word bf_reserve;
  word bf_offset;
} BFHeader;

typedef struct PACKED BitmapInfoHeader_ {
  word bi_size;
  word bi_width;
  word bi_height;
  hword bi_planes;
  hword bi_bitcnt;
  word bi_compress;
  word bi_size_image;
  word pixels_per_meter_h;
  word pixels_per_meter_v;
  word bi_clr_used;
  word bi_clr_important;
} BIHeader;

typedef struct Confuse_args_ {
  bool valid;
  double w1;
  double w2;
} Confuse_args;

#define BIN_LEVEL 19
static const char LEVELS[BIN_LEVEL + 1] = "@W#$OEXC[(/?=^~_.` ";
#define BIN_LEN (255 / BIN_LEVEL)

#define STD_BFHeader(__w, __h, __bpp)                                          \
  (BFHeader) {                                                                 \
    .bf_type = STD_IMAGE_TYPE,                                                 \
    .bf_size = (__w) * (__h) * (__bpp) + STD_HEADER_LEN,                       \
    .bf_reserve = STD_RESERVE, .bf_offset = STD_HEADER_LEN                     \
  }

#define STD_BIHeader(__w, __h, __bpp, __x_resl, __y_resl)                      \
  (BIHeader) {                                                                 \
    .bi_size = STD_BIHEADER_LEN, .bi_width = (__w), .bi_height = (__h),        \
    .bi_planes = STD_PLANES, .bi_bitcnt = (__bpp)*8,                           \
    .bi_compress = STD_COMPRESS_TYPE,                                          \
    .bi_size_image = (__w) * (__h) * (__bpp),                                  \
    .pixels_per_meter_h = (__x_resl), .pixels_per_meter_v = (__y_resl),        \
    .bi_clr_used = STD_CLR_USED, .bi_clr_important = STD_IMPORTANT_CLR         \
  }

typedef struct BitmapImage {
  BFHeader bfHeader;
  BIHeader biHeader;
  size_t psize;

  Mat r;
  Mat g;
  Mat b;
  Mat a;
} BMPImage;

typedef enum { OP_RULE_MAX, OP_RULE_AVG, OP_RULE_WEIGHT, OP_RULE_GAUSS } OpRule;

#define BMPImage_0()                                                           \
  { .psize = 0, .r = Mat(0, 0), .g = Mat(0, 0), .b = Mat(0, 0), .a = Mat(0, 0) }
#define BMPImage_4(__bimg, __w, __h, __x_resl, __y_resl)                       \
  do {                                                                         \
    BFHeader __bf = STD_BFHeader(__w, __h, STD_BIT_COUNT / 8);                 \
    BIHeader __bi =                                                            \
        STD_BIHeader(__w, __h, STD_BIT_COUNT / 8, __x_resl, __y_resl);         \
    Mat_alloc(&__bimg.r, __h, __w);                                            \
    Mat_alloc(&__bimg.g, __h, __w);                                            \
    Mat_alloc(&__bimg.b, __h, __w);                                            \
    __bimg.a = Mat(0, 0);                                                      \
    __bimg.psize = __w * __h;                                                  \
    __bimg.bfHeader = __bf;                                                    \
    __bimg.biHeader = __bi;                                                    \
  } while (0)

int BMPImage_decode(BMPImage *bimg, IStream *istream);
void BMPImage_dbg(const BMPImage *bimg, FILE *output);
int BMPImage_resize(BMPImage *bimg, const word width, const word height);
int BMPImage_encode(BMPImage *bimg, OStream *ostream);
void BMPImage_release(BMPImage *bimg);
int BMPImage_rotate(BMPImage *dst, const BMPImage *src, double angle);
int BMPImage_scale(BMPImage *dst, const BMPImage *src, double scaleX,
                   double scaleY);
int BMPImage_translate(BMPImage *dst, const BMPImage *src, int deltaX,
                       int deltaY);
int BMPImage_flip(BMPImage *dst, const BMPImage *src, bool flipX, bool flipY);
int BMPImage_fft_transform(BMPImage *dst, const BMPImage *src, bool shift);
char *BMP_asciiart(BMPImage *src);

int __BMPImage_cvtclr_rgb2bgr(BMPImage *bimg);
int __BMPImage_cvtclr_rgb2hsv(BMPImage *bimg);
int __BMPImage_cvtclr_rgb2gray(BMPImage *bimg);
int __BMPImage_cvtclr_hsv2rgb(BMPImage *bimg);
int __BMPImage_cvtclr_hsv2gray(BMPImage *bimg);
int BMPImage_cvtclr(BMPImage *bimg, const char *mode);
int BMPImage_confuse(BMPImage *dst, const BMPImage *src1, const BMPImage *src2,
                     OpRule mode, Confuse_args args);

int BMPImage_line(BMPImage *bimg, Line l, color clr);
int BMPImage_rect(BMPImage *bimg, Rect rect, color clr);
int BMPImage_circle(BMPImage *bimg, Circle c, color clr);

int BMPImage_noise_pepper(BMPImage *bimg, double prob);

int BMPImage_clip(BMPImage *bimg, Rect rect);

int BMPImage_conv(BMPImage *res, const BMPImage *basis, const BMPImage *kernel,
                  size_t stride_x, size_t stride_y, bool is_padding);

#endif