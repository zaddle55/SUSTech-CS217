#pragma once
// header template
#include <stddef.h>
#include <stdint.h>
#ifndef MATH_H_
#define MATH_H_

#include "util.h"
#include "error.h"

#define NO_EXPORT __attribute__((visibility("hidden")))
#define EXPORT __attribute__((visibility("default")))
#define EXPORT_C extern "C" EXPORT

// ############################## Extended Number Class ##############################

/* constants for ExtendedNum class */
#define MAX_DIGITS 1000
#define DECIMAL_PLACES 100

/* error codes for ExntendedNum class */
typedef enum {

    EXTENDED_NUM_OK = 0,            /* Stand for a no-fault ExtendedNum*/
    EXTENDED_NUM_OVERFLOW,          /* Stand for an overflow error */
    EXTENDED_NUM_DIVIDE_BY_ZERO,    /* Stand for a division by zero error */
    EXTENDED_NUM_INVALID_INPUT,     /* Stand for an invalid input error */
    EXTENDED_NUM_MEMORY_ERROR       /* Stand for a memory allocation error */

} ExtendedNumError, Exnerr;

typedef struct {
    char* digits;   /* The pointer to valid digits array of the number*/
    int length;     /* The length of the number */
    int decimal;    /* The refernce position of the decimal point */
    int sign;       /* The sign of the number */
    Exnerr error;   /* The error code of the number */
} Exn_struct, *Exn;

const char* Exn_err2str(Exnerr error)
{
    switch ( error )
    {
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

/* function prototypes */

Exn Exn_create(void);
void Exn_release( Exn num );
void Exn_init( Exn num, size_t prec);
Exn atoExn( const char* str );
Exn Exn_abs( const Exn num );
bool Exn_iszero( const Exn num );
bool Exn_isneg( const Exn num );
Exn Exn_neg( const Exn num );
Exn Exn_add( const Exn num1, const Exn num2 );
Exn Exn_sub( const Exn num1, const Exn num2 );
Exn Exn_mul( const Exn num1, const Exn num2 );
Exn Exn_div( const Exn num1, const Exn num2 );
Exn Exn_divmod( const Exn num1, const Exn num2, Exn* mod );
Exn Exn_modulo( const Exn num1, const Exn num2 );
Exn Exn_sqrt( const Exn num );
int Exn_cmp( const Exn num1, const Exn num2 );
void Exn_fmt( const Exn num, char* str, int maxlen );

// ############################## BinOprExpr Class ##############################

/// @brief Binary operand expression class
typedef struct { 
    char binop; // binary operator
    Exn num1; // number 1
    Exn num2; // number 2
} BinOprExpr;

// ############################## Math Func Class ##############################

/// @brief Math function type
/// @details This enum is used to represent the built-in math function type
typedef enum {
    MFUNC_SQRT = 0,
    MFUNC_POWER,
    MFUNC_EXP,
    MFUNC_LOG,
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
bool MFunc_ckarg(const MathFunc* func)
{
    if (func == NULL)
    {
        MEM_ACCESS_NULLPTR("func", "The math function can't NULL");
        return false;
    }
    char* func_n = func->alias;
    switch (func->type) {
        case MFUNC_SQRT:
            if (func->argc != 1)
            {
                FUNCT_ARG_MISMATCH(func_n, 1, func->argc);
                return false;
            }
            break;
        case MFUNC_POWER:
            if (func->argc != 2)
            {
                FUNCT_ARG_MISMATCH(func_n, 2, func->argc);
                return false;
            }
            break;
        case MFUNC_EXP:
            if (func->argc != 1)
            {
                FUNCT_ARG_MISMATCH(func_n, 1, func->argc);
                return false;
            }
            break;
        case MFUNC_LOG:
            if (func->argc != 2)
            {
                FUNCT_ARG_MISMATCH(func_n, 2, func->argc);
                return false;
            }
            break;
        default:
            FUNCT_INVOKE_FAIL(func_n, "no such math function");
            return 0;
    }
    return 1;
}

#endif // MATH_H