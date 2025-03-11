#include <iostream>
using namespace std;

int main() {
    int x = 100;

    int y = (x = 200);
    
    cout << y;

    return 0;
}