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
#include <math.h>

#include "error.h"
#include "util.h"

/* A macro to return the value of a digit character */
#define Ch2val(c) (c - '0')
#ifndef complex
#define complex _Complex double
#endif

// ############################## Extended Number Class ##############################

/* constants for ExtendedNum class */
#define MAX_DIGITS 10000 //! TODO： remove this
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
 * @example 
 */
void Exn_show( const Exn num );

//! Delete later
Exn __Exn_mul_fft( const Exn num1, const Exn num2, int len, int sign );
Exn __Exn_mul_karatsuba( const Exn num1, const Exn num2, int len, int sign );
Exn __Exn_mul_pl( const Exn num1, const Exn num2, int len, int sign );
//! Delete later

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

#endif // MATH_H