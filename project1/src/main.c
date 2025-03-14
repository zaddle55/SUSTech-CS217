
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

void start_interactive(int prec)
{
    enable_raw_mode();
    atexit(disable_raw_mode);
    fprintf(stdout, BANNER);
    fprintf(stdout, "====== Interactive Mode (type \"quit\" or 'q' to leave) ======\r\n");
    MathExpr expr;
    while (1)
    {
        
        char* tokens[TOKEN_BUFFER];
        int token_len = 0;
        char* in = read_line("> ");

        if (in == NULL || in[0] == 'q')
        {
            free(in);
            break;
        }
        add_to_history(in);
        if (MathExpr_build(in, &expr, prec) != 0)
        {
            free(in);
            continue;
        }
        Exn res = MathExpr_eval(&expr);
        if (res == NULL)
        {
            free(in);
            MathExpr_release(&expr);
            continue;
        }
        if (res->error != EXTENDED_NUM_OK)
        {
            __WARNING("The result got an error: %s\r\n", Exn_err2str(res->error));
            Exn_release(&res);
            free(in);
            MathExpr_release(&expr);
            continue;
        }
        char* fmt_res = NULL;
        if (abs(res->decimal - 1) > NORMAL_LIMIT)
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
        free(in);
        MathExpr_release(&expr);
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
        printf("  return the result of binary expression\r\n");
        printf("or: %s [option2]\r\n", argv[0]);
        printf("  start interactive mode\r\n");
        printf("Option1:\r\n");
        printf("  -p <integer>, --precision\tSet the precision of the output\r\n");
        printf("Option2:\r\n");
        printf("  -h, --help\tDisplay this help message\r\n");
        printf("  -v, --verbose\tDisplay the precision\r\n");
        printf("  -p <integer>, --precision <integer>\tStart interactive mode with specified precision\r\n");
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

        if (argc - optind >= 3)
        {

            for (int i = optind + 3; i < argc; i++)
            {
                __WARNING("Extra arguments: %s\r\n", argv[i]);
            }

            char* num1 = argv[optind];
            char op = argv[optind + 1][0];
            char* num2 = argv[optind + 2];
            
            BinOprExpr bin_op;
            Exn n1 = atoExn(num1, precision);
            Exn n2 = atoExn(num2, precision);
            if (n1 == NULL || n2 == NULL)
            {
                __WARNING("Invalid input: %s %c %s\r\n", num1, op, num2);
                if (n1 != NULL) Exn_release(&n1);
                if (n2 != NULL) Exn_release(&n2);
                return 0;
            }
            if (n1->error != EXTENDED_NUM_OK)
            {
                __WARNING("Failed to parse: %s %c %s\r\n", num1, op, num2);
                Exn_release(&n1);
                Exn_release(&n2);
                return 0;
            }
            if (n2->error != EXTENDED_NUM_OK)
            {
                __WARNING("Failed to parse: %s %c %s\r\n", num1, op, num2);
                Exn_release(&n1);
                Exn_release(&n2);
                return 0;
            }
            BinOprExpr_build(&bin_op, op, n1, n2);
            Exn res = BinOprExpr_eval(&bin_op);
            if (res == NULL)
            {
                __WARNING("Failed to evaluate: %s %c %s\r\n", num1, op, num2);
                Exn_release(&n1);
                Exn_release(&n2);
                return 0;
            }

            printf("%s %c %s = ", num1, op, num2);

            Exn_release(&n1);
            Exn_release(&n2);

            if (res != NULL)
            {
                char* fmt_res = NULL;
                if (abs(res->decimal-1) > NORMAL_LIMIT)
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
            else
            {
                printf("Invalid input\r\n");
            }
        }
    }
}