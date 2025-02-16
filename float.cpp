#include <iostream>
#include <iomanip>

int main()
{
    float f1 = 3.14f;
    float f2 = f1 * 10000000000000;
    float f3 = f2 + 5;
    std::cout << std::fixed << std::setprecision(15) << f1 << std::endl;
    std::cout << std::fixed << std::setprecision(15) << f2 << std::endl;
    std::cout << std::fixed << std::setprecision(15) << f3 << std::endl;

} 