
#include <iostream>
#include "functions.h"

int main()
{
    print_hello();
    
    std::cout << "This is main:" << std::endl;
    std::cout << "The factorial of 5 is: " << factorial(5) << std::endl;
    // std::cout << "The factorial of 6 is: " << factorial(6) << std::endl;
    return 0;
}