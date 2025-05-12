#include <iostream>
using namespace std;

class Polygen {
protected:
    int width_, height_;
public:
    Polygen() = default;
    Polygen(int w, int h) { width_ = w; height_ = h; }
    void set_values(int w, int h) {
        width_ = w;
        height_ = h;
    }
    // int area() { return 0; }
    virtual int area(void) = 0;
    void printArea() {
        cout << this->area() << endl;
    }
};

class Rectangle : public Polygen {
public:
    Rectangle(int w, int h): Polygen(w, h) {} ;
    int area() override { return width_ * height_; };
};

class Triangle : public Polygen {
public:
    Triangle(int w, int h): Polygen(w, h) {} ;
    int area() override { return width_ * height_ / 2;}
};

int main() {
    Polygen* ppoly = new Rectangle(4, 5);
    ppoly->printArea();
    delete ppoly;
    ppoly = new Triangle(2, 5);
    ppoly->printArea();
    delete ppoly;
    ppoly = nullptr;
    return 0;
}