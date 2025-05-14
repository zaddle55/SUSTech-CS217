#include <iostream>
using namespace std;

char* gives_ownership() {
    char* p = new char[10];
    p[0] = 'H';
    p[1] = 'e';
    p[2] = 'l';
    p[3] = 'l';
    p[4] = 'o';
    p[5] = '\n';
    return p;
}

int main() {
    
    char* p = gives_ownership();
    cout << "p: " << (void*)p << endl;
    cout << string(p) << endl;
    return 0;
}