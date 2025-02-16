
#include <iostream>
#include "fib.hpp"

int main()
{
    std::cout << "Please input a positive integer:";
    int n;
    std::cin >> n;

    for ( int i = 1; i <= n; i++ )
    {
        std::cout << fib( i ) << " ";
    }
    std::cout << std::endl;
}