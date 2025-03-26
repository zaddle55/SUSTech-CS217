#include <iostream>
using namespace std;

struct Stu {
    int age;
    char name[20];
};

void init(Stu _s)
{
    _s.age = 20;
}

void sum(int arr[], int size)
{
    cout << sizeof arr << endl;
}

int main() {
    Stu s;
    int cookies[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    s.age = 10;
    cout << "Before init: " << s.age << endl;
    init(s);
    cout << "After init: " << s.age << endl;
    sum(cookies, 8);
    
    return 0;
}