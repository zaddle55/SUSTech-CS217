#pragma once
// header template
#ifndef MATH_H
#define MATH_H

// constants defined for extended number class
#define MAX_DIGITS 1000
#define DECIMAL_PLACES 100

// error codes
typedef enum {
    EXTENDED_NUM_OK = 0,
    EXTENDED_NUM_OVERFLOW,
    EXTENDED_NUM_DIVIDE_BY_ZERO,
    EXTENDED_NUM_INVALID_INPUT,
    EXTENDED_NUM_MEMORY_ERROR
} ExtendedNumError;

typedef struct {
    char digits[MAX_DIGITS];
    int length;
    int decimal;  // position of decimal point
    int sign;
    ExtendedNumError error;
} ExtendedNum;

const char* extended_num_error_to_string(ExtendedNumError error)
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

// function prototypes for extended number class
void initen( ExtendedNum* num );
ExtendedNumError atoen( ExtendedNum* num, const char* str );
ExtendedNumError adden( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 );
ExtendedNumError suben( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 );
ExtendedNumError mulen( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 );
ExtendedNumError diven( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 );
int cmpen( const ExtendedNum* num1, const ExtendedNum* num2 );
void fmt( const ExtendedNum* num, char* str, int maxlen );

#endif // MATH_H