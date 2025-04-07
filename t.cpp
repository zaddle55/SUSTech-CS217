#include <iostream>
using namespace std;

float norm_l1(float x, float y); //declaration

float norm_l2(float x, float y); //declaration

float (*norm_ptr)(float x, float y); //norm_ptr is a function pointer

int main() {
    norm_ptr = norm_l1; //pointing to norm_l1 function
    norm_ptr = norm_l2;
norm_ptr (- 3.0f , 4.0f );
}