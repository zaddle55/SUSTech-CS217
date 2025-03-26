#include <iostream>
using namespace std;

struct Student {
    int id;
    char description[1024];
};

void disp_stu(Student* stu)
{
    cout << "ID: " << stu->id << endl;
    cout << "Description: " << stu->description << endl;
}

void disp_stu(Student& stu)
{
    cout << "ID: " << stu.id << endl;
    cout << "Description: " << stu.description << endl;
}

int main() {

    Student stu = {3, "NAZHA"};
    disp_stu(&stu);
    disp_stu(stu);
    return 0;
}