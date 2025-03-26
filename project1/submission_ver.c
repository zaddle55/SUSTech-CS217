#include <stdio.h>

#define bool unsigned char
#define true (unsigned char)1
#define false (unsigned char)0

#define TOKEN_BUFFER 1024
#define DEFALT_PRECISION 100
#define NORMAL_LIMIT 20

// ############################## Test Utils ##############################
typedef struct {
    void (*func)(void);
    const char *name;
} test_entry_t; /* Test Unit Struct */

#define MAX_TESTS 100
static test_entry_t test_registry[MAX_TESTS];
static int test_count = 0;

void register_test(void (*test_func)(void), const char *name);

/**
 * @brief Auxiliary macro to register a test
 * @param test_name The name of the test
 * @code
 * TEST(add)
 * {
 *     assert(1 + 1 == 2);
 * }
 * @endcode
 */
#define TEST(test_name) \
    static void test_##test_name(void); \
    static void __attribute__((constructor)) register_##test_name(void) { \
        register_test(test_##test_name, #test_name); \
    } \
    static void test_##test_name(void)

void run_all_tests(void);

// ############################## Scope Manage Macro ##############################
#define NO_EXPORT __attribute__((visibility("hidden")))
#define EXPORT __attribute__((visibility("default")))
#define EXPORT_C extern "C" EXPORT
#define INLINE __attribute__((always_inline))

// ############################## MINMAX Macro ##############################
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * @file error.h
 * @brief Error handling macros
 * @details This file contains macros for error handling, including panic, warning, and notice messages.
 * @date 2025-03-15
 * @author zaddle
 */

// ############################## Basic Macro ##############################

#define __ERR_OUT(fmt, ...) do { \
    fprintf(stderr, "%s:%d: at function '%s'\r\n  \u2514\u2500\u2500\u2500", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    fprintf(stderr, "\r\n"); \
} while (0)

/// @brief Panic macro for serious errors
/// @param msg Message to print
/// @details This macro prints an error message and exits the program
#define __PANIC(fmt, ...) do { \
    fprintf(stderr, "\033[1;31m [ERROR]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
    exit(1); \
} while (0)

/// @brief Notice macro for non-serious errors
/// @param msg Message to print
/// @details This macro prints a notice message
#define __NOTION(fmt, ...) do { \
    fprintf(stderr, "\033[1;34m [NOTICE]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
} while (0)

/// @brief Warning macro for warnings
/// @param msg Message to print
/// @details This macro prints a warning message
#define __WARNING(fmt, ...) do { \
    fprintf(stderr, "\033[1;33m [WARNING]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
} while (0)

#define ANSI_RESET  "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_PURPLE  "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"

/**
 * @brief print wavy line below the input string in different colors
 * @param __input The input string
 * @param __pos The position of the wavy line
 * @param __len The length of the wavy line
 * @details This macro will print marked input string with wavy line like this:
 * e.g. WAVY_RED("Hello, World!", 6, 5);
 * <<<  Hello, World!
 * <<<         ~~~~~
 *
 */
 #define WAVY(__input, __pos, __len, ANSI_COLOR) \
    fprintf(stderr, "\t%s\r\n\t", (__input)); \
    fprintf(stderr, ANSI_COLOR); \
    for (int __i = 0; __i < (__pos); __i++) fprintf(stderr, " "); \
    fprintf(stderr, "^"); \
    for (int __i = 1; __i < (__len); __i++) fprintf(stderr, "~"); \
    fprintf(stderr, "\r\n"); \
    fprintf(stderr, ANSI_RESET);fprintf(stderr, "\r\n");

// ############################## Math Exception ##############################

#define MATH_EXCEPT_
#define MATH_CVT_FAIL(__from, __to, __why) __WARNING("<Math Exception> Cannot convert '%s' to math type '%s': %s", __from, __to, __why)
#define MATH_PREC_LOSE(__cur, __tar) __NOTION("<Math> Precision loss for current precision = %d, increase to %d at least.", __cur, __tar)
#define MATH_DIV_ZERO(__why) __WARNING("<Math Exception> Division by zero: %s", __why)
#define MATH_SQRT_NEG(__base) __WARNING("<Math Exception> Square root of negative number: %s", __base)
#define MATH_OVERFLOW(__why) __WARNING("<Math Exception> Overflow: %s", __why)
#define MATH_URECG_OP(__op) __WARNING("<Math Exception> Unrecognized operation '%c'", __op)

// ############################## Input Exception ##############################

#define INPUT_EXCEPT_
#define INPUT_OPTION_FAIL(__opt) __WARNING("<Input Exception> No such option '%s'", __opt)
#define INPUT_ARG_FAIL(__arg, __why) __WARNING("<Input Exception> Invalid argument '%s': %s", __arg, __why)

// ############################## Funct Exception ##############################

#define FUNCT_EXCEPT_
#define FUNCT_INVOKE_FAIL(__func, __why) __WARNING("<Function Exception> Cannot invoke function '%s': %s", __func, __why)
#define FUNCT_ARG_MISMATCH(__func, __argc, __inargc) __WARNING("<Function Exception> Argument mismatch in function '%s': the function should have %d arguments but received %d arguments.", __func, __argc, __inargc)

// ############################## MathExpr Exception ##############################

#define MEXPR_EXCEPT_
#define MEXPR_PARSE_FAIL(__expr, __why) __WARNING("<MathExpr Exception> Cannot parse expression '%s': %s", __expr, __why)
#define MEXPR_RESULT_ERROR(__why) __WARNING("<MathExpr Exception> The result got an error: %s", __why)

// ############################## Memory Exception ##############################

#define MEM_EXCEPT_
#define MEM_ALLOC_FAIL(__at, __why) __PANIC("<Memory Exception> Memory allocation at <%p> failed: %s", __at, __why)
#define MEM_FREE_FAIL(__at, __why) __PANIC("<Memory Exception> Memory free failed at <%p>: %s", __at, __why)
#define MEM_ACCESS_NULLPTR(__ptr_n, __why) __PANIC("<Memory Exception> Accessing NULL pointer '%s': %s", __ptr_n, __why)
#define MEM_ACCESS_OOB(__ptr_n, __idx, __size) __PANIC("<Memory Exception> Accessing out of bound at index %d of '%s' with size %d", __idx, __ptr_n, __size)


/**
 * @file shell.h
 * @brief Header file for shell.c
 * @details This file contains the function prototypes and definitions for the context navigation
 *          and command history management in the shell.
 * @date 2025-03-15
 * @author zaddle
 */
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_HISTORY 100
#define MAX_INPUT_SIZE 1024 * 1024

/* function prototype */
void enable_raw_mode();
void disable_raw_mode();
void clear_line(int position);
void refresh_line(const char *prompt, char *buffer);
void add_to_history(const char *cmd);
char* read_line(const char *prompt);
void cleanup_history();

/**
 * @file math.h
 * @brief Header file for math operations
 * @details This file contains the function prototypes and definitions for extended number operations
 *          and mathematical functions.
 * @date 2025-03-15
 * @author zaddle
 */
/* for cplusplus function call  */
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <complex.h>
#include <assert.h>
#include <math.h>

/* A macro to return the value of a digit character */
#define Ch2val(c) (c - '0')
#ifndef complex
#define complex _Complex double
#endif

// ############################## Extended Number Class ##############################

/* constants for ExtendedNum class */
#define MUL_OPTIMIZE_LEAST 1000 /* The least number of digits to use the optimized multiplication */
#define EXN_EPSILON 1e-16

/* error codes for ExntendedNum class */
typedef enum {

    EXTENDED_NUM_OK = 0,            /* Stand for a no-fault ExtendedNum*/
    EXTENDED_NUM_OVERFLOW,          /* Stand for an overflow error */
    EXTENDED_NUM_DIVIDE_BY_ZERO,    /* Stand for a division by zero error */
    EXTENDED_NUM_INVALID_INPUT,     /* Stand for an invalid input error */
    EXTENDED_NUM_MEMORY_ERROR       /* Stand for a memory allocation error */

} ExtendedNumError, Exnerr;

/* mode for ExtendedNumber formatting */
typedef enum {
    EXN_FMT_NORMAL = 0,     /* Normal mode */
    EXN_FMT_SCIENTIFIC,     /* Scientific mode */
    EXN_FMT_PRECISE         /* Strict Precise mode */
} Exnfmt;

const char* Exn_err2str(Exnerr error);

typedef struct {
    char* digits;   /* The pointer to valid digits array of the number*/
    int length;     /* The length of the number */
    int decimal;    /* The refernce position of the decimal point */
    int sign;       /* The sign of the number */
    int precision;   /* The precision of the number */
    Exnerr error;   /* The error code of the number */
} Exn_struct, *Exn;

/* function prototypes */

/**
 * @brief Create an extended number
 * @details Allocate memory for the extended number
 * @note The caller is responsible for releasing the memory
 * @return ExtendedNum* A pointer extended number
 */
Exn Exn_create(int prec);

/**
 * @brief Generate a random extended number with a given precision
 * @param prec The precision of the extended number
 * @return ExtendedNum* A pointer to the random extended number
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_rand(int prec);

/**
 * @brief Generate a random extended number with a given precision and exponent
 * @param prec The precision of the extended number
 * @param exp The exponent of the extended number
 * @return ExtendedNum* A pointer to the random extended number
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_randp(int prec, int exp);

/**
 * @brief Release the memory of the extended number
 * @param num Extended number to release
 * @note Don't use the extended number after releasing
 * or release it again
 */
void Exn_release( Exn* num );

/**
 * @brief transform a string to an extended number
 * @param str The string to transform
 * @return Exn The extended number
 * @code
 * Exn num = atoExn("123.456E-16");
 * if (num != NULL) {
 *     // do something
 *     Exn_release(num);
 * }
 * @endcode
 * @note The caller is responsible for releasing the memory
 */
Exn atoExn( const char* str, int prec );

/**
 * @brief Get the absolute value of the extended number
 * @param num The extended number
 * @return Exn The absolute value of the extended number
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_abs( const Exn num );

/**
 * @brief check if the extended number is zero
 * @param num The extended number
 * @return true if the extended number is zero, false otherwise
 */
bool Exn_iszero( const Exn num );

/**
 * @brief check if the extended number is negative
 * @param num The extended number
 * @return true if the extended number is negative, false otherwise
 */
bool Exn_isneg( const Exn num );

/**
 * @brief Add two extended numbers
 * @param num1 The first addend
 * @param num2 The second addend
 * @return Exn The result of the addition
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_add( const Exn num1, const Exn num2 );

/**
 * @brief Subtract two extended numbers
 * @param num1 The minuend
 * @param num2 The subtrahend
 * @return Exn The result of the subtraction
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_sub( const Exn num1, const Exn num2 );

/**
 * @brief Multiply two extended numbers
 * @param num1 The first factor
 * @param num2 The second factor
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_mul( const Exn num1, const Exn num2 );

/**
 * @brief Divide two extended numbers
 * @param num1 The dividend
 * @param num2 The divisor
 * @return Exn The result of the division
 * @details This function uses newton's method to calculate the division
 * @note The caller is responsible for releasing the memory
 * @attention The divisor must not be zero
 */
Exn Exn_div( const Exn num1, const Exn num2);

/**
 * @brief Process the newton's method according to the given function
 * @param func The function to process
 * @param bound The target scale of the result
 * @param result The result of the newton's method
 */
void Exn_newton(Exn (*func)(Exn, Exn), int bound, Exn param, Exn* result);

Exn Exn_divmod( const Exn num1, const Exn num2, Exn* mod );

Exn Exn_modulo( const Exn num1, const Exn num2 );

Exn Exn_sqrt( const Exn num );

void Exn_round( Exn* num, int scale, int type);

/**
 * @brief Calculate the factorial of a number
 * @param n The number to calculate the factorial
 * @return Exn The result of the factorial
 * @note The caller is responsible for releasing the memory
 * @attention The number must be a non-negative integer
 * @details Divide and conquer algorithm is used to calculate the factorial
 */
Exn fact(int n);

/**
 * @brief Power of an extended number
 * @details Library function ksm() is used to calculate the power of an extended number;
 * this is based on fast exponentiation algorithm
 * @param base The base of the power
 * @param exp The exponent of the power
 * @return Exn The result of the power
 * @note The caller is responsible for releasing the memory
 * @note The exponent must be an integer ranged
 * @see https://en.wikipedia.org/wiki/Exponentiation_by_squaring
 */
Exn ksm( const Exn base, const int exp );

/**
 * @brief Compare two extended numbers
 * @param num1 The first number
 * @param num2 The second number
 * @return int The result of the comparison
 * @retval 0 if the two numbers are equal
 * @retval 1 if num1 > num2
 * @retval -1 if num1 < num2
 */
int Exn_cmp( const Exn num1, const Exn num2 );

/**
 * @brief convert the extended number to an integer
 * @param num The extended number
 * @return int The integer value of the extended number
 * @note This function will truncate the decimal part of the number
 *       and won't handle the overflow
 */
int Exn_toInt( const Exn num );
/**
 * @brief Normalize the extended number
 * @param num The extended number
 * @note This function will modify the extended number
 * @details This function will remove the leading zeros and trailing zeros
 *          and adjust the decimal point accordingly
 *          The function will also check the precision of the number
 */
void Exn_normalize( Exn *num );

/**
 * @brief Format the extended number to a string
 * @param num The extended number
 * @param mode The format mode:
 * EXN_FMT_NORMAL(0): normal format;
 * EXN_FMT_PRECISE(1): precise format;
 * EXN_FMT_SCIENTIFIC(2): scientific format.
 * @return char* The formatted string
 * @note The caller is responsible for releasing the memory of the string
 */
char* Exn_fmt( const Exn num, Exnfmt mode );

/**
 * @brief Output the details of the extended number
 * @param num The extended number
 */
void Exn_show( const Exn num );

// ############################## BinOprExpr Class ##############################

/// @brief Binary operand expression class
typedef struct { 
    char binop; // binary operator
    Exn num1; // number 1
    Exn num2; // number 2
} BinOprExpr;

Exn BinOprExpr_eval(const BinOprExpr* expr);
void BinOprExpr_release(BinOprExpr** expr);
void BinOprExpr_build(BinOprExpr* binop, char op, Exn num1, Exn num2);

// ############################## Math Func Class ##############################

#define POW_LIMIT 0x0000ffff  /* upper limit of the power */
#define FACT_LIMIT 0x0000ffff /* upper limit of the factorial */

/// @brief Math function type
/// @details This enum is used to represent the built-in math function type
typedef enum {
    MFUNC_SQRT = 0,
    MFUNC_POWER,
    MFUNC_DIVMOD,
    MFUNC_MODULO,
    MFUNC_FACT,
    MFUNC_ALL
} MathFunc_T;

/// @brief Math function class
typedef struct {
    int type; // math
    char* alias; // alias of the function
    uint8_t argc; // number of arguments
    Exn* args; // arguments
} MathFunc;

/// @brief Check the arguments for different math functions
/// @param func Math function to check
/// @return true if the arguments are correct, false otherwise
/// @attention please add the argument rules here for each math function
bool MFunc_ckarg(const MathFunc* func);

Exn MFunc_eval(const MathFunc* func);

void MFunc_release(MathFunc** func);

void MFunc_build(MathFunc* func, int type, const char* alias, uint8_t argc, Exn* args);

// ############################## Math Expr Class ##############################
typedef enum {
    MATH_EXPR_BINOP = 0,
    MATH_EXPR_FUNC,
    MATH_EXPR_ECHO
} MExprType;

/// @brief Math expression class
typedef struct {
    MExprType type; // type of the expression
    union {
        BinOprExpr* binop; // binary operand expression
        MathFunc* func; // math function
        Exn echo; // echo expression
    } expr; // expression
} MathExpr;

/// @brief Evaluate the math expression
/// @param expr Math expression to evaluate
/// @return Exn The result of the evaluation
/// @note The caller is responsible for releasing the memory
Exn MathExpr_eval(const MathExpr* expr);

/// @brief Release the memory of the math expression
void MathExpr_release(MathExpr* expr);

/// @brief Create a math expression
int MathExpr_build(char* expr_str, MathExpr* expr, int prec);

#ifdef __cplusplus
}
#endif

// ################################################################################
// ############################## End of Header File ##############################
// ################################################################################

// ################################################################################
// ############################## Start of Impl File ##############################
// ################################################################################

/**
 * @file utils.c
 */
void register_test(void (*test_func)(void), const char *name)
 {
    if (test_count < MAX_TESTS) {
        test_registry[test_count].func = test_func;
        test_registry[test_count].name = name;
        test_count++;
    }
}

void run_all_tests(void) {
    printf("Running %d tests...\r\n\n", test_count);
    
    for (int i = 0; i < test_count; i++) {
        printf("Test '%s': ", test_registry[i].name);
        test_registry[i].func();
        printf("\r\n");
    }
    
    printf("All tests completed.\r\n");
}

/**
 * @file shell.c
 */

char *history[MAX_HISTORY];
int history_count = 0;
int current_history = -1;

struct termios orig_termios;

// 初始化终端为原始模式
void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    // 禁用回显和规范模式，但保留其他功能
    raw.c_lflag &= ~(ECHO | ICANON);
    // 禁用 Ctrl-C 和 Ctrl-Z 信号
    raw.c_lflag &= ~(ISIG);
    // 禁用 Ctrl-S 和 Ctrl-Q 控制流
    raw.c_iflag &= ~(IXON);
    // 禁用 CR 到 NL 的转换
    raw.c_iflag &= ~(ICRNL);
    // 输出处理
    raw.c_oflag &= ~(OPOST);
    // 设置读取超时
    raw.c_cc[VMIN] = 0;  // 最小字符数
    raw.c_cc[VTIME] = 1; // 超时 (十分之一秒)
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// 恢复终端设置
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// 清除当前行
void clear_line(int position) {
    printf("\r");           // 回到行首
    for (int i = 0; i < position + 2; i++) {
        printf(" "); 
    }
    printf("\r");
}

// 刷新显示当前行
void refresh_line(const char *prompt, char *buffer) {
    printf("\r%s%s", prompt, buffer);
    fflush(stdout);
}

// 添加命令
void add_to_history(const char *cmd) {
    if (strlen(cmd) == 0) return;
    
    // 避免连续相同的命令
    if (history_count > 0 && strcmp(history[history_count-1], cmd) == 0) {
        return;
    }

    if (history_count >= MAX_HISTORY) {
        free(history[0]);
        for (int i = 0; i < history_count - 1; i++) {
            history[i] = history[i+1];
        }
        history_count--;
    }
    
    history[history_count] = strdup(cmd);
    history_count++;
}

// 读取一行输入
char* read_line(const char *prompt) {
    char *buffer = (char *)malloc(MAX_INPUT_SIZE);
    int position = 0;
    buffer[0] = '\0';
    current_history = history_count;
    
    printf("%s", prompt);
    fflush(stdout);
    
    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 0) {
            continue; // 超时
        }
        
        // 处理退出和换行
        if (c == 4) { // Ctrl-D
            free(buffer);
            return NULL;
        } else if (c == '\r' || c == '\n') {
            printf("\r\n");
            buffer[position] = '\0';
            return buffer;
        } 
        // 处理退格键
        else if (c == 127 || c == 8) { // Backspace
            if (position > 0) {
                position--;
                buffer[position] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        } 
        // 处理方向键和其他特殊键序列
        else if (c == 27) {
            char seq[3];
            
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;
            
            if (seq[0] == '[') {
                if (seq[1] == 'A') { // 上箭头
                    if (history_count > 0 && current_history > 0) {
                        current_history--;
                        clear_line(position);
                        strcpy(buffer, history[current_history]);
                        position = strlen(buffer);
                        refresh_line(prompt, buffer);
                    }
                } else if (seq[1] == 'B') { // 下箭头
                    if (current_history < history_count) {
                        current_history++;
                        clear_line(position);
                        
                        // 显示较新的历史命令或清空
                        if (current_history == history_count) {
                            buffer[0] = '\0';
                            position = 0;
                        } else {
                            strcpy(buffer, history[current_history]);
                            position = strlen(buffer);
                        }
                        refresh_line(prompt, buffer);
                    }
                }
            }
        } 
        // 处理常规输入
        else if (c >= 32 && c < 127) {
            if (position < MAX_INPUT_SIZE - 1) {
                buffer[position++] = c;
                buffer[position] = '\0';
                printf("%c", c);
                fflush(stdout);
            }
        }
    }
}

// 清理历史记录
void cleanup_history() {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
}

/**
 * @file math.c
 */

// ################################ Extended Number Constants ##############################

static int MAX_DIGITS;

static Exn_struct __zero__ = {.digits = NULL,
                              .length = 1,
                              .decimal = 1,
                              .sign = 0,
                              .precision = 0,
                              .error = EXTENDED_NUM_OK};

static Exn_struct __one__ = {.digits = NULL,
                             .length = 1,
                             .decimal = 1,
                             .sign = 1,
                             .precision = 0,
                             .error = EXTENDED_NUM_OK};

static Exn_struct __two__ = {.digits = NULL,
                             .length = 1,
                             .decimal = 1,
                             .sign = 1,
                             .precision = 0,
                             .error = EXTENDED_NUM_OK};

static Exn_struct __half__ = {.digits = NULL,
                              .length = 1,
                              .decimal = 0,
                              .sign = 1,
                              .precision = 0,
                              .error = EXTENDED_NUM_OK};

static Exn_struct __threehalfs__ = {.digits = NULL,
                                    .length = 2,
                                    .decimal = 1,
                                    .sign = 1,
                                    .precision = 0, 
                                    .error = EXTENDED_NUM_OK};

void init_zero(int prec) {
  __zero__.precision = prec;
  __zero__.digits = (char *)malloc(__zero__.precision * sizeof(char));
  if (__zero__.digits == NULL) {
    MEM_ALLOC_FAIL(&__zero__.digits,
                   "The extended number zero allocation failed");
    return;
  }
  __zero__.digits[0] = '0';
  __zero__.digits[1] = '\0';
}

__attribute((destructor)) void free_zero() {
  if (__zero__.digits) {
    free(__zero__.digits);
  }
}

void init_one(int prec) {
  __one__.precision = prec;
  __one__.digits = (char *)malloc(__one__.precision * sizeof(char));
  if (__one__.digits == NULL) {
    MEM_ALLOC_FAIL(&__one__.digits,
                   "The extended number one allocation failed");
    return;
  }
  __one__.digits[0] = '1';
  __one__.digits[1] = '\0';
}

__attribute((destructor)) void free_one() {
  if (__one__.digits) {
    free(__one__.digits);
  }
}

void init_two(int prec) {
  __two__.precision = prec;
  __two__.digits = (char *)malloc(__two__.precision * sizeof(char));
  if (__two__.digits == NULL) {
    MEM_ALLOC_FAIL(&__two__.digits,
                   "The extended number two allocation failed");
    return;
  }
  __two__.digits[0] = '2';
  __two__.digits[1] = '\0';
}

__attribute((destructor)) void free_two() {
  if (__two__.digits) {
    free(__two__.digits);
  }
}

void init_half(int prec) {
  __half__.precision = prec;
  __half__.digits = (char *)malloc(__half__.precision * sizeof(char));
  if (__half__.digits == NULL) {
    MEM_ALLOC_FAIL(&__half__.digits,
                   "The extended number half allocation failed");
    return;
  }
  __half__.digits[0] = '5';
  __half__.digits[1] = '\0';
}

__attribute((destructor)) void free_half() {
  if (__half__.digits) {
    free(__half__.digits);
  }
}

void init_threehalfs(int prec) {
  __threehalfs__.precision = prec;
  __threehalfs__.digits =
      (char *)malloc(__threehalfs__.precision * sizeof(char));
  if (__threehalfs__.digits == NULL) {
    MEM_ALLOC_FAIL(&__threehalfs__.digits,
                   "The extended number threehalfs allocation failed");
    return;
  }
  __threehalfs__.digits[0] = '1';
  __threehalfs__.digits[1] = '5';
  __threehalfs__.digits[2] = '\0';
}

__attribute((destructor)) void free_threehalfs() {
  if (__threehalfs__.digits) {
    free(__threehalfs__.digits);
  }
}

void init_all_constants(int prec) {
  init_zero(prec);
  init_one(prec);
  init_two(prec);
  init_half(prec);
  init_threehalfs(prec);
}

#define Exn_zero &__zero__             /* Constant Zero */
#define Exn_one &__one__               /* Constant One */
#define Exn_two &__two__               /* Constant Two */
#define Exn_half &__half__             /* Constant Half */
#define Exn_threehalfs &__threehalfs__ /* Constant Three Halfs */

// ################################ Extended Number Functions
// ##############################

const char *Exn_err2str(ExtendedNumError error) {
  switch (error) {
  case EXTENDED_NUM_OK:
    return "No error";
  case EXTENDED_NUM_OVERFLOW:
    return "Number overflow";
  case EXTENDED_NUM_DIVIDE_BY_ZERO:
    return "Division by zero";
  case EXTENDED_NUM_INVALID_INPUT:
    return "Invalid conversion into extended number";
  case EXTENDED_NUM_MEMORY_ERROR:
    return "Memory allocation error";
  default:
    return "Unknown error";
  }
}

// ################################# Extended Number Utils Functions
// ##############################

/**
 * @brief Iterative function for Newton's method to find the inverse
 * @param __n The number of divsor
 * @param __x The initial guess
 * @return Exn The inverse of the number
 * @details The function is:
 *          $x_{n+1} = (2 - n * x_n) * x_n$
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __ntiter_inv(const Exn __n, const Exn __x) {
  Exn tmp1 = Exn_mul(__n, __x);
  if (!tmp1)
    return NULL;
  Exn tmp2 = Exn_sub(Exn_two, tmp1);
  Exn_release(&tmp1);
  if (!tmp2)
    return NULL;
  Exn __nx = Exn_mul(tmp2, __x);
  Exn_release(&tmp2);
  return __nx;
}

/**
 * @brief Iterative function for Newton's method to find the square root
 * @param __n The number of divsor
 * @param __x The initial guess
 * @return Exn The square root of the number
 * @details The function is:
 *          $x_{n+1} = (3/2 - n / 2 * x_n^2) * x_n$
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __ntiter_sqrt(const Exn __n, const Exn __x) {
  Exn tmp1 = Exn_mul(__x, __x);
  if (!tmp1)
    return NULL;
  Exn tmp2 = Exn_mul(__n, tmp1);
  Exn_release(&tmp1);
  if (!tmp2)
    return NULL;
  Exn tmp3 = Exn_mul(Exn_half, tmp2);
  if (!tmp3)
    return NULL;
  Exn tmp4 = Exn_sub(Exn_threehalfs, tmp3);
  Exn_release(&tmp2);
  if (!tmp4)
    return NULL;
  Exn __nx = Exn_mul(tmp4, __x);
  Exn_release(&tmp3);
  Exn_release(&tmp4);
  return __nx;
}

/**
 * @brief Fast Fourier Transform
 * @param fa The array of complex numbers
 * @param n The number of complex numbers
 * @param inv The inverse flag(1 for inverse, 0 for forward)
 * @details The function uses butterfly transform to calculate the FFT
 * @see https://oi-wiki.org/math/poly/fft/
 * @attention include libm for cosine and sine functions
 */
void fft(complex *fa, int n, int inv) {
  // Bit-reversal permutation
  for (int i = 1, j = n / 2, k; i < n - 1; i++) {
    if (i < j) { /* swap fa[i] fa[j] */
      complex temp = fa[i];
      fa[i] = fa[j];
      fa[j] = temp;
    }
    k = n / 2;
    while (j >= k) {
      j = j - k;
      k /= 2;
    }
    if (j < k)
      j += k;
  }
  // Cooley-Tukey iterative FFT
  for (int len = 2; len <= n; len <<= 1) {
    double angle = 2.0 * M_PI / len * (inv ? -1 : 1);
    complex wn = cos(angle) + sin(angle) * I;

    for (int j = 0; j < n; j += len) {
      complex w = 1.0 + 0.0 * I;
      for (int k = j; k < j + len / 2; k++) {
        complex u = fa[k];
        complex v = fa[k + len / 2] * w;

        fa[k] = u + v;
        fa[k + len / 2] = u - v;

        w *= wn;
      }
    }
  }

  // Scale if inverse
  if (inv) {
    for (int i = 0; i < n; i++) {
      fa[i] /= n;
    }
  }
}

int *karatsuba(int n, int *a, int *b) {
  if (n <= 32) {
    int *r = (int *)calloc(n * 2 + 1, sizeof(int));
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= n; ++j) r[i + j] += a[i] * b[j];
    return r;
  }

  int m = n / 2 + 1;
  int *r = (int *)calloc(m * 4 + 1, sizeof(int));
  int *z0, *z1, *z2;

  z0 = karatsuba(m - 1, a, b);
  z2 = karatsuba(n - m, a + m, b + m);

  for (int i = 0; i + m <= n; ++i) a[i] += a[i + m];
  for (int i = 0; i + m <= n; ++i) b[i] += b[i + m];
  z1 = karatsuba(m - 1, a, b);
  for (int i = 0; i + m <= n; ++i) a[i] -= a[i + m];
  for (int i = 0; i + m <= n; ++i) b[i] -= b[i + m];
  for (int i = 0; i <= (m - 1) * 2; ++i) z1[i] -= z0[i];
  for (int i = 0; i <= (n - m) * 2; ++i) z1[i] -= z2[i];

  for (int i = 0; i <= (m - 1) * 2; ++i) r[i] += z0[i];
  for (int i = 0; i <= (m - 1) * 2; ++i) r[i + m] += z1[i];
  for (int i = 0; i <= (n - m) * 2; ++i) r[i + m * 2] += z2[i];

  free(z0);
  free(z1);
  free(z2);
  return r;
}

// ################################ Extended Number Member Functions
// ##############################

Exn Exn_create(int prec) {
  if (prec <= 0) {
    prec = MAX_DIGITS;
  }

  Exn num = (Exn)malloc(sizeof(Exn_struct));
  if (num == NULL) {
    MEM_ALLOC_FAIL(&num, "The extended number allocation failed");
    return NULL;
  }

  num->length = 1;
  num->sign = 0;
  num->decimal = -1;
  num->error = EXTENDED_NUM_OK;
  num->digits = (char *)calloc(prec + 10, sizeof(char));
  num->precision = prec;

  if (num->digits == NULL) {
    MEM_ALLOC_FAIL(&num->digits,
                   "The extended number digits allocation failed");
    free(num);
    return NULL;
  }

  return num;
}

Exn Exn_rand(int prec) {
  Exn num = Exn_create(prec);
  if (num == NULL)
    return NULL;

  int len = rand() % (prec + 1) + 1; // at least 1 digit
  num->length = len;
  num->decimal = rand() % (len + 1);
  num->sign = rand() % 2 ? 1 : -1;

  for (int i = 0; i < len; i++) {
    num->digits[i] = '0' + rand() % 10;
  }
  num->digits[len] = '\0';

  return num;
}

Exn Exn_randp(int prec, int exp) {
  Exn num = Exn_create(prec);
  if (num == NULL)
    return NULL;

  int len = rand() % (prec + 1) + 1; // at least 1 digit
  num->length = len;
  num->decimal = exp;
  num->sign = rand() % 2 ? 1 : -1;

  for (int i = 0; i < len; i++) {
    num->digits[i] = '0' + rand() % 10;
  }
  num->digits[len] = '\0';

  return num;
}

void Exn_release(Exn *num) {
  if (*num == NULL)
    return;
  if ((*num)->digits) {
    free((*num)->digits);
  }
  free(*num);
  *num = NULL;
}

/**
 * @brief Copy the extended number
 * @param dest Destination extended number
 * @param src Source extended number
 * @note This is a deep copy method
 */
NO_EXPORT void Exn_cpy(Exn *dest, const Exn src) {
  Exn_release(dest);
  *dest = Exn_create(src->precision);
  if (dest == NULL) {
    MEM_ALLOC_FAIL(dest, "The extended number copy allocation failed");
    return;
  }
  (*dest)->length = src->length;
  (*dest)->decimal = src->decimal;
  (*dest)->sign = src->sign;
  (*dest)->error = src->error;
  (*dest)->precision = src->precision;
  // for (int i = 0; i < src->length; i++) {
  //     (*dest)->digits[i] = src->digits[i];
  // }
  memcpy((*dest)->digits, src->digits, src->length);
}

/**
 * @brief Move the extended number
 * @param dest Destination extended number
 * @param src Source extended number
 * @note The source extended number will be moved to destination
 */
NO_EXPORT void Exn_mv(Exn *dest, Exn *src) {
  Exn_release(dest);
  *dest = *src;
  *src = NULL;
}

/**
 * @brief Convert a string to an extended number
 * @param str The string to convert
 * @param prec The precision scale of the conversion
 * @return Exn The extended number
 * @attention A string with scientific notation is not supported
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __Exn_fromstr(const char *str, int prec) {
  if (str == NULL || str[0] == '\0')
    return NULL;
  Exn num = Exn_create(prec);
  if (num == NULL)
    return NULL;

  int len = strlen(str);
  int digit_count = 0;
  bool has_decimal = false;
  bool has_digit = false;
  int pos = 0;

  /* skip the leading spaces */
  while (pos < len && isspace(str[pos])) {
    pos++;
  }

  /* extract the sign */
  if (str[pos] == '-') {
    num->sign = -1;
    pos++;
  } else {
    num->sign = 1;
    if (str[pos] == '+') {
      pos++;
    }
  }

  /* skip the leading zeros */
  while (pos < len && str[pos] == '0') {
    pos++;
  }

  /* This is an zero  */
  if (pos == len || (pos == len - 1 && str[pos] == '.')) {
    Exn_cpy(&num, Exn_zero);
    return num;
  }

  /* extract the digits */
  for (; pos < len; pos++) {

    if (str[pos] == '.') { /* encounter a decimal point */
      if (has_decimal) {
        num->error = EXTENDED_NUM_INVALID_INPUT;
        MATH_CVT_FAIL("str", "ExtendedNumber", "too many decimal points");
        WAVY(str, pos, 1, ANSI_PURPLE);
        return num;
      }
      has_decimal = true;
      num->decimal = digit_count;
    } else if (isdigit(str[pos])) { /* encounter a digit */
      has_digit = true;
      if (digit_count < prec) {
        num->digits[digit_count++] = str[pos];
      } else {
        digit_count++;
      }
    } else {
      num->error = EXTENDED_NUM_INVALID_INPUT;
      MATH_CVT_FAIL("str", "ExtendedNumber", "not a digit");
      WAVY(str, pos, len - pos, ANSI_RED);
      return num;
    }
  }

  if (!has_digit) {
    Exn_cpy(&num, Exn_zero);
    return num;
  }

  if (!has_decimal) {
    num->decimal = digit_count;
  }

  if (digit_count > prec)
    MATH_PREC_LOSE(prec, digit_count);

  num->length = digit_count;
  Exn_normalize(&num);
  return num;
}

Exn atoExn(const char *str, int prec) {
  if (str == NULL || str[0] == '\0')
    return NULL;

  int len = strlen(str);
  int e_pos = -1; /* pointer to find 'e' or 'E' symbol */

  for (int i = 0; i < len; i++) {
    if (str[i] == 'e' || str[i] == 'E') {
      e_pos = i;
      break;
    }
  }

  if (e_pos == -1)
    return __Exn_fromstr(str, prec);

  /* extract the base */
  char *base_str = (char *)malloc(e_pos + 1);
  if (base_str == NULL) {
    MEM_ALLOC_FAIL(&base_str, "The base string allocation failed");
    return NULL;
  }
  strncpy(base_str, str, e_pos);
  base_str[e_pos] = '\0';
  Exn base = __Exn_fromstr(base_str, prec);
  free(base_str);

  if (!base || base->error != EXTENDED_NUM_OK) {
    return base;
  }

  /* extract the exponent */
  int exponent = 0;
  int exp_sign = 1;
  int exp_start = e_pos + 1;

  if (exp_start < len) {
    if (str[exp_start] == '+') {
      exp_start++;
    } else if (str[exp_start] == '-') {
      exp_sign = -1;
      exp_start++;
    }

    for (int i = exp_start; i < len; i++) {
      if (isdigit(str[i])) {
        exponent = exponent * 10 + Ch2val(str[i]);
      } else {
        base->error = EXTENDED_NUM_INVALID_INPUT;
        if (str[i] == '.') {
          MATH_CVT_FAIL("str", "ExtendedNumber",
                        "no decimal point in an exponent");
        } else {
          MATH_CVT_FAIL("str", "ExtendedNumber", "not a digit in an exponent");
        }
        WAVY(str, i, len - i, ANSI_CYAN);
        return base;
      }
    }
  }

  exponent *= exp_sign;
  base->decimal += exponent;
  Exn_normalize(&base);
  return base;
}

void Exn_normalize(Exn *num) {
  if (*num == NULL || (*num)->digits == NULL)
    return;

  /* get the index of the first non-zero digit */
  int first_non_zero = 0;
  while (first_non_zero < (*num)->length &&
         (*num)->digits[first_non_zero] == '0') {
    first_non_zero++;
  }

  /* is zero */
  if (first_non_zero == (*num)->length) {
    return;
  }

  /* get the index of the last non-zero digit */
  int last_non_zero = (*num)->length - 1;
  while (last_non_zero >= 0 && (*num)->digits[last_non_zero] == '0') {
    last_non_zero--;
  }

  /* adjust the digits */
  int effective_digits = last_non_zero - first_non_zero + 1;
  if (effective_digits > (*num)->precision) {
    MATH_PREC_LOSE((*num)->precision, effective_digits);
    last_non_zero = first_non_zero + (*num)->precision - 1;
  }

  /* move the digits to eliminate leading zeros */
  if (first_non_zero > 0) {
    for (int i = 0; i <= last_non_zero - first_non_zero; i++) {
      (*num)->digits[i] = (*num)->digits[first_non_zero + i];
    }

    (*num)->decimal -= first_non_zero;
  }

  /* eliminate the trailing zeros */
  for (int i = last_non_zero + 1; i < (*num)->length; i++) {
    (*num)->digits[i] = '\0';
  }

  /* update length (number of valid digit)*/
  (*num)->length = last_non_zero - first_non_zero + 1;
}

void Exn_round( Exn* num, int scale, int type ) {
    if (*num == NULL || (*num)->digits == NULL)
        return;
    
    if (scale < 0) {
        Exn_cpy(num, Exn_zero);
        return;
    }
    
    if (scale > (*num)->precision) {
        scale = (*num)->precision;
    }
    
    if (scale <= (*num)->length) {
        /* round the number */
        int i = (*num)->length;
        (*num)->length = scale;
        for (; i > scale + 1; i--) {
            (*num)->digits[i] = 0;
        }
        int carry = 0;
        switch (type) {
            case 1: /* floor */
                carry = 1;
                break;
            case 0: /* round */
                if ((*num)->digits[scale] >= '5') {
                    carry = 1;
                }
                break;
            case -1: /* ceil */
            default:
                break;
        }
        while (i >= 0 && carry > 0) {
            int digit = (*num)->digits[i] - '0' + carry;
            (*num)->digits[i] = digit % 10 + '0';
            carry = digit / 10;
            i--;
        }
        if (carry > 0) {
            (*num)->sign = 1;
            (*num)->length++;
            (*num)->decimal++;
            for (i = (*num)->length; i > 0; i--) {
                (*num)->digits[i] = (*num)->digits[i - 1];
            }
            (*num)->digits[0] = '1';
        }
        (*num)->digits[(*num)->length] = '\0';
    }

}

void Exn_show(const Exn num) {
  if (num == NULL) {
    fprintf(stdout, "NULL\r\n");
    return;
  }
  fprintf(stdout, "Extended Number at <%p>: {\r\n", num);
  fprintf(stdout, "\tSign: %c\r\n", (num->sign > 0) ? '+' : '-');
  fprintf(stdout, "\tLength: %d\r\n", num->length);
  fprintf(stdout, "\tDecimal: %d\r\n", num->decimal);
  fprintf(stdout, "\tError: %s\r\n", Exn_err2str(num->error));
  fprintf(stdout, "\tDigits: %s\r\n", num->digits);
  fprintf(stdout, "\tPrecision: %d\r\n", num->precision);
  fprintf(stdout, "}\r\n");
}

/**
 * @brief Format the extended number to a normal string
 * @param num The extended number
 * @return char* The formatted string
 * @example Exn_fmtn(num) -> "123.456"
 * @note The caller is responsible for releasing the memory of the string
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
NO_EXPORT char *__Exn_fmtn(Exn num) {
  int buf_len = num->precision + num->decimal + 10;
  char *buf = (char *)malloc(buf_len);
  if (buf == NULL) {
    MEM_ALLOC_FAIL(&buf, "The formatting buffer allocation failed");
    return NULL;
  }

  int pos = 0;
  if (num->sign == -1) {
    buf[pos++] = '-';
  }

  if (num->decimal <= 0) { /* magnitude < 1 */
    buf[pos++] = '0';
    buf[pos++] = '.';
    for (int i = 0; i < -num->decimal; i++) {
      buf[pos++] = '0';
    }
    for (int i = 0; i < num->length; i++) {
      buf[pos++] = num->digits[i];
    }
  } else if (num->decimal >=
             num->length) { /* magnitude >= 1 with trailing zeros*/
    buf[pos++] = num->digits[0];
    for (int i = 1; i < num->length; i++) {
      buf[pos++] = num->digits[i];
    }
    for (int i = 0; i < num->decimal - num->length; i++) {
      buf[pos++] = '0';
    }

  } else { /* magnitude >= 1 without trailing zeros */
    for (int i = 0; i < num->length; i++) {
      if (i == num->decimal) {
        buf[pos++] = '.';
      }
      buf[pos++] = num->digits[i];
    }
  }

  buf[pos] = '\0';
  return buf;
}

/**
 * @brief Format the extended number to a strictly precise string
 * @param num The extended number
 * @return char* The formatted string
 * @example Exn_fmtp(num) -> "123.4560000"
 * @note The caller is responsible for releasing the memory of the string
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
NO_EXPORT char *__Exn_fmtp(Exn num) {

  int prec = num->precision;
  char *str = __Exn_fmtn(num);
  if (str == NULL)
    return NULL;
  int len = strlen(str);
  int digit_n = MAX(num->length, num->decimal); /* exact digit number */

  if (prec <= digit_n) { /* no need to add trailing zeros */
    return str;
  } else { /* add trailing zeros */
    int buf_len = prec + 10;
    char *buf = (char *)malloc(buf_len);
    if (buf == NULL) {
      MEM_ALLOC_FAIL(&buf, "The formatting buffer allocation failed");
      return NULL;
    }

    int pos = 0;
    strncpy(buf, str, len);
    pos = len;

    if (num->decimal > 0 && num->decimal >= num->length) {
      buf[pos++] = '.';
    }

    for (int i = digit_n; i < prec; i++) {
      buf[pos++] = '0';
    }

    buf[pos] = '\0';
    free(str);
    return buf;
  }
}

/**
 * @brief Format the extended number to a scientific notation string
 * @param num The extended number
 * @return char* The formatted string
 * @example Exn_fmts(num) -> "1.23456E+2"
 * @note The caller is responsible for releasing the memory of the string
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
NO_EXPORT char *__Exn_fmts(Exn num) {
  int buf_len = num->length + 10;
  char *buf = (char *)malloc(buf_len);
  if (buf == NULL) {
    MEM_ALLOC_FAIL(&buf, "The formatting buffer allocation failed");
    return NULL;
  }

  int pos = 0;
  if (num->sign == -1) {
    buf[pos++] = '-';
  }

  buf[pos++] = num->digits[0];
  if (num->length > 1) {
    buf[pos++] = '.';
    memcpy(buf + pos, num->digits + 1, num->length - 1);
    pos += num->length - 1;
  }

  buf[pos++] = 'E';
  if (num->decimal < 0) {
    buf[pos++] = '-';
  }

  int exp = num->decimal - 1;
  if (exp < 0) {
    exp = -exp;
  }
  int exp_len = 0;
  int exp_buf[10];
  while (exp > 0) {
    exp_buf[exp_len++] = exp % 10;
    exp /= 10;
  }

  for (int i = exp_len - 1; i >= 0; i--) {
    buf[pos++] = exp_buf[i] + '0';
  }

  buf[pos] = '\0';
  return buf;
}

char *Exn_fmt(Exn num, Exnfmt mode) {
  if (num == NULL)
    return NULL;
  Exn_normalize(&num);
  switch (mode) {
  case EXN_FMT_NORMAL:
    return __Exn_fmtn(num);
  case EXN_FMT_PRECISE:
    return __Exn_fmtp(num);
  case EXN_FMT_SCIENTIFIC:
    return __Exn_fmts(num);
  default:
    return NULL;
  }
}

/**
 * @brief Add two extended numbers ignoring the sign
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the unsinged addition
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __Exn_addu(const Exn num1, const Exn num2) {

  // align the decimal
  int maxDecimal = MAX(num1->decimal, num2->decimal);
  int minDecimal = MIN(num1->decimal, num2->decimal);
  int diff = maxDecimal - minDecimal; /* decimal different */
  int carry = 0;
  int len = MAX(num1->length, num2->length) + diff + 1; // +1 for carry
  int prec = MAX(num1->precision, num2->precision);
  char *align_num1 = (char *)malloc(len);
  char *align_num2 = (char *)malloc(len);
  if (align_num1 == NULL || align_num2 == NULL) {
    MEM_ALLOC_FAIL(&align_num1, "The extended number allocation failed");
    MEM_ALLOC_FAIL(&align_num2, "The extended number allocation failed");
    return NULL;
  }
  Exn result = Exn_create(len);
  int i = len - 1;

  /* align the number */
  memset(align_num1, '0', len);
  memset(align_num2, '0', len);
  if (num1->decimal == minDecimal) {
    memcpy(align_num1 + diff + 1, num1->digits, num1->length);
  } else {
    memcpy(align_num1 + 1, num1->digits, num1->length);
  }
  if (num2->decimal == minDecimal) {
    memcpy(align_num2 + diff + 1, num2->digits, num2->length);
  } else {
    memcpy(align_num2 + 1, num2->digits, num2->length);
  }

  for (; i >= 0 || carry > 0; i--) {
    int sum = Ch2val(align_num1[i]) + Ch2val(align_num2[i]) + carry;
    result->digits[i] = (sum % 10) + '0';
    carry = sum / 10;
  }

  result->length = len;
  result->decimal = maxDecimal + 1;
  result->precision = prec;

  // handle overflow
  if (carry > 0 && i < 0) {
    result->error = EXTENDED_NUM_OVERFLOW;
  }

  free(align_num1);
  free(align_num2);
  return result;
}

/**
 * @brief Subtract two extended numbers ignoring the sign
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the unsigned subtraction
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __Exn_subu(const Exn num1, const Exn num2) {

  // align the decimal
  int maxDecimal = MAX(num1->decimal, num2->decimal);
  int minDecimal = MIN(num1->decimal, num2->decimal);
  int diff = maxDecimal - minDecimal; /* decimal different */
  int borrow = 0;
  int len = MAX(num1->length, num2->length) + diff + 1;
  Exn result = Exn_create(len);
  int i = len - 1;

  /* align decimal */
  char *align_num1 = (char *)malloc(len);
  char *align_num2 = (char *)malloc(len);
  if (align_num1 == NULL || align_num2 == NULL) {
    MEM_ALLOC_FAIL(&align_num1, "The extended number allocation failed");
    MEM_ALLOC_FAIL(&align_num2, "The extended number allocation failed");
    return NULL;
  }

  memset(align_num1, '0', len);
  memset(align_num2, '0', len);
  if (num1->decimal == minDecimal) {
    memcpy(align_num1 + diff + 1, num1->digits, num1->length);
  } else {
    memcpy(align_num1 + 1, num1->digits, num1->length);
  }
  if (num2->decimal == minDecimal) {
    memcpy(align_num2 + diff + 1, num2->digits, num2->length);
  } else {
    memcpy(align_num2 + 1, num2->digits, num2->length);
  }

  for (i = len - 1; i >= 0; i--) {
    int digit1 = Ch2val(align_num1[i]);
    int digit2 = Ch2val(align_num2[i]);
    int diff_ = digit1 - digit2 - borrow;
    if (diff_ < 0) {
      diff_ += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result->digits[i] = diff_ + '0';
  }

  result->length = len;
  result->decimal = maxDecimal + 1;
  result->precision = MAX(num1->precision, num2->precision);
  free(align_num1);
  free(align_num2);

  // handle underflow
  if (borrow > 0 && i < 0) {
    result->error = EXTENDED_NUM_OVERFLOW;
  }
  // remove leading zeros
  return result;
}

Exn Exn_abs(const Exn num) {
  Exn result = NULL;
  Exn_cpy(&result, num);
  result->sign = 1;
  return result;
}

Exn Exn_shift(const Exn num, int shift) {
  if (!num || !num->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return NULL;
  }
  Exn result = NULL;
  Exn_cpy(&result, num);
  result->decimal += shift;
  return result;
}

bool Exn_iszero(const Exn num) { return num->sign == 0; }
bool Exn_isneg(const Exn num) { return num->sign < 0; }

Exn Exn_add(const Exn num1, const Exn num2) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    printf("num1: %s\r\n", Exn_err2str(num1->error));
    printf("num2: %s\r\n", Exn_err2str(num2->error));
    return NULL;
  }
  int sign = num1->sign * num2->sign;
  Exn result = NULL;

  if (sign > 0) {
    result = __Exn_addu(num1, num2);
    result->sign = num1->sign;
  } else if (sign == 0) {
    if (Exn_iszero(num1)) {
      Exn_cpy(&result, num2);
    } else { /* num2 is zero */
      Exn_cpy(&result, num1);
    }
  } else /* sign < 0 */
  {
    Exn num1_ = Exn_abs(num1);
    Exn num2_ = Exn_abs(num2);
    if (Exn_cmp(num1_, num2_) > 0) {
      result = __Exn_subu(num1_, num2_);
      result->sign = num1->sign;
    } else {
      result = __Exn_subu(num2_, num1_);
      result->sign = num2->sign;
    }
    Exn_release(&num1_);
    Exn_release(&num2_);
  }
  Exn_normalize(&result);
  return result;
}

Exn Exn_sub(const Exn num1, const Exn num2) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return NULL;
  }
  int sign = num1->sign * num2->sign;
  Exn result = NULL;

  if (sign > 0) {
    if (Exn_cmp(num1, num2) > 0) {
      result = __Exn_subu(num1, num2);
      result->sign = num1->sign;
    } else {
      result = __Exn_subu(num2, num1);
      result->sign = -num2->sign;
    }
  } else if (sign == 0) {
    if (Exn_iszero(num1)) {
      Exn_cpy(&result, num2);
      result->sign = -num2->sign;
    } else { /* num2 is zero */
      Exn_cpy(&result, num1);
    }
  } else /* sign < 0 */
  {
    Exn num1_ = Exn_abs(num1);
    Exn num2_ = Exn_abs(num2);
    result = __Exn_addu(num1_, num2_);
    result->sign = num1->sign;
    Exn_release(&num1_);
    Exn_release(&num2_);
  }
  Exn_normalize(&result);
  return result;
}

/**
 * @brief Multiply two extended numbers
 * @details This function uses the naive multiplication algorithm with O(n^2)
 * complexity
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
Exn __Exn_mul_pl(const Exn num1, const Exn num2, int len, int sign) {
  Exn result = Exn_create(len);
  if (result == NULL) {
    MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
    return NULL;
  }
  /* initialize the result */
  memset(result->digits, 0, len);

  /* use naive multiplication */
  /* result[i+j] = num1[i] * num2[j] + carry */
  for (int i = num1->length - 1; i >= 0; i--) {
    for (int j = num2->length - 1; j >= 0; j--) {
      int mul = (num1->digits[i] - '0') * (num2->digits[j] - '0');
      int sum = mul + (result->digits[i + j + 1]);
      result->digits[i + j + 1] = sum % 10;
      result->digits[i + j] += sum / 10;
    }
  }

  /* add to char '0' */
  for (int i = 0; i < len; i++) {
    result->digits[i] += '0';
  }

  result->length = len;
  result->decimal = num1->decimal + num2->decimal;
  result->precision = MAX(num1->precision, num2->precision);
  result->sign = sign;

  return result;
}

/**
 * @brief Multiply two extended numbers using Karatsuba algorithm
 * @details This function uses the Karatsuba algorithm with O(n^log2(3))
 * complexity
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
Exn __Exn_mul_karatsuba(const Exn num1, const Exn num2, int len, int sign) {
  Exn result = NULL;
  /* use karatsuba multiplication */
  /**
   *          A    B
   *  x       C    D
   * ----------------
   *    AC  AD+BC  BD
   *
   * AD + BC = (A + B)(C + D) - AC - BD
   *
   *      4 recursive calls       */

   int n = MAX(num1->length, num2->length);
   result = Exn_create(len);
  if (result == NULL) {
    MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
    return NULL;
  }

  int* a = (int*)malloc(sizeof(int) * n);
  int* b = (int*)malloc(sizeof(int) * n);

  /* inverse the number */
  for (int i = 0; i < num1->length; i++) {
    a[i] = Ch2val(num1->digits[num1->length - 1 - i]);
  }
  for (int i = 0; i < num2->length; i++) {
    b[i] = Ch2val(num2->digits[num2->length - 1 - i]);
  }

  int* r = karatsuba(n - 1, a, b);
  
  int car = 0;
  for (int i = 0; i < 2 * n - 1; i++) {
    int sum = r[i] + car;
    r[i] = sum % 10;
    car = sum / 10;
    if (r[i] == 0 && car == 0) {
      result->length = i;
      break;
    } else {
      result->length = i + 1;
    }
  }

  int i = 0;
  if (car > 0) {
    result->digits[i] = car + '0';
    i++;
  }

  /* inverse the result */
  for (int j = result->length - 1; j >= 0; j--) {
    result->digits[i] = r[j] + '0';
    i++;
  }

  free(a);
  free(b);
  free(r);  
  result->sign = sign;
  result->decimal = num1->decimal + num2->decimal - (len != result->length);
  result->precision = MAX(num1->precision, num2->precision);

  return result;
}

/**
 * @brief Multiply two extended numbers using FFT
 * @details This function uses the FFT algorithm with O(nlogn^2) complexity
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
Exn __Exn_mul_fft(const Exn num1, const Exn num2, int len, int sign) {
  /* calc the power of 2 */
  int size = 1;
  while (size < num1->length * 2 || size < num2->length * 2) {
    size <<= 1;
  }

  Exn result = Exn_create(size);
  if (result == NULL) {
    MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
    return NULL;
  }

  complex *fa = (complex *)calloc(size, sizeof(complex));
  complex *fb = (complex *)calloc(size, sizeof(complex));

  if (!fa || !fb) {
    MEM_ALLOC_FAIL(&fa, "The extended number allocation failed");
    MEM_ALLOC_FAIL(&fb, "The extended number allocation failed");
    free(fa);
    free(fb);
    Exn_release(&result);
    return NULL;
  }

  for (int i = num1->length - 1; i >= 0; i--) {
    fa[num1->length - 1 - i] = Ch2val(num1->digits[i]) + 0.0 * I;
  }
  for (int i = num2->length - 1; i >= 0; i--) {
    fb[num2->length - 1 - i] = Ch2val(num2->digits[i]) + 0.0 * I;
  }

  /* FFT */
  fft(fa, size, false);
  fft(fb, size, false);

  /* multiply */
  for (int i = 0; i < size; i++) {
    fa[i] *= fb[i];
  }

  /* IFFT */
  fft(fa, size, true);

  complex *res = fa;
  int car = 0;
  int result_dec = 0;

  for (int i = 0; i < size; i++) {
    int sum = round(creal(res[i])) + car;
    result->digits[i] = (sum % 10) + '0';
    car = sum / 10;
    if (result->digits[i] != '0') {
      result_dec = i;
    }
  }

  result->length = size;

  result->decimal = num1->decimal + num2->decimal - (len != result_dec + 1);
  result->precision = MAX(num1->precision, num2->precision);
  result->sign = sign;
  free(fa);
  free(fb);

  // invert the digits
  for (int i = 0; i < result->length / 2; i++) {
    char temp = result->digits[i];
    result->digits[i] = result->digits[result->length - 1 - i];
    result->digits[result->length - 1 - i] = temp;
  }
  // remove leading zeros
  int first_non_zero = size - result_dec - 1;
  for (int i = 0; i < result->length - first_non_zero; i++) {
    result->digits[i] = result->digits[first_non_zero + i];
  }
  result->length -= first_non_zero;

  return result;
}

Exn Exn_mul(const Exn num1, const Exn num2) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    printf("num1: %s\r\n", Exn_err2str(num1->error));
    printf("num2: %s\r\n", Exn_err2str(num2->error));
    return NULL;
  }
  int sign = num1->sign * num2->sign;

  Exn result = NULL;

  /* if muland is zero */
  if (sign == 0) {
    Exn_cpy(&result, Exn_zero);
    return result;
  }

  /* calc the precision */
  int len = num1->length + num2->length;

  /* choose diff method according to size */
  if (len / 2 < MUL_OPTIMIZE_LEAST) {
    result = __Exn_mul_pl(num1, num2, len, sign);
  } else /* len >= 1000 */
  {
    result = __Exn_mul_fft(num1, num2, len, sign);
  }

  Exn_normalize(&result);
  return result;
}

Exn Exn_div(const Exn num1, const Exn num2) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return NULL;
  }
  if (Exn_iszero(num2)) {
    MATH_DIV_ZERO("The divisor can't be zero");
    return NULL;
  }

  Exn result = NULL;
  /* if dividend is zero */
  if (Exn_iszero(num1)) {
    Exn_cpy(&result, Exn_zero);
    return result;
  }
  int len = MAX(num1->length, num2->length) * 2;
  int prec = MAX(num1->precision, num2->precision);
  int bound = prec * 2 + 1; /* the bound of the iteration */

  /* calc the inverse of divisor */
  Exn inv_num2 = Exn_shift(Exn_one, -num2->decimal);
  Exn_newton(__ntiter_inv, bound, num2, &inv_num2);

  result = Exn_mul(num1, inv_num2);
  Exn_release(&inv_num2);
  if (result) {
    Exn_round(&result, len, 0);
  }

  return result;
}

Exn Exn_divmod(const Exn num1, const Exn num2, Exn *mod) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return NULL;
  }
  if (Exn_iszero(num2)) {
    MATH_DIV_ZERO("The divisor can't be zero");
    return NULL;
  }

  Exn result = NULL;
  /* if dividend is zero */
  if (Exn_iszero(num1)) {
    Exn_cpy(&result, Exn_zero);
    return result;
  }
  int prec = MAX(num1->precision, num2->precision);
  int bound = prec * 2 + 1; /* the bound of the iteration */

  /* calc the inverse of divisor */
  Exn inv_num2 = Exn_shift(Exn_one, -num2->decimal);
  Exn_newton(__ntiter_inv, bound, num2, &inv_num2);

  result = Exn_mul(num1, inv_num2);
  Exn_release(&inv_num2);

  if (result) {
    Exn_round(&result, result->decimal, 0);
    /* calc the mod */
    Exn temp = Exn_mul(result, num2);
    *mod = Exn_sub(num1, temp);
    Exn_release(&temp);
    (*mod)->precision = MAX(num1->precision, num2->precision);
    Exn_normalize(mod);
  }
  return result;
}

Exn Exn_modulo(const Exn num1, const Exn num2) {

  Exn result = NULL;
  Exn mod = NULL;
  result = Exn_divmod(num1, num2, &mod);
  Exn_release(&result);
  return mod;
}

Exn Exn_sqrt(const Exn num) {
  if (!num || !num->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return NULL;
  }
  Exn result = NULL;
  if (Exn_iszero(num)) {
    Exn_cpy(&result, Exn_zero);
    return result;
  }
  int len = num->length * 2 + 1;
  int prec = MAX(num->precision, num->decimal);
  int bound = MAX_DIGITS / 2; /* the bound of the iteration */

  /* select initial value */
  int shift = -num->decimal / 2;
  if (num->decimal % 2 != 0) {
    shift = -num->decimal / 2 - 1;
  }
  result = Exn_shift(Exn_one, shift);
  Exn_newton(__ntiter_sqrt, bound, num, &result);
  Exn_round(&result, len, -1);
  
  /* invert \frac{1}{\sqrt{x}} */
  Exn inv = Exn_shift(Exn_one, -result->decimal);
  Exn_newton(__ntiter_inv, prec, result, &inv);
    Exn_release(&result);
    // inv->decimal = num->decimal / 2;
    Exn_round(&inv, len, 0);
  return inv;
}

Exn fact( int n ) {
    size_t u_bound = (size_t)ceil(((n + 0.5)*log10(n + 1) 
    - n/log(10) + log10(2 * acos(-1) + 1)));

    Exn result = Exn_create(u_bound);
    if (result == NULL) {
        MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
        return NULL;
    }

    result->digits[0] = '1';
    result->sign = 1;
    result->length = 1;
    result->decimal = 1;
    Exn cnt = Exn_shift(Exn_one, 0);

    for (int i = 1; i < n; i++) {
        Exn add = Exn_add(cnt, Exn_one);
        Exn tmp = Exn_mul(result, add);
        Exn_mv(&result, &tmp);
        Exn_mv(&cnt, &add);
    }
    Exn_release(&cnt);
    return result;
}

Exn ksm(Exn num, int exp) {
    Exn result = Exn_shift(Exn_one, 0);
    result->precision = num->precision;
    if (result == NULL) {
        MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
        return NULL;
    }
    Exn a = Exn_shift(num, 0);
    while (exp > 0) {
        Exn tmp = NULL;
        if (exp & 0x1) {
            Exn tmp_ = Exn_mul(result, a);
            Exn_mv(&result, &tmp_);
        }
        tmp = Exn_mul(a, a);
        Exn_mv(&a, &tmp);
        exp >>= 1;
    }
    Exn_release(&a);
    Exn_normalize(&result);
    return result;
}

void Exn_newton(Exn (*func)(Exn, Exn), int bound, Exn param, Exn *result) {
  while ((*result)->length < bound) {
    Exn temp = func(param, *result);
    if (!temp || temp->error != EXTENDED_NUM_OK) {
      Exn_release(&temp);
      break;
    }
    Exn_mv(result, &temp);
  }
  Exn_normalize(result);
}

int Exn_toInt(const Exn num) {
  if (num == NULL) {
    MEM_ACCESS_NULLPTR("num", "The extended number can't be NULL");
    return 0;
  }
  if (num->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return 0;
  }
  int result = 0;
  if (num->decimal <= 0) {
    result = 0;
  } else {
    int dec = num->decimal - 1;
    for (int i = 0; i < num->length && dec >= 0; i++) {
        result += Ch2val(num->digits[i]) * pow(10, dec);
        dec--;
    }
  }
  return result * num->sign;
}

int Exn_cmp(const Exn num1, const Exn num2) {
  if (num1->sign > num2->sign) {
    return 1;
  } else if (num1->sign < num2->sign) {
    return -1;
  } else {
    if (num1->decimal > num2->decimal) {
      return num1->sign;
    } else if (num1->decimal < num2->decimal) {
      return -num1->sign;
    } else {
      int i = 0;
      while (i < num1->length && i < num2->length) {
        if (num1->digits[i] > num2->digits[i]) {
          return num1->sign;
        } else if (num1->digits[i] < num2->digits[i]) {
          return -num1->sign;
        }
        i++;
      }
      if (i < num1->length) {
        return num1->sign;
      } else if (i < num2->length) {
        return -num1->sign;
      } else {
        return 0;
      }
    }
  }
}

// ############################ MathFunc Class ############################

bool MFunc_ckarg(const MathFunc *func) {
  if (func == NULL) {
    MEM_ACCESS_NULLPTR("func", "The math function can't NULL");
    return false;
  }
  char *func_n = func->alias;
  switch (func->type) {
  case MFUNC_SQRT:
    if (func->argc != 1) {
      FUNCT_ARG_MISMATCH(func_n, 1, func->argc);
      return false;
    }
    if (Exn_isneg(func->args[0])) {
      char* num = Exn_fmt(func->args[0], EXN_FMT_NORMAL);
      MATH_SQRT_NEG(num);
      free(num);
      return false;
    }
    break;
  case MFUNC_POWER:
    if (func->argc != 2) {
      FUNCT_ARG_MISMATCH(func_n, 2, func->argc);
      return false;
    }
    if (func->args[1]->sign == -1) {
      INPUT_ARG_FAIL("exponent", "Negative exponent is not supported");
      return false;
    }
    break;
  case MFUNC_FACT:
    if (func->argc != 1) {
      FUNCT_ARG_MISMATCH(func_n, 1, func->argc);
      return false;
    }
    if (Exn_isneg(func->args[0])) {
      INPUT_ARG_FAIL("factorial", "Negative factorial is not supported");
      return false;
    }
    if (Exn_toInt(func->args[0]) > FACT_LIMIT) {
      char* num = Exn_fmt(func->args[0], EXN_FMT_NORMAL);
      free(num);
      return false;
    }
    break;
  case MFUNC_DIVMOD:
    if (func->argc != 2) {
      FUNCT_ARG_MISMATCH(func_n, 2, func->argc);
      return false;
    }
    if (Exn_iszero(func->args[1])) {
      MATH_DIV_ZERO("The divisor can't be zero");
      return false;
    }
    break;
  case MFUNC_MODULO:
    if (func->argc != 2) {
      FUNCT_ARG_MISMATCH(func_n, 2, func->argc);
      return false;
    }
    break;
  default:
    FUNCT_INVOKE_FAIL(func_n, "no such math function");
    return false;
  }
  return true;
}

Exn MFunc_eval(const MathFunc* func) {
  if (func == NULL) {
    MEM_ACCESS_NULLPTR("func", "The math function can't be NULL");
    return NULL;
  }
  if (!MFunc_ckarg(func)) return NULL;
  Exn result = NULL;
  switch (func->type) {
  case MFUNC_SQRT:
    result = Exn_sqrt(func->args[0]);
    break;
  case MFUNC_POWER:
    result = ksm(func->args[0], Exn_toInt(func->args[1]));
    break;
  case MFUNC_FACT:
    result = fact(Exn_toInt(func->args[0]));
    break;
  case MFUNC_MODULO:
    result = Exn_modulo(func->args[0], func->args[1]);
    break;
  case MFUNC_DIVMOD:
    Exn mod = NULL;
    result = Exn_divmod(func->args[0], func->args[1], &mod);
    Exn_release(&mod);
    break;
  }
  return result;
}

void MFunc_build(MathFunc *func, int type, const char *alias, uint8_t argc, Exn *args)
{
    if (func == NULL) {
        MEM_ACCESS_NULLPTR("func", "The math function can't be NULL");
        return;
    }
    func->type = type;
    func->alias = (char *)malloc(strlen(alias) + 1);
    if (func->alias == NULL) {
        MEM_ALLOC_FAIL(&func->alias, "The math function alias allocation failed");
        return;
    }
    strcpy(func->alias, alias);
    func->argc = argc;
    func->args = args;
}

void MFunc_release(MathFunc **func) {
  if (func == NULL) {
    MEM_ACCESS_NULLPTR("func", "The math function can't be NULL");
    return;
  }
  free((*func)->alias);
    for (int i = 0; i < (*func)->argc; i++) {
        Exn_release(&((*func)->args[i]));
    }
    free((*func)->args);
  free(*func);
  *func = NULL;
}

// ############################ MathExpr Class ############################
void splitm(char *str, char *tokens[], int *token_len) {
  char *p = str;
  int i = 0;
  while (*p != '\0') {
    while (*p == ' ') {
      p++;
    }

    if (*p == '\0') {
      break;
    }

    tokens[i++] = p;
    while (*p != ' ' && *p != '\0') {
      p++;
    }

    if (*p == '\0') {
      break;
    }

    *p = '\0';
    p++;
  }

  *token_len = i;
}

void BinOprExpr_build(BinOprExpr *bin_op, char op, Exn opr1, Exn opr2) {
  bin_op->num1 = opr1;
  bin_op->num2 = opr2;
  bin_op->binop = op;
}

Exn BinOprExpr_eval(const BinOprExpr *bin_op) {
  if (bin_op == NULL) {
    MEM_ACCESS_NULLPTR("bin_op", "The bin operation expression can't be NULL");
    return NULL;
  }
  Exn result = NULL;
  switch (bin_op->binop) {
  case '+':
    result = Exn_add(bin_op->num1, bin_op->num2);
    break;
  case '-':
    result = Exn_sub(bin_op->num1, bin_op->num2);
    break;
  case '*':
  case 'x':
    result = Exn_mul(bin_op->num1, bin_op->num2);
    break;
  case '/':
    result = Exn_div(bin_op->num1, bin_op->num2);
    break;
  default:
    MATH_URECG_OP(bin_op->binop);
    break;
  }
  return result;
}

void BinOprExpr_release(BinOprExpr **bin_op) {
    if (bin_op == NULL) {
        MEM_ACCESS_NULLPTR("bin_op", "The bin operation expression can't be NULL");
        return;
    }
    Exn_release(&(*bin_op)->num1);
    Exn_release(&(*bin_op)->num2);
    free(*bin_op);
    *bin_op = NULL;
}

int MathExpr_build(char *expr, MathExpr *math_expr, int prec) {
  if (expr == NULL || math_expr == NULL) {
    INPUT_ARG_FAIL("expr", "The expression can't be NULL");
    return -1;
  }
  char *tokens[TOKEN_BUFFER];
  char* expr_copy = strdup(expr);
  int token_len = 0;
  int expr_len = strlen(expr);
  splitm(expr_copy, tokens, &token_len);
  bool isfunc = token_len > 0 && isalpha(tokens[0][0]);

  if (token_len == 0) {
    INPUT_ARG_FAIL("expr", "The expression can't be empty");
    free(expr_copy);
    return -1;
  }

  /* BinOpExpr case */
  if (token_len >= 3 && !isfunc) {
    Exn opr1 = atoExn(tokens[0], prec);
    Exn opr2 = atoExn(tokens[2], prec);
    if (opr1 == NULL || opr1->error != EXTENDED_NUM_OK) {
      INPUT_ARG_FAIL("opr1", "The first operand has an error");
      Exn_release(&opr1);
      Exn_release(&opr2);
      free(expr_copy);
      return -1;
    }
    if (opr2 == NULL || opr2->error != EXTENDED_NUM_OK) {
      INPUT_ARG_FAIL("opr2", "The second operand has an error");
      Exn_release(&opr1);
      free(expr_copy);
      return -1;
    }
    char op = tokens[1][0];
    if (op != '+' && op != '-' && op != '*' && op != '/' && op != 'x') {
      MATH_URECG_OP(op);
      Exn_release(&opr1);
      Exn_release(&opr2);
      free(expr_copy);
      return -1;
    }
    BinOprExpr *bin_op = (BinOprExpr *)malloc(sizeof(BinOprExpr));
    if (bin_op == NULL) {
      MEM_ALLOC_FAIL(&bin_op, "The bin operation expression allocation failed");
      Exn_release(&opr1);
      Exn_release(&opr2);
      free(expr_copy);
      return -1;
    }
    BinOprExpr_build(bin_op, op, opr1, opr2);
    math_expr->type = MATH_EXPR_BINOP;
    math_expr->expr.binop = bin_op;
    free(expr_copy);
    return 0;
  } else if (token_len == 1 && !isfunc) { /* echo case */
    Exn opr = atoExn(tokens[0], prec);
    if (opr == NULL || opr->error != EXTENDED_NUM_OK) {
      INPUT_ARG_FAIL("opr", "The echo number has an error");
      Exn_release(&opr);
      free(expr_copy);
      return -1;
    }
    math_expr->type = MATH_EXPR_ECHO;
    math_expr->expr.echo = opr;
    free(expr_copy);
    return 0;
  } else { /* math function case */
    free(expr_copy);

    char *func_name;
    int lparen = -1; // find the left parenthesis
    int rparen = -1; // find the right parenthesis

    for (int i = 0; i < expr_len; i++) {
      if (expr[i] == '(') {
        lparen = i;
        break;
      }
    }

    if (lparen == -1) {
      INPUT_ARG_FAIL("expr", "The function call is invalid");
      
      return -1;
    }

    for (int i = lparen + 1; i < expr_len; i++) {
      if (expr[i] == ')') {
        rparen = i;
        break;
      }
    }

    if (rparen == -1) {
      INPUT_ARG_FAIL("expr", "The function call needs a right parenthesis");
      return -1;
    }

    Exn *args = NULL;
    char *arg_s = expr + lparen + 1; /* the start of the argument */
    char *arg_e = NULL;              /* the end of the argument */

    func_name = (char *)malloc(sizeof(char) * (lparen + 1));
    if (func_name == NULL) {
      MEM_ALLOC_FAIL(&func_name, "Memory allocation for function name failed");
      return -1;
    }

    /* Copy function name */
    strncpy(func_name, expr, lparen);
    func_name[lparen] = '\0';

    /* Determine the number of arguments by counting commas */
    int argc = 1; /* At least one argument if there's anything between parentheses */
    if (rparen == lparen + 1) { /* No arguments case */
      argc = 0;
    } else {
      for (int i = lparen + 1; i < rparen; i++) {
        if ((expr)[i] == ',') {
          argc++;
        }
      }
    }

    /* Allocate memory for arguments */
    args = (Exn *)malloc(sizeof(Exn) * argc);
    if (args == NULL && argc > 0) {
      MEM_ALLOC_FAIL(&args, "Memory allocation for function arguments failed");
      free(func_name);
      return -1;
    }

    /* Parse arguments */
    if (argc > 0) {
      char *arg_buffer = (char *)malloc(sizeof(char) * (rparen - lparen));
      if (arg_buffer == NULL) {
        MEM_ALLOC_FAIL(&arg_buffer,
                       "Memory allocation for argument buffer failed");
        free(func_name);
        free(args);
        return -1;
      }

      int arg_index = 0;
      arg_s = expr + lparen + 1;
      arg_e = arg_s;

      while (arg_e < expr + rparen) {
        /* Find end of current argument (comma or closing parenthesis) */
        while (arg_e < expr + rparen && *arg_e != ',') {
          arg_e++;
        }

        /* Copy argument to buffer and trim spaces */
        int arg_len = arg_e - arg_s;
        strncpy(arg_buffer, arg_s, arg_len);
        arg_buffer[arg_len] = '\0';

        /* Trim leading spaces */
        char *trimmed = arg_buffer;
        while (*trimmed && isspace(*trimmed)) {
          trimmed++;
        }

        /* Trim trailing spaces */
        char *end = trimmed + strlen(trimmed) - 1;
        while (end > trimmed && isspace(*end)) {
          *end = '\0';
          end--;
        }

        /* Parse argument into Exn */
        
        args[arg_index] = atoExn(arg_buffer, prec);

        /* Check if parsing succeeded */
        if (args[arg_index] == NULL ||
            args[arg_index]->error != EXTENDED_NUM_OK) {
          char err_msg[100];
          sprintf(err_msg, "Failed to parse argument %d", arg_index + 1);
          INPUT_ARG_FAIL("func args", err_msg);

          /* Clean up */
          for (int i = 0; i < arg_index; i++) {
            Exn_release(&args[i]);
          }
          free(args);
          free(func_name);
          free(arg_buffer);
          return -1;
        }

        arg_index++;

        /* Move to next argument if there is one */
        if (*arg_e == ',') {
          arg_s = arg_e + 1;
          arg_e = arg_s;
        }
      }

      free(arg_buffer);
    }

    /* Determine function type based on name */
    int func_type = -1;
    if (strcmp(func_name, "sqrt") == 0) {
      func_type = MFUNC_SQRT;
    } else if (strcmp(func_name, "power") == 0) {
      func_type = MFUNC_POWER;
    } else if (strcmp(func_name, "div") == 0) {
      func_type = MFUNC_DIVMOD;
    } else if (strcmp(func_name, "mod") == 0) {
      func_type = MFUNC_MODULO;
    } else if (strcmp(func_name, "fact") == 0) {
      func_type = MFUNC_FACT;
    } else {
      FUNCT_INVOKE_FAIL(func_name, "no such math function");
      /* Clean up */
      for (int i = 0; i < argc; i++) {
        Exn_release(&args[i]);
      }
      free(args);
      free(func_name);
      return -1;
    }

    /* Build MathFunc object */
    MathFunc *math_func = (MathFunc *)malloc(sizeof(MathFunc));
    if (math_func == NULL) {
      MEM_ALLOC_FAIL(&math_func, "Memory allocation for math function failed");
      /* Clean up */
      for (int i = 0; i < argc; i++) {
        Exn_release(&args[i]);
      }
      free(args);
      free(func_name);
      return -1;
    }

    MFunc_build(math_func, func_type, func_name, argc, args);

    /* Set the math expression type and function pointer */
    (math_expr)->type =MATH_EXPR_FUNC;
    (math_expr)->expr.func = math_func;

    free(func_name); /* func_name is copied in MFunc_build */
    return 0;
  }
}

Exn MathExpr_eval(const MathExpr *expr) {
  if (expr == NULL) {
    MEM_ACCESS_NULLPTR("expr", "The math expression can't be NULL");
    return NULL;
  }
  Exn result = NULL;
  switch (expr->type) {
  case MATH_EXPR_BINOP:
    result = BinOprExpr_eval(expr->expr.binop);
    break;
  case MATH_EXPR_ECHO:
    Exn_cpy(&result, expr->expr.echo);
    break;
  case MATH_EXPR_FUNC:
    result = MFunc_eval(expr->expr.func);
    break;
    default:
      break;
  }
  return result;
}

void MathExpr_release(MathExpr *math_expr) {
    if (math_expr == NULL) {
      MEM_ACCESS_NULLPTR("math_expr", "The math expression can't be NULL");
      return;
    }
    switch (math_expr->type) {
    case MATH_EXPR_BINOP:
      BinOprExpr_release(&(math_expr)->expr.binop);
      break;
    case MATH_EXPR_ECHO:
      Exn_release(&(math_expr)->expr.echo);
      break;
    case MATH_EXPR_FUNC:
      MFunc_release(&(math_expr)->expr.func);
      break;
    default:
      break;
    }
  }

/**
 * @file main.c
 */
#include <getopt.h>
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
            MEXPR_RESULT_ERROR(Exn_err2str(res->error));
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
    bool verbose = false, help = false;
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
                precision = DEFALT_PRECISION;
            }
            else if (precision <= 0)
            {
                INPUT_ARG_FAIL(optarg, "precision should be a positive number");
                precision = DEFALT_PRECISION;
            }
            break;
        case '?':
            INPUT_OPTION_FAIL(argv[optind - 1]);
            break;
        }
    }

    MAX_DIGITS = precision << 7;
    init_all_constants(MAX_DIGITS);

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
        printf("Math Function:\r\n");
        printf("  sqrt(x)\tSquare root of x\r\n");
        printf("  fact(int)\tFactorial of int\r\n");
        printf("  div(x, y)\tInteger division of x and y\r\n");
        printf("  mod(x, y)\tModulo of x and y\r\n");
        printf("  pow(x, int)\tPower of x to the int\r\n");
    }
    else if (verbose)
    {
        printf("Default precision: %d\r\n", DEFALT_PRECISION);
        printf("Max input characters: %d\r\n", MAX_INPUT_SIZE);
        printf("The least absolute exponent when outputs as scientific: %d\r\n", NORMAL_LIMIT);
        printf("The most history inputs: %d\r\n", MAX_HISTORY);
    }
    else {
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
                MEXPR_PARSE_FAIL(num1, Exn_err2str(n1->error));
                Exn_release(&n1);
                Exn_release(&n2);
                return 0;
            }
            if (n2->error != EXTENDED_NUM_OK)
            {
                MEXPR_PARSE_FAIL(num2, Exn_err2str(n2->error));
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