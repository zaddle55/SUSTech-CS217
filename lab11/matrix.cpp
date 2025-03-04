#include <iostream>
#include <memory>
#include <cmath>
#include <initializer_list>
using namespace std;

class Matrix
{
public:
    size_t rows_;
    size_t cols_;
    shared_ptr<float *> data_;
    Matrix() : rows_(0), cols_(0), data_(nullptr) {};
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(make_shared<float *>(new float[rows * cols]))
    {
        if (rows == 0 || cols == 0)
        {
            throw invalid_argument("Invalid size of matrix");
        }
        if (!data_)
        {
            throw bad_alloc();
        }
        for (size_t i = 0; i < rows * cols; ++i)
        {
            (*data_)[i] = 0;
        }
    };
    ~Matrix() = default;

    void fromArray(std::initializer_list<float> list)
    {
        if (list.size() > rows_ * cols_)
        {
            throw invalid_argument("Invalid size of initializer list");
        }
        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            (*data_)[i++] = *it;
        }
    }

    Matrix(const Matrix &other) : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {};
    Matrix &operator=(const Matrix &other)
    {
        if (this != &other)
        {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = other.data_;
        }
        return *this;
    }

    Matrix(Matrix &&other) noexcept : rows_(other.rows_), cols_(other.cols_), data_(move(other.data_)) {};
    Matrix &operator=(Matrix &&other) noexcept
    {
        if (this != &other)
        {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = move(other.data_);
        }
        return *this;
    }

    float &operator()(size_t i, size_t j)
    {
        return (*data_)[i * cols_ + j];
    }
    const float &operator()(size_t i, size_t j) const
    {
        return (*data_)[i * cols_ + j];
    }

    Matrix operator+(Matrix &rhs) const
    {
        if (rhs.data_ == nullptr)
            throw invalid_argument("Empty matrix");
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
        {
            throw invalid_argument("Invalid size of matrix");
        }
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i)
        {
            (*result.data_)[i] = (*data_)[i] + (*rhs.data_)[i];
        }
        return result;
    }

    friend ostream &operator<<(ostream &os, const Matrix &m)
    {
        for (size_t i = 0; i < m.rows_; ++i)
        {
            for (size_t j = 0; j < m.cols_; ++j)
            {
                os << m(i, j) << " ";
            }
            os << endl;
        }
        return os;
    }
    friend istream &operator>>(istream &is, Matrix &m)
    {
        for (size_t i = 0; i < m.rows_; ++i)
        {
            for (size_t j = 0; j < m.cols_; ++j)
            {
                is >> m(i, j);
            }
        }
        return is;
    }
};

int main()
{

    {
        Matrix a(3, 4);
        Matrix b(3, 4);

        // a.fromArray({
        //     0, 0, 0, 0,
        //     0, 0, 3, 0,
        //     0, 0, 0, 0
        // })
        // b.fromArray({
        //     0, 0, 0, 0,
        //     0, 0, 0, 0,
        //     0, 0, 0, 4
        // });

        a(1, 2) = 3;
        b(2, 3) = 4;

        Matrix c = a + b;

        cout << "a is: " << endl;
        cout << a << endl;
        cout << "b is: " << endl;
        cout << b << endl;
        cout << "c is: " << endl;
        cout << c << endl;

        Matrix d = a;
        cout << "Before assignment, d is: " << endl;
        cout << d << endl;
        d = b;
        cout << "After assignment, d is: " << endl;
        cout << d << endl;
    }
    cout << "All matrices which hold the shared data field are out of scope" << endl;
    return 0;
}