#include <iostream>
#include <cmath>
using namespace std;

class Complex
{
private:
    float real_;
    float imag_;
public:
    Complex(): real_(0), imag_(0) {};
    Complex(float real, float imag): real_(real), imag_(imag) {};
    ~Complex();
    Complex Add(const Complex &c) const {
        return Complex(real_ + c.real_, imag_ + c.imag_);
    }
    Complex Sub(const Complex &c) const {
        return Complex(real_ - c.real_, imag_ - c.imag_);
    }
    void display() const {
        bool is_imag_positive = imag_ >= 0;
        cout << "The value of the complex number is: " << real_ << 
        (is_imag_positive ? " + " : " - ") 
        << abs(imag_) << "i" << endl;
    }
};

Complex::~Complex()
{
    printf("Complex object will be deleted soon\n");
}


int main() {

    // 1. Test default constructor
    Complex cplx1 = Complex(); 
    cplx1.display(); // The value of the complex number is: 0 + 0i

    // 2. Test parameterized constructor
    Complex cplx2 = Complex(1, 2);
    cplx2.display(); // The value of the complex number is: 1 + 2i

    // 3. Test Add method
    Complex cplx3 = cplx1.Add(cplx2);
    cplx3.display(); // The value of the complex number is: 1 + 2i

    // 4. Test Sub method
    Complex cplx4 = cplx1.Sub(cplx2);
    cplx4.display(); // The value of the complex number is: -1 + -2i

    // 5. Test copy
    Complex cplx5 = cplx2;
    cplx5.display(); // The value of the complex number is: 1 + 2i
    
    return 0;
}