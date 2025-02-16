#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>
#include <memory>

template <typename T>
class Mat
{
    // parameters
    size_t rows_;
    size_t cols_;
    const size_t CHANNELS;
    T *data;

    // reference counting
    std::shared_ptr<T> rc;
private:
    /* data */
public:
    enum class MatException
    {
        INDEX_OUT_OF_BOUNDS,
        INVALID_DIMENSIONS,
        MEMORY_ALLOCATION_FAILED
    };
    Mat(/* args */);
    ~Mat();
};

Mat::Mat(/* args */)
{
}

Mat::~Mat()
{
}


#endif // MATRIX_H_