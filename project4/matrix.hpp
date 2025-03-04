#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>
#include <memory>
#include <algorithm>
#include <initializer_list>
#include <exception>
#include <iostream>
#include <string>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ASSERT_ALLOC(ptr) if (ptr == nullptr) { throw std::bad_alloc(); }
#define ASSERT_ARG(cond, argname) if (!(cond)) { throw std::invalid_argument("Invalid argument on " + std::string(argname)); }
#define ASSERT_DIM(cond, dim) if (!(cond)) { throw std::invalid_argument("Invalid dimension of matrix:" + std::string(dim)); }

#ifdef DEBUG_
#define DEBUG_PRINT(fmt, ...) \
    do { \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) \
    do {} while (0)
#endif

struct Rect {
    int x;
    int y;
    int width;
    int height;
};

template <class T, size_t CHANNELS = 1UL>
class Mat
{
    // parameters
    size_t rows_;
    size_t cols_;
    size_t stride_;
    T *data;

    // reference counting
    size_t* refCount;

    // type alias
    
private:
    /* data */
public:

    enum class BinaryOp
    {
        MAT_ADD,
        MAT_SUB,
        MAT_MUL,
        MAT_INV
    };

    size_t rows() const {
        return rows_;
    }

    size_t cols() const {
        return cols_;
    }

    size_t stride() const {
        return stride_;
    }

    size_t channels() const {
        return CHANNELS;
    }

    size_t size() const {
        return rows_ * cols_ * CHANNELS;
    }

    size_t use_count() const {
        return refCount == nullptr ? 0 : *refCount;
    }

    /// @brief create a matrix with the given dimensions
    /// @param rows
    /// @param cols
    /// @param channels
    explicit Mat(const size_t rows, const size_t cols)
    : rows_(rows), cols_(cols), stride_(cols)
    {
        ASSERT_DIM(rows > 0 && cols > 0, "rows, cols, channels");

        data = new T[rows * cols * CHANNELS];
        ASSERT_ALLOC(data);

        refCount = new size_t(1);
        if (refCount == nullptr)
        {
            std::cerr << "Memory allocation on refCount failed" << std::endl;
            delete[] data;
            throw std::bad_alloc();
        }

        for (size_t i = 0; i < rows * cols * CHANNELS; ++i)
        {
            data[i] = T();
        }
    }

    /// @brief create a matrix with one channel from a 2D initializer list
    /// @param list 
    Mat(const std::initializer_list<std::initializer_list<T>> &list)
    {
        ASSERT_ARG(CHANNELS == 1, "channels: channels should be 1");
        ASSERT_ARG(list.size() > 0, "list: list should not be empty");
        rows_ = list.size();
        cols_ = list.begin()->size();
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            cols_ = MAX(cols_, it->size());
        }

        stride_ = cols_;
        data = new T[rows_ * cols_];
        ASSERT_ALLOC(data);

        refCount = new size_t(1);
        if (refCount == nullptr)
        {
            std::cerr << "Memory allocation on refCount failed" << std::endl;
            delete[] data;
            throw std::bad_alloc();
        }
        
        for (size_t j = 0; j < rows_ * cols_; ++j)
        {
            data[j] = T();
        }

        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            std::copy(it->begin(), it->end(), data + (i++) * cols_);
        }

    }

    /// @brief destroy the matrix if the reference count is zero
    ~Mat()
    {
        release();
    }

    /// @brief release the memory of the matrix if the reference count is zero
    void release()
    {
        if (refCount != nullptr)
        {
            if (--(*refCount) == 0)
            {
                DEBUG_PRINT("Matrix data at <%x> reference count: 0\n", this->data);
                delete[] data;
                delete refCount;
            }
            else {
                DEBUG_PRINT("Matrix data at <%x> reference count: %ld\n", this->data, *refCount);
            }
        }
        
    }
    
    /// @brief copy constructor
    /// @param other 
    explicit Mat(const Mat &other)
    {
        rows_ = other.rows_;
        cols_ = other.cols_;
        stride_ = other.stride_;

        data = other.data;
        refCount = other.refCount;
        ++(*refCount);

        DEBUG_PRINT("Matrix data at <%x> reference count: %ld\n", this->data, *refCount);
    }

    /// @brief move constructor
    /// @param other 
    explicit Mat(Mat &&other)
    {
        rows_ = other.rows_;
        cols_ = other.cols_;
        stride_ = other.stride_;

        data = other.data;
        refCount = other.refCount;

        other.data = nullptr;
        other.refCount = nullptr;

        DEBUG_PRINT("Matrix data at <%x> reference count: %ld\n",  this->data, *refCount);
    }

    /// @brief copy assignment
    /// @param other 
    /// @return the reference of the current matrix
    Mat& operator = (const Mat& other)
    {
        if (this != &other)
        {
            release();
            rows_ = other.rows_;
            cols_ = other.cols_;
            stride_ = other.stride_;

            data = other.data;
            refCount = other.refCount;
            ++(*refCount);

            DEBUG_PRINT("Matrix data at <%x> reference count: %ld\n",  this->data, *refCount);
        }
        return *this;
    }

    /// @brief move assignment
    /// @param other
    /// @return the reference of the current matrix
    Mat& operator = (Mat&& other)
    {
        if (this != &other)
        {
            release();
            rows_ = other.rows_;
            cols_ = other.cols_;
            stride_ = other.stride_;

            data = other.data;
            refCount = other.refCount;

            other.data = nullptr;
            other.refCount = nullptr;

            DEBUG_PRINT("Matrix data at <%x> reference count: %ld\n",  this->data, *refCount);
        }
        return *this;
    }

    /// @brief get the element of the matrix at the given index
    /// @details this function won't check the index bound, if needed, use at() instead
    /// @param row
    /// @param col
    /// @param channel
    /// @return writable element at the given index
    T& operator () (const size_t row, const size_t col, const size_t channel = 0) noexcept
    {
        return data[row * CHANNELS * stride_ + col * CHANNELS + channel];
    }

    /// @brief get the read-only element of the matrix at the given index
    /// @details this function won't check the index bound, if needed, use at() instead
    /// @param row
    /// @param col
    /// @param channel
    /// @return read-only element at the given index
    const T& operator () (const size_t row, const size_t col, const size_t channel = 0) const noexcept
    {
        return data[row * CHANNELS * stride_ + col * CHANNELS + channel];
    }

    /// @brief get the element of the matrix at the given index
    /// @param row
    /// @param col
    /// @param channel
    /// @return element at the given index
    T& at(const size_t row, const size_t col, const size_t channel = 0)
    {
        return (*this)(row, col, channel);
    }

    /// @brief get the read-only element of the matrix at the given index
    /// @param row
    /// @param col
    /// @param channel
    /// @return element at the given index
    const T& at(const size_t row, const size_t col, const size_t channel = 0) const
    {
        return (*this)(row, col, channel);
    }

    /// @brief get the element of the matrix at the given packed index
    /// @details this function won't check the index bound
    /// @param index
    /// @return element at the given index
    T& operator [] (const size_t index) noexcept
    {
        return data[index];
    }

    /// @brief get the read-only element of the matrix at the given packed index
    /// @details this function won't check the index bound
    /// @param index
    /// @return element at the given index
    const T& operator [] (const size_t index) const noexcept
    {
        return data[index];
    }

    /// @brief read the matrix from the input stream
    /// @param is 
    /// @param mat 
    /// @return std::istream&
    friend std::istream& operator >> (std::istream &is, Mat &mat)
    {
        std::cout << "Enter the matrix elements:(" 
        << mat.rows_ << "x" << mat.cols_ << "x" << mat.channels() << ")" 
        << std::endl;

        for (size_t i = 0; i < mat.rows_ * mat.cols_ * mat.channels(); ++i)
        {
            // check if the current element is valid to given type
            ASSERT_ARG(is >> mat[i], "data: type does not match");
        }
        return is;
    }

    /// @brief write the matrix to the output stream
    /// @param os 
    /// @param mat 
    /// @return std::ostream&
    friend std::ostream& operator << (std::ostream &os, const Mat &mat)
    {
        os << "Matrix(" 
        << mat.rows_ << "x" << mat.cols_ << "x" << mat.channels() << ")<" 
        << typeid(T).name() << ">:" << std::endl;
        os << "[";
        for (size_t i = 0; i < mat.rows_ * mat.cols_ * mat.channels(); ++i)
        {
            if (i % mat.channels() == 0)
            {
                os << "(";
            }
            os << mat[i];
            if (i % mat.channels() == mat.channels() - 1)
            {
                os << ")";
            }
            os << ", ";
            if ((i + 1) % mat.stride_ == 0)
            {
                os << std::endl;
            }

        }
        os << "]" << std::endl;
        return os;
    }

    /// @brief type cast operator to the raw pointer
    /// @return 
    operator T*() noexcept
    {
        return data;
    }

};


/*************** TYPE ***************/

using Mati8 = Mat<char>;
using Mati16 = Mat<short>;
using Mati32 = Mat<int>;
using Mati64 = Mat<long long>;
using Matf = Mat<float>;
using Matd = Mat<double>;

class Vecf: public Matf
{
public:
    Vecf(size_t size) : Matf(size, 1) {}

    Vecf(const std::initializer_list<float> &list) : Matf({list}) {}
};

class Point2f: public Vecf
{
public:
    Point2f(float x, float y) : Vecf(2) {
        (*this)(0, 0) = x;
        (*this)(1, 0) = y;
    }
};

class Point3f: public Vecf
{
public:
    Point3f(float x, float y, float z) : Vecf(3) {
        (*this)(0, 0) = x;
        (*this)(1, 0) = y;
        (*this)(2, 0) = z;
    }
};

#endif // MATRIX_H_