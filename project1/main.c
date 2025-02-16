
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _Operation
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    UNKNOWN
} Operation;

Operation char2operation( char c )
{
    switch ( c )
    {
    case '+':
        return ADD;
    case '-':
        return SUBTRACT;
    case '*':
        return MULTIPLY;
    case '/':
        return DIVIDE;
    default:
        return UNKNOWN;
    }
}

double calc( double num1, double num2, Operation op )
{
    switch ( op )
    {
    case ADD:
        return num1 + num2;
    case SUBTRACT:
        return num1 - num2;
    case MULTIPLY:
        return num1 * num2;
    case DIVIDE:
        return num1 / num2;
    default:
        return 0;
    }
}

int main( int argc, char *argv[] )
{
    if (argc == 1)
    {
        while ( 1 )
        {
            double num1 = -1 , num2 = -1;
            char op;
            // printf("Enter an expression (or 'quit' to quit): ");
            if (scanf("%lf %c %lf", &num1, &op, &num2) != 3)
            {
                char input[10];
                scanf("%s", input);
                if ( strcmp(input, "quit") == 0 || strcmp(input, "q") == 0 )
                {
                    break;
                }
                else
                {
                    printf("Invalid input \"%s\". Please try again.\n", input);
                    continue;
                }
            }
            Operation operation = char2operation(op);
            double result = calc(num1, num2, operation);
            
            if (operation == UNKNOWN)
            {
                printf("Invalid operation '%c'.\n", op);
            }
            else
            {
                printf("%lf %c %lf = %lf\n", num1, op, num2, result);
            }
        }
        
    }
    else if (argc == 4)
    {
        double num1 = atof(argv[1]);
        double num2 = atof(argv[3]);
        Operation operation = char2operation(argv[2][0]);
        double result = calc(num1, num2, operation);
        
        if (operation == UNKNOWN)
        {
            printf("Invalid operation '%s'.\n", argv[2]);
        }
        else
        {
            printf("%lf %s %lf = %lf\n", num1, argv[2], num2, result);
        }
    }
    else
    {
        printf("Usage: %s <number 1> [+\\-\\*\\/] <number2>\n", argv[0]);
    }
}