#include "math.h"
#include "util.h"

static Exn_struct __zero__ = {
    .digits = NULL,
    .length = 1,
    .decimal = 1,
    .sign = 0,
    .precision = MAX_DIGITS,
    .error = EXTENDED_NUM_OK
};

static Exn_struct __one__ = {
    .digits = NULL,
    .length = 1,
    .decimal = 1,
    .sign = 1,
    .precision = MAX_DIGITS,
    .error = EXTENDED_NUM_OK
};

static Exn_struct __two__ = {
    .digits = NULL,
    .length = 1,
    .decimal = 1,
    .sign = 1,
    .precision = MAX_DIGITS,
    .error = EXTENDED_NUM_OK
};

__attribute((constructor)) void init_zero() {
    __zero__.digits = (char*)malloc(__zero__.precision * sizeof(char));
    if (__zero__.digits == NULL) {
        MEM_ALLOC_FAIL(&__zero__.digits, "The extended number zero allocation failed");
        return;
    }
    __zero__.digits[0] = '0';
    __zero__.digits[1] = '\0';
}

__attribute((destructor)) void free_zero() {
    if (__zero__.digits) {
        free(__zero__.digits);
    }
}

__attribute((constructor)) void init_one() {
    __one__.digits = (char*)malloc(__one__.precision * sizeof(char));
    if (__one__.digits == NULL) {
        MEM_ALLOC_FAIL(&__one__.digits, "The extended number one allocation failed");
        return;
    }
    __one__.digits[0] = '1';
    __one__.digits[1] = '\0';
}

__attribute((destructor)) void free_one() {
    if (__one__.digits) {
        free(__one__.digits);
    }
}

__attribute((constructor)) void init_two() {
    __two__.digits = (char*)malloc(__two__.precision * sizeof(char));
    if (__two__.digits == NULL) {
        MEM_ALLOC_FAIL(&__two__.digits, "The extended number two allocation failed");
        return;
    }
    __two__.digits[0] = '2';
    __two__.digits[1] = '\0';
}

__attribute((destructor)) void free_two() {
    if (__two__.digits) {
        free(__two__.digits);
    }
}

#define Exn_zero &__zero__   /* Constant Zero */
#define Exn_one  &__one__    /* Constant One */
#define Exn_two  &__two__    /* Constant Two */

const char* Exn_err2str( ExtendedNumError error )
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

NO_EXPORT Exn __ntiter_inv( const Exn __n , const Exn __x )
{
    /**
     * x_{n+1} = (2 - n * x_n) * x_n
     */
    Exn tmp1 = Exn_mul(__n, __x);
    if (!tmp1) return NULL;
    Exn tmp2 = Exn_sub(Exn_two, tmp1);
    Exn_release(&tmp1);
    if (!tmp2) return NULL;
    Exn __nx = Exn_mul(tmp2, __x);
    Exn_release(&tmp2);
    return __nx;
}



Exn Exn_create(int prec)
{
    if (prec <= 0) {
        prec = MAX_DIGITS;
    }

    Exn num = (Exn)malloc(sizeof(Exn_struct));
    if (num == NULL) {
        MEM_ALLOC_FAIL(&num, "The extended number allocation failed");
        return NULL;
    }

    num->length = 1;
    num->sign = 0;
    num->decimal = -1;
    num->error = EXTENDED_NUM_OK;
    num->digits = (char*)calloc(prec + 10, sizeof(char));
    num->precision = prec;

    if (num->digits == NULL) {
        MEM_ALLOC_FAIL(&num->digits, "The extended number digits allocation failed");
        free(num);
        return NULL;
    }

    return num;
}

Exn Exn_rand(int prec)
{
    Exn num = Exn_create(prec);
    if (num == NULL) return NULL;

    int len = rand() % (prec + 1);
    num->length = len;
    num->decimal = rand() % (len + 1);
    num->sign = rand() % 2 ? 1 : -1;

    for (int i = 0; i < len; i++) {
        num->digits[i] = '0' + rand() % 10;
    }
    num->digits[len] = '\0';

    return num;
}

Exn Exn_randp(int prec, int exp)
{
    Exn num = Exn_create(prec);
    if (num == NULL) return NULL;

    int len = rand() % (prec + 1);
    num->length = len;
    num->decimal = exp;
    num->sign = rand() % 2 ? 1 : -1;

    for (int i = 0; i < len; i++) {
        num->digits[i] = '0' + rand() % 10;
    }
    num->digits[len] = '\0';

    return num;
}

void Exn_release( Exn* num )
{
    if (*num == NULL) return;
    if ((*num)->digits) {
        free((*num)->digits);
    }
    free(*num);
    *num = NULL;
}

/**
 * @brief Copy the extended number
 * @param dest Destination extended number
 * @param src Source extended number
 * @note This is a deep copy method
 */
NO_EXPORT void Exn_cpy( Exn* dest, const Exn src )
{
    Exn_release(dest);
    *dest = Exn_create(src->precision);
    if (dest == NULL) {
        MEM_ALLOC_FAIL(dest, "The extended number copy allocation failed");
        return;
    }
    (*dest)->length = src->length;
    (*dest)->decimal = src->decimal;
    (*dest)->sign = src->sign;
    (*dest)->error = src->error;
    (*dest)->precision = src->precision;
    // for (int i = 0; i < src->length; i++) {
    //     (*dest)->digits[i] = src->digits[i];
    // }
    memcpy((*dest)->digits, src->digits, src->length);
}

/**
 * @brief Move the extended number
 * @param dest Destination extended number
 * @param src Source extended number
 * @note The source extended number will be moved to destination
 */
NO_EXPORT void Exn_mv( Exn* dest, Exn* src )
{
    Exn_release(dest);
    *dest = *src;
    *src = NULL;
}

/**
 * @brief Convert a string to an extended number
 * @param str The string to convert
 * @param prec The precision scale of the conversion
 * @return Exn The extended number
 * @attention A string with scientific notation is not supported
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __Exn_fromstr( const char* str, int prec )
{
    if (str == NULL || str[0] == '\0') return NULL;
    Exn num = Exn_create(prec);
    if (num == NULL) return NULL;

    int len = strlen(str);
    int digit_count = 0;
    bool has_decimal = false;
    bool has_digit = false;
    int pos = 0;

    /* skip the leading spaces */
    while (pos < len && isspace(str[pos])) {
        pos++;
    }

    /* extract the sign */
    if (str[pos] == '-') {
        num->sign = -1;
        pos++;
    } else {
        num->sign = 1;
        if (str[pos] == '+') {
            pos++;
        }
    }

    /* skip the leading zeros */
    while (pos < len && str[pos] == '0') {
        pos++;
    }

    /* This is an zero  */
    if (pos == len || pos == len - 1 && str[pos] == '.') {
        Exn_cpy(&num, Exn_zero);
        return num;
    }

    /* extract the digits */
    for (; pos < len; pos++) {
        
        if (str[pos] == '.') { /* encounter a decimal point */
            if (has_decimal) {
                num->error = EXTENDED_NUM_INVALID_INPUT;
                MATH_CVT_FAIL("str", "ExtendedNumber", "too many decimal points");
                WAVY(str, pos, 1, ANSI_PURPLE);
                return num;
            }
            has_decimal = true;
            num->decimal = digit_count;
        } else if (isdigit(str[pos])) { /* encounter a digit */
            has_digit = true;
            if (digit_count < prec) {
                num->digits[digit_count++] = str[pos];
            } else {
                digit_count++;
            }
        } else {
            num->error = EXTENDED_NUM_INVALID_INPUT;
            MATH_CVT_FAIL("str", "ExtendedNumber", "not a digit");
            WAVY(str, pos, len - pos, ANSI_RED);
            return num;
        }
    }

    if (!has_digit) {
        Exn_cpy(&num, Exn_zero);
        return num;
    }

    if (!has_decimal) {
        num->decimal = digit_count;
    }

    if (digit_count > prec) MATH_PREC_LOSE(prec, digit_count);

    num->length = digit_count;
    Exn_normalize(num);
    return num;
}

Exn atoExn( const char* str, int prec )
{
    if (str == NULL || str[0] == '\0') return NULL;
    
    int len = strlen(str);
    int e_pos = -1; /* pointer to find 'e' or 'E' symbol */

    for (int i = 0; i < len; i++) {
        if (str[i] == 'e' || str[i] == 'E') {
            e_pos = i;
            break;
        }
    }

    if (e_pos == -1) return __Exn_fromstr(str, prec);

    /* extract the base */
    char* base_str = (char*)malloc(e_pos + 1);
    if (base_str == NULL) {
        MEM_ALLOC_FAIL(&base_str, "The base string allocation failed");
        return NULL;
    }
    strncpy(base_str, str, e_pos);
    base_str[e_pos] = '\0';
    Exn base = __Exn_fromstr(base_str, prec);
    free(base_str);

    if (!base || base->error != EXTENDED_NUM_OK) {
        return base;
    }

    /* extract the exponent */
    int exponent = 0;
    int exp_sign = 1;
    int exp_start = e_pos + 1;
    
    if (exp_start < len) {
        if (str[exp_start] == '+') {
            exp_start++;
        } else if (str[exp_start] == '-') {
            exp_sign = -1;
            exp_start++;
        }
        
        for (int i = exp_start; i < len; i++) {
            if (isdigit(str[i])) {
                exponent = exponent * 10 + Ch2val(str[i]);
            } else {
                base->error = EXTENDED_NUM_INVALID_INPUT;
                if (str[i] == '.') {
                    MATH_CVT_FAIL("str", "ExtendedNumber", "no decimal point in an exponent");
                } else {
                    MATH_CVT_FAIL("str", "ExtendedNumber", "not a digit in an exponent");
                }
                WAVY(str, i, len - i, ANSI_CYAN);
                return base;
            }
        }
    }
    
    exponent *= exp_sign;
    base->decimal += exponent;
    Exn_normalize(base);
    return base;
}

void Exn_normalize(Exn num) {
    if (!num || !num->digits) return;

    // Exn_show(num); //?
    
    /* get the index of the first non-zero digit */
    int first_non_zero = 0;
    while (first_non_zero < num->length && num->digits[first_non_zero] == '0') {
        first_non_zero++;
    }
    
    /* is zero */
    if (first_non_zero == num->length) {
        Exn_cpy(&num, Exn_zero);
        return;
    }
    
    /* get the index of the last non-zero digit */
    int last_non_zero = num->length - 1;
    while (last_non_zero >= 0 && num->digits[last_non_zero] == '0') {
        last_non_zero--;
    }
    
    /* adjust the digits */
    int effective_digits = last_non_zero - first_non_zero + 1;
    if (effective_digits > num->precision) {
        MATH_PREC_LOSE(num->precision, effective_digits);
        last_non_zero = first_non_zero + num->precision - 1;
    }
    
    /* move the digits to eliminate leading zeros */
    if (first_non_zero > 0) {
        for (int i = 0; i <= last_non_zero - first_non_zero; i++) {
            num->digits[i] = num->digits[first_non_zero + i];
        }
        
        num->decimal -= first_non_zero;
    }

    /* eliminate the trailing zeros */
    for (int i = last_non_zero + 1; i < num->length; i++) {
        num->digits[i] = '\0';
    }
    
    /* update length (number of valid digit)*/
    num->length = last_non_zero - first_non_zero + 1;
}

void Exn_show(const Exn num)
{
    if (num == NULL) {
        fprintf(stdout, "NULL\r\n");
        return;
    }
    fprintf(stdout, "Extended Number at <%p>: {\r\n", num);
    fprintf(stdout, "\tSign: %c\r\n", (num->sign>0)? '+' : '-');
    fprintf(stdout, "\tLength: %d\r\n", num->length);
    fprintf(stdout, "\tDecimal: %d\r\n", num->decimal);
    fprintf(stdout, "\tError: %s\r\n", Exn_err2str(num->error));
    fprintf(stdout, "\tDigits: %s\r\n", num->digits);
    fprintf(stdout, "\tPrecision: %d\r\n", num->precision);
    fprintf(stdout, "}\r\n");

}

/**
 * @brief Format the extended number to a normal string
 * @param num The extended number
 * @return char* The formatted string
 * @example Exn_fmtn(num) -> "123.456"
 * @note The caller is responsible for releasing the memory of the string
 * @attention This function won't check the validity of the arguments, since it's an internal function
 */
NO_EXPORT char* __Exn_fmtn(Exn num) {
    int buf_len = num->precision + num->decimal + 10;
    char* buf = (char*)malloc(buf_len);
    if (buf == NULL) {
        MEM_ALLOC_FAIL(&buf, "The formatting buffer allocation failed");
        return NULL;
    }

    int pos = 0;
    if (num->sign == -1) {
        buf[pos++] = '-';
    }

    if (num->decimal <= 0) { /* magnitude < 1 */
        buf[pos++] = '0';
        buf[pos++] = '.';
        for (int i = 0; i < -num->decimal; i++) {
            buf[pos++] = '0';
        }
        for (int i = 0; i < num->length; i++) {
            buf[pos++] = num->digits[i];
        }
    } else if (num->decimal >= num->length) { /* magnitude >= 1 with trailing zeros*/
        buf[pos++] = num->digits[0];
        for (int i = 1; i < num->length; i++) {
            buf[pos++] = num->digits[i];
        }
        for (int i = 0; i < num->decimal - num->length; i++) {
            buf[pos++] = '0';
        }
        
    } else { /* magnitude >= 1 without trailing zeros */
        for (int i = 0; i < num->length; i++) {
            if (i == num->decimal) {
                buf[pos++] = '.';
            }
            buf[pos++] = num->digits[i];
        }
    }

    buf[pos] = '\0';
    return buf;
}

/**
 * @brief Format the extended number to a strictly precise string
 * @param num The extended number
 * @return char* The formatted string
 * @example Exn_fmtp(num) -> "123.4560000"
 * @note The caller is responsible for releasing the memory of the string
 * @attention This function won't check the validity of the arguments, since it's an internal function
 */
NO_EXPORT char* __Exn_fmtp(Exn num) {
    
    int prec = num->precision;
    char* str = __Exn_fmtn(num);
    if (str == NULL) return NULL;
    int len = strlen(str);
    int digit_n = MAX(num->length, num->decimal); /* exact digit number */

    if (prec <= digit_n) { /* no need to add trailing zeros */
        return str;
    } else { /* add trailing zeros */
        int buf_len = prec + 10;
        char* buf = (char*)malloc(buf_len);
        if (buf == NULL) {
            MEM_ALLOC_FAIL(&buf, "The formatting buffer allocation failed");
            return NULL;
        }

        int pos = 0;
        strncpy(buf, str, len);
        pos = len;

        if (num->decimal > 0 && num->decimal >= num->length) {
            buf[pos++] = '.';
        }

        for (int i = digit_n; i < prec; i++) {
            buf[pos++] = '0';
        }

        buf[pos] = '\0';
        free(str);
        return buf;
    }
}

/**
 * @brief Format the extended number to a scientific notation string
 * @param num The extended number
 * @return char* The formatted string
 * @example Exn_fmts(num) -> "1.23456E+2"
 * @note The caller is responsible for releasing the memory of the string
 * @attention This function won't check the validity of the arguments, since it's an internal function
 */
NO_EXPORT char* __Exn_fmts(Exn num) {
    int buf_len = num->length + 10;
    char* buf = (char*)malloc(buf_len);
    if (buf == NULL) {
        MEM_ALLOC_FAIL(&buf, "The formatting buffer allocation failed");
        return NULL;
    }

    int pos = 0;
    if (num->sign == -1) {
        buf[pos++] = '-';
    }

    buf[pos++] = num->digits[0];
    if (num->length > 1) {
        buf[pos++] = '.';
        // for (int i = 1; i < num->length; i++) {
        //     buf[pos++] = num->digits[i];
        // }
        memcpy(buf + pos, num->digits + 1, num->length - 1);
        pos += num->length - 1;
    }

    buf[pos++] = 'E';
    if (num->decimal < 0) {
        buf[pos++] = '-';
    }

    int exp = num->decimal-1;
    if (exp < 0) {
        exp = -exp;
    }
    int exp_len = 0;
    int exp_buf[10];
    while (exp > 0) {
        exp_buf[exp_len++] = exp % 10;
        exp /= 10;
    }

    for (int i = exp_len - 1; i >= 0; i--) {
        buf[pos++] = exp_buf[i] + '0';
    }

    buf[pos] = '\0';
    return buf;
}

char* Exn_fmt(Exn num, Exnfmt mode) {
    if (num == NULL) return NULL;
    switch (mode) {
    case EXN_FMT_NORMAL:
        return __Exn_fmtn(num);
    case EXN_FMT_PRECISE:
        return __Exn_fmtp(num);
    case EXN_FMT_SCIENTIFIC:
        return __Exn_fmts(num);
    default:
        return NULL;
    }
}

/**
 * @brief Add two extended numbers ignoring the sign
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the unsinged addition
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __Exn_addu( const Exn num1, const Exn num2 )
{

    // align the decimal
    int maxDecimal = MAX(num1->decimal, num2->decimal);
    int minDecimal = MIN(num1->decimal, num2->decimal);
    int diff = maxDecimal - minDecimal; /* decimal different */
    int carry = 0;
    int len = MAX(num1->length, num2->length) + diff + 1; // +1 for carry
    int prec = MAX(num1->precision, num2->precision);
    char* align_num1 = (char*)malloc(len);
    char* align_num2 = (char*)malloc(len);
    if (align_num1 == NULL || align_num2 == NULL) {
        MEM_ALLOC_FAIL(&align_num1, "The extended number allocation failed");
        MEM_ALLOC_FAIL(&align_num2, "The extended number allocation failed");
        return NULL;
    }
    Exn result = Exn_create(len);
    int i = len - 1;

    /* align the number */
    memset(align_num1, '0', len);
    memset(align_num2, '0', len);
    if (num1->decimal == minDecimal) {
        memcpy(align_num1 + diff + 1, num1->digits, num1->length);
    } else {
        memcpy(align_num1 + 1, num1->digits, num1->length);
    }
    if (num2->decimal == minDecimal) {
        memcpy(align_num2 + diff + 1, num2->digits, num2->length);
    } else {
        memcpy(align_num2 + 1, num2->digits, num2->length);
    }

    for (; i >= 0 || carry > 0; i--) {
        int sum = Ch2val(align_num1[i]) + Ch2val(align_num2[i]) + carry;
        result->digits[i] = (sum % 10) + '0';
        carry = sum / 10;
    }

    result->length = len;
    result->decimal = maxDecimal + 1;
    result->precision = prec;

    // handle overflow
    if (carry > 0 && i < 0) {
        result->error = EXTENDED_NUM_OVERFLOW;
    }

    free(align_num1);
    free(align_num2);
    return result;
}

/**
 * @brief Subtract two extended numbers ignoring the sign
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the unsigned subtraction
 * @note The caller is responsible for releasing the memory
 */
NO_EXPORT Exn __Exn_subu( const Exn num1, const Exn num2 )
{

    // align the decimal
    int maxDecimal = MAX(num1->decimal, num2->decimal);
    int minDecimal = MIN(num1->decimal, num2->decimal);
    int diff = maxDecimal - minDecimal; /* decimal different */
    int borrow = 0;
    int len = MAX(num1->length, num2->length) + diff + 1;
    Exn result = Exn_create(len);
    int i = len - 1;

    /* align decimal */
    char* align_num1 = (char*)malloc(len);
    char* align_num2 = (char*)malloc(len);
    if (align_num1 == NULL || align_num2 == NULL) {
        MEM_ALLOC_FAIL(&align_num1, "The extended number allocation failed");
        MEM_ALLOC_FAIL(&align_num2, "The extended number allocation failed");
        return NULL;
    }

    memset(align_num1, '0', len);
    memset(align_num2, '0', len);
    if (num1->decimal == minDecimal) {
        memcpy(align_num1 + diff + 1, num1->digits, num1->length);
    } else {
        memcpy(align_num1 + 1, num1->digits, num1->length);
    }
    if (num2->decimal == minDecimal) {
        memcpy(align_num2 + diff + 1, num2->digits, num2->length);
    } else {
        memcpy(align_num2 + 1, num2->digits, num2->length);
    }

    for (i = len - 1; i >= 0; i--) {
        int digit1 = Ch2val(align_num1[i]);
        int digit2 = Ch2val(align_num2[i]);
        int diff_ = digit1 - digit2 - borrow;
        if (diff_ < 0) {
            diff_ += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = diff_ + '0';
    }

    result->length = len;
    result->decimal = maxDecimal + 1;
    result->precision = MAX(num1->precision, num2->precision);
    free(align_num1);
    free(align_num2);


    // handle underflow
    if (borrow > 0 && i < 0) {
        result->error = EXTENDED_NUM_OVERFLOW;
    }
    // remove leading zeros
    Exn_normalize(result);
    return result;
}

Exn Exn_abs( const Exn num )
{
    Exn result = NULL;
    Exn_cpy(&result, num);
    result->sign = 1;
    return result;
}

Exn Exn_shift( const Exn num, int shift )
{
    if (!num || !num->digits) {
        INPUT_ARG_FAIL("operand", "The operand can't be NULL");
        return NULL;
    }
    if (num->error != EXTENDED_NUM_OK) {
        INPUT_ARG_FAIL("operand", "The operand has an error");
        return NULL;
    }
    Exn result = NULL;
    Exn_cpy(&result, num);
    result->decimal += shift;
    // Exn_normalize(result);
    return result;
}

bool Exn_iszero( const Exn num )
{
    return num->sign == 0;
}

Exn Exn_add( const Exn num1, const Exn num2 )
{
    if (!num1 || !num2 || !num1->digits || !num2->digits) {
        INPUT_ARG_FAIL("operand", "The operand can't be NULL");
        return NULL;
    }
    if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
        INPUT_ARG_FAIL("operand", "The operand has an error");
        return NULL;
    }
    int sign = num1->sign * num2->sign;
    Exn result = NULL;

    if (sign > 0) {
        result = __Exn_addu(num1, num2);
        result->sign = num1->sign;
    } else if (sign == 0)
    {
        if (Exn_iszero(num1)) {
            Exn_cpy(&result, num2);
        } else { /* num2 is zero */
            Exn_cpy(&result, num1);
        }
    }
    else /* sign < 0 */
    {
        Exn num1_ = Exn_abs(num1);
        Exn num2_ = Exn_abs(num2);
        if (Exn_cmp(num1_, num2_) > 0) {
            result = __Exn_subu(num1_, num2_);
            result->sign = num1->sign;
        } else {
            result = __Exn_subu(num2_, num1_);
            result->sign = num2->sign;
        }
        Exn_release(&num1_);
        Exn_release(&num2_);
    }
    Exn_normalize(result);
    return result;
}

Exn Exn_sub( const Exn num1, const Exn num2 )
{
    if (!num1 || !num2 || !num1->digits || !num2->digits) {
        INPUT_ARG_FAIL("operand", "The operand can't be NULL");
        return NULL;
    }
    if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
        INPUT_ARG_FAIL("operand", "The operand has an error");
        return NULL;
    }
    int sign = num1->sign * num2->sign;
    Exn result = NULL;

    if (sign > 0) {
        if (Exn_cmp(num1, num2) > 0) {
            result = __Exn_subu(num1, num2);
            result->sign = num1->sign;
        } else {
            result = __Exn_subu(num2, num1);
            result->sign = -num2->sign;
        }
    } else if (sign == 0)
    {
        if (Exn_iszero(num1)) {
            Exn_cpy(&result, num2);
            result->sign = -num2->sign;
        } else { /* num2 is zero */
            Exn_cpy(&result, num1);
        }
    }
    else /* sign < 0 */
    {
        Exn num1_ = Exn_abs(num1);
        Exn num2_ = Exn_abs(num2);
        result = __Exn_addu(num1_, num2_);
        result->sign = num1->sign;
        Exn_release(&num1_);
        Exn_release(&num2_);
    }
    Exn_normalize(result);
    return result;  
}

/**
 * @brief Multiply two extended numbers
 * @details This function uses the naive multiplication algorithm with O(n^2) complexity
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since it's an internal function
 */
Exn __Exn_mul_pl( const Exn num1, const Exn num2, int len, int sign )
{
    Exn result = Exn_create(len);
    if (result == NULL) {
        MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
        return NULL;
    }
    /* initialize the result */
    memset(result->digits, 0, len);

    /* use naive multiplication */
    /* result[i+j] = num1[i] * num2[j] + carry */
    for (int i = num1->length - 1; i >= 0 ; i--) {
        for (int j = num2->length - 1; j >= 0; j--) {
            int mul = (num1->digits[i] - '0') * (num2->digits[j] - '0');
            int sum = mul + (result->digits[i + j + 1]);
            result->digits[i + j + 1] = sum % 10;
            result->digits[i + j] += sum / 10;
        }
    }

    /* add to char '0' */
    for (int i = 0; i < len; i++) {
        result->digits[i] += '0';
    }

    result->length = len;
    result->decimal = num1->decimal + num2->decimal;
    result->precision = MAX(num1->precision, num2->precision);
    result->sign = sign;

    return result;
}

Exn __Exn_mul_fft( const Exn num1, const Exn num2, int len, int sign )
{
    /* calc the power of 2 */
    int size = 1;
    while (size < len) {
        size <<= 1;
    }

    complex* fa = (complex *)calloc(size, sizeof(complex));
    complex* fb = (complex *)calloc(size, sizeof(complex));

    if(!fa || !fb) {
        MEM_ALLOC_FAIL(&fa, "The extended number allocation failed");
        MEM_ALLOC_FAIL(&fb, "The extended number allocation failed");
        return NULL;
    }

}

Exn Exn_mul( const Exn num1, const Exn num2 )
{
    if (!num1 || !num2 || !num1->digits || !num2->digits) {
        INPUT_ARG_FAIL("operand", "The operand can't be NULL");
        return NULL;
    }
    if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
        INPUT_ARG_FAIL("operand", "The operand has an error");
        return NULL;
    }
    int sign = num1->sign * num2->sign;

    Exn result = NULL;

    /* if muland is zero */
    if (sign == 0) {
        Exn_cpy(&result, Exn_zero);
        return result;
    }

    /* calc the precision */
    int len = num1->length + num2->length;

    /* choose diff method according to size */
    result = __Exn_mul_pl(num1, num2, len, sign);
    
    Exn_normalize(result);
    return result;
}

Exn Exn_div(const Exn num1, const Exn num2)
{
    if (!num1 || !num2 || !num1->digits || !num2->digits) {
        INPUT_ARG_FAIL("operand", "The operand can't be NULL");
        return NULL;
    }
    if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
        INPUT_ARG_FAIL("operand", "The operand has an error");
        return NULL;
    }
    if (Exn_iszero(num2)) {
        MATH_DIV_ZERO("The divisor can't be zero");
        return NULL;
    }

    Exn result = NULL;
    /* if dividend is zero */
    if (Exn_iszero(num1)) {
        Exn_cpy(&result, Exn_zero);
        return result;
    }
    int len = MAX(num1->length, num2->length) * 2 + 1;
    int prec = MAX(num1->precision, num2->precision);
    int bound = prec * 2 + 1; /* the bound of the iteration */

    /* calc the inverse of divisor */
    Exn inv_num2 = Exn_shift(Exn_one, -num2->decimal);
    Exn_newton(__ntiter_inv, bound, num2, &inv_num2);

    result = Exn_mul(num1, inv_num2);
    Exn_release(&inv_num2);
    if (result) {
        memset(result->digits + len, 0, result->length - len + 1);
        result->length = len;
        Exn_normalize(result);
    }
    
    return result;
}

void Exn_newton(Exn (*func)(Exn, Exn), int bound, Exn param, Exn *result){
    printf("bound: %d\r\n", bound);
    while((*result)->length < bound){
        Exn temp = func(param, *result);
        if (!temp || temp->error != EXTENDED_NUM_OK) {
            Exn_release(&temp);
            break;
        }
        Exn_mv(result, &temp);
    }
    Exn_normalize(*result);
}

int Exn_cmp( const Exn num1, const Exn num2 )
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