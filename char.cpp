
#include <iostream>

int main()
{
    char c1 = 'C';
    char c2 = 0x37; // '7' in ASCII

    char16_t c3 = u'C'; // unicode
    char32_t c4 = U'开'; // unicode

    std::cout << c1 << ":" << c2 << ":" << c3 << std::endl;
    std::cout << +c1 << ":" << +c2 << ":" << +c3 << std::endl;
    std::cout << c4 << std::endl;
}