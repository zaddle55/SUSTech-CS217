#include <iostream>
using namespace std;

int main() {
    int a, b;
    double c, d, e, f, g;
    char h;

    a = 19.99 + 21.99;
    b = (int)19.99 + 21.99;
    c = 23 / 3;
    d = 23 / 3.0;
    f = 23 / 3.0e4;
    g = 23 / 3.0e5;
    h = 'b' - 32;

    cout << h << endl;
    cout << b << endl;
    cout << a << endl;
    cout << c << endl;
    cout << d << endl;
    cout << g << endl;
    cout << f << endl;

    return 0;
}