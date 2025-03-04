#include <iostream>
using namespace std;

int main() {
    auto a = 10;
    a = 20.5;
    a += 10.5; // a = 30
    cout << "a = " << a << endl;

    auto b = 10.0;
    b = 20.5;
    b += a;    // b = 50.5
    cout << "b = " << b << endl;

    return 0;
}