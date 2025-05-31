#include "../Halide/include/Halide.h"
#include <iostream>
#include <stdio.h>
#include <ctime>

using namespace Halide;

#define HALIDE_TIME_START()                                                    \
  timespec start_time;                                                         \
  clock_gettime(CLOCK_MONOTONIC, &start_time);                                
#define HALIDE_TIME_END(msg)                                                   \
  timespec end_time;                                                         \
  clock_gettime(CLOCK_MONOTONIC, &end_time);                                  \
  double elapsed = (end_time.tv_sec - start_time.tv_sec) +                    \
                   (end_time.tv_nsec - start_time.tv_nsec) / 1e9;             \
  printf("%s: %.3f 秒\n", msg, elapsed);

Func optimized_blur_3x3(Buffer<float> input_buffer) {
  Func blur_x, blur_y("blur_y");
  Var x, y, xi, yi;

  Func clamped;

  // 使用 clamp 避免边界问题
  clamped(x, y) = input_buffer(clamp(x, 0, input_buffer.width() - 1),
                                clamp(y, 0, input_buffer.height() - 1));

  blur_x(x, y) = (clamped(x - 1, y) + clamped(x, y) + clamped(x + 1, y)) / 3;
  blur_y(x, y) = (blur_x(x, y - 1) + blur_x(x, y) + blur_x(x, y + 1)) / 3;

  blur_y.tile(x, y, xi, yi, 64, 16)
      .vectorize(xi, 8)
      .parallel(y);

  blur_x.compute_at(blur_y, y).vectorize(x, 8);
  return blur_y;
}

int main() {
  try {
    printf("开始 Halide 程序...\n");

    // 定义变量
    Var x, y, c;

    // 示例2: 图像处理（正确处理边界）
    printf("\n创建测试图像...\n");
    Buffer<float> input(600, 400); // 较小的测试图像

    // 填充测试数据
    for (int y = 0; y < input.height(); y++) {
      for (int x = 0; x < input.width(); x++) {
        input(x, y) = (x + y) % 256; // 简单的渐变图像
      }
    }
    printf("测试图像创建完成: %dx%dx%d\n", input.width(), input.height(),
           input.channels());

    // 示例3: 安全的模糊函数
    printf("\n创建安全的模糊函数...\n");

    Target target_avx2 = Target("x86-64-linux-avx2");

    printf("编译目标: %s\n", target_avx2.to_string().c_str());

    Func blur = optimized_blur_3x3(input);
    blur.compile_to_lowered_stmt("blur.html", {input}, HTML, target_avx2);
    blur.compile_to_c("./blur_generated.c", {input}, "blur_func", target_avx2);
    // 使用 clamp 避免边界问题
    
    printf("计算模糊效果...\n");
    HALIDE_TIME_START()
    Buffer<float> blur_result;
        blur.realize(blur_result);
    HALIDE_TIME_END("模糊计算时间");
    printf("模糊计算完成\n");
    printf("测试点 (20, 20, 0): %f -> %f\n", input(20, 20, 0),
           blur_result(20, 20, 0));

    return 0;
  } catch (const Halide::CompileError &e) {
    printf("Halide 编译错误: %s\n", e.what());
    return 1;
  }
  catch (const Halide::InternalError &e) {
    printf("Halide 内部错误: %s\n", e.what());
    return 1;
  } catch (const Halide::RuntimeError &e) {
    printf("Halide 运行时错误: %s\n", e.what());
    return 1;
  } catch (const std::exception &e) {
    printf("标准异常: %s\n", e.what());
    return 1;
  } catch (...) {
    printf("未知异常\n");
    return 1;
  }
}