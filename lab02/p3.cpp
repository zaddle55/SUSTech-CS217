#include <cmath>
#include <ios>
#include <iostream>
#include <cfloat>
using namespace std;

int main() {
    cout << fixed;
    float f1 = 1.0f;
    cout << "f1 = " << f1 << endl;

    float _f = 0.1f;
    float f2 = _f + _f + _f + _f + _f + _f + _f + _f + _f + _f;
    cout << "f2 = " << f2 << endl;

    if (abs(f1 - f2) <= FLT_EPSILON) { //? f1 == f2
        cout << "f1 == f2" << endl;
    } else {
        cout << "f1 != f2" << endl;
    }
    return 0;
}