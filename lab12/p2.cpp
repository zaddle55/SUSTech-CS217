#include <iostream>
using namespace std;

class Polygen {
protected:
    int width_, height_;
public:
    Polygen() = default;
    void set_values(int w, int h) {
        width_ = w;
        height_ = h;
    }
    int area() { return 0; }
};

class Rectangle : public Polygen {
public:
    int area() { return width_ * height_; };
};

class Triangle : public Polygen {
public:
    int area() { return width_ * height_ / 2;}
};

int main() {
    Rectangle rect;
    Triangle tri;
    Polygen* ppoly1 = static_cast<Polygen*>(&rect);
    Polygen* ppoly2 = static_cast<Polygen*>(&tri);
    ppoly1->set_values(4, 5);
    ppoly2->set_values(2, 5);
    cout << rect.area() << endl;
    cout << tri.area() << endl;
    cout << ppoly1->area() << endl;
    cout << ppoly2->area() << endl;
    return 0;
}