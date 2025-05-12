#include <iostream>
using namespace std;

class Complex
{
private:
    double real_;
    double imag_;
public:
    Complex(): real_(0), imag_(0) {};
    Complex(int real, int imag = 0): real_(real), imag_(imag) {};
    Complex(float real, float imag = 0): real_(real), imag_(imag) {};
    Complex(double real, double imag = 0): real_(real), imag_(imag) {};
    ~Complex();

    //? Overload non-modifying operators
    Complex operator+(const Complex &c) const {
        return Complex(real_ + c.real_, imag_ + c.imag_);
    }
    Complex operator-(const Complex &c) const {
        return Complex(real_ - c.real_, imag_ - c.imag_);
    }
    Complex operator*(const Complex &c) const {
        return Complex(real_ * c.real_ - imag_ * c.imag_, real_ * c.imag_ + imag_ * c.real_);
    }
    Complex& operator~() {
        this->imag_ = -this->imag_;
        return *this;
    }
    Complex operator/(const Complex &c) const {
        double denominator = c.real_ * c.real_ + c.imag_ * c.imag_;
        double real = (real_ * c.real_ + imag_ * c.imag_) / denominator;
        double imag = (imag_ * c.real_ - real_ * c.imag_) / denominator;
        return Complex(real, imag);
    }

    //? Overload friend operators
    friend Complex operator+(double real, const Complex &c) {
        return c + real;
    }

    friend Complex operator-(double real, const Complex &c) {
        return Complex(real - c.real_, -c.imag_);
    }

    friend Complex operator*(double real, const Complex &c) {
        return c * real;
    }
    friend Complex operator/(double real, const Complex &c) {
        double denominator = c.real_ * c.real_ + c.imag_ * c.imag_;
        double real_ = real * c.real_ / denominator;
        double imag_ = -real * c.imag_ / denominator;
        return Complex(real_, imag_);
    }

    //? Overload comparison operators
    bool operator==(const Complex &c) const {
        return real_ == c.real_ && imag_ == c.imag_;
    }
    bool operator!=(const Complex &c) const {
        return real_ != c.real_ || imag_ != c.imag_;
    }

    //? Overload modifying operators
    Complex &operator+=(const Complex &c) {
        real_ += c.real_;
        imag_ += c.imag_;
        return *this;
    }
    Complex &operator-=(const Complex &c) {
        real_ -= c.real_;
        imag_ -= c.imag_;
        return *this;
    }
    Complex &operator*=(const Complex &c) {
        double real = real_ * c.real_ - imag_ * c.imag_;
        double imag = real_ * c.imag_ + imag_ * c.real_;
        real_ = real;
        imag_ = imag;
        return *this;
    }
    Complex &operator/=(const Complex &c) {
        double denominator = c.real_ * c.real_ + c.imag_ * c.imag_;
        double real = (real_ * c.real_ + imag_ * c.imag_) / denominator;
        double imag = (imag_ * c.real_ - real_ * c.imag_) / denominator;
        real_ = real;
        imag_ = imag;
        return *this;
    }

    //? Overload input/output operators
    friend istream &operator>>(istream &in, Complex &c) {
        in >> c.real_ >> c.imag_;
        return in;
    }
    friend ostream &operator<<(ostream &out, const Complex &c) {
        out << c.real_ << (c.imag_ >= 0 ? "+" : "") << c.imag_ << "i";
        return out;
    }

    //? Defined type conversion operators
    explicit operator double() const {
        return real_;
    }
    explicit operator int() const {
        return static_cast<int>(real_);
    }
    explicit operator float() const {
        return static_cast<float>(real_);
    }
    
};

Complex::~Complex()
{
    // printf("Complex object will be deleted soon\n");
}