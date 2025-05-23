
#include "math.h"
#include "error.h"

// ################################ Extended Number Constants ##############################

static Exn_struct __zero__ = {.digits = NULL,
                              .length = 1,
                              .decimal = 1,
                              .sign = 0,
                              .precision = MAX_DIGITS,
                              .error = EXTENDED_NUM_OK};

static Exn_struct __one__ = {.digits = NULL,
                             .length = 1,
                             .decimal = 1,
                             .sign = 1,
                             .precision = MAX_DIGITS,
                             .error = EXTENDED_NUM_OK};

static Exn_struct __two__ = {.digits = NULL,
                             .length = 1,
                             .decimal = 1,
                             .sign = 1,
                             .precision = MAX_DIGITS,
                             .error = EXTENDED_NUM_OK};

static Exn_struct __half__ = {.digits = NULL,
                              .length = 1,
                              .decimal = 0,
                              .sign = 1,
                              .precision = MAX_DIGITS,
                              .error = EXTENDED_NUM_OK};

static Exn_struct __threehalfs__ = {.digits = NULL,
                                    .length = 2,
                                    .decimal = 1,
                                    .sign = 1,
                                    .precision = MAX_DIGITS, 
                                    .error = EXTENDED_NUM_OK};

__attribute((constructor)) void init_zero() {
  __zero__.digits = (char *)malloc(__zero__.precision * sizeof(char));
  if (__zero__.digits == NULL) {
    MEM_ALLOC_FAIL(&__zero__.digits,
                   "The extended number zero allocation failed");
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
  __one__.digits = (char *)malloc(__one__.precision * sizeof(char));
  if (__one__.digits == NULL) {
    MEM_ALLOC_FAIL(&__one__.digits,
                   "The extended number one allocation failed");
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
  __two__.digits = (char *)malloc(__two__.precision * sizeof(char));
  if (__two__.digits == NULL) {
    MEM_ALLOC_FAIL(&__two__.digits,
                   "The extended number two allocation failed");
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

__attribute((constructor)) void init_half() {
  __half__.digits = (char *)malloc(__half__.precision * sizeof(char));
  if (__half__.digits == NULL) {
    MEM_ALLOC_FAIL(&__half__.digits,
                   "The extended number half allocation failed");
    return;
  }
  __half__.digits[0] = '5';
  __half__.digits[1] = '\0';
}

__attribute((destructor)) void free_half() {
  if (__half__.digits) {
    free(__half__.digits);
  }
}

__attribute((constructor)) void init_threehalfs() {
  __threehalfs__.digits =
      (char *)malloc(__threehalfs__.precision * sizeof(char));
  if (__threehalfs__.digits == NULL) {
    MEM_ALLOC_FAIL(&__threehalfs__.digits,
                   "The extended number threehalfs allocation failed");
    return;
  }
  __threehalfs__.digits[0] = '1';
  __threehalfs__.digits[1] = '5';
  __threehalfs__.digits[2] = '\0';
}

__attribute((destructor)) void free_threehalfs() {
  if (__threehalfs__.digits) {
    free(__threehalfs__.digits);
  }
}

#define Exn_zero &__zero__             /* Constant Zero */
#define Exn_one &__one__               /* Constant One */
#define Exn_two &__two__               /* Constant Two */
#define Exn_half &__half__             /* Constant Half */
#define Exn_threehalfs &__threehalfs__ /* Constant Three Halfs */

// ################################ Extended Number Functions
// ##############################

const char *Exn_err2str(ExtendedNumError error) {
  switch (error) {
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

// ################################# Extended Number Utils Functions
// ##############################

/**
 * @brief Iterative function for Newton's method to find the inverse
 * @param __n The number of divsor
 * @param __x The initial guess
 * @return Exn The inverse of the number
 * @details The function is:
 *          $x_{n+1} = (2 - n * x_n) * x_n$
 * @note The caller is responsible for releasing the memory
 */
 Exn __ntiter_inv(const Exn __n, const Exn __x) {
  Exn tmp1 = Exn_mul(__n, __x);
  if (!tmp1)
    return NULL;
  Exn tmp2 = Exn_sub(Exn_two, tmp1);
  Exn_release(&tmp1);
  if (!tmp2)
    return NULL;
  Exn __nx = Exn_mul(tmp2, __x);
  Exn_release(&tmp2);
  return __nx;
}

/**
 * @brief Iterative function for Newton's method to find the square root
 * @param __n The number of divsor
 * @param __x The initial guess
 * @return Exn The square root of the number
 * @details The function is:
 *          $x_{n+1} = (3/2 - n / 2 * x_n^2) * x_n$
 * @note The caller is responsible for releasing the memory
 */
 Exn __ntiter_sqrt(const Exn __n, const Exn __x) {
  Exn tmp1 = Exn_mul(__x, __x);
  if (!tmp1)
    return NULL;
  Exn tmp2 = Exn_mul(__n, tmp1);
  Exn_release(&tmp1);
  if (!tmp2)
    return NULL;
  Exn tmp3 = Exn_mul(Exn_half, tmp2);
  if (!tmp3)
    return NULL;
  Exn tmp4 = Exn_sub(Exn_threehalfs, tmp3);
  Exn_release(&tmp2);
  if (!tmp4)
    return NULL;
  Exn __nx = Exn_mul(tmp4, __x);
  Exn_release(&tmp3);
  Exn_release(&tmp4);
  return __nx;
}

/**
 * @brief Fast Fourier Transform
 * @param fa The array of complex numbers
 * @param n The number of complex numbers
 * @param inv The inverse flag(1 for inverse, 0 for forward)
 * @details The function uses butterfly transform to calculate the FFT
 * @see https://oi-wiki.org/math/poly/fft/
 * @attention include libm for cosine and sine functions
 */
void fft(complex *fa, int n, int inv) {
  // Bit-reversal permutation
  for (int i = 1, j = n / 2, k; i < n - 1; i++) {
    if (i < j) { /* swap fa[i] fa[j] */
      complex temp = fa[i];
      fa[i] = fa[j];
      fa[j] = temp;
    }
    k = n / 2;
    while (j >= k) {
      j = j - k;
      k /= 2;
    }
    if (j < k)
      j += k;
  }

  // Cooley-Tukey iterative FFT
  for (int len = 2; len <= n; len <<= 1) {
    double angle = 2.0 * M_PI / len * (inv ? -1 : 1);
    complex wn = cos(angle) + sin(angle) * I;

    for (int j = 0; j < n; j += len) {
      complex w = 1.0 + 0.0 * I;
      for (int k = j; k < j + len / 2; k++) {
        complex u = fa[k];
        complex v = fa[k + len / 2] * w;

        fa[k] = u + v;
        fa[k + len / 2] = u - v;

        w *= wn;
      }
    }
  }

  // Scale if inverse
  if (inv) {
    for (int i = 0; i < n; i++) {
      fa[i] /= n;
    }
  }
}

int *karatsuba(int n, int *a, int *b) {
  if (n <= 1024) {
    int *r = (int *)calloc(n * 2 + 1, sizeof(int));
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= n; ++j) r[i + j] += a[i] * b[j];
    return r;
  }

  int m = n / 2 + 1;
  int *r = (int *)calloc(m * 4 + 1, sizeof(int));
  int *z0, *z1, *z2;

  z0 = karatsuba(m - 1, a, b);
  z2 = karatsuba(n - m, a + m, b + m);

  for (int i = 0; i + m <= n; ++i) a[i] += a[i + m];
  for (int i = 0; i + m <= n; ++i) b[i] += b[i + m];
  z1 = karatsuba(m - 1, a, b);
  for (int i = 0; i + m <= n; ++i) a[i] -= a[i + m];
  for (int i = 0; i + m <= n; ++i) b[i] -= b[i + m];
  for (int i = 0; i <= (m - 1) * 2; ++i) z1[i] -= z0[i];
  for (int i = 0; i <= (n - m) * 2; ++i) z1[i] -= z2[i];

  for (int i = 0; i <= (m - 1) * 2; ++i) r[i] += z0[i];
  for (int i = 0; i <= (m - 1) * 2; ++i) r[i + m] += z1[i];
  for (int i = 0; i <= (n - m) * 2; ++i) r[i + m * 2] += z2[i];

  free(z0);
  free(z1);
  free(z2);
  return r;
}


// ################################ Extended Number Member Functions
// ##############################

Exn Exn_create(int prec) {
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
  num->digits = (char *)calloc(prec + 10, sizeof(char));
  num->precision = prec;

  if (num->digits == NULL) {
    MEM_ALLOC_FAIL(&num->digits,
                   "The extended number digits allocation failed");
    free(num);
    return NULL;
  }

  return num;
}

Exn Exn_rand(int prec) {
  Exn num = Exn_create(prec);
  if (num == NULL)
    return NULL;

  int len = rand() % (prec + 1) + 1; // at least 1 digit
  num->length = len;
  num->decimal = rand() % (len + 1);
  num->sign = rand() % 2 ? 1 : -1;

  for (int i = 0; i < len; i++) {
    num->digits[i] = '0' + rand() % 10;
  }
  num->digits[len] = '\0';

  return num;
}

Exn Exn_randp(int prec, int exp) {
  Exn num = Exn_create(prec);
  if (num == NULL)
    return NULL;

  int len = rand() % (prec + 1) + 1; // at least 1 digit
  num->length = len;
  num->decimal = exp;
  num->sign = rand() % 2 ? 1 : -1;

  for (int i = 0; i < len; i++) {
    num->digits[i] = '0' + rand() % 10;
  }
  num->digits[len] = '\0';

  return num;
}

void Exn_release(Exn *num) {
  if (*num == NULL)
    return;
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
 void Exn_cpy(Exn *dest, const Exn src) {
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
 void Exn_mv(Exn *dest, Exn *src) {
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
 Exn __Exn_fromstr(const char *str, int prec) {
  if (str == NULL || str[0] == '\0')
    return NULL;
  Exn num = Exn_create(prec);
  if (num == NULL)
    return NULL;

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
  if (pos == len || (pos == len - 1 && str[pos] == '.')) {
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

  if (digit_count > prec)
    MATH_PREC_LOSE(prec, digit_count);

  num->length = digit_count;
  Exn_normalize(&num);
  return num;
}

Exn atoExn(const char *str, int prec) {
  if (str == NULL || str[0] == '\0')
    return NULL;

  int len = strlen(str);
  int e_pos = -1; /* pointer to find 'e' or 'E' symbol */

  for (int i = 0; i < len; i++) {
    if (str[i] == 'e' || str[i] == 'E') {
      e_pos = i;
      break;
    }
  }

  if (e_pos == -1)
    return __Exn_fromstr(str, prec);

  /* extract the base */
  char *base_str = (char *)malloc(e_pos + 1);
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
          MATH_CVT_FAIL("str", "ExtendedNumber",
                        "no decimal point in an exponent");
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
  Exn_normalize(&base);
  return base;
}

void Exn_normalize(Exn *num) {
  if (*num == NULL || (*num)->digits == NULL)
    return;

  // Exn_show(*num); //?

  /* get the index of the first non-zero digit */
  int first_non_zero = 0;
  while (first_non_zero < (*num)->length &&
         (*num)->digits[first_non_zero] == '0') {
    first_non_zero++;
  }

  /* is zero */
  if (first_non_zero == (*num)->length) {
    Exn_cpy(num, Exn_zero);
    printf("The number is zero\r\n");
    return;
  }

  /* get the index of the last non-zero digit */
  int last_non_zero = (*num)->length - 1;
  while (last_non_zero >= 0 && (*num)->digits[last_non_zero] == '0') {
    last_non_zero--;
  }

  /* adjust the digits */
  int effective_digits = last_non_zero - first_non_zero + 1;
  if (effective_digits > (*num)->precision) {
    MATH_PREC_LOSE((*num)->precision, effective_digits);
    last_non_zero = first_non_zero + (*num)->precision - 1;
  }

  /* move the digits to eliminate leading zeros */
  if (first_non_zero > 0) {
    for (int i = 0; i <= last_non_zero - first_non_zero; i++) {
      (*num)->digits[i] = (*num)->digits[first_non_zero + i];
    }

    (*num)->decimal -= first_non_zero;
  }

  /* eliminate the trailing zeros */
  for (int i = last_non_zero + 1; i < (*num)->length; i++) {
    (*num)->digits[i] = '\0';
  }

  /* update length (number of valid digit)*/
  (*num)->length = last_non_zero - first_non_zero + 1;
}

void Exn_round( Exn* num, int scale, int type ) {
    if (*num == NULL || (*num)->digits == NULL)
        return;
    
    if (scale < 0) {
        Exn_cpy(num, Exn_zero);
        return;
    }
    
    if (scale > (*num)->precision) {
        scale = (*num)->precision;
    }
    
    if (scale <= (*num)->length) {
        /* round the number */
        int i = (*num)->length;
        (*num)->length = scale;
        for (; i > scale + 1; i--) {
            (*num)->digits[i] = 0;
        }
        int carry = 0;
        switch (type) {
            case 1: /* floor */
                carry = 1;
                break;
            case 0: /* round */
                if ((*num)->digits[scale] >= '5') {
                    carry = 1;
                }
                break;
            case -1: /* ceil */
            default:
                break;
        }
        while (i >= 0 && carry > 0) {
            int digit = (*num)->digits[i] - '0' + carry;
            (*num)->digits[i] = digit % 10 + '0';
            carry = digit / 10;
            i--;
        }
        if (carry > 0) {
            (*num)->sign = 1;
            (*num)->length++;
            (*num)->decimal++;
            for (i = (*num)->length; i > 0; i--) {
                (*num)->digits[i] = (*num)->digits[i - 1];
            }
            (*num)->digits[0] = '1';
        }
        (*num)->digits[(*num)->length] = '\0';
    }

}

void Exn_show(const Exn num) {
  if (num == NULL) {
    fprintf(stdout, "NULL\r\n");
    return;
  }
  fprintf(stdout, "Extended Number at <%p>: {\r\n", num);
  fprintf(stdout, "\tSign: %c\r\n", (num->sign > 0) ? '+' : '-');
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
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
 char *__Exn_fmtn(Exn num) {
  int buf_len = num->precision + num->decimal + 10;
  char *buf = (char *)malloc(buf_len);
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
  } else if (num->decimal >=
             num->length) { /* magnitude >= 1 with trailing zeros*/
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
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
 char *__Exn_fmtp(Exn num) {

  int prec = num->precision;
  char *str = __Exn_fmtn(num);
  if (str == NULL)
    return NULL;
  int len = strlen(str);
  int digit_n = MAX(num->length, num->decimal); /* exact digit number */

  if (prec <= digit_n) { /* no need to add trailing zeros */
    return str;
  } else { /* add trailing zeros */
    int buf_len = prec + 10;
    char *buf = (char *)malloc(buf_len);
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
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
 char *__Exn_fmts(Exn num) {
  int buf_len = num->length + 10;
  char *buf = (char *)malloc(buf_len);
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

  int exp = num->decimal - 1;
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

char *Exn_fmt(Exn num, Exnfmt mode) {
  if (num == NULL)
    return NULL;
  Exn_normalize(&num);
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
 Exn __Exn_addu(const Exn num1, const Exn num2) {

  // align the decimal
  int maxDecimal = MAX(num1->decimal, num2->decimal);
  int minDecimal = MIN(num1->decimal, num2->decimal);
  int diff = maxDecimal - minDecimal; /* decimal different */
  int carry = 0;
  int len = MAX(num1->length, num2->length) + diff + 1; // +1 for carry
  int prec = MAX(num1->precision, num2->precision);
  char *align_num1 = (char *)malloc(len);
  char *align_num2 = (char *)malloc(len);
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
 Exn __Exn_subu(const Exn num1, const Exn num2) {

  // align the decimal
  int maxDecimal = MAX(num1->decimal, num2->decimal);
  int minDecimal = MIN(num1->decimal, num2->decimal);
  int diff = maxDecimal - minDecimal; /* decimal different */
  int borrow = 0;
  int len = MAX(num1->length, num2->length) + diff + 1;
  Exn result = Exn_create(len);
  int i = len - 1;

  /* align decimal */
  char *align_num1 = (char *)malloc(len);
  char *align_num2 = (char *)malloc(len);
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
  return result;
}

Exn Exn_abs(const Exn num) {
  Exn result = NULL;
  Exn_cpy(&result, num);
  result->sign = 1;
  return result;
}

Exn Exn_shift(const Exn num, int shift) {
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
  return result;
}

bool Exn_iszero(const Exn num) { return num->sign == 0; }
bool Exn_isneg(const Exn num) { return num->sign < 0; }

Exn Exn_add(const Exn num1, const Exn num2) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    printf("num1: %s\r\n", Exn_err2str(num1->error));
    printf("num2: %s\r\n", Exn_err2str(num2->error));
    return NULL;
  }
  int sign = num1->sign * num2->sign;
  Exn result = NULL;

  if (sign > 0) {
    result = __Exn_addu(num1, num2);
    result->sign = num1->sign;
  } else if (sign == 0) {
    if (Exn_iszero(num1)) {
      Exn_cpy(&result, num2);
    } else { /* num2 is zero */
      Exn_cpy(&result, num1);
    }
  } else /* sign < 0 */
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
  Exn_normalize(&result);
  return result;
}

Exn Exn_sub(const Exn num1, const Exn num2) {
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
  } else if (sign == 0) {
    if (Exn_iszero(num1)) {
      Exn_cpy(&result, num2);
      result->sign = -num2->sign;
    } else { /* num2 is zero */
      Exn_cpy(&result, num1);
    }
  } else /* sign < 0 */
  {
    Exn num1_ = Exn_abs(num1);
    Exn num2_ = Exn_abs(num2);
    result = __Exn_addu(num1_, num2_);
    result->sign = num1->sign;
    Exn_release(&num1_);
    Exn_release(&num2_);
  }
  Exn_normalize(&result);
  return result;
}

/**
 * @brief Multiply two extended numbers
 * @details This function uses the naive multiplication algorithm with O(n^2)
 * complexity
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
Exn __Exn_mul_pl(const Exn num1, const Exn num2, int len, int sign) {
  Exn result = Exn_create(len);
  if (result == NULL) {
    MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
    return NULL;
  }
  /* initialize the result */
  memset(result->digits, 0, len);

  /* use naive multiplication */
  /* result[i+j] = num1[i] * num2[j] + carry */
  for (int i = num1->length - 1; i >= 0; i--) {
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

/**
 * @brief Multiply two extended numbers using Karatsuba algorithm
 * @details This function uses the Karatsuba algorithm with O(n^log2(3))
 * complexity
 * @param num1 The first number
 * @param num2 The second number
  * @param len num1.length + num2.length
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
Exn __Exn_mul_karatsuba(const Exn num1, const Exn num2, int len, int sign) {
  Exn result = NULL;
  /* use karatsuba multiplication */
  /**
   *          A    B
   *  x       C    D
   * ----------------
   *    AC  AD+BC  BD
   *
   * AD + BC = (A + B)(C + D) - AC - BD
   *
   *      4 recursive calls       */
   int n = MAX(num1->length, num2->length);
   result = Exn_create(len);
  if (result == NULL) {
    MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
    return NULL;
  }

  int* a = (int*)calloc(sizeof(int), n);
  int* b = (int*)calloc(sizeof(int), n);

  /* inverse the number */
  for (int i = 0; i < num1->length; i++) {
    a[i] = Ch2val(num1->digits[num1->length - 1 - i]);
  }
  for (int i = 0; i < num2->length; i++) {
    b[i] = Ch2val(num2->digits[num2->length - 1 - i]);
  }

  int* r = karatsuba(n - 1, a, b);
  
  int car = 0;
  for (int i = 0; i < 2 * n - 1; i++) {
    int sum = r[i] + car;
    r[i] = sum % 10;
    car = sum / 10;
    if (r[i] == 0 && car == 0) {
      result->length = i;
      break;
    } else {
      result->length = i + 1;
    }
  }

  int i = 0;
  if (car > 0) {
    result->digits[i] = car + '0';
    i++;
  }

  /* inverse the result */
  for (int j = result->length - 1; j >= 0; j--) {
    result->digits[i] = r[j] + '0';
    i++;
  }

  // Exn_show(result); //?
  
  free(a);
  free(b);
  free(r);  
  result->sign = sign;
  result->decimal = num1->decimal + num2->decimal - (len != result->length);
  result->precision = MAX(num1->precision, num2->precision);

  return result;
}

/**
 * @brief Multiply two extended numbers using FFT
 * @details This function uses the FFT algorithm with O(nlogn^2) complexity
 * @param num1 The first number
 * @param num2 The second number
 * @return Exn The result of the multiplication
 * @note The caller is responsible for releasing the memory
 * @attention This function won't check the validity of the arguments, since
 * it's an internal function
 */
Exn __Exn_mul_fft(const Exn num1, const Exn num2, int len, int sign) {
  /* calc the power of 2 */
  int size = 1;
  while (size < num1->length * 2 || size < num2->length * 2) {
    size <<= 1;
  }

  Exn result = Exn_create(size);
  if (result == NULL) {
    MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
    return NULL;
  }

  complex *fa = (complex *)calloc(size, sizeof(complex));
  complex *fb = (complex *)calloc(size, sizeof(complex));

  if (!fa || !fb) {
    MEM_ALLOC_FAIL(&fa, "The extended number allocation failed");
    MEM_ALLOC_FAIL(&fb, "The extended number allocation failed");
    free(fa);
    free(fb);
    Exn_release(&result);
    return NULL;
  }


  #ifdef __cplusplus
  for (int i = num1->length - 1; i >= 0; i--) {
    fa[num1->length - 1 - i] = Ch2val(num1->digits[i]);
  }
  for (int i = num2->length - 1; i >= 0; i--) {
    fb[num2->length - 1 - i] = Ch2val(num2->digits[i]);
  }

  #else
  for (int i = num1->length - 1; i >= 0; i--) {
    fa[num1->length - 1 - i] = Ch2val(num1->digits[i]) + 0.0 * I;
  }
  for (int i = num2->length - 1; i >= 0; i--) {
    fb[num2->length - 1 - i] = Ch2val(num2->digits[i]) + 0.0 * I;
  }
  #endif
  // //?
  // for (int i = 0; i < size; i++) {
  //     printf("%f + %fi\n", creal(fa[i]), cimag(fa[i])); //?
  // }
  // //?
  // for (int i = 0; i < size; i++) {
  //     printf("%f + %fi\n", creal(fb[i]), cimag(fb[i])); //?
  // }

  /* FFT */
  fft(fa, size, false);
  fft(fb, size, false);

  /* multiply */
  for (int i = 0; i < size; i++) {
    fa[i] *= fb[i];
  }

  // //?
  // for (int i = 0; i < size; i++) {
  //     printf("%f + %fi\n", creal(fa[i]), cimag(fa[i])); //?
  // }

  /* IFFT */
  fft(fa, size, true);

  // //?
  // for (int i = 0; i < size; i++) {
  //     printf("%f + %fi\n", creal(fa[i]), cimag(fa[i])); //?
  // }

  complex *res = fa;
  int car = 0;
  int result_dec = 0;

  for (int i = 0; i < size; i++) {
    int sum = round(creal(res[i])) + car;
    result->digits[i] = (sum % 10) + '0';
    car = sum / 10;
    if (result->digits[i] != '0') {
      result_dec = i;
    }
  }
  // printf("dec: %d\r\n", result_dec + 1); //?

  result->length = size;

  // //?
  // for (int i = 0; i < size; i++) {
  //     printf("prod: %c", result->digits[i]); //?
  // }
  // printf("\n"); //?

  result->decimal = num1->decimal + num2->decimal - (len != result_dec + 1);
  // printf("decimal: %d\r\n", result->decimal); //?
  result->precision = MAX(num1->precision, num2->precision);
  result->sign = sign;
  free(fa);
  free(fb);

  // invert the digits
  for (int i = 0; i < result->length / 2; i++) {
    char temp = result->digits[i];
    result->digits[i] = result->digits[result->length - 1 - i];
    result->digits[result->length - 1 - i] = temp;
  }
  // remove leading zeros
  int first_non_zero = size - result_dec - 1;
  // printf("first_non_zero: %d\r\n", first_non_zero); //?
  for (int i = 0; i < result->length - first_non_zero; i++) {
    result->digits[i] = result->digits[first_non_zero + i];
  }
  result->length -= first_non_zero;

  return result;
}

Exn Exn_mul(const Exn num1, const Exn num2) {
  if (!num1 || !num2 || !num1->digits || !num2->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num1->error != EXTENDED_NUM_OK || num2->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    printf("num1: %s\r\n", Exn_err2str(num1->error));
    printf("num2: %s\r\n", Exn_err2str(num2->error));
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
  if (len / 2 < MUL_OPTIMIZE_LEAST) {
    result = __Exn_mul_pl(num1, num2, len, sign);
  } else /* len >= 1000 */
  {
    result = __Exn_mul_fft(num1, num2, len, sign);
  }

  Exn_normalize(&result);
  return result;
}

Exn Exn_div(const Exn num1, const Exn num2) {
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
  int len = MAX(num1->length, num2->length) * 2;
  int prec = MAX(num1->precision, num2->precision);
  int bound = prec * 2 + 1; /* the bound of the iteration */

  /* calc the inverse of divisor */
  Exn inv_num2 = Exn_shift(Exn_one, -num2->decimal);
  Exn_newton(__ntiter_inv, bound, num2, &inv_num2);

  result = Exn_mul(num1, inv_num2);
  Exn_release(&inv_num2);
  if (result) {
    Exn_round(&result, len, 0);
  }

  return result;
}

Exn Exn_divmod(const Exn num1, const Exn num2, Exn *mod) {
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
  // int len = MAX(num1->length, num2->length) * 2 + 1;
  // int prec = MAX(num1->precision, num2->precision);
  int bound = MAX_DIGITS / 2; /* the bound of the iteration */

  /* calc the inverse of divisor */
  Exn inv_num2 = Exn_shift(Exn_one, -num2->decimal);
  Exn_newton(__ntiter_inv, bound, num2, &inv_num2);

  result = Exn_mul(num1, inv_num2);
  Exn_release(&inv_num2);

  if (result) {
    Exn_round(&result, result->decimal, 0);
    /* calc the mod */
    Exn temp = Exn_mul(result, num2);
    *mod = Exn_sub(num1, temp);
    Exn_release(&temp);
    (*mod)->precision = MAX(num1->precision, num2->precision);
    Exn_normalize(mod);
  }
  return result;
}

Exn Exn_modulo(const Exn num1, const Exn num2) {

  Exn result = NULL;
  Exn mod = NULL;
  result = Exn_divmod(num1, num2, &mod);
  Exn_release(&result);
  return mod;
}

Exn Exn_sqrt(const Exn num) {
  if (!num || !num->digits) {
    INPUT_ARG_FAIL("operand", "The operand can't be NULL");
    return NULL;
  }
  if (num->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return NULL;
  }
  Exn result = NULL;
  if (Exn_iszero(num)) {
    Exn_cpy(&result, Exn_zero);
    return result;
  }
  int len = num->length * 2 + 1;
  int prec = MAX(num->precision, num->decimal);
  int bound = MAX_DIGITS / 2; /* the bound of the iteration */

  /* select initial value */
  int shift = -num->decimal / 2;
  if (num->decimal % 2 != 0) {
    shift = -num->decimal / 2 - 1;
  }
  result = Exn_shift(Exn_one, shift);
  Exn_newton(__ntiter_sqrt, bound, num, &result);
  Exn_round(&result, len, -1);
  // Exn_show(result); //?
  
  /* invert \frac{1}{\sqrt{x}} */
  Exn inv = Exn_shift(Exn_one, -result->decimal);
  Exn_newton(__ntiter_inv, prec, result, &inv);
    Exn_release(&result);
    // inv->decimal = num->decimal / 2;
    Exn_round(&inv, len, 0);
  return inv;
}

Exn fact( int n ) {
    size_t u_bound = (size_t)ceil(((n + 0.5)*log10(n + 1) 
    - n/log(10) + log10(2 * acos(-1) + 1)));

    Exn result = Exn_create(u_bound);
    if (result == NULL) {
        MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
        return NULL;
    }

    result->digits[0] = '1';
    result->sign = 1;
    result->length = 1;
    result->decimal = 1;
    Exn cnt = Exn_shift(Exn_one, 0);

    for (int i = 1; i < n; i++) {
        Exn add = Exn_add(cnt, Exn_one);
        Exn tmp = Exn_mul(result, add);
        Exn_mv(&result, &tmp);
        Exn_mv(&cnt, &add);
    }
    Exn_release(&cnt);
    return result;
}

Exn ksm(Exn num, int exp) {
    Exn result = Exn_shift(Exn_one, 0);
    result->precision = num->precision;
    if (result == NULL) {
        MEM_ALLOC_FAIL(&result, "The extended number allocation failed");
        return NULL;
    }
    Exn a = Exn_shift(num, 0);
    while (exp > 0) {
        Exn tmp = NULL;
        if (exp & 0x1) {
            Exn tmp_ = Exn_mul(result, a);
            Exn_mv(&result, &tmp_);
        }
        tmp = Exn_mul(a, a);
        Exn_mv(&a, &tmp);
        exp >>= 1;
    }
    Exn_release(&a);
    Exn_normalize(&result);
    return result;
}

void Exn_newton(Exn (*func)(Exn, Exn), int bound, Exn param, Exn *result) {
  while ((*result)->length < bound) {
    Exn temp = func(param, *result);
    if (!temp || temp->error != EXTENDED_NUM_OK) {
      Exn_release(&temp);
      break;
    }
    Exn_mv(result, &temp);
  }
  Exn_normalize(result);
}

int Exn_toInt(const Exn num) {
  if (num == NULL) {
    MEM_ACCESS_NULLPTR("num", "The extended number can't be NULL");
    return 0;
  }
  if (num->error != EXTENDED_NUM_OK) {
    INPUT_ARG_FAIL("operand", "The operand has an error");
    return 0;
  }
  int result = 0;
  if (num->decimal <= 0) {
    result = 0;
  } else {
    int dec = num->decimal - 1;
    for (int i = 0; i < num->length && dec >= 0; i++) {
        result += Ch2val(num->digits[i]) * pow(10, dec);
        dec--;
    }
  }
  return result * num->sign;
}

int Exn_cmp(const Exn num1, const Exn num2) {
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