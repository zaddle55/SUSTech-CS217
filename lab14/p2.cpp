#include <iostream>
#include <exception>
#include <string>
using namespace std;
const int UPPER_BOUND = 100;
const int LOWER_BOUND = 0;

class OutOfRangeException: public exception
{
private:
    string message_;
public:
    OutOfRangeException(string& message) {
        message_ = string(message);
    }
    const char* what() const throw()
    {
        return message_.c_str();
    }
};

#define CHECK_RANGE(param_name, param_value) \
    if (param_value < LOWER_BOUND || param_value > UPPER_BOUND) \
    { \
        string message = "The parameter " + string(param_name) + " is " + to_string(param_value) + " which is out of range"; \
        message += " (" + to_string(LOWER_BOUND) + ", " + to_string(UPPER_BOUND) + ")"; \
        throw OutOfRangeException(message); \
    }

float calculateAverage(int m1, int m2, int m3, int m4)
{
    CHECK_RANGE("1", m1);
    CHECK_RANGE("2", m2);
    CHECK_RANGE("3", m3);
    CHECK_RANGE("4", m4);
    return (m1 + m2 + m3 + m4) / 4.0;
}

int main() {
    int in1, in2, in3, in4;
    while (cin >> in1 >> in2 >> in3 >> in4)
    {
        try
        {
            cout << "Average: " << calculateAverage(in1, in2, in3, in4) << endl;
        }
        catch (OutOfRangeException &e)
        {
            cout << e.what() << endl;
        }
    }
    cout << "Bye!" << endl;
    
    return 0;
}