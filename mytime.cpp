#include <iostream>
#include <string>

class MyTime
{
private:
    
public:
    int hour_;
    int minute_;
    MyTime() : hour_(0), minute_(0) {}
    MyTime(int h, int m) : hour_(h), minute_(m) {}
    MyTime operator+(const MyTime &rhs) const
    {
        MyTime sum;
        sum.minute_ = this->minute_ + rhs.minute_;
        sum.hour_ = this->hour_ + rhs.hour_ + sum.minute_ / 60;
        sum.minute_ %= 60;
        return sum;
    }
    MyTime operator+(const int m) const
    {
        MyTime sum;
        sum.minute_ = this->minute_ + m;
        sum.hour_ = this->hour_ + sum.minute_ / 60;
        sum.minute_ %= 60;
        return sum;
    }

    MyTime &operator+=(const MyTime &rhs)
    {
        this->minute_ += rhs.minute_;
        this->hour_ += rhs.hour_ + this->minute_ / 60;
        this->minute_ %= 60;
        return *this;
    }

    MyTime &operator++()
    {
        ++minute_;
        if (minute_ >= 60)
        {
            minute_ -= 60;
            ++hour_;
        }
        return *this;
    }

    MyTime operator++(int)
    {
        MyTime temp = *this;
        ++*this;
        return temp;
    }

    std::string getTime() const
    {
        return std::to_string(hour_) + ":" + std::to_string(minute_);
    }
};

int main()
{
    MyTime t1(1, 30);
    MyTime t2(2, 45);
    MyTime t3 = t1 + t2;
    std::cout << t3.getTime() << std::endl;
    t1 += t2;
    std::cout << t1.getTime() << std::endl;
    MyTime t4 = t1 + 30;
    std::cout << t4.getTime() << std::endl;
    t4++;
    std::cout << t4.getTime() << std::endl;
    return 0;
}
