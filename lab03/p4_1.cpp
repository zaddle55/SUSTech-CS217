#include <iostream>
using namespace std;

int main()
{
    int n = 5;
    int sum; //! Not initialized
    while(n >0){
        sum += n;
        cout << "n = " << n << "  ";
        cout << "sum = " << sum << "  ";
    }
    return 0;
}
