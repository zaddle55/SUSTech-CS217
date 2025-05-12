#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "mcv.decl.h"

const color color::BLACK = color::from_rgba(0, 0, 0, 255);
const color color::WHITE = color::from_rgba(255, 255, 255, 255);
const color color::RED = color::from_rgba(255, 0, 0, 255);
const color color::GREEN = color::from_rgba(0, 255, 0, 255);
const color color::BLUE = color::from_rgba(0, 0, 255, 255);
const color color::YELLOW = color::from_rgba(255, 255, 0, 255);
const color color::CYAN = color::from_rgba(0, 255, 255, 255);
const color color::GRAY = color::from_rgba(128, 128, 128, 255);

namespace mcv {

    template<size_t CHANNELS>
    pMat<CHANNELS> imread(const std::string &filename) {
        int width, height, channels;
        uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, CHANNELS);
        if (!data) {
            throw std::runtime_error("Failed to load image: " + filename + " because " + stbi_failure_reason());
        }
        
        pMat<CHANNELS> img(height, width);
        memcpy(img.get(), data, width * height * CHANNELS);
        stbi_image_free(data);
        
        return img;
    }
    
    template<size_t CHANNELS>
    pMat<CHANNELS> mix(const pMat<CHANNELS>& src1, const pMat<CHANNELS>& src2, float alpha) {
        pMat<CHANNELS> dst(src1.rows(), src1.cols());
        MatOp::op_pwise(dst, src1, src2, 
            [alpha](uint8_t a, uint8_t b) {
                return static_cast<uint8_t>(std::clamp(static_cast<int>(a * alpha + b * (1 - alpha)), 0, 255));
            });
        return dst;
    }

    void write2path(const std::string& filename, 
                    const uint8_t* data, 
                    int width, 
                    int height,
                    int channels,
                    const std::string& format) {
        if (format == "png") {
            stbi_write_png(filename.c_str(), width, height, channels, data, width * channels);
        } else if (format == "jpg" || format == "jpeg") {
            stbi_write_jpg(filename.c_str(), width, height, channels, data, 100);
        } else if (format == "bmp") {
            stbi_write_bmp(filename.c_str(), width, height, channels, data);
        } else if (format == "tga") {
            stbi_write_tga(filename.c_str(), width, height, channels, data);
        } else {
            throw std::runtime_error("Unsupported image format: " + format);
        }
    }

    template<size_t CHANNELS>
    void imwrite(const std::string& filename, const pMat<CHANNELS>& img) {
        std::string format = filename.substr(filename.find_last_of('.') + 1);
        DEBUG_OUT("Writing image to %s with format %s\n", filename.c_str(), format.c_str());
        if (format.empty()) {
            throw std::runtime_error("Filename does not contain an extension: " + filename);
        }
        
        write2path(filename, img.get(), img.cols(), img.rows(), CHANNELS, format);
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> adjustBright(const pMat<CHANNELS>& src, int magnitude, bool isSaturated) {
        if constexpr (CHANNELS == 3) {
            return src.foreach([&](uint8_t val) {
                int newVal = static_cast<int>(val) + magnitude;
                if (isSaturated) {
                    return static_cast<uint8_t>(std::clamp(newVal, 0, 255));
                } else {
                    return static_cast<uint8_t>(newVal);
                }
            });
        } else if constexpr (CHANNELS == 4) {
            pMat<CHANNELS> dst(src.rows(), src.cols());
            dst = src.deepcopy();
            dst.a() = dst.a().foreach([&](uint8_t val) {
                int newVal = static_cast<int>(val) + magnitude;
                if (isSaturated) {
                    return static_cast<uint8_t>(std::clamp(newVal, 0, 255));
                } else {
                    return static_cast<uint8_t>(newVal);
                }
            });
            return dst;
        } else {
            // Handle other channel count cases or throw an error
            throw std::invalid_argument("Unsupported number of channels for adjustBright");
        }
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> cvtColor(const pMat<CHANNELS>& src, color_conversion code) {
        pMat<CHANNELS> dst = src.deepcopy();
        #ifdef MCV_BENCH
        BENCH_START
        #endif
        switch (code) {
            case RGB2BGR:           
            case BGR2RGB:
                dst.foreachPixel([](uint8_t &r, uint8_t &g, uint8_t &b) {
                    std::swap(r, b);
                });
                break;
            case RGB2HSV:
                dst.foreachPixel([](uint8_t &r, uint8_t &g, uint8_t &b) {
                    color rgb = color::from_rgb(r, g, b);
                    color hsv = color::rgb2hsv(rgb);
                    r = hsv.h(); g = hsv.s(); b = hsv.v();
                });
                break;
            case HSV2RGB:
                dst.foreachPixel([](uint8_t &h, uint8_t &s, uint8_t &v) {
                    color hsv = color::from_hsv(h, s, v);
                    color rgb = color::hsv2rgb(hsv);
                    h = rgb.r(); s = rgb.g(); v = rgb.b();
                });
                break;
            case RGB2YUV:
                dst.foreachPixel([](uint8_t &r, uint8_t &g, uint8_t &b) {
                    color rgb = color::from_rgb(r, g, b);
                    color yuv = color::rgb2yuv(rgb);
                    r = yuv.y(); g = yuv.u(); b = yuv.v();
                });
                break;
            case YUV2RGB:
                dst.foreachPixel([](uint8_t &y, uint8_t &u, uint8_t &v) {
                    color yuv = color::from_yuv(y, u, v);
                    color rgb = color::yuv2rgb(yuv);
                    y = rgb.r(); u = rgb.g(); v = rgb.b();
                });
                break;
            case RGB2YCbCr:
                dst.foreachPixel([](uint8_t &r, uint8_t &g, uint8_t &b) {
                    color rgb = color::from_rgb(r, g, b);
                    color ycbcr = color::rgb2ycbcr(rgb);
                    r = ycbcr.y(); g = ycbcr.cb(); b = ycbcr.cr();
                });
                break;
            case YCbCr2RGB:
                dst.foreachPixel([](uint8_t &y, uint8_t &cb, uint8_t &cr) {
                    color ycbcr = color::from_ycbcr(y, cb, cr);
                    color rgb = color::ycbcr2rgb(ycbcr);
                    y = rgb.r(); cb = rgb.g(); cr = rgb.b();
                });
                break;
            case RGB2HLS:
                dst.foreachPixel([](uint8_t &r, uint8_t &g, uint8_t &b) {
                    color rgb = color::from_rgb(r, g, b);
                    color hls = color::rgb2hls(rgb);
                    r = hls.h(); g = hls.l(); b = hls.s();
                });
                break;
            case HLS2RGB:
                dst.foreachPixel([](uint8_t &h, uint8_t &l, uint8_t &s) {
                    color hls = color::from_hls(h, l, s);
                    color rgb = color::hls2rgb(hls);
                    h = rgb.r(); l = rgb.g(); s = rgb.b();
                });
                break;
            default:
                throw std::invalid_argument("Unsupported color conversion code");
        }
        #ifdef MCV_BENCH
        BENCH_END
        #endif
        return dst;
    }

    uint8_t bilinearInterpolation(const Mat_<uint8_t, 1>& chan, double x, double y) {
        int x1 = static_cast<int>(x);
        int y1 = static_cast<int>(y);
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        if (x2 >= chan.cols() || y2 >= chan.rows()) {
            return 0.0; // Out of bounds
        }

        double a = x - x1;
        double b = y - y1;

        double val = (1 - a) * (1 - b) * chan(y1, x1) +
               a * (1 - b) * chan(y1, x2) +
               (1 - a) * b * chan(y2, x1) +
               a * b * chan(y2, x2);
        return static_cast<uint8_t>(val);
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> applyAffineTransform(
                    const pMat<CHANNELS>& src, 
                    const AffineMat& affine,
                    size_t newCols,
                    size_t newRows) {
        pMat<CHANNELS> dst(newRows, newCols);
        dst.fill(0x00000000U); // Fill with black or transparent color

        AffineMat inv = affine.inv();

        for (size_t y = 0; y < newRows; ++y) {
            for (size_t x = 0; x < newCols; ++x) {
                double srcX = inv.a * x + inv.b * y + inv.c;
                double srcY = inv.d * x + inv.e * y + inv.f;

                if (srcX >= 0 && srcX < src.cols() - 1 && srcY >= 0 && srcY < src.rows() - 1) {
                    dst.r(y, x) = bilinearInterpolation(src.r(), srcX, srcY);
                    if constexpr (CHANNELS > 1) {
                        dst.g(y, x) = bilinearInterpolation(src.g(), srcX, srcY);
                        dst.b(y, x) = bilinearInterpolation(src.b(), srcX, srcY);
                    }
                    if constexpr (CHANNELS == 4) {
                        dst.a(y, x) = bilinearInterpolation(src.a(), srcX, srcY);
                    }
                }
            }
        }
        return dst;
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> rotate(const pMat<CHANNELS>& src, double angle) {
        double rad = angle * M_PI / 180.0;
        double cosTheta = cos(rad);
        double sinTheta = sin(rad);

        auto newCols = static_cast<size_t>(abs(src.cols() * cosTheta) + abs(src.rows() * sinTheta));
        auto newRows = static_cast<size_t>(abs(src.rows() * cosTheta) + abs(src.cols() * sinTheta));

        double centerX = src.cols() / 2.0;
        double centerY = src.rows() / 2.0;
        double newCenterX = newCols / 2.0;
        double newCenterY = newRows / 2.0;
        double translateX = newCenterX - centerX * cosTheta + centerY * sinTheta;
        double translateY = newCenterY - centerX * sinTheta - centerY * cosTheta;

        AffineMat affine(angle, 1.0, 1.0, translateX, translateY);
        return applyAffineTransform(src, affine, newCols, newRows);
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> scale(const pMat<CHANNELS>& src, double scaleX, double scaleY) {
        auto newCols = static_cast<size_t>(src.cols() * scaleX);
        auto newRows = static_cast<size_t>(src.rows() * scaleY);

        AffineMat affine(0, scaleX, scaleY, 0, 0);
        return applyAffineTransform(src, affine, newCols, newRows);
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> translate(const pMat<CHANNELS>& src, double translateX, double translateY) {
        AffineMat affine(0, 1.0, 1.0, translateX, translateY);
        return applyAffineTransform(src, affine, src.cols(), src.rows());
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> flip(const pMat<CHANNELS>& src, bool flipX, bool flipY) {
        AffineMat affine;
        if (flipX) {
            affine = AffineMat(0, -1.0, 1.0, src.cols(), 0);
        } else if (flipY) {
            affine = AffineMat(0, 1.0, -1.0, 0, src.rows());
        } else if (flipX && flipY) {
            affine = AffineMat(0, -1.0, -1.0, src.cols(), src.rows());
        } else {
            return src.deepcopy(); // No flipping
        }
        return applyAffineTransform(src, affine, src.cols(), src.rows());
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> line(const pMat<CHANNELS>& src, Line line, color clr) {
        pMat<CHANNELS> dst = src.deepcopy();
        int x0 = line.x0;
        int y0 = line.y0;
        int x1 = line.x1;
        int y1 = line.y1;
        double stroke = (line.stroke < 0.5) ? 0.5 : line.stroke;

        // Bresenham's line algorithm
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        double ed = dx + dy == 0 ? 1.0 : sqrtf((double)dx * dx + (double)dy * dy);
        double wd = (stroke + 1.0) / 2.0; // Half stroke width

        #define BLEND_PIXEL(px, py) \
        do { \
            if ((px) >= 0 && (px) < dst.cols() && (py) >= 0 && (py) < dst.rows()) { \
                dst.r(py, px) = clr.r(); \
                dst.g(py, px) = clr.g(); \
                dst.b(py, px) = clr.b(); \
                if constexpr (CHANNELS == 4) { \
                    dst.a(py, px) = clr.a(); \
                } \
            } \
        } while(0)                                               \

        while (true) {
            BLEND_PIXEL(x0, y0);

            int e2 = err, x2 = x0;

            // X step
            if (2 * e2 >= -dx) {
                e2 += dy;
                for (int y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx) {
                    y2 += sy;
                    BLEND_PIXEL(x0, y2);
                }

                if (x0 == x1) break;
                e2 = err;
                err -= dy;
                x0 += sx;
            }

            // Y step
            if (2 * e2 <= dy) {
                for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy) {
                    x2 += sx;
                    BLEND_PIXEL(x2, y0);
                }
                if (y0 == y1) break;
                err += dx;
                y0 += sy;
            }
        }
        #undef BLEND_PIXEL
        return dst;
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> rect(const pMat<CHANNELS>& src, Rect rect, color clr) {
        pMat<CHANNELS> dst = src.deepcopy();
        pMat<CHANNELS> region = dst(rect);
        region.fill(clr);

        return dst;
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> circle(const pMat<CHANNELS>& src, Circle circle, color clr) {
        pMat<CHANNELS> dst = src.deepcopy();
        int r = circle.radius;
        int x = r, y = 0, err = 0;
        int i;

        #define BLEND_PIXEL(px, py, alpha) \
        do { \
            if constexpr (CHANNELS == 4) { \
                if ((px) >= 0 && (px) < dst.cols() && (py) >= 0 && (py) < dst.rows()) { \
                    dst.r(py, px) = clr.r(); \
                    dst.g(py, px) = clr.g(); \
                    dst.b(py, px) = clr.b(); \
                    dst.a(py, px) = static_cast<uint8_t>(255 * (alpha)); \
                } \
            } else { \
                if ((px) >= 0 && (px) < dst.cols() && (py) >= 0 && (py) < dst.rows()) { \
                    dst.r(py, px) = \
                        static_cast<uint8_t>(src.r(py, px) * (1.0f - (alpha)) + clr.r() * (alpha)); \
                    dst.g(py, px) = \
                        static_cast<uint8_t>(src.g(py, px) * (1.0f - (alpha)) + clr.g() * (alpha)); \
                    dst.b(py, px) = \
                        static_cast<uint8_t>(src.b(py, px) * (1.0f - (alpha)) + clr.b() * (alpha)); \
                } \
            } \
        } while(0)

        for (r = 2 * r + 1; x > 0; err += ++y * 2 - 1) {
            if (err + 2 * x + 1 < r)
                err += ++x * 2 - 1;
            for (; err > 0; err -= --x * 2 + 1) {
                double alpha = static_cast<double>(err) / r;
                BLEND_PIXEL(circle.rx + x, circle.ry + y, 1.0 - alpha);
                BLEND_PIXEL(circle.rx - y, circle.ry + x, 1.0 - alpha);
                BLEND_PIXEL(circle.rx - x, circle.ry - y, 1.0 - alpha);
                BLEND_PIXEL(circle.rx + y, circle.ry - x, 1.0 - alpha);
            }
            for (i = x; i > 0; i--) {
                BLEND_PIXEL(circle.rx + i, circle.ry + y, 1.0);
                BLEND_PIXEL(circle.rx - y, circle.ry + i, 1.0);
                BLEND_PIXEL(circle.rx - i, circle.ry - y, 1.0);
                BLEND_PIXEL(circle.rx + y, circle.ry - i, 1.0);
            }
        }
        BLEND_PIXEL(circle.rx, circle.ry, 1.0);
        #undef BLEND_PIXEL
        return dst;
    }

    template<size_t CHANNELS>
    pMat<CHANNELS> clip(const pMat<CHANNELS>& src, Rect rect) {
        if (rect.x < 0 || rect.y < 0 || rect.x + rect.dx > src.cols() || rect.y + rect.dy > src.rows()) {
            throw std::out_of_range("Clip rectangle is out of bounds");
        }
        return src(rect).deepcopy();
    }

    template<size_t CHANNELS>
    pMat<1> im2gray(const pMat<CHANNELS>& src) {
         if constexpr (CHANNELS == 1) {
            return src;
        } else {
            pMat<1> dst(src.rows(), src.cols());
            fast_toGray::compute(src, dst);
            return dst;
        }
    }

    template<size_t CHANNELS>
    pMat<1> dft(const pMat<CHANNELS>& src, bool shift) {
        pMat<1> grayScale = im2gray(src);
        fast_dft proc;
        pMat<1> dst(proc.getDFTShape(grayScale.shape()));
        proc.forward(grayScale);
        proc.computeDFT(dst, shift);
        return dst;
    }

    template<size_t CHANNELS>
    FreqMat dftNoNormalize(const pMat<CHANNELS>& src, bool shift) {
        pMat<1> grayScale = im2gray(src);
        fast_dft proc;
        FreqMat dst(proc.getDFTShape(grayScale.shape()));
        proc.forward(grayScale);
        proc.computeDFT(dst, shift);
        return dst;
    }

    void processLowpass(FreqMat& src, double cutoff) {
        ASSERTS(cutoff > 0.0, "Cutoff frequency must be positive");
        fast_dft::lowpassfilter(src.get(), src.rows(), src.cols(), cutoff);
    }

    void processHighpass(FreqMat& src, double cutoff) {
        ASSERTS(cutoff > 0.0, "Cutoff frequency must be positive");
        fast_dft::highpassfilter(src.get(), src.rows(), src.cols(), cutoff);
    }

    pMat<1> idft(const FreqMat& src, bool shift) {
        fast_dft proc;
        pMat<1> dst(proc.getDFTShape(src.shape()));
        proc.forward(src);
        proc.computeIDFT(dst, shift);
        return dst;
    }

    pMat<3> discardAlpha(const pMat<4>& src) {
        return src.split(3);
    }

    pMat<4> addAlpha(const pMat<3>& src, const pMat<1>& alpha) {
        return src.merge(alpha);
    }
    
    template<size_t CHANNELS>
    pMat<1> Sobel(const pMat<CHANNELS>& src) {
        pMat<1> grayScale = im2gray(src);
        pMat<1> featX = conv2d(grayScale, SOBEL_X);
        pMat<1> featY = conv2d(grayScale, SOBEL_Y);

        return mix(featX, featY);
    }

    template<size_t CHANNELS>
    void asciiArt(const pMat<CHANNELS>& src, 
                        int width,
                        int height,
                        const std::string& charset) {
        #define TERM_RATIO 2.5 // width/height ratio of terminal
        if (charset.empty()) {
            throw std::invalid_argument("Charset cannot be empty");
        }

        if (width <= 0 && height <= 0) {
            throw std::invalid_argument("Width and Height must be positive");
        } else if (width <= 0) {
            double scale = static_cast<double>(height) / src.rows();
            width = static_cast<int>(src.cols() * TERM_RATIO * scale);
        } else if (height <= 0) {
            double scale = static_cast<double>(width) / src.cols();
            height = static_cast<int>(src.rows() / TERM_RATIO * scale);
        }

        // perform scaling
        double scaleX = static_cast<double>(width) / src.cols();
        double scaleY = static_cast<double>(height) / src.rows();
        pMat<CHANNELS> scaled = scale(src, scaleX, scaleY);

        // convert to grayscale
        pMat<1> grayScale = im2gray(scaled);

        // create buffer for ASCII art
        std::string buffer;

        for (size_t y = 0; y < grayScale.rows(); ++y) {
            for (size_t x = 0; x < grayScale.cols(); ++x) {
                uint8_t pixelValue = grayScale(y, x);
                size_t index = static_cast<size_t>(pixelValue * (charset.size() - 1) / 255);
                buffer += charset[index];
            }
            buffer += '\n';
        }
        std::cout << buffer;
    }

    // Explicit instantiation for 1, 3 and 4 channels
    template pMat<1> imread<1>(const std::string&);
    template pMat<3> imread<3>(const std::string&);
    template pMat<4> imread<4>(const std::string&);
    
    template pMat<1> mix<1>(const pMat<1>&, const pMat<1>&, float);
    template pMat<3> mix<3>(const pMat<3>&, const pMat<3>&, float);
    template pMat<4> mix<4>(const pMat<4>&, const pMat<4>&, float);

    template void imwrite<1>(const std::string&, const pMat<1>&);
    template void imwrite<3>(const std::string&, const pMat<3>&);
    template void imwrite<4>(const std::string&, const pMat<4>&);

    template pMat<1> adjustBright<1>(const pMat<1>&, int, bool);
    template pMat<3> adjustBright<3>(const pMat<3>&, int, bool);
    template pMat<4> adjustBright<4>(const pMat<4>&, int, bool);

    template pMat<3> cvtColor<3>(const pMat<3>&, color_conversion);
    // template pMat<4> cvtColor<4>(const pMat<4>&, color_conversion);

    template pMat<1> applyAffineTransform<1>(const pMat<1>&, const AffineMat&, size_t, size_t);
    template pMat<3> applyAffineTransform<3>(const pMat<3>&, const AffineMat&, size_t, size_t);
    template pMat<4> applyAffineTransform<4>(const pMat<4>&, const AffineMat&, size_t, size_t);

    template pMat<1> rotate<1>(const pMat<1>&, double);
    template pMat<3> rotate<3>(const pMat<3>&, double);
    template pMat<4> rotate<4>(const pMat<4>&, double);

    template pMat<1> scale<1>(const pMat<1>&, double, double);
    template pMat<3> scale<3>(const pMat<3>&, double, double);
    template pMat<4> scale<4>(const pMat<4>&, double, double);

    template pMat<1> translate<1>(const pMat<1>&, double, double);
    template pMat<3> translate<3>(const pMat<3>&, double, double);
    template pMat<4> translate<4>(const pMat<4>&, double, double);

    template pMat<1> flip<1>(const pMat<1>&, bool, bool);
    template pMat<3> flip<3>(const pMat<3>&, bool, bool);
    template pMat<4> flip<4>(const pMat<4>&, bool, bool);

    template pMat<3> line<3>(const pMat<3>&, Line, color);
    template pMat<4> line<4>(const pMat<4>&, Line, color);

    template pMat<3> rect<3>(const pMat<3>&, Rect, color);
    template pMat<4> rect<4>(const pMat<4>&, Rect, color);

    template pMat<3> circle<3>(const pMat<3>&, Circle, color);
    template pMat<4> circle<4>(const pMat<4>&, Circle, color);

    template pMat<1> clip<1>(const pMat<1>&, Rect);
    template pMat<3> clip<3>(const pMat<3>&, Rect);
    template pMat<4> clip<4>(const pMat<4>&, Rect);

    template pMat<1> im2gray<1>(const pMat<1>&);
    template pMat<1> im2gray<3>(const pMat<3>&);
    template pMat<1> im2gray<4>(const pMat<4>&);

    template pMat<1> dft<1>(const pMat<1>&, bool);
    template pMat<1> dft<3>(const pMat<3>&, bool);
    template pMat<1> dft<4>(const pMat<4>&, bool);
    template FreqMat dftNoNormalize<1>(const pMat<1>&, bool);
    template FreqMat dftNoNormalize<3>(const pMat<3>&, bool);
    template FreqMat dftNoNormalize<4>(const pMat<4>&, bool);

    template pMat<1> Sobel<1>(const pMat<1>&);
    template pMat<1> Sobel<3>(const pMat<3>&);
    template pMat<1> Sobel<4>(const pMat<4>&);

    template void asciiArt<1>(const pMat<1>&, int, int, const std::string&);
    template void asciiArt<3>(const pMat<3>&, int, int, const std::string&);
    template void asciiArt<4>(const pMat<4>&, int, int, const std::string&);

} // namespace mcv