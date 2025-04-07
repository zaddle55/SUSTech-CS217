#ifndef BMP_H_
#define BMP_H_
#include "pmat.h"
#include "stream.h"
#include "color.h"

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

#define ABS(__x) (((__x) >= 0)? (__x) : -(__x))

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

#define STD_BFHeader(__w, __h, __bpp) (BFHeader) { \
    .bf_type = STD_IMAGE_TYPE, \
    .bf_size = (__w) * (__h) * (__bpp) + STD_HEADER_LEN, \
    .bf_reserve = STD_RESERVE, \
    .bf_offset = STD_HEADER_LEN \
}

#define STD_BIHeader(__w, __h, __bpp, __x_resl, __y_resl) (BIHeader) { \
    .bi_size = STD_BIHEADER_LEN, \
    .bi_width = (__w), \
    .bi_height = (__h), \
    .bi_planes = STD_PLANES, \
    .bi_bitcnt = (__bpp) * 8, \
    .bi_compress = STD_COMPRESS_TYPE, \
    .bi_size_image = (__w) * (__h) * (__bpp), \
    .pixels_per_meter_h = (__x_resl), \
    .pixels_per_meter_v = (__y_resl), \
    .bi_clr_used = STD_CLR_USED, \
    .bi_clr_important = STD_IMPORTANT_CLR \
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

#define BMPImage() { \
    .psize = 0, \
    .r = Mat(0, 0), \
    .g = Mat(0, 0), \
    .b = Mat(0, 0), \
    .a = Mat(0, 0) \
}
int BMPImage_decode(BMPImage* bimg, IStream* istream);
void BMPImage_dbg(const BMPImage *bimg, FILE *output);
int BMPImage_resize(BMPImage *bimg, const word width, const word height);
int BMPImage_encode(BMPImage* bimg, OStream* ostream);
void BMPImage_release(BMPImage* bimg);
int BMPImage_rotate(BMPImage *dst, const BMPImage *src, double angle);
int BMPImage_scale(BMPImage *dst, const BMPImage *src, 
                double scaleX, double scaleY);
int BMPImage_translate(BMPImage *dst, const BMPImage *src,
                    int deltaX, int deltaY);
int BMPImage_flip(BMPImage *dst, const BMPImage *src, bool flipX, bool flipY);
int BMPImage_fft_transform(BMPImage *dst, const BMPImage *src, bool shift);

    int __BMPImage_cvtclr_rgb2bgr(BMPImage *bimg);
int __BMPImage_cvtclr_rgb2hsv(BMPImage *bimg);
int __BMPImage_cvtclr_rgb2gray(BMPImage *bimg);
int __BMPImage_cvtclr_hsv2rgb(BMPImage *bimg);
int __BMPImage_cvtclr_hsv2gray(BMPImage *bimg);

#endif