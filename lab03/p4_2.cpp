#include <iostream>
using namespace std;

int main()
{
    unsigned int n = 5;  //! unsigned int >0 always (5 -> 3 -> 1 -> 0xffffffff)
    int sum; //! Not initialized
    while(n >0){
        sum += n;
        cout << "n = " << (n-=2) <<endl;
        cout << "sum = " << sum << "  ";
    }
    return 0;
}
