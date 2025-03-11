#include <stdlib.h>
#include <stdio.h>
int main()
{
    char* str = (char *)malloc(4*sizeof(char));
    scanf("%s", str);
    for (int i = 0; i < 10; i++)
    {
        if (str[i] == '\0')
        {
            printf("0");
        }
        else {
            printf("%c", str[i]);
        }
    }
    free(str);
    return 0;
}