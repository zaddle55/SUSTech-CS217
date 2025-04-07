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

template<typename T>
void qsort(T *arr, int start, int end, int (*cmp)(const T&, const T&))
{
    if (start >= end)
    {
        return;
    }
    T pivot = arr[start];
    int i = start;
    int j = i + 1;
    while (j <= end)
    {
        if (cmp(arr[j], pivot) < 0)
        {
            swap(arr[++i], arr[j]);
        }
        j++;
    }
    swap(arr[start], arr[i]);
    qsort(arr, start, i - 1, cmp);
    qsort(arr, i + 1, end, cmp);
}

int main()
{
    int a = 3, b = 2;
    COMP_OUT(int, a, b);

    float c = 1.0, d = 2.0;
    COMP_OUT(float, c, d);

    stuinfo e = {"Tom", 20}, f = {"Jerry", 20};
    COMP_OUT(stuinfo, e, f);

    char g = 'a', h = 'b';
    COMP_OUT(char, g, h);

    // qsort
    int arr[] = {3, 1, 4, 7, 5, 9};
    qsort(arr, 0, 5, Compare<int>);
    cout << "Sorted array: ";
    for (int i = 0; i < 6; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    stuinfo stuArr[] = {{"Tom", 20}, {"Jerry", 18}, {"Alice", 22}};
    qsort(stuArr, 0, 2, Compare<stuinfo>);
    cout << "Sorted stuArr: ";
    for (int i = 0; i < 3; i++)
    {
        cout << stuArr[i].name << " " << stuArr[i].age << " ";
    }
    cout << endl;

    return 0;
}
