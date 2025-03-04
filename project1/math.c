#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "error.h"
#include "math.h"

/**
 * @brief Create an extended number
 * @details Allocate memory for the extended number
 * @note The caller is responsible for releasing the memory
 * @return ExtendedNum* A pointer extended number
 */
ExtendedNum* Exn_create(void)
{
    ExtendedNum* num = (ExtendedNum*)malloc(sizeof(ExtendedNum));
    if (num == NULL) {
        MEM_ALLOC_FAIL(&num, "The extended number allocation failed");
    }
    Exn_init(num, MAX_DIGITS);
    return num;
}

void Exn_release( ExtendedNum* num )
{
    if (num != NULL) {
        free(num);
        if (num->digits != NULL) {
            free(num->digits);
        }
    }
}

/// @brief Initialize the extended number
/// @param num Extended number to initialize
/// @param prec Precision of the number
void Exn_init( ExtendedNum* num, size_t prec)
{
    if (num == NULL) {
        MEM_ACCESS_NULLPTR("num", "The extended number can't be NULL");
        return;
    }
    if (num != NULL && num->digits != NULL) {
        free(num->digits);
    }
    num->digits = (char*)malloc(prec * sizeof(char));
    if (num->digits == NULL) {
        MEM_ALLOC_FAIL(&num->digits, "The extended number digits allocation failed");
    }
    num->length = 0;
    num->decimal = -1;
    num->sign = 1;
    num->error = EXTENDED_NUM_OK;
}

ExtendedNumError atoExn( ExtendedNum* num, const char* str )
{
    if (str == NULL || str[0] == '\0') {
        num->error = EXTENDED_NUM_INVALID_INPUT;
        return num->error;
    }
    Exn_init(num, strlen(str));
    
    if (str == NULL || str[0] == '\0') {
        num->error = EXTENDED_NUM_INVALID_INPUT;
        return num->error;
    }
    
    int i = 0;
    if (str[0] == '-') {
        num->sign = -1;
        i++;
    } else if (str[0] == '+') {
        i++;
    }

    int j = 0;
    int hasDigits = 0;
    int eFound = 0;
    int exponent = 0;
    int expSign = 1;
    
    // 解析主要数字部分和小数点
    while (str[i] != '\0' && !eFound && j < MAX_DIGITS) {
        if (str[i] == '.') {
            if (num->decimal >= 0) {
                num->error = EXTENDED_NUM_INVALID_INPUT;
                return num->error;
            }
            num->decimal = j;
        } else if (isdigit(str[i])) {
            num->digits[j++] = str[i];
            hasDigits = 1;
        } else if (str[i] == 'e' || str[i] == 'E') {
            eFound = 1;
        } else {
            num->error = EXTENDED_NUM_INVALID_INPUT;
            return num->error;
        }
        i++;
    }

    // 解析指数部分
    if (eFound) {
        if (str[i] == '-') {
            expSign = -1;
            i++;
        } else if (str[i] == '+') {
            i++;
        }
        
        int expValue = 0;
        while (str[i] != '\0' && isdigit(str[i])) {
            expValue = expValue * 10 + (str[i] - '0');
            i++;
        }
        exponent = expValue * expSign;
    }

    if (!hasDigits || str[i] != '\0') {
        num->error = EXTENDED_NUM_INVALID_INPUT;
        return num->error;
    }

    num->length = j;

    // 处理指数部分对小数点位置的影响
    if (exponent != 0) {
        if (num->decimal < 0) {
            num->decimal = num->length;
        }
        
        // 调整小数点位置
        num->decimal += exponent;
        
        // 如果指数为正，需要在末尾补零
        if (exponent > 0) {
            for (i = 0; i < exponent && num->length < MAX_DIGITS; i++) {
                num->digits[num->length++] = '0';
            }
        }
        
        // 如果指数为负且小数点移到数字之前，需要在前面补零
        if (num->decimal < 0) {
            int shift = -num->decimal;
            if (num->length + shift >= MAX_DIGITS) {
                num->error = EXTENDED_NUM_INVALID_INPUT;
                return num->error;
            }
            
            // 将数字向右移动
            for (i = num->length - 1; i >= 0; i--) {
                num->digits[i + shift] = num->digits[i];
            }
            // 补零
            for (i = 0; i < shift; i++) {
                num->digits[i] = '0';
            }
            num->length += shift;
            num->decimal = 0;
        }
    }

    return EXTENDED_NUM_OK;
}

NO_EXPORT ExtendedNumError __unsigned_adden( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 )
{
    Exn_init(result, num1->length + num2->length);

    // align the decimal
    int maxDecimal = num1->decimal > num2->decimal ? num1->decimal : num2->decimal;
    int carry = 0;
    int i = (num1->length > num2->length ? num1->length : num2->length) - 1;

    while (i >= 0 || carry > 0) {
        int digit1 = (i < num1->length ? num1->digits[i] - '0' : 0);
        int digit2 = (i < num2->length ? num2->digits[i] - '0' : 0);
        int sum = digit1 + digit2 + carry;
        if (sum >= 10) {
            sum -= 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result->digits[i] = sum + '0';
        i--;
    }

    result->length = (num1->length > num2->length ? num1->length : num2->length);
    result->decimal = maxDecimal;

    // handle overflow
    if (carry > 0 && i < 0) {
        result->error = EXTENDED_NUM_OVERFLOW;
        return result->error;
    }
    else {
        return EXTENDED_NUM_OK;
    }

}

NO_EXPORT ExtendedNumError __unsigned_suben( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 )
{
    Exn_init(result, num1->length + num2->length);

    // align the decimal
    int maxDecimal = num1->decimal > num2->decimal ? num1->decimal : num2->decimal;
    int borrow = 0;
    int i = (num1->length > num2->length ? num1->length : num2->length) - 1;

    while (i >= 0 || borrow > 0) {
        int digit1 = (i < num1->length ? num1->digits[i] - '0' : 0);
        int digit2 = (i < num2->length ? num2->digits[i] - '0' : 0);
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = diff + '0';
        i--;
    }

    result->length = (num1->length > num2->length ? num1->length : num2->length);
    result->decimal = maxDecimal;

    // handle underflow
    if (borrow > 0 && i < 0) {
        result->error = EXTENDED_NUM_OVERFLOW;
        return result->error;
    }
    else {
        return EXTENDED_NUM_OK;
    }

}

ExtendedNumError Exn_abs( ExtendedNum* result, const ExtendedNum* num )
{
    Exn_init(result, num->length);
    result->sign = 1;
    result->length = num->length;
    result->decimal = num->decimal;
    for (int i = 0; i < num->length; i++) {
        result->digits[i] = num->digits[i];
    }
    return EXTENDED_NUM_OK;
}

ExtendedNumError Exn_add( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 )
{
    int sign = num1->sign * num2->sign;

    if (sign > 0) {
        result->sign = num1->sign;
        return __unsigned_adden(result, num1, num2);
    } else {
        int cmp = Exn_cmp(num1, num2);
        if (cmp == 0) {
            return EXTENDED_NUM_OK;
        } else if (cmp > 0) {
            result->sign = num1->sign;
            return __unsigned_suben(result, num1, num2);
        } else {
            result->sign = num2->sign;
            return __unsigned_suben(result, num2, num1);
        }
    }

}

Exnerr Exn_sub( ExtendedNum* result, const ExtendedNum* num1, const ExtendedNum* num2 )
{
    ExtendedNum num2Copy;
    Exn_abs(&num2Copy, num2);
    return Exn_add(result, num1, &num2Copy);
}

int Exn_cmp( const ExtendedNum* num1, const ExtendedNum* num2 )
{
    if (num1->sign > num2->sign) {
        return 1;
    } else if (num1->sign < num2->sign) {
        return -1;
    } else {
        if (num1->decimal > num2->decimal) {
            return num1->sign;
        } else if (num1->decimal < num2->decimal) {
            return -num1->sign;
        } else {
            int i = 0;
            while (i < num1->length && i < num2->length) {
                if (num1->digits[i] > num2->digits[i]) {
                    return num1->sign;
                } else if (num1->digits[i] < num2->digits[i]) {
                    return -num1->sign;
                }
                i++;
            }
            if (i < num1->length) {
                return num1->sign;
            } else if (i < num2->length) {
                return -num1->sign;
            } else {
                return 0;
            }
        }
    }
}

void fmt( const ExtendedNum* num, char* str, int maxlen )
{
    if (num->error != EXTENDED_NUM_OK) {
        snprintf(str, maxlen, "Error: %s", Exn(err2str)(num->error));
        return;
    }
    
    int pos = 0;
    if (num->sign == -1 && pos < maxlen - 1) {
        str[pos++] = '-';
    }
    
    int started = 0;
    for (int i = 0; i < num->length && pos < maxlen - 1; i++) {
        if (i == num->decimal && num->decimal >= 0) {
            if (!started) {
                str[pos++] = '0';
            }
            str[pos++] = '.';
        }
        if (num->digits[i] != '0' || started || i >= num->decimal) {
            str[pos++] = num->digits[i];
            started = 1;
        }
    }
    
    if (pos == 0 || (pos == 1 && str[0] == '-')) {
        str[pos++] = '0';
    }
    
    str[pos] = '\0';
}

void test_atoen()
{
    ExtendedNum num;
    char input[MAX_DIGITS];
    scanf("%s", input);
    ExtendedNumError error = atoExn(&num, input);
    if (error == EXTENDED_NUM_OK) {
        printf("Number: ");
        for (int i = 0; i < num.length; i++) {
            putchar(num.digits[i]);
        }
        printf("\nDecimal position: %d\n", num.decimal);
        printf("Sign: %d\n", num.sign);
    } else {
        printf("Error: %s\n", Exn(err2str)(error));
    }
}

void test_fmt()
{
    ExtendedNum num;
    char str[MAX_DIGITS];
    ExtendedNumError error = atoExn(&num, "5.E990");
    if (error == EXTENDED_NUM_OK) {
        fmt(&num, str, MAX_DIGITS);
        printf("Formatted: %s\n", str);
    } else {
        printf("Error: %s\n", Exn(err2str)(error));
    }
}

void test_add_primary()
{
    ExtendedNum num1, num2, result;
    atoExn(&num1, "123.456");
    atoExn(&num2, "789.012");
    Exn_add(&result, &num1, &num2);
    char str[MAX_DIGITS];
    fmt(&result, str, MAX_DIGITS);
    printf("Result: %s\n", str);
}

void test_add_pos_neg()
{
    ExtendedNum num1, num2, result;
    atoExn(&num1, "123.456");
    atoExn(&num2, "-789.012");
    Exn_add(&result, &num1, &num2);
    char str[MAX_DIGITS];
    fmt(&result, str, MAX_DIGITS);
    printf("Result: %s\n", str);
}

int main()
{
    // test
    // test_atoen();
    test_fmt();
    test_add_primary();
    test_add_pos_neg();

    return 0;
}