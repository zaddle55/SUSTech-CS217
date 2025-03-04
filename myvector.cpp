#include <iostream>

template <class T>
class MyVector
{
private:
    std::size_t size_;
    T *data;
public:
    MyVector() = default;
    MyVector(size_t size) : size_(size), data(new T[size]) {}
    ~MyVector() { delete[] data; }
    MyVector(const MyVector& other) = delete;
    MyVector& operator = (const MyVector& other) = delete;
    T& operator [] (size_t index) { return data[index]; }
    const T& operator [] (size_t index) const { return data[index]; }

};

template<>
class MyVector<bool>
{
private:
    std::size_t size_;
    uint8_t *data;
public:
    MyVector<bool>(size_t size) : size_(size), data(new unsigned char[(size + 7) / 8]) {}
    ~MyVector<bool>() { delete[] data; }
    MyVector<bool>(const MyVector<bool>& other) = delete;
    MyVector<bool>& operator = (const MyVector<bool>& other) = delete;
    const bool operator [] (size_t index) const { return data[index / 8] & (1 << (index % 8)); }
    void set(size_t index, bool value) {
        if (value) {
            data[index / 8] |= (1 << (index % 8));
        } else {
            data[index / 8] &= ~(1 << (index % 8));
        }
    }
};


using namespace std;

int main() {

    MyVector<bool> vec(10);
    vec.set(0, -16);
    vec.set(1, false);
    vec.set(2, true);
    cout << vec[0] << endl;
    cout << vec[1] << endl;
    cout << vec[2] << endl;
    cout << vec[3] << endl;
    
    
    return 0;
}