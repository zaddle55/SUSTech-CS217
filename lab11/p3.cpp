#include <iostream>
#include <memory>
using namespace std;

class B;

class A {
public:
    // shared_ptr<B> b;
    weak_ptr<B> b; // Use weak_ptr to avoid circular reference
    A() { cout << "Constructor A" << endl; }
    ~A() { cout << "Destructor A" << endl; }
};

class B {
public:
    // shared_ptr<A> a;
    weak_ptr<A> a; // Use weak_ptr to avoid circular reference
    B() { cout << "Constructor B" << endl; }
    ~B() { cout << "Destructor B" << endl; }
};

int main() {
    
    shared_ptr<A> spa = make_shared<A>();
    shared_ptr<B> spb = make_shared<B>();

    // spa->b = spb;
    // spb->a = spa;
    spa->b = weak_ptr<B>(spb);
    spb->a = weak_ptr<A>(spa);
    cout << "spa.use_count() = " << spa.use_count() << endl;
    cout << "spb.use_count() = " << spb.use_count() << endl;

    return 0;
}