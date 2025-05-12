#include <iostream>
#include <memory>
using namespace std;

int main() {
    double *p_reg = new double(5);
    shared_ptr<double> pd;
    // pd = p_reg;
    pd = shared_ptr<double>(p_reg, [](double *p) {
        cout << "delete p_reg" << endl;
        delete p;
    });
    cout << "*pd = " << *pd << endl;
    // shared_ptr<double> p_shared = p_reg;
    shared_ptr<double> p_shared(p_reg, [](double *p) {
        cout << "delete p_shared" << endl;
        delete p;
    });
    cout << "*p_shared = " << *p_shared << endl;
    string str("Hello, World!");
    shared_ptr<string> p_str(&str, [](string *p) {
        cout << "delete p_str" << endl;
        // delete p;
    });
    cout << "*p_str = " << *p_str << endl;
    return 0;
}