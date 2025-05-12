#include <iostream>
#include "complex.h"
using namespace std;

int main() {
    Complex a(3, 4);
    Complex b(2, 6);

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "~b = " << ~b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a - 2 = " << a - 2 << endl;
    cout << "a * b = " << a * b << endl;
    cout << "2 * a = " << 2 * a << endl;
    cout << "====================" << endl;

    Complex c = b;
    cout << "c = " << c << endl;
    cout << "b == c? " << boolalpha << (b == c) << endl;
    cout << "b != c? " << boolalpha << (b != c) << endl;
    cout << "a == b? " << boolalpha << (a == b) << endl;
    cout << "====================" << endl;

    Complex d;
    cout << "Enter a complex number(real part and imaginary part): ";
    cin >> d;
    cout << "Before assignment, d = " << d << endl;
    d = c;
    cout << "After assignment, d = " << d << endl;
    cout << "====================" << endl;

    // Complex e = 5.3;
    // cout << "e = " << e << endl;
    // cout << "e + 2 = " << e + 2 << endl;
    // cout << "(double) e = " << static_cast<double>(e) << endl;
    // cout << "(int) e = " << static_cast<int>(e) << endl;
    // cout << "====================" << endl;

    // Complex f(1, 1);
    // cout << "f = " << f << endl;
    // cout << "f / 2 = " << f / 2 << endl;
    // cout << "2 / f = " << 2 / f << endl;
    // cout << "====================" << endl;

    return 0;
}