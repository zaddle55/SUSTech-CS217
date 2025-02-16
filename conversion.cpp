#include <iostream>

int main()
{
    int num_int = 0x7ABCDEF0;
    float num_int_float = num_int;
    int num_int2 = (int) num_int_float;
    std::cout.setf(std::ios::dec, std::ios::floatfield);
    std::cout << "num_int: " << num_int << std::endl;
    std::cout << "num_int_float: " << num_int_float << std::endl;
    std::cout << "num_int2: " << num_int2 << std::endl;
    std::cout << "num_int == num_int2: " << (num_int == num_int2) << std::endl;
    unsigned char a = 255;
    unsigned char b = 1;
    int sum = a + b; // conversion a and b to int before sum
    std::cout << "sum: " << sum << std::endl;
}