#ifndef MATH_H_
#define MATH_H_

/* for cplusplus function call  */
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <complex.h>
#include <assert.h>

#include "util.h"
#include "error.h"

/* A macro to return the value of a digit character */
#define Ch2val(c) (c - '0')
#ifndef complex
#define complex _Complex double
#endif

// ############################## Extended Number Class ##############################

/* constants for ExtendedNum class */
#define MAX_DIGITS 10000
#define DECIMAL_PLACES 100

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
 * @brief Get the negative value of the extended number
 * @param num The extended number
 * @return Exn The negative value of the extended number
 * @note The caller is responsible for releasing the memory
 */
Exn Exn_neg( const Exn num );

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

/**
 * @brief Power of an extended number
 * @details Library function ksm() is used to calculate the power of an extended number;
 * this is based on fast exponentiation algorithm
 * @param base The base of the power
 * @param exp The exponent of the power
 * @return Exn The result of the power
 * @note The caller is responsible for releasing the memory
 * @note The exponent must be an integer ranged
 */
Exn ksm( const Exn base, const int exp );
int Exn_cmp( const Exn num1, const Exn num2 );
void Exn_normalize( Exn num );

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
 * @example 
 */
void Exn_show( const Exn num );

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
bool MFunc_ckarg(const MathFunc* func);

#ifdef __cplusplus
}
#endif

#endif // MATH_H