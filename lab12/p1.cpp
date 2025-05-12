#include <iostream>
using namespace std;

class Base {
private:
    int x;
protected:
    int y;
public:
    int z;
    void funcBase(Base &b) {
        ++x;
        ++y;
        ++z;
        ++b.x;
        ++b.y;
        ++b.z;
    }
};

class Derived : public Base {
public:
    void funcBase(Base &b, Derived &d) {
        ++x;
        ++y;
        ++z;
        ++b.x;
        ++b.y;
        ++b.z;
        ++d.x;
        ++d.y;
        ++d.z;
    }
};

void fun(Base &b, Derived &d) {
    ++x;
    ++y;
    ++z;
    ++b.x;
    ++b.y;
    ++b.z;
    ++d.x;
    ++d.y;
    ++d.z;
}

int main() {
    
    return 0;
}