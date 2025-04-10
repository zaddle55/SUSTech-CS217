#ifndef COLOR_H_
#define COLOR_H_
#define PACKED __attribute__((__packed__))

#include <stdint.h>
#include <float.h>
#include <math.h>
#define MAX(a, b, c) ({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    typeof(c) _c = (c); \
    _a > _b ? (_a > _c ? _a : _c) : (_b > _c ? _b : _c); \
})
#define MIN(a, b, c) ({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    typeof(c) _c = (c); \
    _a < _b ? (_a < _c ? _a : _c) : (_b < _c ? _b : _c); \
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

#define rgb(__c, __r, __g, __b) \
    __c.rgb[2] = __r; \
    __c.rgb[1] = __g; \
    __c.rgb[0] = __b; \

#define hsv(__c, __h, __s, __v) \
    __c.hsv[2] = __h; \
    __c.hsv[1] = __s; \
    __c.hsv[0] = __v; \

#define gray(__c, __gray) \
    __c.gray[2] = __c.gray[1] = __c.gray[0] = __gray;

color cvtclr_rgb2hsv(color clr);
color cvtclr_hsv2rgb(color clr);
color cvtclr_rgb2gray(color clr);
color cvtclr_hsv2gray(color clr);
color fromHex(uint32_t hex);
int hex_to_ansi16(int hex_color);

#endif