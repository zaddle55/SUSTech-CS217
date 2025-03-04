#include <iostream>
using namespace std;

// class Container
// {
// private:
//     /* data */
//     int n_;
// }; // no defined operations

class Container
{
private:
    /* data */
    int n_;
public:
    Container(/* args */) { cout << "Default constructor" << endl; }
    Container(int n = 0): n_(n) { cout << "Constructor with default value" << endl; }
    Container(Container &c) { cout << "Copy constructor" << endl; }

    Container &operator=(Container &c) { cout << "Copy assignment operator" << endl; return *this; }
};


int main() {

    // Container ctn; //! ambiguous call to constructor
    Container ctn1(1);

    Container ctn2(ctn1); // copy constructor
    Container ctn3 = ctn1; // copy constructor
    ctn3 = ctn2; // copy assignment operator
    
    return 0;
}