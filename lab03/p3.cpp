#include <iostream>
#include <cstddef>
using namespace std;

int main() {
    for (size_t n = 2; n >= 0; --n)  //! size_t is unsigned
    {
        cout << "n = " << n << endl;
    }
    return 0;
}