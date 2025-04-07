#ifndef TEST_H_
#define TEST_H_
#include <assert.h>
#include <stdio.h>

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

inline void register_test(void (*test_func)(void), const char *name)
 {
    if (test_count < MAX_TESTS) {
        test_registry[test_count].func = test_func;
        test_registry[test_count].name = name;
        test_count++;
    }
}

inline void run_all_tests(void) {
    printf("Running %d tests...\r\n\n", test_count);
    
    for (int i = 0; i < test_count; i++) {
        printf("Test '%s': ", test_registry[i].name);
        test_registry[i].func();
        printf("\r\n");
    }
    
    printf("All tests completed.\r\n");
}

#endif