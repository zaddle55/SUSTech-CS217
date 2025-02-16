#include <string>
#include <iostream>

using namespace std;

#define COMP_OUT(type, a, b) \
    cout << "Compare the two " << #type << "s: " << Compare<type>(a, b) << endl;

struct stuinfo
{
    /* data */
    string name;
    int age;
};

template <typename T>
int Compare(const T &a, const T &b)
{
    if (a > b)
    {
        return 1;
    }
    else if (a < b)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

template<>
int Compare(const stuinfo &a, const stuinfo &b)
{
    if (a.age > b.age)
    {
        return 1;
    }
    else if (a.age < b.age)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    int a = 3, b = 2;
    COMP_OUT(int, a, b);

    float c = 1.0, d = 2.0;
    COMP_OUT(float, c, d);

    stuinfo e = {"Tom", 20}, f = {"Jerry", 20};
    COMP_OUT(stuinfo, e, f);

    return 0;
}
