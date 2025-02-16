#include <iostream>
#include "lib/libvabs.hpp"

using namespace std;

#define PRINT_ARR(arr, n) \
    for (int i = 0; i < (n); i++) \
    { \
        cout << arr[i] << " "; \
    } \
    cout << endl;

template <typename T>
T* rand_arr(size_t n)
{
    T *arr = new T[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100 - 50 + rand() / (RAND_MAX + 1.0);
    }
    return arr;
}

int main()
{
    int *int_arr = rand_arr<int>(10);

    cout << "--- Before vabs ---" << endl;
    PRINT_ARR(int_arr, 10);
    vabs(int_arr, 10);
    cout << "--- After vabs ---" << endl;
    PRINT_ARR(int_arr, 10);

    delete[] int_arr;

    float *float_arr = rand_arr<float>(8);

    cout << "--- Before vabs ---" << endl;
    PRINT_ARR(float_arr, 8);
    vabs(float_arr, 8);
    cout << "--- After vabs ---" << endl;
    PRINT_ARR(float_arr, 8);

    delete[] float_arr;

    double *double_arr = rand_arr<double>(6);

    cout << "--- Before vabs ---" << endl;
    PRINT_ARR(double_arr, 6);
    vabs(double_arr, 6);
    cout << "--- After vabs ---" << endl;
    PRINT_ARR(double_arr, 6);

    delete[] double_arr;

    return 0;
}