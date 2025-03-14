
#ifndef UTIL_H_
#define UTIL_H_
#include <stdio.h>

#define bool unsigned char
#define true (unsigned char)1
#define false (unsigned char)0

#define INPUT_BUFFER 1024 * 1024
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

#endif // UTIL_H_