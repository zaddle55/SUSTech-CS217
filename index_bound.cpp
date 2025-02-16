#include <iostream>

int main()
{
    int num_arr[5];

    for ( int i = -1 ; i <= 5; i++ )
    {
        num_arr[i] = i * i;
    }

    for ( int i = -1 ; i <= 5; i++ )
    {
        std::cout << "num_arr[" << i << "] = " << num_arr[i] << std::endl;
    }

    return 0;
}