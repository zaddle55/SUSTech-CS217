#include <cstddef>
#include <iostream>
using namespace std;

int main() {
    size_t x_ = 0; //? int is not enough
    size_t y_ = 0; //? int is not enough

    char op = 0;

    cin >> x_ >> op >> y_;

    if (op == '*') {
        cout << x_ * y_ << endl;
    } else if (op == '/') {
        cout << x_ / y_ << endl;
    } else if (op == '+') {
        cout << x_ + y_ << endl;
    } else if (op == '-') {
        cout << x_ - y_ << endl;
    } else {
        cerr << "Invalid operator" << endl;
    }
    
    return 0;
}