#include "../inc/bmp.h"
#include "../inc/test.h"

static const char* bmp_path_1 = "./img/test1.bmp";
static const char* bmp_path_2 = "./img/test2.bmp";

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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
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
    BMPImage bimg = BMPImage();
    BMPImage res = BMPImage();
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
    BMPImage bimg = BMPImage();
    BMPImage res = BMPImage();
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
    BMPImage bimg = BMPImage();
    BMPImage res = BMPImage();
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
    BMPImage bimg = BMPImage();
    BMPImage res = BMPImage();
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

// TEST(bmp_fft) {
//   FILE *fp = fopen(bmp_path_1, "rb");
//   if (fp == NULL) {
//     perror("Error opening file\n");
//     return;
//   }
//   IStream istream = Stream(istream);
//   IStream_fromFileStream(&istream, fp);
//   fclose(fp);
//   BMPImage bimg = BMPImage();
//   BMPImage res = BMPImage();
//   if (BMPImage_decode(&bimg, &istream) != 0) {
//     perror("Failed to decode bmp image");
//     IStream_close(&istream);
//     return;
//   }
//   // NOTION("Got here");
//   IStream_close(&istream);

//   BMPImage_dbg(&bimg, stdout);
//   BMPImage_fft_transform(&res, &bimg, true);
//   BMPImage_dbg(&res, stdout);

//   OStream ostream = Stream(ostream);
//   if (BMPImage_encode(&res, &ostream) != 0) {
//     perror("Failed to encode bmp image");
//     OStream_close(&ostream);
//     BMPImage_release(&bimg);
//     BMPImage_release(&res);
//     return;
//   }

//   fp = fopen("out11.bmp", "wb");
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

int main()
{
    run_all_tests();
}