#include <iostream>
#include "stuinfo.hpp"

void inputstu( stuinfo stu[], int n )
{
    for ( int i = 0; i < n; i++ )
    {
        std::cin.ignore();
        std::cout << "Enter student name: ";
        std::cin.getline( stu[i].name, 20 );

        int sum = 0;

        for ( int j = 0; j < 3; j++ )
        {
            std::cout << "Enter student score " << j + 1 << ": ";
            std::cin >> stu[i].score[j];
            sum += stu[i].score[j];
            std::cin.ignore();
        }

        stu[i].ave = sum / 3.0;
    }
}

void showstu( const stuinfo stu[], int n )
{
    for ( int i = 0; i < n; i++ )
    {
        std::cout << "Student " << i + 1 << " name: " << stu[i].name << ",";
        std::cout << " scores: " << stu[i].score[0] << ", " << stu[i].score[1] << ", " << stu[i].score[2] << ",";
        std::cout << " average: " << stu[i].ave << std::endl;
    }
}
