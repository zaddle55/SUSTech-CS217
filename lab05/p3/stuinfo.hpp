#pragma once

#ifndef STUINFO_HPP
#define STUINFO_HPP

struct stuinfo
{
    char name[20];
    double score[3];
    double ave;
};

// asks the user to enter each of the preceding items of information to set the corresponding members of the structure and compute the average score for each student.
void inputstu( stuinfo stu[], int n );

// displays the contents of the structure, one student one line.
void showstu( const stuinfo stu[], int n );

#endif // STUINFO_HPP