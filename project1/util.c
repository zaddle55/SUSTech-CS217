#include "util.h"

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