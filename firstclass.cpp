#include <iostream>
#include <cstring>

class Student
{
private:
    /* data */
    char name[20];
    int born;
    bool male; // 0 is female, 1 is male

public:
    Student(/* args */): born(1990), male(1)
    {
        setName("Default");
    }
    Student(const char *n, int b, bool g);
    ~Student();
    inline void setName(const char *s)
    {
        strncpy(name, s, sizeof(name));
    }
    inline void setBorn(int b)
    {
        born = b;
    }
    inline void setGender(bool g)
    {
        male = g;
    }
    inline void printInfo()
    {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Born: " << born << std::endl;
        std::cout << "Gender: " << (male ? "Male" : "Female") << std::endl;
    }
};

Student::Student(const char *n, int b, bool g)
{
    setName(n);
    born = b;
    male = g;
}

Student::~Student()
{
    printf("Student %s is deleted\n", this->name);
};

int main()
{
    Student s;
    s.printInfo();
    s.setName("Tom");
    s.setBorn(1990);
    s.setGender(1);
    s.printInfo();
    // s.born = 1991; // Error: born is private

    Student xue = Student{"Xue", 1991, 0};
    xue.printInfo();

    Student *zhou = new Student("Zhou", 1992, 1);
    zhou->printInfo();
    delete zhou;
    return 0;
}
