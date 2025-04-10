#include "../inc/bmp.h"
#include "../inc/test.h"

static const char* bmp_path_1 = "./img/test1.bmp";
static const char *bmp_path_2 = "./img/test2.bmp";
static const char *bmp_path_3 = "./img/test3.bmp";
static const char *bmp_path_4 = "./img/test4.bmp";
static const char *bmp_path_5 = "./img/test5.bmp";
static const char *bmp_path_6 = "./img/test6.bmp";

// TEST(bmp_conv) {
//   FILE *fp = fopen(bmp_path_1, "rb");
//   if (fp == NULL) {
//     perror("Error opening file\n");
//     return;
//   }
//   IStream istream = Stream(istream);
//   IStream_fromFileStream(&istream, fp);
//   BMPImage bimg = BMPImage_0();
//   BMPImage res = BMPImage_0();
//   if (BMPImage_decode(&bimg, &istream) != 0) {
//     perror("Failed to decode bmp image");
//     IStream_close(&istream);
//     return;
//   }
//   fclose(fp);
//   BMPImage kernel = BMPImage_0();
//   // NOTION("Got here");
//   IStream_close(&istream);
//   IStream kistream = Stream(kistream);
//   fp = fopen(bmp_path_6, "rb");
//   if (fp == NULL) {
//     perror("Error opening file\n");
//     return;
//   }
//   IStream_fromFileStream(&kistream, fp);
//   if (BMPImage_decode(&kernel, &kistream) != 0) {
//     perror("Failed to decode bmp image");
//     IStream_close(&kistream);
//     return;
//   }
//   BMPImage_dbg(&bimg, stdout);
//   BMPImage_conv(&res, &bimg, &kernel, 1, 1, true);
//   BMPImage_dbg(&res, stdout);
//   fclose(fp);

//   OStream ostream = Stream(ostream);
//   if (BMPImage_encode(&res, &ostream) != 0) {
//     perror("Failed to encode bmp image");
//     OStream_close(&ostream);
//     BMPImage_release(&bimg);
//     BMPImage_release(&res);
//     return;
//   }

//   fp = fopen("out16.bmp", "wb");
//   if (fp == NULL) {
//     perror("Error opening file\n");
//     return;
//   }
//   OStream_toFileStream(&ostream, fp);
//   OStream_close(&ostream);
//   BMPImage_release(&bimg);
//   BMPImage_release(&res);
//   fclose(fp);
// }

TEST(basic_decode)
{
    FILE* fp = fopen(bmp_path_2, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    BMPImage_release(&bimg);
}

TEST(basic_encode)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out1.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
}

TEST(cvtclr_rgb2bgr)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    __BMPImage_cvtclr_rgb2bgr(&bimg);
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out2.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
}

TEST(cvtclr_rgb2hsv)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    __BMPImage_cvtclr_rgb2hsv(&bimg);
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out3.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
    color clr;
    rgb(clr, 0xff, 0x00, 0xff);
    printf("R: %u, G: %u, B: %u\n",clr.rgb[2],clr.rgb[1], clr.rgb[0]);
    clr = cvtclr_rgb2hsv(clr);
    printf("H: %u, S: %u, V: %u\n",clr.hsv[2],clr.hsv[1], clr.hsv[0]);
}

TEST(cvtclr_rgb2gray)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    __BMPImage_cvtclr_rgb2gray(&bimg);
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out4.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
    color clr;
    rgb(clr, 0xff, 0x00, 0xff);
    printf("R: %u, G: %u, B: %u\n",clr.rgb[2],clr.rgb[1], clr.rgb[0]);
    clr = cvtclr_rgb2gray(clr);
    printf("Gray: %u\n",clr.gray[2]);
}

TEST(cvtclr_hsv2rgb)
{
    FILE* fp = fopen("out3.bmp", "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    __BMPImage_cvtclr_hsv2rgb(&bimg);
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out5.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
    color clr;
    hsv(clr, 1, 0x00, 0xff);
    printf("H: %u, S: %u, V: %u\n",clr.hsv[2],clr.hsv[1], clr.hsv[0]);
    clr = cvtclr_hsv2rgb(clr);
    printf("R: %u, G: %u, B: %u\n",clr.rgb[2],clr.rgb[1], clr.rgb[0]);
}

TEST(cvtclr_hsv2gray)
{
    FILE* fp = fopen("out3.bmp", "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    __BMPImage_cvtclr_hsv2gray(&bimg);
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out6.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
}

TEST(bmp_rotate)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    BMPImage res = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    BMPImage_rotate(&res, &bimg, 367.0);
    BMPImage_dbg(&res, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&res, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        BMPImage_release(&res);
        return;
    }

    fp = fopen("out7.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    fclose(fp);
}

TEST(bmp_scale)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    BMPImage res = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    BMPImage_scale(&res, &bimg, 0.1, 0.7);
    BMPImage_dbg(&res, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&res, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        BMPImage_release(&res);
        return;
    }

    fp = fopen("out8.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    fclose(fp);
}

TEST(bmp_translate)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    BMPImage res = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    BMPImage_translate(&res, &bimg, 100, -80);
    BMPImage_dbg(&res, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&res, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        BMPImage_release(&res);
        return;
    }

    fp = fopen("out9.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    fclose(fp);
}

TEST(bmp_flip)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    BMPImage res = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    BMPImage_flip(&res, &bimg, true, true);
    BMPImage_dbg(&res, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&res, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        BMPImage_release(&res);
        return;
    }

    fp = fopen("out10.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    fclose(fp);
}

TEST(bmp_art) {
  FILE *fp = fopen(bmp_path_6, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);

  char *buf = BMP_asciiart(&bimg);
  fp = fopen("ascii.txt", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  fprintf(fp, "%s", buf);
  fclose(fp);
  BMPImage_release(&bimg);
  free(buf);
}

TEST(bmp_fft) {
  FILE *fp = fopen(bmp_path_5, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);

  __BMPImage_cvtclr_rgb2gray(&bimg);
  BMPImage_dbg(&bimg, stdout);
  BMPImage_fft_transform(&res, &bimg, true);
  
  BMPImage_dbg(&res, stdout);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return;
  }

  fp = fopen("out11.bmp", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
}

TEST(bmp_line)
{
    FILE* fp = fopen(bmp_path_1, "rb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    IStream istream = Stream(istream);
    IStream_fromFileStream(&istream, fp);
    fclose(fp);
    BMPImage bimg = BMPImage_0();
    if (BMPImage_decode(&bimg, &istream) != 0)
    {
        perror("Failed to decode bmp image");
        IStream_close(&istream);
        return;
    }
    // NOTION("Got here");
    IStream_close(&istream);
    
    BMPImage_dbg(&bimg, stdout);
    color black;
    rgb(black, 0x00, 0x00, 0x00);
    color cyan;
    rgb(cyan, 0x00, 0xff, 0xff);
    Line l = {0, 100, 100, 600, 10};
    BMPImage_line(&bimg, l, cyan);
    BMPImage_dbg(&bimg, stdout);

    OStream ostream = Stream(ostream);
    if (BMPImage_encode(&bimg, &ostream) != 0)
    {
        perror("Failed to encode bmp image");
        OStream_close(&ostream);
        BMPImage_release(&bimg);
        return;
    }

    fp = fopen("out12.bmp", "wb");
    if (fp == NULL) {
        perror("Error opening file\n");
        return;
    }
    OStream_toFileStream(&ostream, fp);
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    fclose(fp);
}

TEST(bmp_rect) {
  FILE *fp = fopen(bmp_path_1, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);
  color black;
  rgb(black, 0x00, 0x00, 0x00);
  color cyan;
  rgb(cyan, 0x00, 0xff, 0xff);
  Rect r = {2, 40, 700, 689};
  BMPImage_rect(&bimg, r, cyan);
  BMPImage_dbg(&bimg, stdout);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return;
  }

  fp = fopen("out14.bmp", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
}

TEST(bmp_circle) {
  FILE *fp = fopen(bmp_path_1, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);
  color black;
  rgb(black, 0x00, 0x00, 0x00);
  color cyan;
  rgb(cyan, 0x00, 0xff, 0xff);
  Circle c = {50.0, 422, 96};
  BMPImage_circle(&bimg, c, cyan);
  BMPImage_dbg(&bimg, stdout);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return;
  }

  fp = fopen("out13.bmp", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
}

TEST(bmp_pepper_salt) {
  FILE *fp = fopen(bmp_path_1, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);
  BMPImage_noise_pepper(&bimg, 0.1);
  BMPImage_dbg(&bimg, stdout);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return;
  }

  fp = fopen("out14.bmp", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
}

TEST(bmp_clip) {
  FILE *fp = fopen(bmp_path_1, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);
  color black;
  rgb(black, 0x00, 0x00, 0x00);
  color cyan;
  rgb(cyan, 0x00, 0xff, 0xff);
  Rect r = {100, 40, 701, 689};
  BMPImage_clip(&bimg, r);
  BMPImage_dbg(&bimg, stdout);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return;
  }

  fp = fopen("out15.bmp", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
}

TEST(bmp_confuse) {
  FILE *fp = fopen(bmp_path_1, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage lhs = BMPImage_0();
  BMPImage rhs = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&lhs, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  // NOTION("Got here");
  IStream_close(&istream);
  Stream _istream = Stream(_istream);
  fp = fopen("out13.bmp", "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream_fromFileStream(&_istream, fp);
  fclose(fp);
  if (BMPImage_decode(&rhs, &_istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&_istream);
    return;
  }
  IStream_close(&_istream);

  BMPImage_dbg(&lhs, stdout);
  BMPImage_confuse(&res, &lhs, &rhs, OP_RULE_MAX, (Confuse_args){false});
  BMPImage_dbg(&res, stdout);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    BMPImage_release(&res);
    return;
  }

  fp = fopen("out17.bmp", "wb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&lhs);
  BMPImage_release(&rhs);
  BMPImage_release(&res);
  fclose(fp);
}

TEST(clr_fromhex) {
  int hex = 0xFF0022;
  color clr = fromHex(hex);
  assert(clr.rgb[2] == 0xFF);
  assert(clr.rgb[1] == 0x00);
  assert(clr.rgb[0] == 0x22);
}

int main() {
  srand(time(NULL));
    run_all_tests();
}