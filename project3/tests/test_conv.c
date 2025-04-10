#include "../inc/pmat.h"
#include "../inc/test.h"
#include "../inc/bmp.h"

TEST(test_conv3x3) {
  Mat base = Mat(6, 5);
  Mat_alloc(&base, 6, 5);
  for (size_t idx = 0; idx < 6 * 5; idx++) {
    base.bytes[idx] = 18;
  }
  Mat kernel = Mat(3, 3);
  Mat_alloc(&kernel, 3, 3);
  for (size_t idx = 0; idx < 9; idx++) {
    kernel.bytes[idx] = 255;
  }
  Mat res = Mat(0, 0);
  Mat_conv2d(&res, &base, &kernel, 1, 1, true);
  for (size_t idx = 0; idx < res.cols * res.rows; idx++) {
    printf("%4u ", res.bytes[idx]);
    if (idx % res.cols == res.cols - 1)
      printf("\n");
  }
  Mat_release(&base);
  Mat_release(&kernel);
  Mat_release(&res);
}

TEST(test_conv5x3) {
  Mat base = Mat(6, 5);
  Mat_alloc(&base, 6, 5);
  for (size_t idx = 0; idx < 6 * 5; idx++) {
    base.bytes[idx] = rand() % 256;
  }
  Mat kernel = Mat(5, 3);
  Mat_alloc(&kernel, 5, 3);
  for (size_t idx = 0; idx < 15; idx++) {
    kernel.bytes[idx] = rand() % 256;
  }
  Mat res = Mat(0, 0);
  Mat_conv2d(&res, &base, &kernel, 1, 1, true);
  for (size_t idx = 0; idx < res.cols * res.rows; idx++) {
    printf("%4u ", res.bytes[idx]);
    if (idx % res.cols == res.cols - 1)
      printf("\n");
  }
  Mat_release(&base);
  Mat_release(&kernel);
  Mat_release(&res);
}

static const char *bmp_path_1 = "./img/test1.bmp";
static const char *bmp_path_2 = "./img/test2.bmp";
static const char *bmp_path_3 = "./img/test3.bmp";
static const char *bmp_path_4 = "./img/test4.bmp";
static const char *bmp_path_5 = "./img/test5.bmp";
static const char *bmp_path_6 = "./img/test6.bmp";

TEST(bmp_conv) {
  FILE *fp = fopen(bmp_path_4, "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&istream);
    return;
  }
  fclose(fp);
  BMPImage kernel = BMPImage_0();
  // NOTION("Got here");
  IStream_close(&istream);
  IStream kistream = Stream(kistream);
  fp = fopen("out15.bmp", "rb");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }
  IStream_fromFileStream(&kistream, fp);
  if (BMPImage_decode(&kernel, &kistream) != 0) {
    perror("Failed to decode bmp image");
    IStream_close(&kistream);
    return;
  }
  BMPImage_dbg(&bimg, stdout);
  BMPImage_conv(&res, &bimg, &kernel, 1, 1, true);
  BMPImage_dbg(&res, stdout);
  fclose(fp);

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return;
  }

  fp = fopen("out16.bmp", "wb");
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

int main() {
  srand(time(NULL));
  run_all_tests();
}