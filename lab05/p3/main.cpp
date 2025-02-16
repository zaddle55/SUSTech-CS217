#include <iostream>
#include "stuinfo.hpp"

int main()
{
    std::cout << "Enter the number of students: ";
    int n;
    std::cin >> n;

    stuinfo *stu = new stuinfo[n];

    inputstu( stu, n );
    showstu( stu, n );

    delete[] stu;

    return 0;
}