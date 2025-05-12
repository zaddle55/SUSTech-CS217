#ifndef STREAM_H_
#define STREAM_H_
#include <bits/types/FILE.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t byte;
typedef uint16_t hword;
typedef uint32_t word;
typedef uint64_t dword;

#define BUFFER_SIZE 4096

// Base64 encoding table
static const byte base64_table[] = {
    /* 'A' */ 65,  /* 'B' */ 66,  /* 'C' */ 67,  /* 'D' */ 68,
    /* 'E' */ 69,  /* 'F' */ 70,
    /* 'G' */ 71,  /* 'H' */ 72,  /* 'I' */ 73,  /* 'J' */ 74,
    /* 'K' */ 75,  /* 'L' */ 76,
    /* 'M' */ 77,  /* 'N' */ 78,  /* 'O' */ 79,  /* 'P' */ 80,
    /* 'Q' */ 81,  /* 'R' */ 82,
    /* 'S' */ 83,  /* 'T' */ 84,  /* 'U' */ 85,  /* 'V' */ 86,
    /* 'W' */ 87,  /* 'X' */ 88,
    /* 'Y' */ 89,  /* 'Z' */ 90,  /* 'a' */ 97,  /* 'b' */ 98,
    /* 'c' */ 99,  /* 'd' */ 100,
    /* 'e' */ 101, /* 'f' */ 102, /* 'g' */ 103, /* 'h' */ 104,
    /* 'i' */ 105, /* 'j' */ 106,
    /* 'k' */ 107, /* 'l' */ 108, /* 'm' */ 109, /* 'n' */ 110,
    /* 'o' */ 111, /* 'p' */ 112,
    /* 'q' */ 113, /* 'r' */ 114, /* 's' */ 115, /* 't' */ 116,
    /* 'u' */ 117, /* 'v' */ 118,
    /* 'w' */ 119, /* 'x' */ 120, /* 'y' */ 121, /* 'z' */ 122,
    /* '0' */ 48,  /* '1' */ 49,
    /* '2' */ 50,  /* '3' */ 51,  /* '4' */ 52,  /* '5' */ 53,
    /* '6' */ 54,  /* '7' */ 55,
    /* '8' */ 56,  /* '9' */ 57,  /* '+' */ 43,  /* '/' */ 47};

typedef struct Stream_ {
  uint64_t ptr;
  uint64_t capacity;
  uint64_t size;
  bool has_closed;
  byte *buffer;
} Stream, IStream, OStream;

#define Stream(__s)                                                            \
  {                                                                            \
      .ptr = 0,                                                                \
      .capacity = BUFFER_SIZE,                                                 \
      .size = 0,                                                               \
                                                                               \
      .has_closed = false,                                                     \
      .buffer = NULL,                                                          \
  };                                                                           \
  __s.buffer = (byte *)malloc(BUFFER_SIZE * sizeof(byte));                     \
  if (__s.buffer == NULL) {                                                    \
    exit(1);                                                                   \
  }

void Stream_toBase64(Stream *stream);
void IStream_fromFileStream(IStream *istream, FILE *fp);
uint64_t IStream_availableBytes(const IStream *istream);
uint64_t IStream_dilate(IStream *istream, uint64_t new_capacity);
uint64_t IStream_readByte(IStream *istream, byte *buffer);
uint64_t IStream_readBytes(IStream *istream, byte *buffer, uint64_t num_bytes);
uint64_t IStream_readHWord(IStream *istream, hword *buffer);
uint64_t IStream_readWord(IStream *istream, word *buffer);
uint64_t IStream_readDWord(IStream *istream, dword *buffer);
void IStream_close(IStream *istream);

void OStream_toFileStream(OStream *ostream, FILE *fp);
uint64_t OStream_availableBytes(const OStream *ostream);
uint64_t OStream_writeBytes(OStream *ostream, const byte *bytes,
                            uint64_t num_bytes);
uint64_t OStream_dilate(OStream *ostream, uint64_t new_capacity);
uint64_t OStream_writeByte(OStream *ostream, const byte byte);
uint64_t OStream_writeHWord(OStream *ostream, const hword hword);
uint64_t OStream_writeWord(OStream *ostream, const word word);
uint64_t OStream_writeDWord(OStream *ostream, const dword dword);
void OStream_close(OStream *ostream);

#endif // STREAM_H_

#ifndef COLOR_H_
#define COLOR_H_
#define PACKED __attribute__((__packed__))

#include <float.h>
#include <math.h>
#include <stdint.h>
#define MAX(a, b, c)                                                           \
  ({                                                                           \
    typeof(a) _a = (a);                                                        \
    typeof(b) _b = (b);                                                        \
    typeof(c) _c = (c);                                                        \
    _a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c);                       \
  })
#define MIN(a, b, c)                                                           \
  ({                                                                           \
    typeof(a) _a = (a);                                                        \
    typeof(b) _b = (b);                                                        \
    typeof(c) _c = (c);                                                        \
    _a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c);                       \
  })
typedef union PACKED {
  uint8_t rgb[3];
  uint8_t hsv[3];
  uint8_t gray[3];
} color3, color;

typedef struct PACKED {
  uint8_t argb[4];
} color4;

#define R_COEF 0.299
#define G_COEF 0.587
#define B_COEF 0.114

#define rgb(__c, __r, __g, __b)                                                \
  __c.rgb[2] = __r;                                                            \
  __c.rgb[1] = __g;                                                            \
  __c.rgb[0] = __b;

#define hsv(__c, __h, __s, __v)                                                \
  __c.hsv[2] = __h;                                                            \
  __c.hsv[1] = __s;                                                            \
  __c.hsv[0] = __v;

#define gray(__c, __gray) __c.gray[2] = __c.gray[1] = __c.gray[0] = __gray;

color cvtclr_rgb2hsv(color clr);
color cvtclr_hsv2rgb(color clr);
color cvtclr_rgb2gray(color clr);
color cvtclr_hsv2gray(color clr);
color fromHex(uint32_t hex);
int hex_to_ansi16(int hex_color);

#endif // COLOR_H_

#ifndef PMAT_H_
#define PMAT_H_

#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// ############ Shape ############ //

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

// ############ Mat ############ //

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
MatErr Mat_confuse_max(Mat *res, const Mat *lhs, const Mat *rhs);
MatErr Mat_pad(Mat *base, uint8_t padVal, size_t padL, size_t padT, size_t padR,
            size_t padD);
MatErr Mat_fill(Mat* mat, const uint8_t value);
MatErr Mat_fillr(Mat* mat, size_t sx, size_t sy, size_t ex, size_t ey, const uint8_t value);
MatErr Mat_clip(Mat *base, size_t sx, size_t sy, size_t ex, size_t ey);
MatErr Mat_adds(Mat *res, const Mat *lhs, const int32_t rhs);
double bilinearInterpolation(const Mat *channel, double x, double y);
MatErr Mat_conv2d(Mat *res, const Mat *basis, const Mat *kernel,
                  size_t stride_x, size_t stride_y, bool is_padding);

// ############ Affine Mat ############ //

typedef struct Matf64_2x3_ {
  double a, b, c; // x' = ax + by + c
  double d, e, f; // y' = dx + ey + f
} Matf64_2x3, AffineMat;
AffineMat AffineMat_create(double angle, double scaleX, double scaleY,
                           double translateX, double translateY);
AffineMat AffineMat_inv(const AffineMat *m);


// ############ FFT Mat ############ //
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

#endif // PMAT_H_

#ifndef ERROR_H_
#define ERROR_H_
#include <stdio.h>
#include <stdlib.h>

// ############################## Basic Macro ##############################

#define __ERR_OUT(fmt, ...) do { \
    fprintf(stderr, "%s:%d: at function '%s'\r\n  \u2514\u2500\u2500\u2500", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    fprintf(stderr, "\r\n"); \
} while (0)

/// @brief Panic macro for serious errors
/// @param msg Message to print
/// @details This macro prints an error message and exits the program
#define PANIC(fmt, ...) do { \
    fprintf(stderr, "\033[1;31m [ERROR]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
    exit(1); \
} while (0)

/// @brief Notice macro for non-serious errors
/// @param msg Message to print
/// @details This macro prints a notice message
#define NOTION(fmt, ...) do { \
    fprintf(stderr, "\033[1;34m [NOTICE]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
} while (0)

/// @brief Warning macro for warnings
/// @param msg Message to print
/// @details This macro prints a warning message
#define WARNING(fmt, ...)                                                      \
  do {                                                                         \
    fprintf(stderr, "\033[1;33m [WARNING]: \033[0m");                          \
    __ERR_OUT(fmt, ##__VA_ARGS__);                                             \
  } while (0)

/// @brief Warning macro for warnings
/// @param msg Message to print
/// @details This macro prints a warning message
#define DBG(expr)                                                         \
  do {                                                                         \
    fprintf(stderr, "\033[1;32m [DEBUG]: \033[0m");                            \
    __ERR_OUT("%s: %d\n", #expr, expr);                                                    \
  } while (0)

#define ANSI_RESET  "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_PURPLE  "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"

/**
 * @brief print wavy line below the input string in different colors
 * @param __input The input string
 * @param __pos The position of the wavy line
 * @param __len The length of the wavy line
 * @details This macro will print marked input string with wavy line like this:
 * e.g. WAVY_RED("Hello, World!", 6, 5);
 * <<<  Hello, World!
 * <<<         ~~~~~
 *
 */
 #define WAVY(__input, __pos, __len, ANSI_COLOR) \
    fprintf(stderr, "\t%s\r\n\t", (__input)); \
    fprintf(stderr, ANSI_COLOR); \
    for (int __i = 0; __i < (__pos); __i++) fprintf(stderr, " "); \
    fprintf(stderr, "^"); \
    for (int __i = 1; __i < (__len); __i++) fprintf(stderr, "~"); \
    fprintf(stderr, "\r\n"); \
    fprintf(stderr, ANSI_RESET);fprintf(stderr, "\r\n");


// ############################## Memory Exception ##############################

#define MEM_EXCEPT_
#define MEM_ALLOC_FAIL(__at, __why) PANIC("<Memory Exception> Memory allocation at <%p> failed: %s", __at, __why)
#define MEM_FREE_FAIL(__at, __why) PANIC("<Memory Exception> Memory free failed at <%p>: %s", __at, __why)
#define MEM_ACCESS_NULLPTR(__ptr_n, __why) PANIC("<Memory Exception> Accessing NULL pointer '%s': %s", __ptr_n, __why)
#define MEM_ACCESS_OOB(__ptr_n, __idx, __size) PANIC("<Memory Exception> Accessing out of bound at index %d of '%s' with size %d", __idx, __ptr_n, __size)

#endif // ERROR_H_

#ifndef BMP_H_
#define BMP_H_

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

#endif // BMP_H_

// Impl Files go here

color cvtclr_rgb2hsv(color clr)
{
    double rn = (double)clr.rgb[2] / 0xFF;
    double gn = (double)clr.rgb[1] / 0xFF;
    double bn = (double)clr.rgb[0] / 0xFF;

    double cmax = MAX(rn, gn, bn);
    double cmin = MIN(rn, gn, bn);
    double delta = cmax - cmin;
    
    double hn = 0., sn = 0., vn = 0.;
    
    /* calculate h value */
    if (delta < FLT_EPSILON) { // 避免浮点精度问题
        hn = 0;
    } else if (cmax == rn) {
        hn = 60 * (((gn - bn) / delta) + (gn < bn ? 6 : 0)); // 修正公式
    } else if (cmax == gn) {
        hn = 60 * (((bn - rn) / delta) + 2);
    } else { // cmax == bn
        hn = 60 * (((rn - gn) / delta) + 4);
    }
    
    /* calculate s value */
    sn = (cmax < FLT_EPSILON) ? 0 : (delta / cmax);
    
    /* calculate v value */
    vn = cmax;
    
    color _clr;
    hsv(_clr, (uint8_t)(hn / 2), // 这里除以2是为了映射到0-180的范围
                      (uint8_t)(sn * 255), 
                      (uint8_t)(vn * 255));
    // 标准HSV：H范围0-360，S范围0-1，V范围0-1
    return _clr;
}

color fromHex(uint32_t hex) {
  color clr;
  rgb(clr, hex >> 16, hex >> 8, hex);
  return clr;
}

color cvtclr_hsv2rgb(color clr)
{
    double hn = clr.hsv[2] * 2.;
    double sn = (double)clr.hsv[1] / 0xFF;
    double vn = (double)clr.hsv[0] / 0xFF;

    int hi = (int)(hn/60);
    double f = hn/60-hi;
    double p = vn * (1-sn);
    double q = vn * (1-f*sn);
    double t = vn * (1-(1-f)*sn);

    color _clr;
    switch (hi) {
        case 0:
            rgb(_clr, (uint8_t)(vn * 255), 
            (uint8_t)(t * 255), 
            (uint8_t)(p * 255));
            break;
        case 1:
            rgb(_clr, (uint8_t)(q * 255),
             (uint8_t)(vn * 255),
             (uint8_t)(p * 255));
            break;
        case 2:
            rgb(_clr, (uint8_t)(p * 255), 
            (uint8_t)(vn * 255), 
            (uint8_t)(t * 255));
            break;
        case 3:
            rgb(_clr, (uint8_t)(p * 255), 
            (uint8_t)(q * 255), 
            (uint8_t)(vn * 255));
            break;
        case 4:
            rgb(_clr, (uint8_t)(t * 255), 
            (uint8_t)(p * 255), 
            (uint8_t)(vn * 255));
            break;
        default:
            rgb(_clr, (uint8_t)(vn * 255), 
            (uint8_t)(p * 255), 
            (uint8_t)(q * 255));
            break;
    }
    return _clr;
}

color cvtclr_hsv2gray(color clr)
{
    color _clr = cvtclr_hsv2rgb(clr);
    return cvtclr_rgb2gray(_clr);
}

color cvtclr_rgb2gray(color clr)
{
    double gray = clr.rgb[2] * R_COEF 
                + clr.rgb[1] * G_COEF 
                + clr.rgb[0] * B_COEF;
    color _clr;
    gray(_clr, gray)
    return _clr;
}

color cvtclr_gray2rgb(color clr)
{
    color _clr;
    rgb(_clr, clr.gray[2], clr.gray[1], clr.gray[0]);
    return _clr;
}

int hex_to_ansi16(int hex_color) {
    int r = hex_color >> 16 & 0xFF;
    int g = hex_color >> 8 & 0xFF;
    int b = hex_color & 0xFF;
    
    // calculate the bit values
    int r_bit = (r > 127) ? 1 : 0;
    int g_bit = (g > 127) ? 1 : 0;
    int b_bit = (b > 127) ? 1 : 0;
    
    // convert to ANSI 16 color
    int ansi_color = (r_bit << 2) | (g_bit << 1) | b_bit;
    
    // calculate brightness
    int brightness = (r + g + b) / 3;
    if (brightness > 160) {
        ansi_color += 8;  // is high brightness
    }
    
    return ansi_color;
}

size_t base64_size(size_t input_size) {
    return ((input_size + 2) / 3) * 4 + 1; // +1 for null terminator
  }
  
  void Stream_toBase64(Stream *stream) {
    if (!stream) {
      WARNING("Stream can't be null");
      return;
    }
    if (stream->has_closed) {
      WARNING("Stream has been closed!");
      return;
    }
    size_t i = 0, j = 0;
    byte a, b, c;
  
    /* allocate memory for output string */
    size_t base64_len = base64_size(stream->size);
    char *output = (char *)calloc(base64_len, sizeof(byte));
    if (!output) {
      WARNING("Fail to allocate memory for output string.");
      return;
    }
  
    for (; i < base64_len; i += 3, j += 4) {
      a = stream->buffer[i];
      b = (i + 1 < base64_len) ? stream->buffer[i + 1] : 0;
      c = (i + 2 < base64_len) ? stream->buffer[i + 2] : 0;
  
      word w = (word)a << 16 | (word)b << 8 | (word)c;
  
      byte mask = 0x3F;
      output[j] = base64_table[(w >> 18) & mask];
      output[j + 1] = base64_table[(w >> 12) & mask];
      output[j + 2] =
          (i + 1 < base64_len) ? base64_table[(w >> 6) & mask] : 61 /* '=' */;
      output[j + 3] =
          (i + 2 < base64_len) ? base64_table[w & mask] : 61 /* '=' */;
    }
  
    output[j] = 0;
    memcpy(stream->buffer, output, base64_len);
  }
  
  void IStream_fromFileStream(IStream *istream, FILE *fp) {
    if (!istream || !fp)
      return;
    if (istream->has_closed) {
      WARNING("Stream has been closed!");
      return;
    }
    fseek(fp, 0, SEEK_END);
    int64_t file_size = ftello(fp);
    if (file_size <= 0) {
      WARNING("The size of file must be positive, not %ld", file_size);
      return;
    }
    rewind(fp);
  
    if (istream->capacity < file_size) {
      uint64_t capacity = IStream_dilate(istream, ((uint64_t)file_size << 1) + 1);
      if (capacity == 0ULL) {
        WARNING("The memory fails to reallocate!");
        return;
      }
    }
  
    istream->size = fread(istream->buffer, 1, file_size, fp);
    istream->ptr = 0;
    istream->has_closed = false;
  }
  
  uint64_t IStream_dilate(IStream *istream, uint64_t new_capacity) {
    if (!istream)
      return 0ULL;
    if (istream->buffer) {
      byte *new_buffer = (byte *)realloc(istream->buffer, new_capacity);
      if (new_buffer) {
        istream->buffer = new_buffer;
        istream->capacity = new_capacity;
      } else {
        WARNING("The memory fails to reallocate!");
        return 0ULL;
      }
    } else {
      istream->buffer = (byte *)malloc(new_capacity);
      if (!istream->buffer) {
        WARNING("The memory fails to allocate!");
        return 0ULL;
      }
      istream->capacity = new_capacity;
    }
  
    return istream->capacity;
  }
  
  inline uint64_t IStream_availableBytes(const IStream *istream) {
    if (istream->has_closed) {
      WARNING("Stream has been closed!");
      return 0ULL;
    }
    return istream->size - istream->ptr;
  }
  
  uint64_t IStream_readByte(IStream *istream, byte *buffer) {
    if (!istream || !buffer || !istream->buffer)
      return 0ULL;
    if (IStream_availableBytes(istream) == 0ULL) {
      WARNING("No more bytes can be read from stream");
      return 0ULL;
    }
    *buffer = istream->buffer[istream->ptr++];
    return 1ULL;
  }
  
  uint64_t IStream_readBytes(IStream *istream, byte *buffer, uint64_t num_bytes) {
    if (!istream || !buffer || !istream->buffer)
      return 0ULL;
    if (istream->has_closed) {
      WARNING("Stream has been closed!");
      return 0ULL;
    }
    uint64_t available_bytes = IStream_availableBytes(istream);
    uint64_t read_bytes =
        (available_bytes >= num_bytes) ? num_bytes : available_bytes;
    memcpy(buffer, istream->buffer + istream->ptr, read_bytes);
    istream->ptr += read_bytes;
    return read_bytes;
  }
  
  uint64_t IStream_readHWord(IStream *istream, hword *buffer) {
    if (!istream || !buffer)
      return 0ULL;
    return IStream_readBytes(istream, (byte *)buffer, 2ULL);
  }
  
  uint64_t IStream_readWord(IStream *istream, word *buffer) {
    if (!istream || !buffer)
      return 0ULL;
    return IStream_readBytes(istream, (byte *)buffer, 4ULL);
  }
  
  uint64_t IStream_readDWord(IStream *istream, dword *buffer) {
    if (!istream || !buffer)
      return 0ULL;
    return IStream_readBytes(istream, (byte *)buffer, 8ULL);
  }
  
  void IStream_close(IStream *istream) {
    if (!istream)
      return;
    istream->has_closed = true;
    if (istream->buffer != NULL) {
      free(istream->buffer);
      istream->buffer = NULL;
    }
  }
  
  void OStream_toFileStream(OStream *ostream, FILE *fp) {
    if (!ostream || !fp)
      return;
    if (ostream->has_closed) {
      WARNING("Stream has been closed!");
      return;
    }
    uint64_t wbytes = fwrite(ostream->buffer, 1, ostream->size, fp);
    fflush(fp);
    if (wbytes != ostream->size) {
      WARNING("The actual bytes written in file stream is %zu, expected %zu!",
              wbytes, ostream->size);
    }
  }
  
  uint64_t OStream_availableBytes(const OStream *ostream) {
    return ostream->capacity - ostream->size;
  }
  
  inline uint64_t OStream_dilate(OStream *ostream, uint64_t new_capacity) {
    return IStream_dilate((IStream *)ostream, new_capacity);
  }
  
  uint64_t OStream_writeByte(OStream *ostream, const byte byte) {
    if (!ostream || !ostream->buffer)
      return 0ULL;
    if (ostream->has_closed) {
      WARNING("Stream has been closed!");
      return 0ULL;
    }
    if (OStream_availableBytes(ostream) == 0ULL) {
      uint64_t capacity = OStream_dilate(ostream, (ostream->size << 1) + 1);
      if (capacity == 0ULL) {
        WARNING("Failed to dilate new capacity!");
        return 0ULL;
      }
    }
    ostream->buffer[ostream->ptr++] = byte;
    ostream->size = ostream->ptr;
    return 1ULL;
  }
  
  uint64_t OStream_writeBytes(OStream *ostream, const byte *bytes,
                              uint64_t num_bytes) {
    if (!ostream || !ostream->buffer || !bytes)
      return 0ULL;
    if (ostream->has_closed) {
      WARNING("Stream has been closed!");
      return 0ULL;
    }
    uint64_t available_bytes = OStream_availableBytes(ostream);
    if (available_bytes < num_bytes) {
      uint64_t capacity = OStream_dilate(ostream, (ostream->size << 1) + 1);
      if (capacity == 0ULL) {
        WARNING("The memory fails to reallocate!");
        return 0ULL;
      }
      available_bytes = OStream_availableBytes(ostream);
    }
    uint64_t write_bytes =
        (available_bytes >= num_bytes) ? num_bytes : available_bytes;
    memcpy(ostream->buffer + ostream->ptr, bytes, write_bytes);
    ostream->ptr += write_bytes;
    ostream->size = ostream->ptr;
    return write_bytes;
  }
  
  uint64_t OStream_writeHWord(OStream *ostream, const hword hword) {
    if (!ostream)
      return 0ULL;
    return OStream_writeBytes(ostream, (byte *)&hword, 2ULL);
  }
  
  uint64_t OStream_writeWord(OStream *ostream, const word word) {
    if (!ostream)
      return 0ULL;
    return OStream_writeBytes(ostream, (byte *)&word, 4ULL);
  }
  
  uint64_t OStream_writeDWord(OStream *ostream, const dword dword) {
    if (!ostream)
      return 0ULL;
    return OStream_writeBytes(ostream, (byte *)&dword, 8ULL);
  }
  
  void OStream_close(OStream *ostream) {
    if (!ostream)
      return;
    ostream->has_closed = true;
    if (ostream->buffer) {
      free(ostream->buffer);
      ostream->buffer = NULL;
    }
}

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

  mat->bytes = (uint8_t *)realloc(mat->bytes, rows * cols * sizeof(uint8_t));
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
  for (size_t i = sy; i <= ey; i++) {
    for (size_t j = sx; j <= ex; j++) {
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
  if (sx >= base->cols || sy >= base->rows || ex >= base->cols ||
      ey >= base->rows)
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
  if (frac_l < 0 || frac_r < 0) {
    WARNING("The weight of lhs and rhs should be positive!");
    return MAT_OUT_OF_BOUND;
  }

  for (size_t i = 0; i < psize; i++) {
    res->bytes[i] = (uint8_t)(lhs->bytes[i] * frac_l + rhs->bytes[i] * frac_r);
  }
  return MAT_OK;
}

MatErr Mat_confuse_max(Mat *res, const Mat *lhs, const Mat *rhs) {
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
    if (lhs->bytes[i] > rhs->bytes[i])
      res->bytes[i] = lhs->bytes[i];
    else
      res->bytes[i] = rhs->bytes[i];
  }
  return MAT_OK;
}

MatErr Mat_adds(Mat *res, const Mat *lhs, const int32_t rhs) {
  if (!res || !lhs || !rhs) {
    WARNING("Arguments of input can't be empty!");
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
    NOTION(
        "Overflow may occur with the rhs as %d, please make sure the pixel of "
        "result is within range of [0, 255]",
        rhs);
  return MAT_OK;
}

double bilinearInterpolation(const Mat *channel, double x, double y) {
  if (!channel || !channel->bytes) {
    WARNING("Channel can't be null!");
    return 0;
  }
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

  AffineMat inv = {0, 0, 0, 0, 0, 0};
  if (!m) {
    WARNING("Affine Mat can't b null!");
    return inv;
  }

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
    }
  }

  // 将临时矩阵拷贝回原矩阵
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
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
      u8mat->bytes[i * cols + j] = (uint8_t)(((mag) / (maxMag)) * 255);
    }
  }
}

MatErr Mat_conv2d(Mat *res, const Mat *basis, const Mat *kernel,
                  size_t stride_x, size_t stride_y, bool is_padding) {
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

          int64_t in_y = in_y_start + k_y - (is_padding ? k_center_y : 0);
          int64_t in_x = in_x_start + k_x - (is_padding ? k_center_x : 0);

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
          gmat.cdata[i * newWidth + j] = src->g.bytes[i * width + j] + 0.0 * I;
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

#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH 1024
#define MAX_CLR_MODE 100
#define ERR_INVALID_COMMAND 1
#define ERR_MISSING_ARGS 2
#define ERR_INVALID_FORMAT 3
#define ERR_FILE_NOT_FOUND 4
#define ERR_INVALID_PARAM 5
#define ERR_SIZE_MISMATCH 6
#define ERR_UNSUPPORTED_FORMAT 7
#define ERR_UNSUPPORTED_CONVERSION 8

typedef enum {
  CMD_UNKNOWN,
  CMD_OP,
  CMD_CONV,
  CMD_CVTCLR,
  CMD_FFT,
  CMD_SCALE,
  CMD_TRANS,
  CMD_FLIP,
  CMD_ROTATE,
  CMD_NOISE,
  CMD_ART,
  CMD_LINE,
  CMD_RECT,
  CMD_CIRCLE,
  CMD_CLIP,
  CMD_B64,
  CMD_INFO
} CommandType;

typedef struct {
  CommandType cmd;
  char src1[MAX_PATH];
  char src2[MAX_PATH];
  char dest[MAX_PATH];

  // Op command params
  OpRule rule;
  float w1;
  float w2;

  // conv params
  bool padding;

  // CVT color params
  char colorMode[MAX_CLR_MODE];

  // FFT params
  bool shift;

  // Scale params
  double scaleX;
  double scaleY;

  // Trans params
  int transX;
  int transY;

  // Rotate angle
  double angle;

  // Flip axis
  bool flipX;
  bool flipY;

  // Noise params
  float probability;

  // Art params
  int color;

  // Line, Rect, Circle params
  int x0, y0, x1, y1;
  int x, y, dx, dy;
  int rx, ry;
  double r;
} CommandParams;

// Function prototypes
void print_usage();
int parse_command(int argc, char *argv[], CommandParams *params);
int execute_command(CommandParams *params);
int check_files_exist(const char *files[], int count);
int parse_op_command(int argc, char *argv[], CommandParams *params);
int parse_conv_command(int argc, char *argv[], CommandParams *params);
int parse_cvtclr_command(int argc, char *argv[], CommandParams *params);
int parse_fft_command(int argc, char *argv[], CommandParams *params);
int parse_scale_command(int argc, char *argv[], CommandParams *params);
int parse_trans_command(int argc, char *argv[], CommandParams *params);
int parse_rotate_command(int argc, char *argv[], CommandParams *params);
int parse_flip_command(int argc, char *argv[], CommandParams *params);
int parse_noise_command(int argc, char *argv[], CommandParams *params);
int parse_art_command(int argc, char *argv[], CommandParams *params);
int parse_line_command(int argc, char *argv[], CommandParams *params);
int parse_rect_command(int argc, char *argv[], CommandParams *params);
int parse_circle_command(int argc, char *argv[], CommandParams *params);
int parse_clip_command(int argc, char *argv[], CommandParams *params);
int parse_b64_command(int argc, char *argv[], CommandParams *params);
int parse_info_command(int argc, char *argv[], CommandParams *params);
int parse_hex_color(const char *str);

// Image processing function prototypes (to be implemented elsewhere)
int image_merge(const char *src1, const char *src2, const char *dest,
                OpRule rule, float w1, float w2);
int image_conv(const char *src1, const char *src2, const char *dest,
                bool is_padding);
int image_convert_color(const char *src, const char *dest, const char* mode);
int image_fft(const char *src, const char *dest, bool shift);
int image_scale(const char *src, const char *dest, double scaleX, double scaleY);
int image_translate(const char *src, const char *dest, int tx, int ty);
int image_add_noise(const char *src, const char *dest, float probability);
int image_rotate(const char *src, const char *dest, double angle);
int image_flip(const char *src, const char *dest, bool flipX, bool flipY);
int image_to_ascii(const char *src,
                                                      int color);
int image_draw_line(const char *src, const char *dest, int color, int x0,
                    int y0, int x1, int y1);
int image_draw_rect(const char *src, const char *dest, int color, int x, int y,
                    int dx, int dy);
int image_draw_circle(const char *src, const char *dest, int color, double r,
                      int rx, int ry);
int image_clip(const char *src, const char *dest, int x, int y, int dx, int dy);
int image_to_base64(const char *src, const char *dest);
int image_info(const char *src);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage();
    return ERR_INVALID_COMMAND;
  }

  CommandParams params = {CMD_UNKNOWN};
  int result = parse_command(argc, argv, &params);

  if (result != 0) {
    return result;
  }

  return execute_command(&params);
}

void print_usage() {
  printf("CImageProcess - BMP Image Processing CLI\n");
  printf("Supported formats: 24-bit BMP (RGB, BGR, HSV, grayscale)\n\n");
  printf("Commands:\n");
  printf("  cimage op [src1.ext] + [src2.ext] -o [dest.ext] -r [rule] "
         "[--w1=xxx] [--w2=xxx]\n");
  printf("  cimage conv [src1.ext] x [src2.ext] -o [dest.ext] [--no-padding]\n");
  printf("  cimage cvtclr -i [src.ext] -o [dest.ext] -m [mode]\n");
  printf("  cimage fft -i [src.ext] -o [dest.ext] [--no-shift]\n");
  printf("  cimage scale -i [src.ext] -o [dest.ext] --sx=[scaleX] "
         "--sy=[scaleY]\n");
  printf("  cimage rotate -i [src.ext] -o [dest.ext] -a [angle]\n");
  printf("  cimage flip -i [src.ext] -o [dest.ext] --x --y");
  printf("  cimage trans -i [src.ext] -o [dest.ext] --tx=[transX] "
         "--ty=[transY]\n");
  printf("  cimage noise -i [src.ext] -o [dest.ext] -p [probability]\n");
  printf("  cimage art -i [src.ext] -c [color]\n");
  printf("  cimage line -i [src.ext] -o [dest.ext] -c [color] --x0=[xxx] "
         "--y0=[xxx] --x1=[xxx] --y1=[xxx]\n");
  printf("  cimage rect -i [src.ext] -o [dest.ext] -c [color] --x=[xxx] "
         "--y=[xxx] --dx=[xxx] --dy=[xxx]\n");
  printf("  cimage circle -i [src.ext] -o [dest.ext] -c [color] --r=[xxx] "
         "--rx=[xxx] --ry=[xxx]\n");
  printf("  cimage clip -i [src.ext] -o [dest.ext] --x=[xxx] --y=[xxx] "
         "--dx=[xxx] --dy=[xxx]\n");
  printf("  cimage b64 -i [src.ext] -o [dest.ext]\n");
  printf("  cimage info -i [src.ext]\n");
  printf("\nFor detailed help on each command, use: cimage [command] --help\n");
}

int parse_command(int argc, char *argv[], CommandParams *params) {
  if (argc < 2) {
    printf("Error: No command specified\n");
    print_usage();
    return ERR_INVALID_COMMAND;
  }

  if (strcmp(argv[1], "op") == 0) {
    params->cmd = CMD_OP;
    return parse_op_command(argc, argv, params);
  } else if (strcmp(argv[1], "conv") == 0) {
    params->cmd = CMD_CONV;
    return parse_conv_command(argc, argv, params);
  } else if (strcmp(argv[1], "cvtclr") == 0) {
    params->cmd = CMD_CVTCLR;
    return parse_cvtclr_command(argc, argv, params);
  } else if (strcmp(argv[1], "fft") == 0) {
    params->cmd = CMD_FFT;
    return parse_fft_command(argc, argv, params);
  } else if (strcmp(argv[1], "scale") == 0) {
    params->cmd = CMD_SCALE;
    return parse_scale_command(argc, argv, params);
  } else if (strcmp(argv[1], "trans") == 0) {
    params->cmd = CMD_TRANS;
    return parse_trans_command(argc, argv, params);
  } else if (strcmp(argv[1], "rotate") == 0) {
    params->cmd = CMD_ROTATE;
    return parse_rotate_command(argc, argv, params);
  } else if (strcmp(argv[1], "flip") == 0) {
      params->cmd = CMD_FLIP;
      return parse_flip_command(argc, argv, params);
  } else if (strcmp(argv[1], "noise") == 0) {
    params->cmd = CMD_NOISE;
    return parse_noise_command(argc, argv, params);
  } else if (strcmp(argv[1], "art") == 0) {
    params->cmd = CMD_ART;
    return parse_art_command(argc, argv, params);
  } else if (strcmp(argv[1], "line") == 0) {
    params->cmd = CMD_LINE;
    return parse_line_command(argc, argv, params);
  } else if (strcmp(argv[1], "rect") == 0) {
    params->cmd = CMD_RECT;
    return parse_rect_command(argc, argv, params);
  } else if (strcmp(argv[1], "circle") == 0) {
    params->cmd = CMD_CIRCLE;
    return parse_circle_command(argc, argv, params);
  } else if (strcmp(argv[1], "clip") == 0) {
    params->cmd = CMD_CLIP;
    return parse_clip_command(argc, argv, params);
  } else if (strcmp(argv[1], "b64") == 0) {
    params->cmd = CMD_B64;
    return parse_b64_command(argc, argv, params);
  } else if (strcmp(argv[1], "info") == 0) {
    params->cmd = CMD_INFO;
    return parse_info_command(argc, argv, params);
  } else {
    printf("Error: Unknown command '%s'\n", argv[1]);
    print_usage();
    return ERR_INVALID_COMMAND;
  }
}

int parse_op_command(int argc, char *argv[], CommandParams *params) {
  // Initialize defaults
  params->rule = OP_RULE_AVG;
  params->w1 = 0.5f;
  params->w2 = 0.5f;

  // Check for minimum required arguments
  if (argc < 6) {
    printf("Error: Insufficient arguments for 'op' command\n");
    printf("Usage: cimage op [src1.ext] + [src2.ext] -o [dest.ext] -r [rule] "
           "[--w1=xxx] [--w2=xxx]\n");
    return ERR_MISSING_ARGS;
  }

  // Parse src1 + src2 format
  if (strcmp(argv[3], "+") != 0) {
    printf("Error: Expected '+' operator between source files\n");
    return ERR_INVALID_FORMAT;
  }

  strncpy(params->src1, argv[2], MAX_PATH - 1);
  strncpy(params->src2, argv[4], MAX_PATH - 1);

  // Check if files exist
  const char *files[] = {params->src1, params->src2};
  if (check_files_exist(files, 2) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  // Parse optional arguments
  int i = 5;
  bool output_set = false;

  while (i < argc) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      strncpy(params->dest, argv[i + 1], MAX_PATH - 1);
      output_set = true;
      i += 2;
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      if (strcmp(argv[i + 1], "max") == 0) {
        params->rule = OP_RULE_MAX;
      } else if (strcmp(argv[i + 1], "avg") == 0) {
        params->rule = OP_RULE_AVG;
      } else if (strcmp(argv[i + 1], "weight") == 0) {
        params->rule = OP_RULE_WEIGHT;
      } else if (strcmp(argv[i + 1], "gauss") == 0) {
        params->rule = OP_RULE_GAUSS;
      } else {
        printf("Error: Unknown rule '%s'. Valid rules are: max, avg, weight, "
               "gauss\n",
               argv[i + 1]);
        return ERR_INVALID_PARAM;
      }
      i += 2;
    } else if (strncmp(argv[i], "--w1=", 5) == 0) {
      char *value = argv[i] + 5;
      params->w1 = atof(value);
      if (params->w1 < 0.0f || params->w1 >= 1.0f) {
        printf("Error: Weight w1 must be in range [0, 1)\n");
        return ERR_INVALID_PARAM;
      }
      i++;
    } else if (strncmp(argv[i], "--w2=", 5) == 0) {
      char *value = argv[i] + 5;
      params->w2 = atof(value);
      if (params->w2 < 0.0f || params->w2 >= 1.0f) {
        printf("Error: Weight w2 must be in range [0, 1)\n");
        return ERR_INVALID_PARAM;
      }
      i++;
    } else {
      printf("Error: Unknown option '%s' for 'op' command\n", argv[i]);
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if weight is required for weight rule
  if (params->rule == OP_RULE_WEIGHT && (params->w1 + params->w2 != 1.0f)) {
    printf("Warning: Weights do not sum to 1.0. Normalizing weights.\n");
    float sum = params->w1 + params->w2;
    if (sum > 0) {
      params->w1 /= sum;
      params->w2 /= sum;
    } else {
      params->w1 = params->w2 = 0.5f;
    }
  }

  return 0;
}

int parse_conv_command(int argc, char *argv[], CommandParams *params) {
  // Initialize defaults
  params->padding = true;

  // Check for minimum required arguments
  if (argc < 6) {
    printf("Error: Insufficient arguments for 'conv' command\n");
    printf("Usage: cimage conv [src1.ext] x [src2.ext] -o [dest.ext] [--no-padding]\n");
    return ERR_MISSING_ARGS;
  }

  // Parse src1 + src2 format
  if (strcmp(argv[3], "x") != 0) {
    printf("Error: Expected 'x' operator between source files\n");
    return ERR_INVALID_FORMAT;
  }

  strncpy(params->src1, argv[2], MAX_PATH - 1);
  strncpy(params->src2, argv[4], MAX_PATH - 1);

  // Check if files exist
  const char *files[] = {params->src1, params->src2};
  if (check_files_exist(files, 2) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  // Parse optional arguments
  int i = 5;
  bool output_set = false;

  while (i < argc) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      strncpy(params->dest, argv[i + 1], MAX_PATH - 1);
      output_set = true;
      i += 2;
    } else if (strncmp(argv[i], "--no-padding", 12) == 0) {
      params->padding = false;
      i++;
    } else {
      printf("Error: Unknown option '%s' for 'op' command\n", argv[i]);
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  return 0;
}

int parse_cvtclr_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, mode_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:m:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'm':
      strncpy(params->colorMode, optarg, MAX_CLR_MODE - 1);
      mode_set = true;
      break;
    case 'h':
      printf("Usage: cimage cvtclr -i [src.ext] -o [dest.ext] -m [mode]\n");
      printf("Modes: rgb2bgr, rgb2hsv, rgb2gray, bgr2rgb, bgr2hsv, bgr2gray, "
             "hsv2rgb, hsv2bgr, hsv2gray\n");
      return 1;
    default:
      printf("Error: Unknown option for 'cvtclr' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!mode_set) {
    printf("Error: Color conversion mode (-m) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_fft_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->shift = true;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;

  static struct option long_options[] = {{"no-shift", no_argument, 0, 'n'},
                                         {"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:nh", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'n':
      params->shift = false;
      break;
    case 'h':
      printf("Usage: cimage fft -i [src.ext] -o [dest.ext] [--no-shift]\n");
      printf("Performs FFT on the source image and saves the spectrum\n");
      printf("  --no-shift: Don't shift the zero frequency to the center\n");
      return 1;
    default:
      printf("Error: Unknown option for 'fft' command\n");  
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_scale_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->scaleX = params->scaleY = 1.0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, scale_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"sx", required_argument, 0, '1'},
                                         {"sy", required_argument, 0, '2'},
      {0, 0, 0, 0},
};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) !=
         -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '1':
      params->scaleX = atof(optarg);
      scale_set = true;
      break;
    case '2':
      params->scaleY = atof(optarg);
      scale_set = true;
      break;
    case 'h':
      printf("Usage: cimage scale -i [src.ext] -o [dest.ext] --sx=[scaleX] "
             "--sy=[scaleY] -m [mode]\n");
      printf("Scales the image by the given factor\n");
      printf("  -s: Scale factor (positive number)\n");
      return 1;
    default:
      printf("Error: Unknown option for 'scale' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!scale_set) {
    printf("Error: Scale factor (--sx or --sy) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_trans_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->transX = 0;
  params->transY = 0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, trans_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"tx", required_argument, 0, '1'},
                                         {"ty", required_argument, 0, '2'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '1':
      params->transX = atoi(optarg);
      trans_set = true;
      break;
    case '2':
      params->transY = atoi(optarg);
      trans_set = true;
      break;
    case 'h':
      printf("Usage: cimage trans -i [src.ext] -o [dest.ext] --tx=[transX] --ty="
             "[transY]\n");
      printf("Translates the image by the given offsets\n");
      return 1;
    default:
      printf("Error: Unknown option for 'trans' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!trans_set) {
    printf("Error: Neither X nor Y translation values (--tx or --ty) has been specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_rotate_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->angle = 0.0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, angle_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:a:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'a':
      params->angle = atof(optarg);
      angle_set = true;
      break;
    case 'h':
      printf(
          "Usage: cimage rotate -i [src.ext] -o [dest.ext] -a [angle]\n");
      printf("Rotate the image at the center by the given angle\n");
      return 1;
    default:
      printf("Error: Unknown option for 'rotate' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!angle_set) {
    printf("Error: The angle (-a) must be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_flip_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->flipX = false;
  params->flipY = false;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"x", no_argument, 0, '1'},
                                         {"y", no_argument, 0, '2'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '1':
      params->flipX = true;
      break;
    case '2':
      params->flipY = true;
      break;
    case 'h':
      printf(
          "Usage: cimage flip -i [src.ext] -o [dest.ext] --x --y\n");
      printf("Flip the image by the given directions\n");
      return 1;
    default:
      printf("Error: Unknown option for 'flip' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_noise_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->probability = 0.05f;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, prob_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:p:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'p':
      params->probability = atof(optarg);
      if (params->probability < 0 || params->probability > 1) {
        printf("Error: Probability must be in range [0, 1]\n");
        return ERR_INVALID_PARAM;
      }
      prob_set = true;
      break;
    case 'h':
      printf(
          "Usage: cimage noise -i [src.ext] -o [dest.ext] -p [probability]\n");
      printf("Adds salt and pepper noise to the image\n");
      printf("  -p: Noise probability (0.0 to 1.0)\n");
      return 1;
    default:
      printf("Error: Unknown option for 'noise' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!prob_set) {
    printf("Error: Noise probability (-p) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_art_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0xFFFFFF; // White

  // Parse with getopt
  int opt;
  bool input_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case 'h':
      printf("Usage: cimage art -i [src.ext] -c [color]\n");
      printf("Converts image to ASCII art and prints to terminal\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      return 1;
    default:
      printf("Error: Unknown option for 'art' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_line_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0x000000; // Black

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool x0_set = false, y0_set = false, x1_set = false, y1_set = false;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},     {"x0", required_argument, 0, '0'},
      {"y0", required_argument, 0, '1'}, {"x1", required_argument, 0, '2'},
      {"y1", required_argument, 0, '3'}, {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case '0': // x0
      params->x0 = atoi(optarg);
      x0_set = true;
      break;
    case '1': // y0
      params->y0 = atoi(optarg);
      y0_set = true;
      break;
    case '2': // x1
      params->x1 = atoi(optarg);
      x1_set = true;
      break;
    case '3': // y1
      params->y1 = atoi(optarg);
      y1_set = true;
      break;
    case 'h':
      printf("Usage: cimage line -i [src.ext] -o [dest.ext] -c [color] "
             "--x0=[xxx] --y0=[xxx] --x1=[xxx] --y1=[xxx]\n");
      printf("Draws a line on the image\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      printf("  --x0, --y0: Starting point coordinates\n");
      printf("  --x1, --y1: Ending point coordinates\n");
      return 1;
    default:
      printf("Error: Unknown option for 'line' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!x0_set || !y0_set || !x1_set || !y1_set) {
    printf("Error: Line coordinates (--x0, --y0, --x1, --y1) must all be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_rect_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0x000000; // Black

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool x_set = false, y_set = false, dx_set = false, dy_set = false;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},     {"x", required_argument, 0, '0'},
      {"y", required_argument, 0, '1'},  {"dx", required_argument, 0, '2'},
      {"dy", required_argument, 0, '3'}, {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case '0': // x
      params->x = atoi(optarg);
      x_set = true;
      break;
    case '1': // y
      params->y = atoi(optarg);
      y_set = true;
      break;
    case '2': // dx
      params->dx = atoi(optarg);
      dx_set = true;
      break;
    case '3': // dy
      params->dy = atoi(optarg);
      dy_set = true;
      break;
    case 'h':
      printf("Usage: cimage rect -i [src.ext] -o [dest.ext] -c [color] "
             "--x=[xxx] --y=[xxx] --dx=[xxx] --dy=[xxx]\n");
      printf("Draws a rectangle on the image\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      printf("  --x, --y: Top-left corner coordinates\n");
      printf("  --dx, --dy: Width and height of the rectangle\n");
      return 1;
    default:
      printf("Error: Unknown option for 'rect' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!x_set || !y_set || !dx_set || !dy_set) {
    printf("Error: Rectangle parameters (--x, --y, --dx, --dy) must all be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_circle_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0x000000; // Black
  params->r = 0;
  params->rx = 0;
  params->ry = 0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool r_set = false, rx_set = false, ry_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"r", required_argument, 0, '0'},
                                         {"rx", required_argument, 0, '1'},
                                         {"ry", required_argument, 0, '2'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case '0': // r
      params->r = atof(optarg);
      r_set = true;
      break;
    case '1': // rx
      params->rx = atoi(optarg);
      rx_set = true;
      break;
    case '2': // ry
      params->ry = atoi(optarg);
      ry_set = true;
      break;
    case 'h':
      printf("Usage: cimage circle -i [src.ext] -o [dest.ext] -c [color] "
             "--r=[radius] --rx=[centerX] --ry=[centerY]\n");
      printf("Draws a circle on the image\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      printf("  --r: Radius of the circle\n");
      printf("  --rx, --ry: Center coordinates\n");
      return 1;
    default:
      printf("Error: Unknown option for 'circle' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!r_set || !rx_set || !ry_set) {
    printf(
        "Error: Circle parameters (--r, --rx, --ry) must all be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (params->r <= 0) {
    printf("Error: Radius must be positive\n");
    return ERR_INVALID_PARAM;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_clip_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool x_set = false, y_set = false, dx_set = false, dy_set = false;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},     {"x", required_argument, 0, '0'},
      {"y", required_argument, 0, '1'},  {"dx", required_argument, 0, '2'},
      {"dy", required_argument, 0, '3'}, {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '0': // x
      params->x = atoi(optarg);
      x_set = true;
      break;
    case '1': // y
      params->y = atoi(optarg);
      y_set = true;
      break;
    case '2': // dx
      params->dx = atoi(optarg);
      dx_set = true;
      break;
    case '3': // dy
      params->dy = atoi(optarg);
      dy_set = true;
      break;
    case 'h':
      printf("Usage: cimage clip -i [src.ext] -o [dest.ext] --x=[xxx] "
             "--y=[xxx] --dx=[xxx] --dy=[xxx]\n");
      printf("Clips a rectangular region from the image\n");
      printf("  --x, --y: Top-left corner coordinates\n");
      printf("  --dx, --dy: Width and height of the region\n");
      return 1;
    default:
      printf("Error: Unknown option for 'clip' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!x_set || !y_set || !dx_set || !dy_set) {
    printf("Error: Clipping parameters (--x, --y, --dx, --dy) must all be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  if (params->dx <= 0 || params->dy <= 0) {
    printf("Error: Width and height must be positive\n");
    return ERR_INVALID_PARAM;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_b64_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'h':
      printf("Usage: cimage b64 -i [src.ext] -o [dest.ext]\n");
      printf("Encodes the image to base64 and saves to the output file\n");
      return 1;
    default:
      printf("Error: Unknown option for 'b64' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_info_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'h':
      printf("Usage: cimage info -i [src.ext]\n");
      printf("Displays information about the image\n");
      return 1;
    default:
      printf("Error: Unknown option for 'info' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int execute_command(CommandParams *params) {
  switch (params->cmd) {
  case CMD_OP:
    return image_merge(params->src1, params->src2, params->dest, params->rule,
                       params->w1, params->w2);
  case CMD_CONV:
    return image_conv(params->src1, params->src2, params->dest,params->padding);
  case CMD_CVTCLR:
    return image_convert_color(params->src1, params->dest, params->colorMode);
  case CMD_FFT:
    return image_fft(params->src1, params->dest, params->shift);
  case CMD_SCALE:
    return image_scale(params->src1, params->dest, params->scaleX,
                       params->scaleY);
  case CMD_TRANS:
    return image_translate(params->src1, params->dest, params->transX,
                           params->transY);
  case CMD_NOISE:
    return image_add_noise(params->src1, params->dest, params->probability);
  case CMD_ROTATE:
    return image_rotate(params->src1, params->dest, params->angle);
  case CMD_FLIP:
    return image_flip(params->src1, params->dest, params->flipX, params->flipY);
  case CMD_ART:
    return image_to_ascii(params->src1, params->color);
  case CMD_LINE:
    return image_draw_line(params->src1, params->dest, params->color,
                           params->x0, params->y0, params->x1, params->y1);
  case CMD_RECT:
    return image_draw_rect(params->src1, params->dest, params->color, params->x,
                           params->y, params->dx, params->dy);
  case CMD_CIRCLE:
    return image_draw_circle(params->src1, params->dest, params->color,
                             params->r, params->rx, params->ry);
  case CMD_CLIP:
    return image_clip(params->src1, params->dest, params->x, params->y,
                      params->dx, params->dy);
  case CMD_B64:
    return image_to_base64(params->src1, params->dest);
  case CMD_INFO:
    return image_info(params->src1);
  default:
    printf("Error: Unknown command\n");
    return ERR_INVALID_COMMAND;
  }
}

int check_files_exist(const char *files[], int count) {
  for (int i = 0; i < count; i++) {
    if (access(files[i], F_OK) != 0) {
      printf("Error: File '%s' not found\n", files[i]);
      return ERR_FILE_NOT_FOUND;
    }
  }
  return 0;
}

int parse_hex_color(const char *str) {
  if (!str) {
    printf("Error: Color string is NULL\n");
    return -1;
  }

  // Skip "0x" or "#" prefix if present
  if (strncmp(str, "0x", 2) == 0) {
    str += 2;
  } else if (str[0] == '#') {
    str++;
  }

  // Check if the remaining string is valid
  size_t len = strlen(str);
  if (len != 6) {
    printf("Error: Color must be a 6-digit hexadecimal value (e.g., FF0000 for "
           "red)\n");
    return -1;
  }

  // Check if all characters are hexadecimal
  for (size_t i = 0; i < len; i++) {
    if (!isxdigit((unsigned char)str[i])) {
      printf("Error: Invalid hexadecimal character in color value\n");
      return -1;
    }
  }

  int color = (int)strtol(str, NULL, 16);
  return color;
}

// Stub implementations for the image processing functions
int image_merge(const char *src1, const char *src2, const char *dest,
                OpRule rule, float w1, float w2) {
  NOTION("Merging images %s and %s to %s using rule %d\n", src1, src2, dest,
         rule);
  
  FILE *fp1 = fopen(src1, "rb");
  FILE *fp2 = fopen(src2, "rb");
  if (fp1 == NULL) {
      WARNING("Error opening file: %s", src1);
      return 1;
      fclose(fp2);
  }
  if (fp2 == NULL) {
    WARNING("Error opening file: %s", src2);
    return 1;
    fclose(fp1);
  }
  IStream is1 = Stream(is1);
  IStream is2 = Stream(is2);
  IStream_fromFileStream(&is1, fp1);
  IStream_fromFileStream(&is2, fp2);
  fclose(fp1);
  fclose(fp2);

  BMPImage lhs = BMPImage_0();
  BMPImage rhs = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&lhs, &is1) != 0 ||
      BMPImage_decode(&rhs, &is2) != 0) {
    WARNING("Failed to decode bmp image!");
    IStream_close(&is1);
    IStream_close(&is2);
    return 1;
  }

  Confuse_args args = {rule == OP_RULE_WEIGHT, w1, w2};
  int ret = BMPImage_confuse(&res, &lhs, &rhs, rule, args);
  if (ret != 0) {
    WARNING("Failed to confuse image!");
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  FILE *fp3 = fopen(dest, "wb");
  if (fp3 == NULL) {
    WARNING("Error opening file: %s", src2);
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  OStream os = Stream(os);
  BMPImage_encode(&res, &os);
  OStream_toFileStream(&os, fp3);
  fclose(fp3);

  BMPImage_release(&res);
  BMPImage_release(&lhs);
  BMPImage_release(&rhs);

  return 0;
}

int image_conv(const char *src1, const char *src2, const char *dest,
                bool is_padding) {
  NOTION("Convolving images %s and %s to %s", src1, src2, dest);
  
  FILE *fp1 = fopen(src1, "rb");
  FILE *fp2 = fopen(src2, "rb");
  if (fp1 == NULL) {
      WARNING("Error opening file: %s", src1);
      return 1;
      fclose(fp2);
  }
  if (fp2 == NULL) {
    WARNING("Error opening file: %s", src2);
    return 1;
    fclose(fp1);
  }
  IStream is1 = Stream(is1);
  IStream is2 = Stream(is2);
  IStream_fromFileStream(&is1, fp1);
  IStream_fromFileStream(&is2, fp2);
  fclose(fp1);
  fclose(fp2);

  BMPImage lhs = BMPImage_0();
  BMPImage rhs = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&lhs, &is1) != 0 ||
      BMPImage_decode(&rhs, &is2) != 0) {
    WARNING("Failed to decode bmp image!");
    IStream_close(&is1);
    IStream_close(&is2);
    return 1;
  }

  int ret = BMPImage_conv(&res, &lhs, &rhs, 1, 1, is_padding);
  if (ret != 0) {
    WARNING("Failed to convolving image!");
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  FILE *fp3 = fopen(dest, "wb");
  if (fp3 == NULL) {
    WARNING("Error opening file: %s", src2);
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  OStream os = Stream(os);
  BMPImage_encode(&res, &os);
  OStream_toFileStream(&os, fp3);
  fclose(fp3);

  BMPImage_release(&res);
  BMPImage_release(&lhs);
  BMPImage_release(&rhs);
  return 0;
}

int image_convert_color(const char *src, const char *dest, const char* mode) {
  NOTION("Converting color mode of %s to %s using mode '%s'\n", src, dest, mode);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_cvtclr(&bimg, mode);
  if (ret != 0) {
    WARNING("Failed to convert color mode of the image!");
    return ret; 
  }

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_fft(const char *src, const char *dest, bool shift) {
  NOTION("Performing FFT on %s to %s with shift=%d\n", src, dest, shift);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);

  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  BMPImage res = BMPImage_0();
  BMPImage_fft_transform(&res, &bimg, shift);
  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_scale(const char *src, const char *dest, double scaleX, double scaleY)    {
  NOTION("Scaling %s to %s by factor X as %.2f and factor Y as %.2f\n", src, dest, scaleX,
         scaleY);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_scale(&res, &bimg, scaleX, scaleY);
  if (ret != 0) {
    WARNING("Failed to scale the image!");
    return ret;
  }
  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_translate(const char *src, const char *dest, int tx, int ty) {
  NOTION("Translating %s to %s by (%d, %d)\n", src, dest, tx, ty);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_translate(&res, &bimg, tx, ty);
  if (ret != 0) {
    WARNING("Failed to scale the image!");
    return ret;
  }
    
  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_rotate(const char *src, const char *dest, double angle) {
  NOTION("Rotating %s to %s by %lf°\n", src, dest, angle);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_rotate(&res, &bimg, angle);
  if (ret != 0) {
    WARNING("Failed to rotate the image!");
    return ret;
  }  

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_flip(const char *src, const char *dest, bool flipX, bool flipY) {
  NOTION("Flipping %s to %s\n", src, dest);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_flip(&res, &bimg, flipX, flipY);
  if (ret != 0) {
    WARNING("Failed to flip the image!");
    return ret;
  } 

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_add_noise(const char *src, const char *dest, float probability) {
  NOTION("Adding noise to %s and saving to %s with probability %.2f\n", src,
         dest, probability);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_noise_pepper(&bimg, probability);
  if (ret != 0) {
    WARNING("Failed to noise the image!");
    return ret;
  }   

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_to_ascii(const char *src, int color) {
  NOTION("Converting %s to ASCII art with color 0x%06X\n", src,
         color);
  

  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);

  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  char *ascii_str = BMP_asciiart(&bimg);
  if (ascii_str == NULL) {
    WARNING("Failed to convert image to ASCII art!");
    return 1;
  }
  int ansi_clr = hex_to_ansi16(color);
  if (ansi_clr < 8) {
    fprintf(stderr, "\033[%dm", 30 + ansi_clr);
  } else {
    fprintf(stderr, "\033[%dm", 82 + ansi_clr);
  }
  printf("%s\n", ascii_str);
  fprintf(stderr, "\033[0m");
  free(ascii_str);
  BMPImage_release(&bimg);

  return 0;
}

int image_draw_line(const char *src, const char *dest, int color, int x0,
                    int y0, int x1, int y1) {
  NOTION(
      "Drawing line on %s to %s with color 0x%06X from (%d, %d) to (%d, %d)\n",
      src, dest, color, x0, y0, x1, y1);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_line(&bimg, (Line){x0, y0, x1, y1, 5.0}, fromHex(color));
  if (ret != 0) {
    WARNING("Failed to draw line on the image!");
    return ret;
  }   

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_draw_rect(const char *src, const char *dest, int color, int x, int y,
                    int dx, int dy) {
  NOTION("Drawing rectangle on %s to %s with color 0x%06X at (%d, %d) with "
         "size %dx%d\n",
         src, dest, color, x, y, dx, dy);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_rect(&bimg, (Rect){x, y, dx, dy}, fromHex(color));
  if (ret != 0) {
    WARNING("Failed to draw rectangle on the image!");
    return ret;
  }  

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_draw_circle(const char *src, const char *dest, int color, double r,
                      int rx, int ry) {
  NOTION("Drawing circle on %s to %s with color 0x%06X, radius %.2f at center "
         "(%d, %d)\n",
         src, dest, color, r, rx, ry);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_circle(&bimg, (Circle){r, rx, ry}, fromHex(color));
  if (ret != 0) {
    WARNING("Failed to draw circle on the image!");
    return ret;
  }

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_clip(const char *src, const char *dest, int x, int y, int dx,
               int dy) {
  NOTION("Clipping %s to %s from (%d, %d) with size %dx%d\n", src, dest, x, y,
         dx, dy);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_clip(&bimg, (Rect){x, y, dx, dy});
  if (ret != 0) {
    WARNING("Failed to clip the image!");
    return ret;
  }

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_to_base64(const char *src, const char *dest) {
  NOTION("Converting %s to base64 and saving to %s\n", src, dest);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }

  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  Stream_toBase64(&istream);

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    NOTION("Error opening file %s", dest);
    return 1;
  }

  OStream_toFileStream(&istream, fp);

  fclose(fp);
  return 0;
}

int image_info(const char *src) {
  NOTION("Displaying information for image %s\n", src);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);
  BMPImage_release(&bimg);
  return 0;
}