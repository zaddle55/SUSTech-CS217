
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "math.h"
#include "error.h"
#include "util.h"

#define BANNER " \
                         _       _ __          \n\
               ____ ___  (_)___  (_) /_  _____ \n\
              / __ `__ \\/ / __ \\/ / __ \\/ ___/\n\
             / / / / / / / / / / / /_/ / /__  \n\
            /_/ /_/ /_/_/_/ /_/_/_.___/\\___/  \n\
                                              \n\
"

/// Tokenize the input string to a list of strings
/// @param in The input string
/// @param len The length of the output list
/// @return The list of strings
/// @example split("1 + 2", &len) -> ["1", "+", "2"]
/// @example split("1-2", &len) -> ["1", "-", "2"]
/// @example split("1x2", &len) -> ["1", "x", "2"]
/// @example split("1/2+3", &len) -> ["1", "/", "2", "+3"]
/// @example split("+1-+2", &len) -> ["+1", "-", "+2"]
/// @example split("exp(19, )") -> ["exp", "19"]
/// @example split("exp(19, 2)") -> ["exp", "19", "2"]
/// @example split("pow(2, -3)") -> ["pow", "2", "-3"]
char** split(const char* in, int* len)
{
    if (!in || !len) return NULL;
    
    // Determine the maximum possible number of tokens (worst case: each character is a token)
    int max_tokens = strlen(in) + 1;
    char** tokens = (char**)malloc(max_tokens * sizeof(char*));
    if (!tokens) return NULL;
    
    // Initialize token count
    *len = 0;
    
    // Current position in the input string
    int pos = 0;
    int input_len = strlen(in);
    
    while (pos < input_len) {
        // Skip whitespace
        while (pos < input_len && isspace(in[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        // Handle function names (letters followed by opening parenthesis)
        if (isalpha(in[pos])) {
            int start = pos;
            
            // Read function name
            while (pos < input_len && (isalnum(in[pos]) || in[pos] == '_')) {
                pos++;
            }
            
            // Check if this might be part of scientific notation (e.g., "1.23e+4")
            if ((pos - start == 1) && (in[start] == 'e' || in[start] == 'E') && 
                pos < input_len && (in[pos] == '+' || in[pos] == '-') &&
                *len > 0) {
                
                // Look at previous token to see if it's a number
                char* prev_token = tokens[*len - 1];
                bool is_prev_number = false;
                
                // Check if previous token is a valid number
                if (prev_token) {
                    is_prev_number = true;
                    for (int i = 0; prev_token[i]; i++) {
                        if (!isdigit(prev_token[i]) && prev_token[i] != '.') {
                            is_prev_number = false;
                            break;
                        }
                    }
                }
                
                if (is_prev_number) {
                    // This is likely scientific notation
                    // Remove the previous token (number)
                    char* number_part = prev_token;
                    (*len)--;
                    
                    // Start position for the full scientific notation
                    start = pos - 1; // Include 'e' or 'E'
                    pos++; // Skip over '+' or '-'
                    
                    // Continue reading the exponent part
                    while (pos < input_len && isdigit(in[pos])) {
                        pos++;
                    }
                    
                    // Calculate length of scientific notation part
                    int sci_len = pos - start;
                    
                    // Combine number part with scientific notation
                    int number_len = strlen(number_part);
                    char* sci_notation = (char*)malloc(number_len + sci_len + 1);
                    
                    if (!sci_notation) {
                        // Cleanup on memory allocation failure
                        free(number_part);
                        for (int i = 0; i < *len; i++) {
                            free(tokens[i]);
                        }
                        free(tokens);
                        *len = 0;
                        return NULL;
                    }
                    
                    // Copy number part
                    strcpy(sci_notation, number_part);
                    
                    // Append scientific notation part
                    strncpy(sci_notation + number_len, in + start, sci_len);
                    sci_notation[number_len + sci_len] = '\0';
                    
                    // Free the previous number part
                    free(number_part);
                    
                    // Add the combined token
                    tokens[(*len)++] = sci_notation;
                    continue;
                }
            }
            
            // Extract the function name
            int name_len = pos - start;
            char* func_name = (char*)malloc(name_len + 1);
            if (!func_name) {
                // Cleanup on memory allocation failure
                for (int i = 0; i < *len; i++) {
                    free(tokens[i]);
                }
                free(tokens);
                *len = 0;
                return NULL;
            }
            
            strncpy(func_name, in + start, name_len);
            func_name[name_len] = '\0';
            tokens[(*len)++] = func_name;
            
            // Skip whitespace after function name
            while (pos < input_len && isspace(in[pos])) {
                pos++;
            }
            
            // Handle opening parenthesis after function name
            if (pos < input_len && (in[pos] == '(' || in[pos] == ')')) {
                pos++; // Skip the opening parenthesis
            }
            
            continue;
        }
        
        // Handle operators (+, -, *, /, etc.)
        if (strchr("+-*/()^%,", in[pos])) {
            // Special case: +/- as part of a number
            if ((in[pos] == '+' || in[pos] == '-') && 
                (pos == 0 || 
                 strchr("+-*/^(,", in[pos-1]) || 
                 isspace(in[pos-1]))) {
                
                // This +/- is likely part of a number, not a separate operator
                int start = pos++;
                
                // Read the number
                while (pos < input_len && (isdigit(in[pos]) || in[pos] == '.')) {
                    pos++;
                }
                
                // Check for scientific notation
                if (pos < input_len && (in[pos] == 'e' || in[pos] == 'E')) {
                    pos++; // Skip the 'e' or 'E'
                    
                    // Handle exponent sign if present
                    if (pos < input_len && (in[pos] == '+' || in[pos] == '-')) {
                        pos++;
                    }
                    
                    // Read exponent digits
                    while (pos < input_len && isdigit(in[pos])) {
                        pos++;
                    }
                }
                
                // Extract the number with sign
                int num_len = pos - start;
                char* num_str = (char*)malloc(num_len + 1);
                if (!num_str) {
                    // Cleanup on memory allocation failure
                    for (int i = 0; i < *len; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    *len = 0;
                    return NULL;
                }
                
                strncpy(num_str, in + start, num_len);
                num_str[num_len] = '\0';
                tokens[(*len)++] = num_str;
            } else {
                // This is a standalone operator
                char* op = (char*)malloc(2);
                if (!op) {
                    // Cleanup on memory allocation failure
                    for (int i = 0; i < *len; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    *len = 0;
                    return NULL;
                }
                
                op[0] = in[pos++];
                op[1] = '\0';
                tokens[(*len)++] = op;
            }
            
            continue;
        }
        
        // Handle numbers (including scientific notation)
        if (isdigit(in[pos]) || in[pos] == '.') {
            int start = pos;
            
            // Read the number
            while (pos < input_len && (isdigit(in[pos]) || in[pos] == '.')) {
                pos++;
            }
            
            // Check for scientific notation
            if (pos < input_len && (in[pos] == 'e' || in[pos] == 'E')) {
                pos++; // Skip the 'e' or 'E'
                
                // Handle exponent sign if present
                if (pos < input_len && (in[pos] == '+' || in[pos] == '-')) {
                    pos++;
                }
                
                // Read exponent digits
                while (pos < input_len && isdigit(in[pos])) {
                    pos++;
                }
            }
            
            // Extract the number
            int num_len = pos - start;
            char* num_str = (char*)malloc(num_len + 1);
            if (!num_str) {
                // Cleanup on memory allocation failure
                for (int i = 0; i < *len; i++) {
                    free(tokens[i]);
                }
                free(tokens);
                *len = 0;
                return NULL;
            }
            
            strncpy(num_str, in + start, num_len);
            num_str[num_len] = '\0';
            tokens[(*len)++] = num_str;
            
            continue;
        }
        
        // Unknown character - just skip it
        pos++;
    }
    
    // If no tokens were found, ensure at least one empty token is returned
    if (*len == 0) {
        tokens[0] = strdup("");
        *len = 1;
    }
    
    return tokens;
}

void start_interactive()
{
    fprintf(stdout, BANNER);
    fprintf(stdout, "====== Interactive Mode (type \"quit\" or 'q' to leave) ======\n");

    while (1)
    {
        char str[INPUT_BUFFER];
        fprintf(stdout, "> ");
        fgets(str, INPUT_BUFFER, stdin);

        if (str[0] == 'q')
        {
            break;
        }

        char* p = str;
        while (*p == ' ')
        {
            p++;
        }

        char* num1, *num2;
        char op = '+';
        int token_len = 0;
        char** tokens = split(p, &token_len);
        // print tokens
        for (int i = 0; i < token_len; i++)
        {
            printf("token[%d]: %s\n", i, tokens[i]);
        }
    }
}

int main( int argc, char *argv[] )
{
    int opt = 0;
    bool verbose = false, help = false, has_precision = false;
    int precision = 2;

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

    if (use_interactive) start_interactive();
    else if (help)
    {
        printf("Usage: %s operand1 operation operand2 [option1]\n", argv[0]);
        printf("or: %s [option2]\n", argv[0]);
        printf("Option1:\n");
        printf("  -p <digit-int>, --precision\tSet the precision of the output\n");
        printf("Option2:\n");
        printf("  -h, --help\tDisplay this help message\n");
        printf("  -v, --verbose\tDisplay the precision\n");
        printf("  -p <digit-num>, --precision <digit-num>\tStart interactive mode with specified precision\n");
    }
    else if (verbose)
    {
        printf("Precision: %d\n", precision);
    }
    else {
        for (int i = optind; i < argc; i++)
        {
            printf("%s\n", argv[i]);
        }
    }
}