#pragma once
#include <iostream>

class Rational {
private:
    static int id;
    int numerator;
    int denominator;
public:
    Rational(int n=0, int d=1): numerator(n), denominator(d) {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        std::cout << "Construct_" << ++id << ", n : " << numerator << " , d: " << denominator << std::endl;
    }

    int getN() const { return numerator; }
    int getD() const { return denominator; }

    // Rational operator+(const Rational& r) const {
    //     return Rational(numerator * r.denominator + r.numerator * denominator, denominator * r.denominator);
    // }
    // Rational operator-(const Rational& r) const {
    //     return Rational(numerator * r.denominator - r.numerator * denominator, denominator * r.denominator);
    // }
    // Rational operator*(const Rational& r) const {
    //     return Rational(numerator * r.numerator, denominator * r.denominator);
    // }
    // Rational operator/(const Rational& r) const {
    //     return Rational(numerator * r.denominator, denominator * r.numerator);
    // }

    operator double() const {
        return static_cast<double>(numerator) / denominator;
    }
    Rational& operator=(const Rational& r) {
        if (this != &r) {
            numerator = r.numerator;
            denominator = r.denominator;
        }
        return *this;
    }
    bool operator==(const Rational& r) const {
        return numerator * r.denominator == r.numerator * denominator;
    }
    bool operator!=(const Rational& r) const {
        return !(*this == r);
    }
    bool operator<(const Rational& r) const {
        return numerator * r.denominator < r.numerator * denominator;
    }
    bool operator>(const Rational& r) const {
        return numerator * r.denominator > r.numerator * denominator;
    }
    bool operator<=(const Rational& r) const {
        return !(*this > r);
    }
    bool operator>=(const Rational& r) const {
        return !(*this < r);
    }


    // friend Rational operator+(double n, const Rational& r) {
    //     return Rational(r.numerator + n * r.denominator, r.denominator);
    // }
    // friend Rational operator-(double n, const Rational& r) {
    //     return Rational(n * r.denominator - r.numerator, r.denominator);
    // }
    // friend Rational operator*(double n, const Rational& r) {
    //     return Rational(n * r.numerator, r.denominator);
    // }
    // friend Rational operator/(double n, const Rational& r) {
    //     return Rational(n * r.denominator, r.numerator);
    // }

    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        os << r.numerator << "/" << r.denominator;
        return os;
    }
};

int Rational::id = 0;

// const Rational operator+(const Rational& lhs, const Rational& rhs) {
//     return Rational(lhs.getN() * rhs.getD() + rhs.getN() * lhs.getD(), lhs.getD() * rhs.getD());
// }
// const Rational operator-(const Rational& lhs, const Rational& rhs) {
//     return Rational(lhs.getN() * rhs.getD() - rhs.getN() * lhs.getD(), lhs.getD() * rhs.getD());
// }
// const Rational operator*(const Rational& lhs, const Rational& rhs) {
//     return Rational(lhs.getN() * rhs.getN(), lhs.getD() * rhs.getD());
// }
// const Rational operator/(const Rational& lhs, const Rational& rhs) {
//     return Rational(lhs.getN() * rhs.getD(), lhs.getD() * rhs.getN());
// }