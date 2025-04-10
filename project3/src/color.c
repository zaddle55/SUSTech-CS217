#include "../inc/color.h"

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
        hn = 60 * (((bn - rn) / delta) + 2); // 修正公式
    } else { // cmax == bn
        hn = 60 * (((rn - gn) / delta) + 4); // 修正公式
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