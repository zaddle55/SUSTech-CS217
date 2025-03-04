#ifndef ERROR_H_
#define ERROR_H_
#include <stdio.h>
#include <stdlib.h>

// ############################## Basic Macro ##############################

#define __ERR_OUT(fmt, ...) do { \
    fprintf(stderr, "%s:%d: at function '%s'\n  \u2514\u2500\u2500\u2500", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    fprintf(stderr, "\n"); \
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

// ############################## Math Exception ##############################

#define MATH_EXCEPT_
#define MATH_CVT_FAIL(__from, __to, __why) __WARNING("<Math Exception> Cannot convert '%s' to math type '%s': %s", __from, __to, __why)
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

// ############################## Memory Exception ##############################

#define MEM_EXCEPT_
#define MEM_ALLOC_FAIL(__at, __why) __PANIC("<Memory Exception> Memory allocation at <%p> failed: %s", __at, __why)
#define MEM_FREE_FAIL(__at, __why) __PANIC("<Memory Exception> Memory free failed at <%p>: %s", __at, __why)
#define MEM_ACCESS_NULLPTR(__ptr_n, __why) __PANIC("<Memory Exception> Accessing NULL pointer '%s': %s", __ptr_n, __why)
#define MEM_ACCESS_OOB(__ptr_n, __idx, __size) __PANIC("<Memory Exception> Accessing out of bound at index %d of '%s' with size %d", __idx, __ptr_n, __size)

#endif // ERROR_H_