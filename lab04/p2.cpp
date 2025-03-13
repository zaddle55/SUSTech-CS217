#include <cstdio>
#include <iostream>
#include <string.h>
using namespace std;
int main()
{
    int cards[4]{};
    int hands[4];
    int price[] = {(int)2.8,(int)3.7,5,9,'C', 'D'}; 
    char direction[4] {'L',82,'U',68};
    char* p = (char*)&direction + sizeof(direction);
    *p = '\0';  // 将direction后面的第一个字节设置为\0
    cout << strlen(direction) << endl;
    char title[] = "DeepSeek is an awesome tool.";
    
    cout << "sizeof(cards) = " << sizeof(cards) << ",sizeof of cards[0] = " << sizeof(cards[0]) << endl;
    cout << "sizeof(price) = " << sizeof(price) << ",sizeof of price[0] = " << sizeof(price[0]) << endl;
    cout << "sizeof(direction) = " << sizeof(direction) << ",length of direction = " << strlen(direction) << endl;
    cout << "sizeof(title) = " << sizeof(title) << ",length of title = " << strlen(title) << endl;

    return 0;
}
