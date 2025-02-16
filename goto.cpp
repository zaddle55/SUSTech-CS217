#include <iostream>

float my_square(float x)
{
    float result = 0.0f;
    if ( x <= 0 || x >= 1.0f )
    {
        std::cerr << "Out of range (0-1)" << std::endl;
        goto EXIT_ERROR;
    }
    result = x * x;
    return result;

    EXIT_ERROR:
        // do some error handling
        return 0.0f;
}

int main()
{
    float val;
    std::cout << "Enter a value between 0 and 1: ";
    std::cin >> val;

    float res = my_square( val );

    std::cout << "Result: " << res << std::endl;
    return 0;
}