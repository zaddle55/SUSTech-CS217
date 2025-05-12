#ifndef MCV_DECL_H_
#define MCV_DECL_H_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cfloat>
#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <functional>
#include <complex>
#include <utility>
#include <immintrin.h>


// ############################################## //
// ############## Constants & Defs ############## //
// ############################################## //

#define CPLUSPLUS_VER 201703L
#define MCV_VERSION "0.1.0"
#define MCV_VERSION_MAJOR 0
#define MCV_VERSION_MINOR 1
#define MCV_VERSION_PATCH 0

#ifdef MCV_DBG
#define DEBUG_OUT(fmt, ...) \
    fprintf(stderr, "\033[1;32m [DEBUG]: \033[0m %s:%d: at function '%s'\r\n", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_OUT(fmt, ...)
#endif
#define DEBUG_VAR(var) \
do { \
    auto __debug_var = (var); \
    DEBUG_OUT("%s = %s\n", #var, __debug_var) \
} while (0)
                    // debug
#define DEBUG_EXPR(expr) \
do { \
    auto __debug_expr = (expr); \
    DEBUG_OUT("%s = %s\n", #expr, (__debug_expr) ? "true" : "false") \
} while (0)
                    // debug

#define ASSERTS(cond, fmt) if (!(cond)) { throw std::invalid_argument("Assertion failed on `" + std::string(#cond)  + "` due to " + std::string(fmt)); }
#define ASSERT_MALLOC(ptr) if (!(ptr)) { throw std::bad_alloc(); }
#define ASSERT_NULL(ptr, fmt) if (!(ptr)) { throw std::invalid_argument(fmt); }

// ######################################## //
// ############## Timer Defs ############## //
// ######################################## //

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#else
    #include <unistd.h>
    #include <sys/resource.h>
    #include <sys/time.h>
    #include <sys/times.h>
#endif

// 用于存储不同类型的计时结果
struct TimingResult {
    std::string name;        // 计时器名称
    double elapsed_seconds;  // 耗时（秒）
    
    TimingResult(const std::string& n, double e) : name(n), elapsed_seconds(e) {}
};

class PreciseTimer {
private:
    std::string timer_name;
    
    // 用于高精度墙钟时间
    std::chrono::high_resolution_clock::time_point wall_start;
    
    // 用于CPU时间
    clock_t cpu_start;
    
    // 用于系统调用时间（POSIX系统）
#ifndef _WIN32
    struct tms tms_start;
    struct timeval sys_start;
#else
    FILETIME proc_creation_time;
    FILETIME proc_exit_time;
    FILETIME proc_kernel_time_start;
    FILETIME proc_user_time_start;
    FILETIME proc_kernel_time_end;
    FILETIME proc_user_time_end;
#endif

    bool running;
    std::vector<TimingResult> results;

public:
    PreciseTimer(const std::string& name = "Timer") : timer_name(name), running(false) {}
    
    // 开始计时
    void start() {
        if (running) {
            std::cerr << "Timer already running\n";
            return;
        }
        
        // 记录墙钟时间
        wall_start = std::chrono::high_resolution_clock::now();
        
        // 记录CPU时间
        cpu_start = clock();
        
        // 记录系统时间
#ifdef _WIN32
        HANDLE process = GetCurrentProcess();
        GetProcessTimes(process, &proc_creation_time, &proc_exit_time, 
                        &proc_kernel_time_start, &proc_user_time_start);
#else
        times(&tms_start);
        gettimeofday(&sys_start, nullptr);
#endif
        
        running = true;
    }
    
    // 停止计时并返回结果
    void stop() {
        if (!running) {
            std::cerr << "Timer not running\n";
            return;
        }
        
        // 墙钟时间（Wall Clock Time）
        auto wall_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> wall_elapsed = wall_end - wall_start;
        results.push_back(TimingResult("Wall Time", wall_elapsed.count()));
        
        // CPU时间
        clock_t cpu_end = clock();
        double cpu_elapsed = static_cast<double>(cpu_end - cpu_start) / CLOCKS_PER_SEC;
        results.push_back(TimingResult("CPU Time", cpu_elapsed));
        
#ifdef _WIN32
        // Windows系统的用户和内核时间
        HANDLE process = GetCurrentProcess();
        GetProcessTimes(process, &proc_creation_time, &proc_exit_time, 
                        &proc_kernel_time_end, &proc_user_time_end);
        
        // 将FILETIME转换为秒
        auto filetime_to_seconds = [](const FILETIME& ft_start, const FILETIME& ft_end) -> double {
            ULARGE_INTEGER start, end;
            start.LowPart = ft_start.dwLowDateTime;
            start.HighPart = ft_start.dwHighDateTime;
            end.LowPart = ft_end.dwLowDateTime;
            end.HighPart = ft_end.dwHighDateTime;
            
            // Windows的FILETIME是以100纳秒为单位的
            return static_cast<double>(end.QuadPart - start.QuadPart) / 10000000.0;
        };
        
        double kernel_time = filetime_to_seconds(proc_kernel_time_start, proc_kernel_time_end);
        double user_time = filetime_to_seconds(proc_user_time_start, proc_user_time_end);
        
        results.push_back(TimingResult("Kernel Time", kernel_time));
        results.push_back(TimingResult("User Time", user_time));
        results.push_back(TimingResult("System Time", kernel_time + user_time));
#else
        // POSIX系统的用户和系统时间
        struct tms tms_end;
        struct timeval sys_end;
        
        times(&tms_end);
        gettimeofday(&sys_end, nullptr);
        
        double user_time = static_cast<double>(tms_end.tms_utime - tms_start.tms_utime) / sysconf(_SC_CLK_TCK);
        double system_time = static_cast<double>(tms_end.tms_stime - tms_start.tms_stime) / sysconf(_SC_CLK_TCK);
        
        results.push_back(TimingResult("User Time", user_time));
        results.push_back(TimingResult("System Time", system_time));
        results.push_back(TimingResult("Total Process Time", user_time + system_time));
        
        // 计算实际系统时间
        double real_sys_time = 
            (sys_end.tv_sec - sys_start.tv_sec) + 
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
        
        results.push_back(TimingResult("Real System Time", real_sys_time));
#endif
        
        running = false;
    }
    
    // 重置计时器
    void reset() {
        if (running) {
            stop();
        }
        results.clear();
        running = false;
    }
    
    // 打印所有计时结果
    void printResults() const {
        std::cout << "===== " << timer_name << " Timing Results =====\n";
        for (const auto& result : results) {
            std::cout << std::setw(20) << std::left << result.name << ": " 
                      << std::fixed << std::setprecision(9) << result.elapsed_seconds 
                      << " seconds\n";
        }
        std::cout << "=====================================\n";
    }
    
    // 获取特定类型的计时结果
    double getElapsedTime(const std::string& type = "Wall Time") const {
        for (const auto& result : results) {
            if (result.name == type) {
                return result.elapsed_seconds;
            }
        }
        std::cerr << "Timing type '" << type << "' not found\n";
        return -1.0;
    }
    
    // 计算多次运行的统计信息
    static void calculateStatistics(const std::vector<double>& times) {
        if (times.empty()) {
            std::cout << "No timing data available\n";
            return;
        }
        
        double min_time = *std::min_element(times.begin(), times.end());
        double max_time = *std::max_element(times.begin(), times.end());
        double sum = std::accumulate(times.begin(), times.end(), 0.0);
        double mean = sum / times.size();
        
        // 计算标准差
        double sq_sum = std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
        double stddev = std::sqrt(sq_sum / times.size() - mean * mean);
        
        std::cout << "Statistics over " << times.size() << " runs:\n";
        std::cout << "  Min: " << std::fixed << std::setprecision(9) << min_time << " seconds\n";
        std::cout << "  Max: " << max_time << " seconds\n";
        std::cout << "  Mean: " << mean << " seconds\n";
        std::cout << "  StdDev: " << stddev << " seconds\n";
    }
    
    // 执行多次计时并返回统计数据
    template<typename Func>
    static void benchmark(const std::string& name, Func func, int iterations = 10) {
        std::vector<double> wall_times;
        std::vector<double> cpu_times;
        
        for (int i = 0; i < iterations; ++i) {
            PreciseTimer timer(name + " Run " + std::to_string(i+1));
            timer.start();
            func();  // 执行被测试的函数
            timer.stop();
            
            wall_times.push_back(timer.getElapsedTime("Wall Time"));
            cpu_times.push_back(timer.getElapsedTime("CPU Time"));
            
            if (i == 0) {  // 只打印第一次运行的详细信息
                timer.printResults();
            }
        }
        
        std::cout << "\n===== Wall Time Statistics =====\n";
        calculateStatistics(wall_times);
        
        std::cout << "\n===== CPU Time Statistics =====\n";
        calculateStatistics(cpu_times);
    }
};

// CPU周期计数器类（仅限x86/x64架构）
class CycleCounter {
private:
    unsigned long long start_cycle;
    unsigned long long end_cycle;
    bool running;
    
    // 读取CPU的时间戳计数器（TSC）
    unsigned long long readTSC() {
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
        return __rdtsc();  // MSVC intrinsic
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
        unsigned int lo, hi;
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        return ((unsigned long long)hi << 32) | lo;
#else
        // 不支持的架构
        return 0;
#endif
    }
    
public:
    CycleCounter() : running(false), start_cycle(0), end_cycle(0) {}
    
    void start() {
        if (running) return;
        start_cycle = readTSC();
        running = true;
    }
    
    void stop() {
        if (!running) return;
        end_cycle = readTSC();
        running = false;
    }
    
    unsigned long long getCycles() const {
        return end_cycle - start_cycle;
    }
    
    // 估算CPU频率（粗略）
    static double estimateCPUFrequencyGHz() {
        CycleCounter counter;
        std::chrono::high_resolution_clock::time_point t_start = 
            std::chrono::high_resolution_clock::now();
        
        counter.start();
        // 睡眠100毫秒
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
        counter.stop();
        
        std::chrono::high_resolution_clock::time_point t_end = 
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = t_end - t_start;
        
        // 计算MHz
        return counter.getCycles() / (elapsed.count() * 1e9);
    }
};

#define BENCH_START \
    PreciseTimer __timer(std::string("Function '") + __func__ + "()'"); \
    __timer.start(); \
    CycleCounter __cycle_counter; \
    __cycle_counter.start();
#define BENCH_END \
    __cycle_counter.stop(); \
    __timer.stop(); \
    __timer.printResults(); \
    std::cout << "Cycles: " << __cycle_counter.getCycles() << "\n"; \
    std::cout << "Estimated CPU Frequency: " << CycleCounter::estimateCPUFrequencyGHz() << " GHz\n"; \
    std::cout << "=====================================\n";


// ############################################## //
// ############## Color Space Defs ############## //
// ############################################## //

enum color_space {
    RGB = 0,
    BGR = 1,
    RGBA = 2,
    BGRA = 3,
    GRAY = 4,
    YUV = 5,
    HSV = 6,
    HLS = 7,
    YCbCr = 8
};

enum color_conversion {
    RGB2BGR = 0,
    BGR2RGB = 1,
    RGB2HSV = 2,
    HSV2RGB = 3,
    RGB2HLS = 4,
    HLS2RGB = 5,
    RGB2YUV = 6,
    YUV2RGB = 7,
    RGB2YCbCr = 8,
    YCbCr2RGB = 9
};

struct color {
    uint32_t raw_data_;

    static const color BLACK;
    static const color WHITE;
    static const color RED;
    static const color GREEN;
    static const color BLUE;
    static const color YELLOW;
    static const color CYAN;
    static const color GRAY;

    color(uint32_t raw_data) : raw_data_(raw_data) {}
    color(const std::string& hex) {
        if (hex.size() == 6 || hex.size() == 8) {
            raw_data_ = std::stoul(hex, nullptr, 16);
        } else {
            throw std::invalid_argument("Invalid hex color format, use like \"RRGGBB\" or \"AARRGGBB\".\n");
        }
    }
    color(uint8_t v2 = 0, uint8_t v1 = 0, uint8_t v0 = 0, uint8_t v3 = 255) {
        raw_data_ = ((uint32_t)v3 << 24) | ((uint32_t)v2 << 16) | ((uint32_t)v1 << 8) | (uint32_t)v0;
    }
    inline uint8_t v2() const { return (raw_data_ >> 16) & 0xFF; }
    inline uint8_t v1() const { return (raw_data_ >> 8) & 0xFF; }
    inline uint8_t v0() const { return raw_data_ & 0xFF; }
    inline uint8_t v3() const { return (raw_data_ >> 24) & 0xFF; }

    static color from_hex(uint32_t hex) {
        return color(hex);
    }
    static color from_rgb(uint8_t r, uint8_t g, uint8_t b) {
        return color(r, g, b);
    }
    static color from_bgr(uint8_t b, uint8_t g, uint8_t r) {
        return color(r, g, b);
    }
    static color from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return color(r, g, b, a);
    }
    static color from_bgra(uint8_t b, uint8_t g, uint8_t r, uint8_t a) {
        return color(r, g, b, a);
    }
    static color from_gray(uint8_t gray) {
        return color(gray, gray, gray);
    }
    static color from_yuv(uint8_t y, uint8_t u, uint8_t v) {
        return color(y, u, v);
    }
    static color from_hsv(uint8_t h, uint8_t s, uint8_t v) {
        return color(h, s, v);
    }
    static color from_hls(uint8_t h, uint8_t l, uint8_t s) {
        return color(h, s, l);
    }
    static color from_ycbcr(uint8_t y, uint8_t cb, uint8_t cr) {
        return color(y, cb, cr);
    }

    uint8_t operator [] (size_t index) const {
        // read-only element
        ASSERTS(index < 4, "index out of bounds");
        return ((uint8_t*)&raw_data_)[index];
    }

    uint8_t r() const { return v2(); }
    uint8_t g() const { return v1(); }
    uint8_t b() const { return v0(); }
    uint8_t a() const { return v3(); }
    uint8_t y() const { return v2(); }
    uint8_t u() const { return v1(); }
    uint8_t v() const { return v0(); }
    uint8_t h() const { return v2(); }
    uint8_t s() const { return v1(); }
    uint8_t l() const { return v0(); }
    uint8_t cb() const { return v2(); }
    uint8_t cr() const { return v1(); }

    static color rgb2bgr(const color& c) {
        return color(c.b(), c.g(), c.r());
    }
    
    static color bgr2rgb(const color& c) {
        return color(c.r(), c.g(), c.b());
    }
    
    static color rgb2hsv(const color& c) {
        // RGB to HSV conversion
        float r = c.r() / 255.0f;
        float g = c.g() / 255.0f;
        float b = c.b() / 255.0f;
    
        float max = std::max(r, std::max(g, b));
        float min = std::min(r, std::min(g, b));
        float delta = max - min;
    
        float h, s, v;
        v = max;
    
        if (delta < FLT_EPSILON) {
            h = 0;
            s = 0;
        } else {
            s = delta / max;
    
            if (r == max) {
                h = (g - b) / delta;
            } else if (g == max) {
                h = 2 + (b - r) / delta;
            } else {
                h = 4 + (r - g) / delta;
            }
    
            h *= 60;
            if (h < 0) h += 360;
        }
    
        return color::from_hsv((uint8_t)(h / 2), (uint8_t)(s * 255), (uint8_t)(v * 255));
    }
    
    static color hsv2rgb(const color& c) {
        // HSV to RGB conversion
        float h = c.h() * 2.0f;
        float s = c.s() / 255.0f;
        float v = c.v() / 255.0f;
    
        float r, g, b;
    
        int i = (int)(h / 60) % 6;
        float f = h / 60 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);
    
        switch (i) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }
    
        return color::from_rgb((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
    }
    
    static color rgb2hls(const color& c) {
        // RGB to HLS conversion
        float r = c.r() / 255.0f;
        float g = c.g() / 255.0f;
        float b = c.b() / 255.0f;
    
        float max = std::max(r, std::max(g, b));
        float min = std::min(r, std::min(g, b));
        float delta = max - min;
    
        float h, l, s;
        l = (max + min) / 2;
    
        if (delta < FLT_EPSILON) {
            h = 0;
            s = 0;
        } else {
            if (l < 0.5f) {
                s = delta / (max + min);
            } else {
                s = delta / (2 - max - min);
            }
    
            if (r == max) {
                h = (g - b) / delta;
            } else if (g == max) {
                h = 2 + (b - r) / delta;
            } else {
                h = 4 + (r - g) / delta;
            }
    
            h *= 60;
            if (h < 0) h += 360;
        }
    
        return color::from_hls((uint8_t)(h / 2), (uint8_t)(l * 255), (uint8_t)(s * 255));
    }
    
    static color hls2rgb(const color& c) {
        // HLS to RGB conversion
        float h = c.h() * 2.0f;
        float l = c.l() / 255.0f;
        float s = c.s() / 255.0f;
    
        float r, g, b;
    
        if (s == 0) {
            r = g = b = l;
        } else {
            float q = (l < 0.5f) ? (l * (1 + s)) : (l + s - l * s);
            float p = 2 * l - q;
    
            float h_ = h / 360;
            float t = h_ + 1.0f / 3;
            if (t > 1) t -= 1;
            float t2 = h_ - 1.0f / 3;
            if (t2 < 0) t2 += 1;
            float t3 = h_ - 2.0f / 3;
            if (t3 < 0) t3 += 1;
            r = p + (q - p) * 6 * ((t < 1.0f / 6) ? t : ((t < 1.0f / 2) ? 1 : ((t < 2.0f / 3) ? (2.0f / 3 - t) : 0)));
            g = p + (q - p) * 6 * ((t2 < 1.0f / 6) ? t2 : ((t2 < 1.0f / 2) ? 1 : ((t2 < 2.0f / 3) ? (2.0f / 3 - t2) : 0)));
            b = p + (q - p) * 6 * ((t3 < 1.0f / 6) ? t3 : ((t3 < 1.0f / 2) ? 1 : ((t3 < 2.0f / 3) ? (2.0f / 3 - t3) : 0)));
        }
    
        return color::from_rgb((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
    }

    static color rgb2yuv(const color& c) {
        // RGB to YUV conversion
        float r = c.r() / 255.0f;
        float g = c.g() / 255.0f;
        float b = c.b() / 255.0f;
    
        float y = 0.299f * r + 0.587f * g + 0.114f * b;
        float u = -0.14713f * r - 0.28886f * g + 0.436f * b;
        float v = 0.615f * r - 0.51499f * g - 0.10001f * b;
    
        return color::from_yuv((uint8_t)(y * 255), (uint8_t)((u + 0.5) * 255), (uint8_t)((v + 0.5) * 255));
    }

    static color yuv2rgb(const color& c) {
        // YUV to RGB conversion
        float y = c.y() / 255.0f;
        float u = c.u() / 255.0f - 0.5f;
        float v = c.v() / 255.0f - 0.5f;
    
        float r = y + 1.402f * v;
        float g = y - 0.344136f * u - 0.714136f * v;
        float b = y + 1.772f * u;
    
        return color::from_rgb((uint8_t)(std::clamp(r, 0.0f, 255.0f)), 
                               (uint8_t)(std::clamp(g, 0.0f, 255.0f)), 
                               (uint8_t)(std::clamp(b, 0.0f, 255.0f)));
    }

    static color rgb2ycbcr(const color& c) {
        // RGB to YCbCr conversion
        float r = c.r() / 255.0f;
        float g = c.g() / 255.0f;
        float b = c.b() / 255.0f;
    
        float y = 0.299f * r + 0.587f * g + 0.114f * b;
        float cb = -0.168736f * r - 0.331364f * g + 0.5f * b + 128;
        float cr = 0.5f * r - 0.418688f * g - 0.081312f * b + 128;
    
        return color::from_ycbcr((uint8_t)(y * 255), (uint8_t)(cb), (uint8_t)(cr));
    }

    static color ycbcr2rgb(const color& c) {
        // YCbCr to RGB conversion
        float y = c.y() / 255.0f;
        float cb = c.cb() - 128;
        float cr = c.cr() - 128;
    
        float r = y + 1.402f * cr;
        float g = y - 0.344136f * cb - 0.714136f * cr;
        float b = y + 1.772f * cb;
    
        return color::from_rgb((uint8_t)(std::clamp(r, 0.0f, 255.0f)), 
                               (uint8_t)(std::clamp(g, 0.0f, 255.0f)), 
                               (uint8_t)(std::clamp(b, 0.0f, 255.0f)));
    }
};

// ########################################## //
// ############## Shape Class ############## //
// ########################################## //

namespace mcv {

    struct Shape {
        size_t rows_;
        size_t cols_;
        size_t channels_;

        Shape() = default;

        Shape(size_t rows, size_t cols, size_t channels) :
            rows_(rows), cols_(cols), channels_(channels) {}

        bool operator == (Shape& other) const {
            return rows_ == other.rows_ && cols_ == other.cols_ && channels_ == other.channels_;
        }

        bool operator != (Shape& other) const {
            return !(*this == other);
        }

        friend std::ostream& operator << (std::ostream& os, const Shape& shape) {
            os << "(" << shape.rows_ << ", " << shape.cols_ << ", " << shape.channels_ << ")";
            return os;
        }
    };

    struct Line {
        int x0;
        int y0;
        int x1;
        int y1;
        double stroke;

        Line() = default;
        Line(int x0, int y0, int x1, int y1, double stroke = 1.0) :
            x0(x0), y0(y0), x1(x1), y1(y1), stroke(stroke) {}
    };

    struct Circle {
        int rx;
        int ry;
        int radius;

        Circle() = default;
        Circle(int rx, int ry, int radius) :
            rx(rx), ry(ry), radius(radius) {}
    };

    struct Rectf {
        float x;
        float y;
        float dx;
        float dy;

        Rectf() = default;

        Rectf(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}
        Rectf(float x, float y, Shape shape) : x(x), y(y), dx(shape.rows_), dy(shape.cols_) {}
    };

    struct Rect {
        size_t x;
        size_t y;
        size_t dx;
        size_t dy;

        Rect() = default;

        Rect(size_t dx, size_t dy) : x(0), y(0), dx(dx), dy(dy) {}
        Rect(size_t x, size_t y, size_t dx, size_t dy) : x(x), y(y), dx(dx), dy(dy) {}
        Rect(size_t x, size_t y, Shape shape) : x(x), y(y), dx(shape.rows_), dy(shape.cols_) {}
        Rect(Rectf rectf): x(rectf.x), y(rectf.y), dx(rectf.dx), dy(rectf.dy) {}
    };
}


// ########################################## //
// ############## Matrix Class ############## //
// ########################################## //

namespace mcv {

    template<typename T, size_t CHANNELS>
    class Mat_;

    template<typename T, size_t CHANNELS>
    class ConstChannelView;

    template<typename T, size_t CHANNELS>
    class ChannelView;

    class fast_conv {
    public:
    enum class DataLayout {
        NCHW,  // input[batch, inChannel, inHeight, inWidth]
        NHWC   // input[batch, inHeight, inWidth, inChannel]
    };
    
    enum class KernelLayout {
        OIHW,  // kernel [outChannel, inChannel, kernelH, kernelW]
        HWOI   // kernel [kernelH, kernelW, inChannel, outChannel]
    };
    
    fast_conv(int inChannel, int inHeight, int inWidth,
               int outChannel, int kernelH, int kernelW,
               int strideH = 1, int strideW = 1, int padH = 0, int padW = 0,
               DataLayout dataLayout = DataLayout::NCHW,
               KernelLayout kernelLayout = KernelLayout::OIHW)
        : inChannel_(inChannel), inHeight_(inHeight), inWidth_(inWidth),
          outChannel_(outChannel), kernelH_(kernelH), kernelW_(kernelW),
          strideH_(strideH), strideW_(strideW), padH_(padH), padW_(padW),
          dataLayout_(dataLayout), kernelLayout_(kernelLayout) {
        
        outHeight_ = (inHeight_ + 2 * padH_ - kernelH_) / strideH_ + 1;
        outWidth_ = (inWidth_ + 2 * padW_ - kernelW_) / strideW_ + 1;
        outSize_ = outHeight_ * outWidth_;
        
        kernelSize_ = kernelH_ * kernelW_;

        kernelPackWidth_ = 4;  // Pack 4 output channels together
        kernelPackHeight_ = inChannel_ * kernelSize_;
        
        packOutChannels_ = outChannel_ / kernelPackWidth_;
        remainOutChannels_ = outChannel_ % kernelPackWidth_;
        
        int packedKernelSize = ((packOutChannels_ + (remainOutChannels_ > 0 ? 1 : 0)) 
                              * kernelPackWidth_ * kernelPackHeight_);
        packedKernel_.resize(packedKernelSize);
    }
    
    void setWeights(const float* kernel) {
        transformKernel(kernel, packedKernel_.data());
    }
    
    void compute(const float* input, float* output) {
        // 1. im2col: Convert input feature map to column matrix
        const int colMatSize = inChannel_ * kernelSize_ * outSize_;
        std::vector<float> im2colMatrix(colMatSize);
        
        im2col(input, im2colMatrix.data());
        
        // 2. Kernel packing: Pack the im2col matrix
        const int packSize = 8;  // Pack 8 columns at a time
        const int packCols = outSize_ / packSize + (outSize_ % packSize > 0 ? 1 : 0);
        const int packHeight = inChannel_ * kernelSize_;
        const int packWidth = packSize;
        
        std::vector<float> packedIm2col(packHeight * packWidth * packCols);
        packMatrix(im2colMatrix.data(), packedIm2col.data(), packSize);
        
        // 3. Perform sGEMM: Multiply packed kernel with packed im2col matrix
        std::fill(output, output + outChannel_ * outSize_, 0.0f);
        packedSgemm(packedKernel_.data(), packedIm2col.data(), output);
    }

private:
    // Dimensions
    int inChannel_, inHeight_, inWidth_;
    int outChannel_, kernelH_, kernelW_;
    int strideH_, strideW_, padH_, padW_;
    int outHeight_, outWidth_, outSize_;
    int kernelSize_;
    
    // Layout
    DataLayout dataLayout_;
    KernelLayout kernelLayout_;
    
    // Packing parameters
    int kernelPackWidth_;
    int kernelPackHeight_;
    int packOutChannels_;
    int remainOutChannels_;
    
    std::vector<float> packedKernel_;

    void transformKernel(const float* kernel, float* packedKernel) {
        const int kernelSize = kernelH_ * kernelW_;
        std::vector<float> tempKernel;
        
        if (kernelLayout_ == KernelLayout::HWOI) {
            tempKernel.resize(outChannel_ * inChannel_ * kernelH_ * kernelW_);
            // HWOI -> OIHW
            for (int kh = 0; kh < kernelH_; ++kh) {
                for (int kw = 0; kw < kernelW_; ++kw) {
                    for (int oc = 0; oc < outChannel_; ++oc) {
                        for (int ic = 0; ic < inChannel_; ++ic) {
                            // Original HWOI: kernel[((kh * kernelW_ + kw) * outChannel_ + oc) * inChannel_ + ic]
                            // Target OIHW: tempKernel[(oc * inChannel_ + ic) * kernelH_ * kernelW_ + kh * kernelW_ + kw]
                            int srcIdx = ((kh * kernelW_ + kw) * outChannel_ + oc) * inChannel_ + ic;
                            int dstIdx = (oc * inChannel_ + ic) * kernelH_ * kernelW_ + kh * kernelW_ + kw;
                            tempKernel[dstIdx] = kernel[srcIdx];
                        }
                    }
                }
            }
            kernel = tempKernel.data();
        }
        
        // Pack 4 output channels together
        for (int oc = 0; oc < packOutChannels_; ++oc) {
            int c = oc * 4;
            const float* k0 = kernel + c * inChannel_ * kernelSize;
            const float* k1 = kernel + (c + 1) * inChannel_ * kernelSize;
            const float* k2 = kernel + (c + 2) * inChannel_ * kernelSize;
            const float* k3 = kernel + (c + 3) * inChannel_ * kernelSize;
            
            float* destPtr = packedKernel + oc * kernelPackWidth_ * kernelPackHeight_;
            
            for (int i = 0; i < inChannel_ * kernelSize; ++i) {
                destPtr[0] = k0[0];
                destPtr[1] = k1[0];
                destPtr[2] = k2[0];
                destPtr[3] = k3[0];
                
                destPtr += 4;
                k0 += 1;
                k1 += 1;
                k2 += 1;
                k3 += 1;
            }
        }
        
        // Handle remaining
        for (int oc = packOutChannels_ * 4; oc < outChannel_; ++oc) {
            const float* k0 = kernel + oc * inChannel_ * kernelSize;
            float* destPtr = packedKernel + (packOutChannels_ * kernelPackWidth_ * kernelPackHeight_) + 
                           (oc - packOutChannels_ * 4) * kernelPackHeight_;
            
            for (int i = 0; i < inChannel_ * kernelSize; ++i) {
                destPtr[0] = k0[0];
                destPtr += 1;
                k0 += 1;
            }
        }
    }

    void im2col(const float* src, float* im2colBuffer) {
        const int HW = inHeight_ * inWidth_;
        
        for (int cc = 0; cc < inChannel_; ++cc) {
            float* dstPtr = im2colBuffer + cc * kernelH_ * kernelW_ * outSize_;
            
            for (int kh = 0; kh < kernelH_; ++kh) {
                for (int kw = 0; kw < kernelW_; ++kw) {
                    int inputRowOffset = kh - padH_;
                    int inputColOffset = kw - padW_;
                    
                    for (int oh = 0; oh < outHeight_; ++oh) {
                        int inputRow = oh * strideH_ + inputRowOffset;
                        
                        for (int ow = 0; ow < outWidth_; ++ow) {
                            int inputCol = ow * strideW_ + inputColOffset;
                            
                            if (inputRow >= 0 && inputRow < inHeight_ && 
                                inputCol >= 0 && inputCol < inWidth_) {
                                if (dataLayout_ == DataLayout::NCHW) {
                                    // NCHW: src[cc * HW + inputRow * inWidth_ + inputCol]
                                    *dstPtr = src[cc * HW + inputRow * inWidth_ + inputCol];
                                } else {
                                    // NHWC: src[inputRow * inWidth_ * inChannel_ + inputCol * inChannel_ + cc]
                                    *dstPtr = src[inputRow * inWidth_ * inChannel_ + inputCol * inChannel_ + cc];
                                }
                            } else {
                                *dstPtr = 0; // Zero padding
                            }
                            dstPtr++;
                        }
                    }
                }
            }
        }
    }

    // Pack im2col matrix for better memory access patterns
    void packMatrix(const float* src, float* dst, int packSize) {
        const int cols = outSize_;
        const int rows = inChannel_ * kernelSize_;
        
        // Number of full packs
        int colPacks = cols / packSize;
        
        // Process full packs
        for (int i = 0; i < colPacks; ++i) {
            const float* srcPtr = src + i * packSize;
            float* dstPtr = dst + i * rows * packSize;
            
            for (int j = 0; j < rows; ++j) {
                // Copy packSize elements at once
                #ifdef __x86_64__
                if (packSize == 8) {
                    _mm256_storeu_ps(dstPtr, _mm256_loadu_ps(srcPtr));
                } else
                #endif
                {
                    for (int k = 0; k < packSize; ++k) {
                        dstPtr[k] = srcPtr[k];
                    }
                }
                
                dstPtr += packSize;
                srcPtr += cols;
            }
        }
        
        // Handle remaining columns
        int remainCols = cols % packSize;
        if (remainCols > 0) {
            const float* srcPtr = src + colPacks * packSize;
            float* dstPtr = dst + colPacks * rows * packSize;
            
            for (int j = 0; j < rows; ++j) {
                for (int k = 0; k < remainCols; ++k) {
                    dstPtr[k] = srcPtr[k];
                }
                // Pad with zeros
                for (int k = remainCols; k < packSize; ++k) {
                    dstPtr[k] = 0;
                }
                
                dstPtr += packSize;
                srcPtr += cols;
            }
        }
    }

    // Optimized matrix multiplication(sGEMM) using packed matrices
    // This function only works for specific packing sizes and configurations
    void packedSgemm(const float* packedA, const float* packedB, float* C) {
        const int kernelSize = kernelH_ * kernelW_;
        const int K = kernelSize * inChannel_;
        const int N = outHeight_ * outWidth_;
        const int packSize = 8;
        
        // Process 4 output channels at a time
        for (int oc = 0; oc < packOutChannels_; ++oc) {
            int c = oc * 4;
            float* destPtr0 = C + c * outSize_;
            float* destPtr1 = C + (c + 1) * outSize_;
            float* destPtr2 = C + (c + 2) * outSize_;
            float* destPtr3 = C + (c + 3) * outSize_;
            
            // Process 8 output elements at a time
            int i = 0;
            for (; i + packSize - 1 < N; i += packSize) {
                const float* ptrB = packedB + (i / packSize) * K * packSize;
                const float* ptrA = packedA + oc * 4 * K;
                
                #ifdef __x86_64__
                // AVX implementation for x86
                __m256 sum0 = _mm256_setzero_ps();
                __m256 sum1 = _mm256_setzero_ps();
                __m256 sum2 = _mm256_setzero_ps();
                __m256 sum3 = _mm256_setzero_ps();
                
                for (int j = 0; j < K; ++j) {
                    __m256 b = _mm256_loadu_ps(ptrB);
                    
                    __m256 a0 = _mm256_set1_ps(ptrA[0]);
                    __m256 a1 = _mm256_set1_ps(ptrA[1]);
                    __m256 a2 = _mm256_set1_ps(ptrA[2]);
                    __m256 a3 = _mm256_set1_ps(ptrA[3]);
                    
                    sum0 = _mm256_add_ps(sum0, _mm256_mul_ps(a0, b));
                    sum1 = _mm256_add_ps(sum1, _mm256_mul_ps(a1, b));
                    sum2 = _mm256_add_ps(sum2, _mm256_mul_ps(a2, b));
                    sum3 = _mm256_add_ps(sum3, _mm256_mul_ps(a3, b));
                    
                    ptrA += 4;
                    ptrB += packSize;
                }
                
                _mm256_storeu_ps(destPtr0, sum0);
                _mm256_storeu_ps(destPtr1, sum1);
                _mm256_storeu_ps(destPtr2, sum2);
                _mm256_storeu_ps(destPtr3, sum3);
                #else
                // Fallback to scalar implementation
                float sum0[packSize] = {0};
                float sum1[packSize] = {0};
                float sum2[packSize] = {0};
                float sum3[packSize] = {0};
                
                for (int j = 0; j < K; ++j) {
                    for (int n = 0; n < packSize; ++n) {
                        sum0[n] += ptrA[0] * ptrB[n];
                        sum1[n] += ptrA[1] * ptrB[n];
                        sum2[n] += ptrA[2] * ptrB[n];
                        sum3[n] += ptrA[3] * ptrB[n];
                    }
                    ptrA += 4;
                    ptrB += packSize;
                }
                
                for (int n = 0; n < packSize; ++n) {
                    destPtr0[n] = sum0[n];
                    destPtr1[n] = sum1[n];
                    destPtr2[n] = sum2[n];
                    destPtr3[n] = sum3[n];
                }
                #endif
                
                destPtr0 += packSize;
                destPtr1 += packSize;
                destPtr2 += packSize;
                destPtr3 += packSize;
            }
            
            // Handle remaining output elements (less than packSize)
            for (; i < N; ++i) {
                const float* ptrB = packedB + (i / packSize) * K * packSize + (i % packSize);
                const float* ptrA = packedA + oc * 4 * K;
                
                float sum0 = 0;
                float sum1 = 0;
                float sum2 = 0;
                float sum3 = 0;
                
                for (int j = 0; j < K; ++j) {
                    float bVal = *ptrB;
                    
                    sum0 += ptrA[0] * bVal;
                    sum1 += ptrA[1] * bVal;
                    sum2 += ptrA[2] * bVal;
                    sum3 += ptrA[3] * bVal;
                    
                    ptrA += 4;
                    ptrB += packSize;
                }
                
                destPtr0[0] = sum0;
                destPtr1[0] = sum1;
                destPtr2[0] = sum2;
                destPtr3[0] = sum3;
                
                destPtr0 += 1;
                destPtr1 += 1;
                destPtr2 += 1;
                destPtr3 += 1;
            }
        }
        
        // Handle remaining output channels
        for (int oc = packOutChannels_ * 4; oc < outChannel_; ++oc) {
            float* destPtr = C + oc * outSize_;
            
            for (int i = 0; i < N; ++i) {
                const float* ptrB = packedB + (i / packSize) * K * packSize + (i % packSize);
                const float* ptrA = packedA + (packOutChannels_ * 4 * K) + 
                                   (oc - packOutChannels_ * 4) * K;
                
                float sum = 0;
                
                for (int j = 0; j < K; ++j) {
                    sum += ptrA[0] * *ptrB;
                    ptrA += 1;
                    ptrB += packSize;
                }
                
                destPtr[i] = sum;
            }
        }
    }};

    // Check if a type is a scalar (int, float, double, etc.)
    #define is_scalar(type) (std::is_arithmetic<type>::value || std::is_enum<type>::value || std::is_same<type, std::complex<float>>::value || \
              std::is_same<type, std::complex<double>>::value)


    // Mat Operations
    namespace MatOp {

        template<typename T, typename BinOp_t, size_t CHANNELS>
        void op_ewise(mcv::Mat_<T, CHANNELS>& dst, const mcv::Mat_<T, CHANNELS>& lhs,
                            T rhs, BinOp_t op) {
            // elementwise operation
            if (lhs.rows() != dst.rows() || lhs.cols() != dst.cols()) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            #pragma omp parallel for collapse(3)
            for (size_t r_ = 0; r_ < lhs.rows(); r_++)
                for (size_t c_ = 0; c_ < lhs.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        dst(r_, c_, l_) = op(lhs(r_, c_, l_), rhs);
        }

        template<typename V, typename T, typename U, size_t CHANNELS, typename BinOp_t>
        void op_ewise_hyb(mcv::Mat_<V, CHANNELS>& dst, const mcv::Mat_<T, CHANNELS>& lhs, const U rhs,
                            BinOp_t op) {
            static_assert(is_scalar(U), "Element to operate must be a scalar type");
            // elementwise operation
            if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            #pragma omp parallel for collapse(3)
            for (size_t r_ = 0; r_ < lhs.rows(); r_++)
                for (size_t c_ = 0; c_ < lhs.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        dst(r_, c_, l_) = op(lhs(r_, c_, l_), rhs(r_, c_));
        }

        template<typename T, size_t CHANNELS, typename BinOp_t>
        void op_pwise(mcv::Mat_<T, CHANNELS>& dst, const mcv::Mat_<T, CHANNELS>& lhs, const mcv::Mat_<T, CHANNELS>& rhs, 
                        BinOp_t op) {
            // piecewise operation
            if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            #pragma omp parallel for collapse(3)
            for (size_t r_ = 0; r_ < lhs.rows(); r_++)
                for (size_t c_ = 0; c_ < lhs.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        dst(r_, c_, l_) = op(lhs(r_, c_, l_), rhs(r_, c_, l_));
        }

        template<typename V, typename T, typename U, size_t CHANNELS, typename BinOp_t>
        void op_pwise_hyb(mcv::Mat_<V, CHANNELS>& dst, const mcv::Mat_<T, CHANNELS>& lhs, const mcv::Mat_<U, CHANNELS>& rhs, 
                            BinOp_t op) {
            // piecewise operation
            if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            #pragma omp parallel for collapse(3)
            for (size_t r_ = 0; r_ < lhs.rows(); r_++)
                for (size_t c_ = 0; c_ < lhs.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        dst(r_, c_, l_) = op(lhs(r_, c_, l_), rhs(r_, c_));
        }

        template<typename T, size_t CHANNELS, typename UnaryOp_t>
        void assign_pwise(mcv::Mat_<T, CHANNELS>& lhs, const mcv::Mat_<T, CHANNELS>& rhs, 
            UnaryOp_t op) {
            // assignment
            if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            #pragma omp parallel for collapse(3)
            for (size_t r_ = 0; r_ < lhs.rows(); r_++)
                for (size_t c_ = 0; c_ < lhs.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        lhs(r_, c_, l_) = op(rhs(r_, c_, l_));
        }

        template<typename T, typename U, size_t CHANNELS, typename UnaryOp_t>
        void assign_pwise_hyb(mcv::Mat_<T, CHANNELS>& lhs, const mcv::Mat_<U, CHANNELS>& rhs, 
                                UnaryOp_t op) {
            // piecewise operation
            if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            #pragma omp parallel for collapse(3)
            for (size_t r_ = 0; r_ < lhs.rows(); r_++)
                for (size_t c_ = 0; c_ < lhs.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        lhs(r_, c_, l_) = op(rhs(r_, c_, l_));
        }

        inline Shape getConv2dOutShape(const Shape& src, const Shape& kernel, size_t stride = 1, bool padding = false) {
            ASSERTS(kernel.rows_ % 2 == 1 && kernel.cols_ % 2 == 1, "kernel size should be odd");
            ASSERTS(src.rows_ >= kernel.rows_ && src.cols_ >= kernel.cols_, "kernel size should be smaller than src");
            
            size_t pad_h = padding ? (kernel.rows_ - 1) / 2 : 0;
            size_t pad_w = padding ? (kernel.cols_ - 1) / 2 : 0;
            
            size_t output_h = padding ? 
                            (src.rows_ + 2 * pad_h - kernel.rows_) / stride + 1 : 
                            (src.rows_ - kernel.rows_) / stride + 1;
            size_t output_w = padding ? 
                            (src.cols_ + 2 * pad_w - kernel.cols_) / stride + 1 : 
                            (src.cols_ - kernel.cols_) / stride + 1;
            
            return Shape(output_h, output_w, kernel.channels_);
        }

        template<typename D, typename S, typename K, size_t OUT_CHANNELS, size_t IN_CHANNELS>
        Mat_<D, OUT_CHANNELS> conv2d(const mcv::Mat_<S, IN_CHANNELS>& src, 
                          const mcv::Mat_<K, IN_CHANNELS * OUT_CHANNELS>& kernel,
                          size_t stride = 1, bool padding = false) {
            ASSERTS(kernel.rows() % 2 == 1 && kernel.cols() % 2 == 1, "kernel size should be odd");
            ASSERTS(src.rows() >= kernel.rows() && src.cols() >= kernel.cols(), "kernel size should be smaller than src");

            Shape outShape = getConv2dOutShape(src.shape(), kernel.shape(), stride, padding);

            if constexpr (std::is_same_v<D, S> && std::is_same_v<S, K>) {
                if constexpr (std::is_same_v<S, float>) {
                    // fase conv
                    fast_conv fc(IN_CHANNELS, src.rows(), src.cols(), 
                        OUT_CHANNELS, kernel.rows(), kernel.cols(),
                        stride, stride,
                        (padding ? (kernel.rows() - 1) / 2 : 0),
                        (padding ? (kernel.cols() - 1) / 2 : 0),
                        fast_conv::DataLayout::NHWC,
                        fast_conv::KernelLayout::HWOI);
                    fc.setWeights(kernel.get());
                    Mat_<D, OUT_CHANNELS> result(outShape.rows_, outShape.cols_);
                    fc.compute(src.get(), result.get());
                    return result;
                }
            } else {
                static_assert(std::is_convertible_v<S, D>, "src and kernel should be convertible to D");
                static_assert(std::is_convertible_v<K, D>, "src and kernel should be convertible to D");
            }
            
            size_t kernel_h = kernel.rows();
            size_t kernel_w = kernel.cols();
            size_t pad_h = padding ? (kernel_h - 1) / 2 : 0;
            size_t pad_w = padding ? (kernel_w - 1) / 2 : 0;

            
            
            size_t output_h = outShape.rows_;
            size_t output_w = outShape.cols_;
            
            Mat_<D, OUT_CHANNELS> result(output_h, output_w);
            #pragma omp parallel for
            for (size_t oh = 0; oh < output_h; ++oh) {
                for (size_t ow = 0; ow < output_w; ++ow) {
                    for (size_t oc = 0; oc < OUT_CHANNELS; ++oc) {
                        D sum = D();
                        for (size_t kh = 0; kh < kernel_h; ++kh) {
                            for (size_t kw = 0; kw < kernel_w; ++kw) {
                                int64_t ih = oh * stride + kh - pad_h;
                                int64_t iw = ow * stride + kw - pad_w;
                                
                                if (ih >= 0 && ih < src.rows() && iw >= 0 && iw < src.cols()) {
                                    for (size_t ic = 0; ic < IN_CHANNELS; ++ic) {
                                        sum += src(ih, iw, ic) * 
                                            kernel(kh, kw, oc * IN_CHANNELS + ic);
                                    }
                                }
                            }
                        }
                        result(oh, ow, oc) = sum;
                    }
                }
            }  
            return result;
        }

        template<typename T>
        inline Mat_<T, 3> merge(const mcv::Mat_<T, 1>& first, const mcv::Mat_<T, 1>& second, const mcv::Mat_<T, 1>& third) {
                return first.merge(second).merge(third);
        }

        template<typename T, typename... Args>
        inline auto merge(const mcv::Mat_<T, 1>& first, const mcv::Mat_<T, 1>& second, const mcv::Mat_<T, 1>& third, Args&&... args) {
            auto merged = merge(first, second, third);
            if constexpr (sizeof...(args) > 0) {
                return merged.merge(args...);
            } else {
                return merged;
            }
        }

    } // namespace MatOp

    template<typename T, size_t CHANNELS=1UL>
    class Mat_ 
    {
    static_assert(is_scalar(T), "T must be a scalar type");
    protected:
        // parameters
        size_t rows_;
        size_t cols_;
        size_t stride_;

        // pointer
        T* ptr;

        // container(shared)
        std::shared_ptr<T> data;

        // type alias
        using Ptr = std::shared_ptr<T>;
        using ConstPtr = std::shared_ptr<const T>;

    public:
        // constructor
        Mat_() = default;
        ~Mat_() = default;
        Mat_(size_t rows, size_t cols) : rows_(rows), cols_(cols), stride_(cols) {
            // allocate memory
            data = std::shared_ptr<T>(new T[rows * cols * CHANNELS], [](T* p) { delete[] p; });
            ptr = data.get();
            DEBUG_OUT("Mat_<%s> at %p Allocated (%zu x %zu x %zu)\n", typeid(T).name(), this, rows_, cols_, CHANNELS);
        }
        explicit Mat_(Shape shape) : Mat_(shape.rows_, shape.cols_) {
            DEBUG_OUT("Mat_<%s> at %p Allocated (%zu x %zu x %zu)\n", typeid(T).name(), this, shape.rows_, shape.cols_, CHANNELS);
        }

        template<size_t M, size_t N>
        Mat_(const T(&list)[M][N][CHANNELS]) : Mat_(M, N) {
            for (size_t r_ = 0; r_ < rows_; r_++)
                for (size_t c_ = 0; c_ < cols_; c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        (*this)(r_, c_, l_) = list[r_][c_][l_];
        }

        template<size_t M, size_t N>
        Mat_(const T(&list)[M][N]) : Mat_(M, N) {
            static_assert(CHANNELS == 1, "Channels should be 1");
            for (size_t r_ = 0; r_ < rows_; r_++)
                for (size_t c_ = 0; c_ < cols_; c_++)
                    (*this)(r_, c_, 0) = list[r_][c_];
        }

        // copy constructor
        Mat_(const Mat_& other) : rows_(other.rows_), cols_(other.cols_), stride_(other.stride_), data(other.data), ptr(other.ptr) {
        }
        Mat_(const Mat_& other, const Rect& roi) : rows_(roi.dy), cols_(roi.dx), stride_(other.stride_), data(other.data) {
            DEBUG_OUT("Mat_<%s> at %p Making ROI from %p\n", typeid(T).name(), this, &other);
            ptr = other.ptr + (roi.x * stride_ + roi.y) * CHANNELS;
        }
        Mat_(const Mat_& other, const Rectf& roi) : Mat_(other, Rect(roi)) {
            DEBUG_OUT("Mat_<%s> at %p Making ROI from %p\n", typeid(T).name(), this, &other);
        }

        template<typename U>
        explicit Mat_(const Mat_<U, CHANNELS>& other) : rows_(other.rows_), cols_(other.cols_), stride_(other.stride_) {
            static_assert(!std::is_same_v<T, U>, "Cannot convert same type");
            DEBUG_OUT("Mat_<%s> at %p Converting from %s into %s\n", typeid(T).name(), this, typeid(U).name(), typeid(T).name());
            data = std::shared_ptr<T>(new T[rows_ * cols_ * CHANNELS], [](T* p) { delete[] p; });
            ptr = data.get();
            MatOp::assign_pwise_hyb(*this, other, [](const U& val) { return static_cast<T>(val); });
        }

        // copy assignment
        Mat_& operator = (const Mat_ &other) {
            if (this != &other) {
                DEBUG_OUT("Mat_<%s> at %p Copying from %p\n", typeid(T).name(), this, &other);
                MatOp::assign_pwise(*this, other, [](T& val) { return val; });
            }
            return *this;
        }

        // deep copy
        Mat_ deepcopy() const {
            Mat_ res(rows_, cols_);
            MatOp::assign_pwise(res, *this, [](T val) { return val; });
            return res;
        }

        template<typename U>
        Mat_& operator = (const Mat_<U, CHANNELS>& other) {
            static_assert(!std::is_same_v<T, U>, "Cannot assign same type");
            if (this != &other) {
                DEBUG_OUT("Mat_<%s> at %p Converting from %s into %s\n", typeid(T).name(), this, typeid(U).name(), typeid(T).name());
                MatOp::assign_pwise_hyb(*this, other, [](const U& val) { return static_cast<T>(val); });
            }
            return *this;
        }

        // member functions
        virtual size_t rows() const { return rows_; }
        virtual size_t cols() const { return cols_; }
        inline size_t stride() const { return stride_; }
        inline size_t channels() const { return CHANNELS; }
        inline size_t psize() const { return rows() * cols() * CHANNELS; }
        inline Shape shape() const { return Shape(rows(), cols(), CHANNELS); }
        inline size_t use_count() const { return data.use_count(); }
        inline T* get() { return ptr; }
        inline const T* get() const { return ptr; }

        // operations
        template<typename Func = std::function<T(T)>>
        Mat_ foreach(Func func) const {
            // apply function to each element
            Mat_ res(*this);
            MatOp::assign_pwise(res, *this, func);
            return res;
        }

        template<typename U, typename Func>
        Mat_<U, CHANNELS> transform(Func func) const {
            // transform operation
            Mat_<U, CHANNELS> res(rows_, cols_);
            MatOp::assign_pwise_hyb(res, *this, func);
            return res;
        }

        template<typename Func>
        T reduce(Func func, T init = T()) const {
            // reduce operation
            for (size_t r_ = 0; r_ < rows_; r_++)
                for (size_t c_ = 0; c_ < cols_; c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        init = func(init, (*this)(r_, c_, l_));
            return init;
        }

        template<typename Pred>
        std::vector<std::tuple<size_t, size_t, size_t>> map(Pred pred) {
            // apply function to each element
            std::vector<std::tuple<size_t, size_t, size_t>> res;
            for (size_t r_ = 0; r_ < rows_; r_++) {
                for (size_t c_ = 0; c_ < cols_; c_++) {
                    for (size_t l_ = 0; l_ < CHANNELS; l_++) {
                        if (pred((*this)(r_, c_, l_)))
                            res.push_back(std::make_tuple(r_, c_, l_));
                    }
                }
            }
                
            return res;
        }
    
        T& operator [] (size_t index) {
            // read/write element
            ASSERTS(index < psize(), "index out of bounds");
            return ptr[index];
        }
        const T& operator [] (size_t index) const {
            // read-only element
            ASSERTS(index < psize(), "index out of bounds");
            return ptr[index];
        }

        // ROI
        Mat_ operator () (const Rect& roi) {
            // read/write element
            return Mat_(*this, roi);
        }

        Mat_ operator () (const Rectf& roi) {
            // read/write element
            return Mat_(*this, roi);
        }

        virtual T& operator () (size_t row, size_t col, size_t channel = 0) {
            // read/write element
            return ptr[(row * stride_ + col) * CHANNELS + channel];
        }

        virtual T operator () (size_t row, size_t col, size_t channel = 0) const {
            // read-only element
            return ptr[(row * stride_ + col) * CHANNELS + channel];
        }

        T& at(size_t row, size_t col, size_t channel = 0) {
            // read/write element
            ASSERTS(row < rows() && col < cols() && channel < CHANNELS, "index out of bounds");
            return (*this)(row, col, channel);
        }

        T at(size_t row, size_t col, size_t channel = 0) const {
            ASSERTS(row < rows() && col < cols() && channel < CHANNELS, "index out of bounds");
            return (*this)(row, col, channel);
        }

        ConstChannelView<T, CHANNELS> const_slice(const size_t channel) const {
            if (channel >= CHANNELS)
                throw std::out_of_range("channel out of range");
            return ConstChannelView<T, CHANNELS>(*this, channel);
        }

        ChannelView<T, CHANNELS> slice(const size_t channel) {
            if (channel >= CHANNELS)
                throw std::out_of_range("channel out of range");
            return ChannelView<T, CHANNELS>(*this, channel);
        }

        Mat_<T, CHANNELS+1> merge(const Mat_<T, 1>& other) const {
            // merge operation
            if (other.rows() != rows_ || other.cols() != cols_) {
                throw std::invalid_argument("the matrices should have the same size");
            }
            Mat_<T, CHANNELS+1> res(rows_, cols_);
            for (size_t l_ = 0; l_ < CHANNELS; l_++)
                res.slice(l_) = this->const_slice(l_);
            auto newChan = res.slice(CHANNELS);
            newChan = other;
            return res;
        }

        Mat_<T, CHANNELS-1> split(size_t channel) const {
            // split operation
            if (channel >= CHANNELS) {
                throw std::out_of_range("channel out of range");
            }
            Mat_<T, CHANNELS-1> res(rows_, cols_);
            size_t rs_ = 0;
            for (size_t l_ = 0; l_ < CHANNELS; l_++) {
                if (l_ != channel)
                    res.slice(rs_++) = this->const_slice(l_);
            }
            return res;
        }
        
        void zeros() {
            // zero operation
            MatOp::assign_pwise(*this, *this, []() { return T(); });
        }

        void fill(T val) {
            // fill operation
            MatOp::assign_pwise(*this, *this, [val]() { return val; });
        }

        template<typename derived_t>
        derived_t& cast() const {
            return *static_cast<derived_t*>(this);
        }
        template<typename derived_t>
        const derived_t& constcast() const {
            return *static_cast<const derived_t*>(this);
        }

        template<typename U>
        Mat_<U, CHANNELS> astype() const {
            // type conversion
            Mat_<U, CHANNELS> res(rows_, cols_);
            MatOp::assign_pwise_hyb(res, *this, [](const T& val) { return static_cast<U>(val); });
            return res;
        }

        // arithmetic operations overloads
        Mat_<T, CHANNELS> operator + (const Mat_<T, CHANNELS>& other) const {
            // addition
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise(res, *this, other, std::plus<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator - (const Mat_<T, CHANNELS>& other) const {
            // subtraction
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise(res, *this, other, std::minus<T>());
            return res;
        }

        virtual Mat_<T, CHANNELS> operator * (const Mat_<T, CHANNELS>& other) const {
            // multiplication
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise(res, *this, other, std::multiplies<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator / (const Mat_<T, CHANNELS>& other) const {
            // division
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise(res, *this, other, std::divides<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator % (const Mat_<T, CHANNELS>& other) const {
            // modulo
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise(res, *this, other, std::modulus<T>());
            return res;
        }

        Mat_<T, CHANNELS>& operator += (const Mat_<T, CHANNELS>& other) {
            // addition assignment
            MatOp::op_pwise(*this, *this, other, std::plus<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator -= (const Mat_<T, CHANNELS>& other) {
            // subtraction assignment
            MatOp::op_pwise(*this, *this, other, std::minus<T>());
            return *this;
        }

        virtual Mat_<T, CHANNELS>& operator *= (const Mat_<T, CHANNELS>& other) {
            // multiplication assignment
            MatOp::op_pwise(*this, *this, other, std::multiplies<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator /= (const Mat_<T, CHANNELS>& other) {
            // division assignment
            MatOp::op_pwise(*this, *this, other, std::divides<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator %= (const Mat_<T, CHANNELS>& other) {
            // modulo assignment
            MatOp::op_pwise(*this, *this, other, std::modulus<T>());
            return *this;
        }

        Mat_<bool, CHANNELS> operator == (const Mat_<T, CHANNELS>& other) const {
            // equality
            Mat_<bool, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise_hyb(res, *this, other, std::equal_to<T>());
            return res;
        }

        Mat_<bool, CHANNELS> operator != (const Mat_<T, CHANNELS>& other) const {
            // inequality
            Mat_<bool, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise_hyb(res, *this, other, std::not_equal_to<T>());
            return res;
        }

        Mat_<bool, CHANNELS> operator < (const Mat_<T, CHANNELS>& other) const {
            // less than
            Mat_<bool, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise_hyb(res, *this, other, std::less<T>());
            return res;
        }

        Mat_<bool, CHANNELS> operator > (const Mat_<T, CHANNELS>& other) const {
            // greater than
            Mat_<bool, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise_hyb(res, *this, other, std::greater<T>());
            return res;
        }

        Mat_<bool, CHANNELS> operator <= (const Mat_<T, CHANNELS>& other) const {
            // less than or equal to
            Mat_<bool, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise_hyb(res, *this, other, std::less_equal<T>());
            return res;
        }

        Mat_<bool, CHANNELS> operator >= (const Mat_<T, CHANNELS>& other) const {
            // greater than or equal to
            Mat_<bool, CHANNELS> res(rows_, cols_);
            MatOp::op_pwise_hyb(res, *this, other, std::greater_equal<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator + (const T& other) const {
            // addition
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_ewise(res, *this, other, std::plus<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator - (const T& other) const {
            // subtraction
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_ewise(res, *this, other, std::minus<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator * (const T& other) const {
            // multiplication
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_ewise(res, *this, other, std::multiplies<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator / (const T& other) const {
            // division
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_ewise(res, *this, other, std::divides<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator % (const T& other) const {
            // modulo
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::op_ewise(res, *this, other, std::modulus<T>());
            return res;
        }

        Mat_<T, CHANNELS>& operator += (const T& other) {
            // addition assignment
            MatOp::op_ewise(*this, *this, other, std::plus<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator -= (const T& other) {
            // subtraction assignment
            MatOp::op_ewise(*this, *this, other, std::minus<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator *= (const T& other) {
            // multiplication assignment
            MatOp::op_ewise(*this, *this, other, std::multiplies<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator /= (const T& other) {
            // division assignment
            MatOp::op_ewise(*this, *this, other, std::divides<T>());
            return *this;
        }

        Mat_<T, CHANNELS>& operator %= (const T& other) {
            // modulo assignment
            MatOp::op_ewise(*this, *this, other, std::modulus<T>());
            return *this;
        }

        // unary operations
        Mat_<T, CHANNELS> operator - () const {
            // negation
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::assign_pwise(res, *this, std::negate<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator ~ () const {
            // bitwise negation
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::assign_pwise(res, *this, std::bit_not<T>());
            return res;
        }

        Mat_<T, CHANNELS> operator ! () const {
            // logical negation
            Mat_<T, CHANNELS> res(rows_, cols_);
            MatOp::assign_pwise(res, *this, std::logical_not<T>());
            return res;
        }

        friend std::istream& operator >> (std::istream& is, Mat_<T, CHANNELS>& mat) {
            // input stream
            for (size_t r_ = 0; r_ < mat.rows(); r_++)
                for (size_t c_ = 0; c_ < mat.cols(); c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        is >> mat(r_, c_, l_);
            return is;
        }

        friend std::ostream& operator << (std::ostream& os, const Mat_<T, CHANNELS>& mat) {
            // output stream
            for (size_t r_ = 0; r_ < mat.rows_; r_++)
                for (size_t c_ = 0; c_ < mat.cols_; c_++)
                    for (size_t l_ = 0; l_ < CHANNELS; l_++)
                        os << +mat(r_, c_, l_) << ", ";
            return os;
        }

    };

    template<typename T, size_t CHANNELS>
    class ConstChannelView : public Mat_<T, 1UL> {
    protected:
        // parameters
        size_t channel_offset_;
        Mat_<T, CHANNELS> par_;
    public:

        inline size_t rows() const override { return par_.rows(); }
        inline size_t cols() const override { return par_.cols(); }

        // constructor
        ConstChannelView(const Mat_<T, CHANNELS>& mat, size_t channel) : channel_offset_(channel), par_(mat) {
            // DEBUG_OUT("ConstChannelView at 0x%p sliced from Mat_<%s, %lu> at %lu\n", this, typeid(T).name(), CHANNELS, channel_offset_);
            this->rows_ = par_.rows();
            this->cols_ = par_.cols();
            this->stride_ = par_.stride();
        }

        // operator
        T operator () (size_t row, size_t col, size_t _ = 0) const override {
            return par_(row, col, channel_offset_);
        }
    };

    template<typename T, size_t CHANNELS>
    class ChannelView : public ConstChannelView<T, CHANNELS> {
    public:

        ChannelView(const Mat_<T, CHANNELS>& mat, size_t channel) : ConstChannelView<T, CHANNELS>(mat, channel) {
            this->rows_ = this->par_.rows();
            this->cols_ = this->par_.cols();
            this->stride_ = this->par_.stride();
        }

        // operator
        T operator () (size_t row, size_t col, size_t _ = 0) const override {
            return this->par_(row, col, this->channel_offset_);
        }
        T& operator () (size_t row, size_t col, size_t _ = 0) override {
            return this->par_(row, col, this->channel_offset_);
        }
        T& at(size_t row, size_t col, size_t _ = 0) {
            return this->par_.at(row, col, this->channel_offset_);
        }
        ChannelView& operator = (const Mat_<T, 1>& other) {
            if (this != &other) {
                DEBUG_OUT("ChannelView at %p Copying from %p\n", this, &other);
                MatOp::assign_pwise(*this, other, [](const T& val) { return val; });
            }
            return *this;
        }
    };
}

// ########################################### //
// ############## Matx Class ################ //
// ########################################### //
namespace mcv {
    
    template<typename T, size_t R, size_t C>
    class Matx {
        static_assert(is_scalar(T), "T must be a scalar type");

    private:
        std::array<T, R * C> data_;
    public:
        inline size_t rows() { return R; }
        inline size_t cols() { return C; }
        inline size_t psize() { return R * C; }
        inline T* get() { return data_.data(); }

        // constructor
        Matx() = default;
        Matx(const T(&list)[R][C]) {
            for (size_t r_ = 0; r_ < R; r_++)
                for (size_t c_ = 0; c_ < C; c_++)
                    data_[r_ * C + c_] = list[r_][c_];
        }
        Matx(const T(&list)[R]) {
            static_assert(C == 1, "C should be 1");
            for (size_t r_ = 0; r_ < R; r_++)
                data_[r_] = list[r_];
        }

        // copy constructor
        Matx(const Matx& other) {
            DEBUG_OUT("Matx<%s, %zu, %zu> at %p Copying from %p\n", typeid(T).name(), R, C, this, &other);
            data_ = other.data_;
        }

        // copy assignment
        Matx& operator = (const Matx& other) {
            if (this != &other) {
                DEBUG_OUT("Matx<%s, %zu, %zu> at %p Copying from %p\n", typeid(T).name(), R, C, this, &other);
                data_ = other.data_;
            }
            return *this;
        }

        T operator () (size_t row, size_t col, size_t _ = 0) const {
            // read-only element
            ASSERTS(row < R && col < C, "index out of bounds");
            return data_[row * C + col];
        }

        T& operator () (size_t row, size_t col, size_t _ = 0) {
            // read/write element
            ASSERTS(row < R && col < C, "index out of bounds");
            return data_[row * C + col];
        }

        operator Mat_<T, 1>() const {
            // convert to Mat_<T, 1>
            Mat_<T, 1> res(R, C);
            for (size_t r_ = 0; r_ < R; r_++)
                for (size_t c_ = 0; c_ < C; c_++)
                    res(r_, c_) = data_[r_ * C + c_];
            return res;
        }

        friend std::ostream& operator << (std::ostream& os, const Matx& mat) {
            // output stream
            for (size_t r_ = 0; r_ < R; r_++)
                for (size_t c_ = 0; c_ < C; c_++)
                    os << +mat(r_, c_) << ", ";
            return os;
        }
    };

    struct AffineMat {
        double a, b, c; // x' = ax + by + c
        double d, e, f; // y' = dx + ey + f

        AffineMat() : a(0), b(0), c(0), d(0), e(0), f(0) {}

        AffineMat(double a, double b, double c, double d, double e, double f)
            : a(a), b(b), c(c), d(d), e(e), f(f) {}
        AffineMat(double angle, double scaleX, double scaleY,
                  double translateX, double translateY) {
            double rad = angle * M_PI / 180.0;
            a = scaleX * cos(rad);
            b = -scaleY * sin(rad);
            d = scaleX * sin(rad);
            e = scaleY * cos(rad);

            c = translateX;
            f = translateY;
        }

        [[nodiscard]] AffineMat inv() const {
            AffineMat inv(0, 0, 0, 0, 0, 0);
            double det = a * e - b * d;
            if (std::abs(det) < FLT_EPSILON) {
                throw std::runtime_error("Matrix is singular and cannot be inverted");
            }

            inv.a = e / det;
            inv.b = -b / det;
            inv.c = (b * f - c * e) / det;

            inv.d = -d / det;
            inv.e = a / det;
            inv.f = (c * d - a * f) / det;
            return inv;
        }
    };

}

// ########################################### //
// ############## Filter Factory ############# //
// ########################################### //

namespace mcv {
    
    template<size_t K>
    inline constexpr Matx<float, K, K> getGaussianKernel(float sigma) {
        // Gaussian kernel
        static_assert(K % 2 == 1, "Kernel size must be odd");
        Matx<float, K, K> kernel;
        float sum = 0.0F;
        float mean = static_cast<float>(K - 1) / 2.0F;
        float sig2 = 2.0F * sigma * sigma;
        for (size_t i = 0; i < K; ++i) {
            for (size_t j = 0; j < K; ++j) {
                float x = static_cast<float>(i) - mean;
                float y = static_cast<float>(j) - mean;
                kernel(i, j) = exp(-(x * x + y * y) / sig2);
                sum += kernel(i, j);
            }
        }
        // Normalize the kernel
        for (size_t i = 0; i < K; ++i) {
            for (size_t j = 0; j < K; ++j) {
                kernel(i, j) /= sum;
            }
        }
        return kernel;
    }

    template<size_t K>
    inline constexpr Matx<float, K, K> getBoxFilter() {
        // Box filter
        static_assert(K % 2 == 1, "Kernel size must be odd");
        Matx<float, K, K> kernel;
        float value = 1.0F / (K * K);
        for (size_t i = 0; i < K; ++i) {
            for (size_t j = 0; j < K; ++j) {
                kernel(i, j) = value;
            }
        }
        return kernel;
    }

    template<size_t K>
    inline constexpr Matx<float, K, K> getLaplacianKernel() {
        // Laplacian kernel
        static_assert(K % 2 == 1, "Kernel size must be odd");
        Matx<float, K, K> kernel;
        float value = -1.0F / (K * K);
        for (size_t i = 0; i < K; ++i) {
            for (size_t j = 0; j < K; ++j) {
                kernel(i, j) = value;
            }
        }
        kernel(K / 2, K / 2) = 1.0F - value * (K * K - 1);
        return kernel;
    }

    template<size_t K>
    inline constexpr Matx<float, K, K> getSobelKernel() {
        // Sobel kernel
        static_assert(K % 2 == 1, "Kernel size must be odd");
        Matx<float, K, K> kernel;
        for (int64_t i = 0; i < K; ++i) {
            for (int64_t j = 0; j < K; ++j) {
                kernel(i, j) = static_cast<float>(i - j);
            }
        }
        return kernel;
    }

    inline const Matx<float, 3, 3> SOBEL_X(
        {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        }
    );

    inline const Matx<float, 3, 3> SOBEL_Y(
        {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}
        }
    );
}

// ########################################### //
// ############## pMatrix Class ############## //
// ########################################### //

namespace mcv {
    // PMatrix class
    template<size_t CHANNELS=3UL>
    class pMat: public Mat_<uint8_t, CHANNELS> {
        static_assert((CHANNELS >= 3UL || CHANNELS == 1), "Channel count must be 1 or at least 3");
        // PMatrix specific declarations
        public:
            using Mat_<uint8_t, CHANNELS>::Mat_;
            using Mat_<uint8_t, CHANNELS>::operator();
            using Mat_<uint8_t, CHANNELS>::at;
            using Mat_<uint8_t, CHANNELS>::rows;
            using Mat_<uint8_t, CHANNELS>::cols;
            using Mat_<uint8_t, CHANNELS>::channels;
            using Ptr = std::shared_ptr<uint8_t>;
            using ConstPtr = std::shared_ptr<const uint8_t>;

            // constructor
            pMat() : Mat_<uint8_t, CHANNELS>() {}

            // copy constructor
            pMat(const Mat_<uint8_t, CHANNELS>& other) : Mat_<uint8_t, CHANNELS>(other) {
                DEBUG_OUT("pMat<%s> at %p Copying from %p\n", typeid(uint8_t).name(), this, &other);
            }

            // copy assignment
            pMat& operator = (const Mat_<uint8_t, CHANNELS>& other) {
                if (this != &other) {
                    DEBUG_OUT("pMat<%s> at %p Copying from %p\n", typeid(uint8_t).name(), this, &other);
                    MatOp::assign_pwise(*this, other, [](const uint8_t& val) { return val; });
                }
                return *this;
            }

            // roi
            pMat operator () (const Rect& roi) const {
                // read/write element
                return pMat(*this, roi);
            }
            pMat operator () (const Rectf& roi) const {
                // read/write element
                return pMat(*this, roi);
            }

            void fill(color clr) {
                if constexpr (CHANNELS == 4UL) {
                    foreachPixel([&](uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
                        r = clr.r();
                        g = clr.g();
                        b = clr.b();
                        a = clr.a();
                    });
                } else if constexpr(CHANNELS == 3UL) { // RGB case
                    foreachPixel([&](uint8_t& r, uint8_t& g, uint8_t& b) {
                        r = clr.r();
                        g = clr.g();
                        b = clr.b();
                    });
                } else {
                    MatOp::assign_pwise(*this, *this, [clr](uint8_t _) {
                        return clr.r();
                    });
                }
            }

            template<typename Func>
            void foreachPixel(Func func) {
                // apply function to each pixel
                pMat<CHANNELS> res(rows(), cols());
                auto chanR = this->r();
                auto chanG = this->g();
                auto chanB = this->b();
                if constexpr (CHANNELS == 4UL) {
                    auto chanA = this->a();
                    for (size_t r_ = 0; r_ < rows(); r_++)
                        for (size_t c_ = 0; c_ < cols(); c_++)
                            func(chanR(r_, c_), chanG(r_, c_), chanB(r_, c_), chanA(r_, c_));
                } else { // RGB case
                    for (size_t r_ = 0; r_ < rows(); r_++)
                        for (size_t c_ = 0; c_ < cols(); c_++)
                            func(chanR(r_, c_), chanG(r_, c_), chanB(r_, c_));
                }
            }

            ChannelView<uint8_t, CHANNELS> r() {
                // return red channel
                return this->slice(0);
            }

            uint8_t& r(size_t row, size_t col) {
                // return red channel
                return operator()(row, col, 0);
            }

            ConstChannelView<uint8_t, CHANNELS> r() const {
                // return red channel
                return this->const_slice(0);
            }

            uint8_t r(size_t row, size_t col) const {
                // return red channel
                return operator()(row, col, 0);
            }

            ChannelView<uint8_t, CHANNELS> g() {
                // return green channel
                static_assert(CHANNELS >= 3UL, "Green channel is only available for 3 channels");
                return this->slice(1);
            }

            uint8_t& g(size_t row, size_t col) {
                // return green channel
                static_assert(CHANNELS >= 3UL, "Green channel is only available for 3 channels");
                return operator()(row, col, 1);
            }

            ConstChannelView<uint8_t, CHANNELS> g() const {
                // return green channel
                static_assert(CHANNELS >= 3UL, "Green channel is only available for 3 channels");
                return this->const_slice(1);
            }

            uint8_t g(size_t row, size_t col) const {
                // return green channel
                static_assert(CHANNELS >= 3UL, "Green channel is only available for 3 channels");
                return operator()(row, col, 1);
            }

            ChannelView<uint8_t, CHANNELS> b() {
                // return blue channel
                static_assert(CHANNELS >= 3UL, "Blue channel is only available for 3 channels");
                return this->slice(2);
            }

            uint8_t& b(size_t row, size_t col) {
                // return blue channel
                static_assert(CHANNELS >= 3UL, "Blue channel is only available for 3 channels");
                return operator()(row, col, 2);
            }

            ConstChannelView<uint8_t, CHANNELS> b() const {
                // return blue channel
                static_assert(CHANNELS >= 3UL, "Blue channel is only available for 3 channels");
                return this->const_slice(2);
            }

            uint8_t b(size_t row, size_t col) const {
                // return blue channel
                static_assert(CHANNELS >= 3UL, "Blue channel is only available for 3 channels");
                return operator()(row, col, 2);
            }

            ChannelView<uint8_t, CHANNELS> a() {
                // return alpha channel
                static_assert(CHANNELS == 4UL, "Alpha channel is only available for 4 channels");
                return this->slice(3);
            }

            uint8_t& a(size_t row, size_t col) {
                // return alpha channel
                static_assert(CHANNELS == 4UL, "Alpha channel is only available for 4 channels");
                return operator()(row, col, 3);
            }

            ConstChannelView<uint8_t, CHANNELS> a() const {
                // return alpha channel
                static_assert(CHANNELS == 4UL, "Alpha channel is only available for 4 channels");
                return this->const_slice(3);
            }

            uint8_t a(size_t row, size_t col) const {
                // return alpha channel
                static_assert(CHANNELS == 4UL, "Alpha channel is only available for 4 channels");
                return operator()(row, col, 3);
            }
    };

    // other
}

// ############################################### //
// ############## FastMath Defines ############### //
// ############################################### //

#include <immintrin.h>
namespace mcv {
    // fast math defines

    inline static float fast_exp(float x) {
        x = 1.0F + x / 256.0F;
        x *= x; x *= x; x *= x; x *= x;
        x *= x; x *= x; x *= x; x *= x;
        return x;
    }

    inline static float fast_exp(double x) {
        x = 1.0 + x / 1024.0;
        x *= x; x *= x; x *= x; x *= x;
        x *= x; x *= x; x *= x; x *= x;
        x *= x; x *= x;
        return static_cast<float>(x);
    }
    
    class fast_dft {
        using cplx_t = std::complex<double>;
    private:
        std::unique_ptr<cplx_t[]> dft2d_data_;
        std::unique_ptr<double[]> mag_data_;

        size_t dft_rows_;
        size_t dft_cols_;

        static int log2_uint32(uint32_t n) {
            int k = 0;
            while ((1U << k) <= n)
                ++k;
            return k - 1;
        }

        static void bit_permute(cplx_t* a, uint32_t n, int k) {
            for (uint32_t i = 0; i < n; ++i) {
                uint32_t t = i;
                t = ((t >> 1) & 0x55555555) | ((t & 0x55555555) << 1);
                t = ((t >> 2) & 0x33333333) | ((t & 0x33333333) << 2);
                t = ((t >> 4) & 0x0F0F0F0F) | ((t & 0x0F0F0F0F) << 4);
                t = ((t >> 8) & 0x00FF00FF) | ((t & 0x00FF00FF) << 8);
                t = (t >> 16) | (t << 16);
                t >>= (32 - k);

                if (i < t) {
                    std::swap(a[i], a[t]);
                }
            }
        }
    public:
        fast_dft() : dft_rows_(0), dft_cols_(0) {}
        ~fast_dft() {
            dft2d_data_.reset();
            mag_data_.reset();
        }
        fast_dft(const fast_dft&) = delete;
        fast_dft& operator=(const fast_dft&) = delete;
        fast_dft(fast_dft&&) = default;
        fast_dft& operator=(fast_dft&&) = default;

        Shape getDFTShape(Shape imshape) {
            dft_rows_ = 1U;
            while (dft_rows_ < imshape.rows_) {
                dft_rows_ <<= 1;
            }
            dft_cols_ = 1U;
            while (dft_cols_ < imshape.cols_) {
                dft_cols_ <<= 1;
            }
            return Shape(dft_rows_, dft_cols_, 1);
        }
        
        static void dft1d(cplx_t* x, uint32_t N, bool inverse) {
            int k = log2_uint32(N);
            if ((N & (N - 1)) != 0) {
                throw std::invalid_argument("N should be power of 2");
            }
            bit_permute(x, N, k);

            // Butterfly operation
            for (uint32_t len = 2; len <= N; len <<= 1) {
                cplx_t wn = std::polar(1.0, (inverse ? -2.0 : 2.0) * M_PI / len);
                for (uint32_t i = 0; i < N; i += len) {
                    cplx_t w = cplx_t(1.0, 0.0);
                    for (uint32_t j = 0; j < len / 2; j++) {
                        cplx_t u = x[i + j];
                        cplx_t t = w * x[i + j + len / 2];

                        x[i + j] = u + t;
                        x[i + j + len / 2] = u - t;

                        w *= wn;
                    }
                }
            }

            // If inverse, divide by N
            if (inverse) {
                for (uint32_t i = 0; i < N; i++) {
                    x[i] /= N;
                }
            }
        }

        static void dft2d(cplx_t* mat, uint32_t rows, uint32_t cols, bool inverse) {
            // 2D FFT implementation
            // ...
            if ((rows & (rows - 1)) != 0) {
                throw std::invalid_argument("Rows should be power of 2");
            }
            if ((cols & (cols - 1)) != 0) {
                throw std::invalid_argument("Cols should be power of 2");
            }

            // Temporary buffer
            std::unique_ptr<cplx_t[]> temp(new cplx_t[std::max(rows, cols)]);

            // FFT on row
                for (size_t i = 0; i < rows; i++) {
                    fast_dft::dft1d(mat + i * cols, cols, inverse);
                }

                // FFT on column
                for (size_t j = 0; j < cols; j++) {
                    for (size_t i = 0; i < rows; i++) {
                        temp[i] = mat[i * cols + j];
                    }
                    fast_dft::dft1d(temp.get(), rows, inverse);
                    for (size_t i = 0; i < rows; i++) {
                        mat[i * cols + j] = temp[i];
                    }
                }
        }

        template<typename T>
        void forward(const Mat_<T, 1>& mat) {
            getDFTShape(mat.shape());
            size_t imrows = mat.rows();
            size_t imcols = mat.cols();
            dft2d_data_ = std::make_unique<cplx_t[]>(dft_rows_ * dft_cols_);
            for (size_t i = 0; i < dft_rows_; i++) {
                for (size_t j = 0; j < dft_cols_; j++) {
                    if (i < imrows && j < imcols) {
                        dft2d_data_[i * dft_cols_ + j] = cplx_t(mat(i, j));
                    } else {
                        dft2d_data_[i * dft_cols_ + j] = cplx_t(0.0, 0.0);
                    }
                }
            }
            mag_data_ = std::make_unique<double[]>(dft_rows_ * dft_cols_);
        }

        template<typename T>
        void computeDFT(Mat_<T, 1>& result, bool shift = true) {
            if (!dft2d_data_) {
                throw std::runtime_error("DFT data not initialized");
            }
            #ifdef MCV_BENCH
            BENCH_START
            #endif
            fast_dft::dft2d(dft2d_data_.get(), dft_rows_, dft_cols_, false);
            
            if (shift) {
                fast_dft::fftshift(dft2d_data_.get(), dft_rows_, dft_cols_);
            }
            #ifdef MCV_BENCH
            BENCH_END
            #endif
            if constexpr (std::is_same_v<T, cplx_t>) { // return frequency domain
                for (size_t i = 0; i < dft_rows_ * dft_cols_; i++) {
                    result[i] = dft2d_data_[i];
                }
            }
            else if constexpr (std::is_same_v<T, uint8_t>) { // return normalized magnitude
                fast_dft::normalize(dft2d_data_.get(), mag_data_.get(), dft_rows_, dft_cols_);
                for (size_t i = 0; i < dft_rows_ * dft_cols_; i++) {
                    result[i] = static_cast<T>(mag_data_[i]);
                }
            } else {
                for (size_t i = 0; i < dft_rows_ * dft_cols_; i++) {
                    result[i] = static_cast<T>(dft2d_data_[i].real());
                }
            }
        }

        template<typename T>
        void computeIDFT(Mat_<T, 1> result, bool shift = true) {
            if (!dft2d_data_) {
                throw std::runtime_error("DFT data not initialized");
            }
            if (shift) {
                fast_dft::fftshift(dft2d_data_.get(), dft_rows_, dft_cols_);
            }
            fast_dft::dft2d(dft2d_data_.get(), dft_rows_, dft_cols_, true);
            for (size_t i = 0; i < dft_rows_ * dft_cols_; i++) {
                result[i] = static_cast<T>(dft2d_data_[i].real());
            }
        }

        static void fftshift(cplx_t* x, uint32_t rows, uint32_t cols) {
            ASSERT_NULL(x, "FFT data is null");

            ASSERTS(rows % 2 == 0, "Rows should be even");
            ASSERTS(cols % 2 == 0, "Cols should be even");

            uint32_t halfR = rows >> 1;
            uint32_t halfC = cols >> 1;

            for (uint32_t i = 0; i < halfR; i++) {
                for (uint32_t j = 0; j < halfC; j++) {
                    std::swap(x[i * cols + j], x[(i + halfR) * cols + (j + halfC)]);
                }
            }
            for (uint32_t i = 0; i < halfR; i++) {
                for (uint32_t j = halfC; j < cols; j++) {
                    std::swap(x[i * cols + j], x[(i + halfR) * cols + (j - halfC)]);
                }
            }    

        }

        static void lowpassfilter(cplx_t* x, uint32_t rows, uint32_t cols, double cutoff) {
            // Low-pass filter
            // ...
            double cutoff2 = cutoff * cutoff;
            double ctX = cols / 2.0;
            double ctY = rows / 2.0;
            for (uint32_t i = 0; i < rows; i++) {
                for (uint32_t j = 0; j < cols; j++) {
                    double dist2 = ((i - ctY) * (i - ctY)) + ((j - ctX) * (j - ctX));
                    if (dist2 > cutoff2) {
                        x[i * cols + j] = cplx_t(0.0, 0.0);
                    }
                }
            }
        }

        static void highpassfilter(cplx_t* x, uint32_t rows, uint32_t cols, double cutoff) {
            // High-pass filter
            // ...
            double cutoff2 = cutoff * cutoff;
            double ctX = cols / 2.0;
            double ctY = rows / 2.0;
            for (uint32_t i = 0; i < rows; i++) {
                for (uint32_t j = 0; j < cols; j++) {
                    double dist2 = ((i - ctY) * (i - ctY)) + ((j - ctX) * (j - ctX));
                    if (dist2 <= cutoff2) {
                        x[i * cols + j] = cplx_t(0.0, 0.0);
                    }
                }
            }
        }

        static void normalize(const cplx_t* x, double* mag, uint32_t rows, uint32_t cols) {
            // Magnitude calculation
            // ...
            double maxMag = 0.0;
            for (uint32_t idx = 0; idx < rows * cols; idx++) {
                mag[idx] = log10(1.0 + abs(x[idx]));
                if (mag[idx] > maxMag) {
                    maxMag = mag[idx];
                }
            }
            // Normalize to [0, 255]
            for (uint32_t idx = 0; idx < rows * cols; idx++) {
                mag[idx] = (mag[idx] / maxMag) * 255.0;
            }
        }
    };

    class fast_toGray {
    private:
        #define R_COEF 0.299
        #define G_COEF 0.587
        #define B_COEF 0.114

        static void rgb2gray(const uint8_t* src, uint8_t* dst, size_t rows, size_t cols) {
            constexpr int B_WT = int(256 * B_COEF + 0.5);
            constexpr int G_WT = int(256 * G_COEF + 0.5);
            constexpr int R_WT = 256 - B_WT - G_WT;

            for (size_t i = 0; i < rows; ++i) {
                const uint8_t* ps = src + i * cols * 3;
                uint8_t* pd = dst + i * cols;
                #if (true)
                size_t j = 0;
                for (; j < cols - 12; j += 12, ps += 36) {
                    __m128i p1aL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 0))), _mm_setr_epi16(R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT)); //1
                    __m128i p2aL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 1))), _mm_setr_epi16(G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT)); //2
                    __m128i p3aL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 2))), _mm_setr_epi16(B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT)); //3

                    __m128i p1aH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 8))), _mm_setr_epi16(B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT));//4
                    __m128i p2aH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 9))), _mm_setr_epi16(G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT));//5
                    __m128i p3aH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 10))), _mm_setr_epi16(R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT));//6

                    __m128i p1bL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 18))), _mm_setr_epi16(R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT));//7
                    __m128i p2bL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 19))), _mm_setr_epi16(G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT));//8
                    __m128i p3bL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 20))), _mm_setr_epi16(B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT));//9

                    __m128i p1bH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 26))), _mm_setr_epi16(B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT));//10
                    __m128i p2bH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 27))), _mm_setr_epi16(G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT, B_WT));//11
                    __m128i p3bH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(ps + 28))), _mm_setr_epi16(R_WT, G_WT, B_WT, R_WT, G_WT, B_WT, R_WT, G_WT));//12

                    __m128i sumaL = _mm_add_epi16(p3aL, _mm_add_epi16(p1aL, p2aL));//13
                    __m128i sumaH = _mm_add_epi16(p3aH, _mm_add_epi16(p1aH, p2aH));//14
                    __m128i sumbL = _mm_add_epi16(p3bL, _mm_add_epi16(p1bL, p2bL));//15
                    __m128i sumbH = _mm_add_epi16(p3bH, _mm_add_epi16(p1bH, p2bH));//16
                    __m128i sclaL = _mm_srli_epi16(sumaL, 8);//17
                    __m128i sclaH = _mm_srli_epi16(sumaH, 8);//18
                    __m128i sclbL = _mm_srli_epi16(sumbL, 8);//19
                    __m128i sclbH = _mm_srli_epi16(sumbH, 8);//20
                    __m128i shftaL = _mm_shuffle_epi8(sclaL, _mm_setr_epi8(0, 6, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));//21
                    __m128i shftaH = _mm_shuffle_epi8(sclaH, _mm_setr_epi8(-1, -1, -1, 18, 24, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));//22
                    __m128i shftbL = _mm_shuffle_epi8(sclbL, _mm_setr_epi8(-1, -1, -1, -1, -1, -1, 0, 6, 12, -1, -1, -1, -1, -1, -1, -1));//23
                    __m128i shftbH = _mm_shuffle_epi8(sclbH, _mm_setr_epi8(-1, -1, -1, -1, -1, -1, -1, -1, -1, 18, 24, 30, -1, -1, -1, -1));//24
                    __m128i accumL = _mm_or_si128(shftaL, shftbL);//25
                    __m128i accumH = _mm_or_si128(shftaH, shftbH);//26
                    __m128i h3 = _mm_or_si128(accumL, accumH);//27
                    //__m128i h3 = _mm_blendv_epi8(accumL, accumH, _mm_setr_epi8(0, 0, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, 1, 1, 1, 1));
                    _mm_storeu_si128((__m128i *)(pd + j), h3);//28
                }
                for (; j < cols; j++, ps += 3) {//29
                    pd[j] = (R_WT * ps[0] + G_WT * ps[1] + B_WT * ps[2]) >> 8;//30
                }
                #else
                for (size_t j = 0; j < cols; ++j, ps += 3) {
                    pd[j] = (ps[0] * R_WT + ps[1] * G_WT + ps[2] * B_WT) >> 8;
                }
                #endif
            }
        }

        static void rgba2gray(const uint8_t* src, uint8_t* dst, size_t rows, size_t cols) {
            int R_WT = int(256 * R_COEF + 0.5);
            int G_WT = int(256 * G_COEF + 0.5);
            int B_WT = int(256 * B_COEF + 0.5);
            for (size_t i = 0; i < rows; ++i) {
                const uint8_t* ps = src + i * 4 * cols;
                uint8_t* pd = dst + i * cols;
                for (size_t j = 0; j < cols; ++j) {
                    int r = ps[j * 4 + 0];
                    int g = ps[j * 4 + 1];
                    int b = ps[j * 4 + 2];
                    pd[j] = (r * R_WT + g * G_WT + b * B_WT) >> 8;
                }
            }
        }
    public:
        template<size_t CHANNELS>
        static void compute(const Mat_<uint8_t, CHANNELS>& src, Mat_<uint8_t, 1>& dst) {
            static_assert(CHANNELS >= 3, "At least RGB or RGBA images are supported");
            size_t rows = src.rows();
            size_t cols = src.cols();

            #ifdef MCV_BENCH
            BENCH_START;
            #endif
            if constexpr (CHANNELS == 3) {
                rgb2gray(src.get(), dst.get(), rows, cols);
            } else if constexpr (CHANNELS == 4) {
                rgba2gray(src.get(), dst.get(), rows, cols);
            }
            #ifdef MCV_BENCH
            BENCH_END;
            #endif
        }
    };
}

// ############################################### //
// ############## ImgProc Functions ############## //
// ############################################### //

namespace mcv {

    typedef Mat_<std::complex<double>, 1> FreqMat;
    typedef Mat_<float, 1> Matf;
    typedef Mat_<double, 1> Matd;

    // function declarations
    template<size_t CHANNELS>
    pMat<CHANNELS> imread(const std::string& filename);

    template<size_t CHANNELS>
    pMat<CHANNELS> mix(const pMat<CHANNELS>& src1, const pMat<CHANNELS>& src2, float alpha = 0.5F);
    
    template<size_t CHANNELS=3UL>
    pMat<1> dft(const pMat<CHANNELS>& src, bool shift = true);

    template<size_t CHANNELS=3UL>
    FreqMat dftNoNormalize(const pMat<CHANNELS>& src, bool shift = true);

    void processLowpass(FreqMat& src, double cutoff);
    void processHighpass(FreqMat& src, double cutoff);

    pMat<1> idft(const FreqMat& src, bool shift = true);

    template<typename T, size_t CHANNELS=3UL, size_t K>
    pMat<CHANNELS> conv2d(const pMat<CHANNELS>& src, Matx<T, K, K> kernel, size_t stride = 1, bool padding = true) {
        Shape outShape = MatOp::getConv2dOutShape(src.shape(), Shape(K, K, 1), stride, padding);
        pMat<CHANNELS> dst(outShape);
        #ifdef MCV_BENCH
        BENCH_START;
        #endif
        for (size_t C = 0; C < CHANNELS; ++C) {
            auto ch = src.const_slice(C);
            dst.slice(C) = MatOp::conv2d<uint8_t, uint8_t, T, 1>(ch, kernel, stride, padding);
        }
        #ifdef MCV_BENCH
        BENCH_END;
        #endif
        return dst;
    }

    template <size_t CHANNELS=3UL>
    pMat<CHANNELS> conv2d(const pMat<CHANNELS>& src, const Matf& kernel, size_t stride = 1, bool padding = true) {
        Shape outShape = MatOp::getConv2dOutShape(src.shape(), kernel.shape(), stride, padding);
        pMat<CHANNELS> dst(outShape);
        #ifdef MCV_BENCH
        BENCH_START;
        #endif
        for (size_t C = 0; C < CHANNELS; ++C) {
            auto ch = src.const_slice(C);
            dst.slice(C) = MatOp::conv2d<uint8_t, uint8_t, float, 1>(ch, kernel, stride, padding);
        }
        #ifdef MCV_BENCH
        BENCH_END;
        #endif
        return dst;
    }

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> conv2d(const pMat<CHANNELS>& src, const Matd& kernel, size_t stride = 1, bool padding = true) {
        Shape outShape = MatOp::getConv2dOutShape(src.shape(), kernel.shape(), stride, padding);
        pMat<CHANNELS> dst(outShape);
        #ifdef MCV_BENCH
        BENCH_START;
        #endif
        for (size_t C = 0; C < CHANNELS; ++C) {
            auto ch = src.const_slice(C);
            dst.slice(C) = MatOp::conv2d<uint8_t, uint8_t, double, 1>(ch, kernel, stride, padding);
        }
        #ifdef MCV_BENCH
        BENCH_END;
        #endif
        return dst;
    }

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> adjustBright(const pMat<CHANNELS>& src, int magnitude = 0, bool isSaturated = true);

    template<size_t CHANNELS=3UL>
    void imwrite(const std::string& filename, const pMat<CHANNELS>& img);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> cvtColor(const pMat<CHANNELS>& src, color_conversion code);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> applyAffineTransform(
                    const pMat<CHANNELS>& src, 
                    const AffineMat& affine,
                    size_t newCols,
                    size_t newRows);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> rotate(const pMat<CHANNELS>& src, double angle);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> scale(const pMat<CHANNELS>& src, double scaleX, double scaleY);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> translate(const pMat<CHANNELS>& src, double translateX, double translateY);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> flip(const pMat<CHANNELS>& src, bool flipX, bool flipY);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> line(const pMat<CHANNELS>& src, 
                        Line line,
                        color clr = 0xFF000000);
    
    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> circle(const pMat<CHANNELS>& src, 
                        Circle circle,
                        color clr = 0xFF000000);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> rect(const pMat<CHANNELS>& src, 
                        Rect rect,
                        color clr = 0xFF000000);

    template<size_t CHANNELS=3UL>
    pMat<CHANNELS> clip(const pMat<CHANNELS>& src, 
                        Rect rect);

    template<size_t CHANNELS=3UL>
    pMat<1> im2gray(const pMat<CHANNELS>& src);

    pMat<3> discardAlpha(const pMat<4>& src);
    pMat<4> mergeAlpha(const pMat<3>& src, const pMat<1>& alpha);

    template<size_t CHANNELS=3UL>
    pMat<1> Sobel(const pMat<CHANNELS>& src);

    template<size_t CHANNELS=3UL>
    void asciiArt(const pMat<CHANNELS>& src, 
                        int width = -1,
                        int height = -1,
                        const std::string& charset = "@#%*+=-:. ");

}

#endif // MCV_DECL_H_