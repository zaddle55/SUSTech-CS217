#include "../inc/pmat.h"
#include "../inc/test.h"

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

int main() {
  srand(time(NULL));
  run_all_tests();
}