#include <iostream>
using namespace std;

// struct Stu {
//     int age;
//     char name[20];
// };

// void init(Stu _s)
// {
//     _s.age = 20;
// }

// void sum(int arr[], int size)
// {
//     cout << sizeof arr << endl;
// }

class Foo {
    static int x;
    float y;
public:
    Foo() : y(0) {}
    void setX(int val) { x = val; int y = 0; y +=9.0 ;}
    void setY(float val) { y = val; }
    int getX() { return x; }
    void print() {
        cout << "x: " << x << ", y: " << y << endl;
    }
};

int main() {
    float a = 409.2675;
    int _b = 0xc1a6fae1;
    float b = *(float*)&_b;
    float c = -a;
    long long _d = 0xc0611bf1a9fbe76d;
    double d = *(double*)&_d;
    printf("a = %x\n", *(int*)&a);
    printf("b = %f\n", b);
    printf("c = %x\n", *(int*)&c);
    printf("d = %f\n", d);
    printf("sizeof a = %zu\n", sizeof(Foo));
    Foo f;
    cout << "sizeof f = " << sizeof(f) << endl;
    f.setX(10);
    
    
    return 0;
}