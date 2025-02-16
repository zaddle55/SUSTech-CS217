#include <iostream>
using namespace std;
class Demo
{
public:
    static int num;
    static void display()
    {
        cout << "The value of the static member variable num is: " << num << endl;
    }
};
int Demo::num = 0;
int main()
{
    Demo obj;
    obj.display();
    Demo::display();
    return 0;
}
