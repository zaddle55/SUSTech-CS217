#include <iostream>
#include <cmath>
using namespace std;

class Point {
private:
    double x, y;
public:
    Point(double newX, double newY): x(newX), y(newY) {}
    Point(const Point &p): x(p.x), y(p.y) {}

    double getX() const { return x; }
    double getY() const { return y; }
};

class Line {
private:
    Point p1, p2;
    double distance;
public:
    Line(Point p1, Point p2): p1(p1), p2(p2) {
        distance = sqrt(pow(p1.getX() - p2.getX(), 2) 
        + pow(p1.getY() - p2.getY(), 2));
    }
    Line(const Line &l): p1(l.p1), p2(l.p2), distance(l.distance) {}
    double getDistance() const { return distance; }
};

int main() {

    Point a(8, 9), b(1, 2);
    Point c = a;
    cout << "point a: x=" << a.getX() << ", y=" << a.getY() << endl;
    cout << "point b: x=" << b.getX() << ", y=" << b.getY() << endl;
    cout << "point c: x=" << c.getX() << ", y=" << c.getY() << endl;

    cout << "----------------" << endl;
    Line line1(a, b);
    cout << "line1's distance: " << line1.getDistance() << endl;
    Line line2(line1);
    cout << "line2's distance: " << line2.getDistance() << endl;
    
    return 0;
}