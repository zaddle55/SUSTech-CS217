
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "math.h"
#include "shell.h"

#define BANNER " \
                         _       _ __          \r\n\
               ____ ___  (_)___  (_) /_  _____ \r\n\
              / __ `__ \\/ / __ \\/ / __ \\/ ___/\r\n\
             / / / / / / / / / / / /_/ / /__  \r\n\
            /_/ /_/ /_/_/_/ /_/_/_.___/\\___/  \r\n\
                                              \r\n\
"
void split(char* str, char* tokens[], int* token_len)
{
    char* p = str;
    int i = 0;
    while (*p != '\0')
    {
        while (*p == ' ')
        {
            p++;
        }

        if (*p == '\0')
        {
            break;
        }

        tokens[i++] = p;
        while (*p != ' ' && *p != '\0')
        {
            p++;
        }

        if (*p == '\0')
        {
            break;
        }

        *p = '\0';
        p++;
    }

    *token_len = i;
}

void start_interactive(int prec)
{
    enable_raw_mode();
    atexit(disable_raw_mode);
    fprintf(stdout, BANNER);
    fprintf(stdout, "====== Interactive Mode (type \"quit\" or 'q' to leave) ======\r\n");

    while (1)
    {
        
        char* tokens[TOKEN_BUFFER];
        int token_len = 0;
        char* in = read_line("> ");

        if (in == NULL || in[0] == 'q' )
        {
            free(in);
            break;
        }

        if (strlen(in) == 0)
        {
            free(in);
            continue;
        } else {
            add_to_history(in);
        }

        char* p = in;
        while (*p == ' ')
        {
            p++;
        }

        char* nl = strchr(p, '\n');
        if (nl != NULL)
        {
            *nl = '\0';
        }

        if (isalpha(*p))
        {
            // todo: handle math function
            free(in);
            fprintf(stdout, "Math function is not supported yet\r\n");
            continue;
        } else {
            split(p, tokens, &token_len);
        }

        char* num1, *num2;
        char op = '+';
        // print tokens
        for (int i = 0; i < token_len; i++)
        {
            printf("token[%d]: %s\r\n", i, tokens[i]);
        }
        Exn res = NULL;

        if (token_len >= 3)
        {
            num1 = tokens[0];
            op = tokens[1][0];
            num2 = tokens[2];
            Exn n1 = atoExn(num1, prec);
            Exn n2 = atoExn(num2, prec);       
            switch (op)
            {
            case '+':
                res = Exn_add(n1, n2);
                break;
            case '-':
                res = Exn_sub(n1, n2);
                break;
            case '*':
            case 'x':
                res = Exn_mul(n1, n2);
                break;
            case '/':
                res = Exn_div(n1, n2);
                break;
            default:
                MATH_URECG_OP(op);
                break;
            }

            printf("%s %c %s = ", num1, op, num2);

            Exn_release(&n1);
            Exn_release(&n2);
        }
        else {
            // todo: function
            printf("Invalid input\r\n");
        }

        if (res != NULL)
        {
            Exn_show(res);
            char* fmt_res = NULL;
            if (res->decimal > NORMAL_LIMIT)
            {
                fmt_res = Exn_fmt(res, EXN_FMT_SCIENTIFIC);
            }
            else
            {
                fmt_res = Exn_fmt(res, EXN_FMT_NORMAL);
            }
            printf("%s\r\n", fmt_res);
            free(fmt_res);
            Exn_release(&res);
        }
        free(in);
    }

    fprintf(stdout, "=====================================================\r\n");
    fprintf(stdout, "Free the memory and exit...\r\n");
    fprintf(stdout, "=====================================================\r\n");
    cleanup_history();
    fprintf(stdout, "Bye!\r\n");
}

int main( int argc, char *argv[] )
{
    int opt = 0;
    bool verbose = false, help = false, has_precision = false;
    int precision = DEFALT_PRECISION;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"precision", required_argument, 0, 'p'},
        {0, 0, 0, 0}};

    int opt_idx = 0;
    while ((opt = getopt_long(argc, argv, 
        "p:hv", long_options, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'h':
            help = 1;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'p':
            char* flag;
            precision = strtol(optarg, &flag, 10);
            if (*flag) // check if the argument is a number
            {
                INPUT_ARG_FAIL(optarg, "precision should be a digital integer");
            }
            else if (precision <= 0)
            {
                INPUT_ARG_FAIL(optarg, "precision should be a positive number");
            }
            else
            {
                has_precision = true;
            }
            break;
        case '?':
            INPUT_OPTION_FAIL(argv[optind - 1]);
            break;
        }
    }

    bool use_interactive = (verbose == 0)
        && (help == 0)
        && (argc - optind == 0);

    if (use_interactive) start_interactive(precision);
    else if (help)
    {
        printf("Usage: %s operand1 operation operand2 [option1]\r\n", argv[0]);
        printf("or: %s [option2]\r\n", argv[0]);
        printf("Option1:\r\n");
        printf("  -p <digit-int>, --precision\tSet the precision of the output\r\n");
        printf("Option2:\r\n");
        printf("  -h, --help\tDisplay this help message\r\n");
        printf("  -v, --verbose\tDisplay the precision\r\n");
        printf("  -p <digit-num>, --precision <digit-num>\tStart interactive mode with specified precision\r\n");
    }
    else if (verbose)
    {
        printf("Precision: %d\r\n", precision);
    }
    else {
        for (int i = optind; i < argc; i++)
        {
            printf("%s\r\n", argv[i]);
        }
    }
}