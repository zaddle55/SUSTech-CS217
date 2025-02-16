
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/swap.h"

void test_swap()
{
    srand( time( NULL ) );

    int num1 = rand() % 100;
    int num2 = rand() % 100;

    printf( "Before swap: num1 = %d, num2 = %d\n", num1, num2 );
    swap( &num1, &num2 );
    printf( "After swap: num1 = %d, num2 = %d\n", num1, num2 );
}

int main()
{
    test_swap();

    return 0;
}