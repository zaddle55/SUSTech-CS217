#include "error.h"

int main() {

    // test notice
    __NOTION("This is a notice message");

    // test wavy
    WAVY("This sentence will be marked from postion 5-10", 5, 5, ANSI_PURPLE);
    
    // test Math Exception from this line
    #ifdef MATH_EXCEPT_
    MATH_CVT_FAIL("something", "long int", "not a digit");
    MATH_DIV_ZERO("the denominator is zero");
    MATH_SQRT_NEG("-1");
    MATH_OVERFLOW("the number is too large");
    MATH_URECG_OP('a');
    #endif
    // test Math Exception to this line

    // test Input Exception from this line
    #ifdef INPUT_EXCEPT_
    INPUT_OPTION_FAIL("-o");
    INPUT_ARG_FAIL("arg", "not a valid argument");
    #endif
    // test Input Exception to this line

    // test Funct Exception from this line
    #ifdef FUNCT_EXCEPT_
    FUNCT_INVOKE_FAIL("function()", "not a function");
    FUNCT_ARG_MISMATCH("function()", 3, 2);
    #endif
    // test Funct Exception to this line

    // test Memory Exception from this line
    #ifdef MEM_EXCEPT_
    void* some_ptr = malloc(1);
    free(some_ptr);
    MEM_ALLOC_FAIL(some_ptr, "memory allocation failed");
    #endif
    // test Memory Exception to this line

    return 0;
}