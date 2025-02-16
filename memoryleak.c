#include <stdio.h>
#include <stdlib.h>

int main()
{
    for (int i = 0; i < 16; i++) {
        int* p = (int*)malloc(1024 * 1024 * 1024);
        *p = i;
    }
    printf("Memory leak test\n");

    return 0;
}