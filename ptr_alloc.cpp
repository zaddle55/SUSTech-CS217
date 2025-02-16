#include <iostream>

int *p = new int[6553200];

int main()
{
    if ( !p )
    {
        std::cout << "Memory allocation failed" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Memory allocation succeeded" << std::endl;
        delete[] p;
        return 0;
    }
}
