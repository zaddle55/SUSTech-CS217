#ifndef ERROR_H_
#define ERROR_H_
#include <stdio.h>
#include <stdlib.h>

// ############################## Basic Macro ##############################

#define __ERR_OUT(fmt, ...) do { \
    fprintf(stderr, "%s:%d: at function '%s'\r\n  \u2514\u2500\u2500\u2500", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    fprintf(stderr, "\r\n"); \
} while (0)

/// @brief Panic macro for serious errors
/// @param msg Message to print
/// @details This macro prints an error message and exits the program
#define PANIC(fmt, ...) do { \
    fprintf(stderr, "\033[1;31m [ERROR]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
    exit(1); \
} while (0)

/// @brief Notice macro for non-serious errors
/// @param msg Message to print
/// @details This macro prints a notice message
#define NOTION(fmt, ...) do { \
    fprintf(stderr, "\033[1;34m [NOTICE]: \033[0m"); \
    __ERR_OUT(fmt, ##__VA_ARGS__); \
} while (0)

/// @brief Warning macro for warnings
/// @param msg Message to print
/// @details This macro prints a warning message
#define WARNING(fmt, ...)                                                      \
  do {                                                                         \
    fprintf(stderr, "\033[1;33m [WARNING]: \033[0m");                          \
    __ERR_OUT(fmt, ##__VA_ARGS__);                                             \
  } while (0)

/// @brief Warning macro for warnings
/// @param msg Message to print
/// @details This macro prints a warning message
#define DBG(expr)                                                         \
  do {                                                                         \
    fprintf(stderr, "\033[1;32m [DEBUG]: \033[0m");                            \
    __ERR_OUT("%s: %d\n", #expr, expr);                                                    \
  } while (0)

#define ANSI_RESET  "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_PURPLE  "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"

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


// ############################## Memory Exception ##############################

#define MEM_EXCEPT_
#define MEM_ALLOC_FAIL(__at, __why) PANIC("<Memory Exception> Memory allocation at <%p> failed: %s", __at, __why)
#define MEM_FREE_FAIL(__at, __why) PANIC("<Memory Exception> Memory free failed at <%p>: %s", __at, __why)
#define MEM_ACCESS_NULLPTR(__ptr_n, __why) PANIC("<Memory Exception> Accessing NULL pointer '%s': %s", __ptr_n, __why)
#define MEM_ACCESS_OOB(__ptr_n, __idx, __size) PANIC("<Memory Exception> Accessing out of bound at index %d of '%s' with size %d", __idx, __ptr_n, __size)

#endif // ERROR_H_