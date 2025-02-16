#include <iostream>
#include <cstring>

int main()
{
    char rabbit[16] = { 'P', 'e', 't', 'e', 'r' };
    std::cout << "String length is: " << strlen(rabbit) << std::endl;
    for (int i = 0; i < sizeof(rabbit); i++)
    {
        std::cout << i << ":" << +rabbit[i] << "(" << rabbit[i] << ")" << std::endl;
    }

    char bad_pig[3] = { 'P', 'i', 'g' };
    char good_pig[4] = { 'P', 'i', 'g', '\0' };

    std::cout << "bad pig name is: " << bad_pig << std::endl;
    std::cout << "good pig name is: " << good_pig << std::endl;
}