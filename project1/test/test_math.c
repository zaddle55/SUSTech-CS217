#include "math.h"
#include "util.h"
#include <time.h>
#define TIME_START clock_t start = clock();
#define TIME_END clock_t end = clock(); \
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC; \
    fprintf(stdout, "Time spent: %f seconds\r\n", time_spent);

TEST(fromstring_basic)
{
    Exn num = atoExn("0.123456", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(fromstring_sci)
{
    Exn num = atoExn("543.45E-16", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(fromstring_overflow)
{
    Exn num = atoExn("987.654321E16", 5);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(fromstring_overflow_advance)
{
    Exn num = atoExn("9876543.21E16", 5);
    Exn num2 = atoExn("9.87654321E22", 5);
    if (num == NULL || num2 == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    Exn_show(num);
    Exn_show(num2);
    Exn_release(&num2);
    Exn_release(&num);
}

TEST(fromstring_fit)
{
    Exn num = atoExn("9876543.21E16", 9);
    Exn num2 = atoExn("9.87654321E22", 9);
    if (num == NULL || num2 == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    Exn_show(num);
    Exn_show(num2);
    Exn_release(&num2);
    Exn_release(&num);
}

TEST(fromstring_base_nodigit)
{
    Exn num = atoExn("-5&42.", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(fromstring_exp_nodigit)
{
    Exn num = atoExn("2.E--3", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(fromstring_multipledecimal)
{
    Exn num = atoExn("1.2.E-1", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(fromstring_decimal_in_exp)
{
    Exn num = atoExn("1.2E-1.2", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    char str[100];
    Exn_show(num);
    Exn_release(&num);
}

TEST(normalize_basic)
{
    /* leading zeros vanishment */
    Exn num1 = atoExn("00070.", 10);

    /* trailing zeros vanishment */
    Exn num2 = atoExn("123.7000", 10);

    /* both leading and trailing zeros vanishment */
    Exn num3 = atoExn("000123.7040", 10);

    Exn_show(num1);
    Exn_show(num2);
    Exn_show(num3);

    Exn_release(&num1);
    Exn_release(&num2);
    Exn_release(&num3);
}

TEST(fromstring_zeros)
{
    Exn num = atoExn("0.000000", 10);
    if (num == NULL) {
        fprintf(stderr, "Failed to convert string to extended number\n");
        return;
    }
    Exn_show(num);
    Exn_release(&num);
}


TEST(fmt_comprehensive)
{
    Exn num = atoExn("-70.45E-1", 4);
    char* num_n = Exn_fmt(num, EXN_FMT_NORMAL);
    char* num_p = Exn_fmt(num, EXN_FMT_PRECISE);
    char* num_s = Exn_fmt(num, EXN_FMT_SCIENTIFIC);

    assert(strcmp(num_n, "-7.045") == 0);
    assert(strcmp(num_p, "-7.045") == 0);
    fprintf(stdout, "Formatted number: %s\n", num_s);
    assert(strcmp(num_s, "-7.045E") == 0);

    Exn_release(&num);
    free(num_n);
    free(num_p);
    free(num_s);
}

TEST(fmt_add)
{
    Exn num1 = atoExn("70.45", 4);
    Exn num2 = atoExn("0.45", 4);
    Exn num3 = Exn_add(num1, num2);
    char* num_n = Exn_fmt(num3, EXN_FMT_NORMAL);
    char* num_p = Exn_fmt(num3, EXN_FMT_PRECISE);
    char* num_s = Exn_fmt(num3, EXN_FMT_SCIENTIFIC);

    fprintf(stdout, "Formatted number: %s\n", num_n);
    assert(strcmp(num_n, "70.9") == 0);
    assert(strcmp(num_p, "70.90") == 0);
    fprintf(stdout, "Formatted number: %s\n", num_s);
    assert(strcmp(num_s, "7.09E1") == 0);

    Exn_release(&num1);
    Exn_release(&num2);
    Exn_release(&num3);
    free(num_n);
    free(num_p);
    free(num_s);
}

TEST(add_basic)
{
    Exn na1 = atoExn("90.30", 20);
    Exn na2 = atoExn("-0.909", 20);
    Exn sum1 = Exn_add(na1, na2);
    Exn_show(sum1);
    char* sum1_s = Exn_fmt(sum1, EXN_FMT_SCIENTIFIC);
    assert(strcmp(sum1_s, "8.9391E1") == 0);
    free(sum1_s);
    Exn_release(&na1);
    Exn_release(&na2);
    Exn_release(&sum1);
}

TEST(add_both_sci)
{
    Exn na1 = atoExn("9123.30E2", 20);
    Exn na2 = atoExn("-0.909E-2", 20);
    Exn sum1 = Exn_add(na1, na2);
    Exn_show(sum1);
    char* sum1_s = Exn_fmt(sum1, EXN_FMT_SCIENTIFIC);
    assert(strcmp(sum1_s, "9.1232999091E5") == 0);
    free(sum1_s);
    Exn_release(&na1);
    Exn_release(&na2);
    Exn_release(&sum1);
}

TEST(add_no_overlap)
{
    Exn na1 = atoExn("987654321.", 20);
    Exn na2 = atoExn("0.123456789", 20);
    Exn sum1 = Exn_add(na1, na2);
    Exn_show(sum1);
    char* sum1_s = Exn_fmt(sum1, EXN_FMT_NORMAL);
    assert(strcmp(sum1_s, "987654321.123456789") == 0);
    free(sum1_s);
    Exn_release(&na1);
    Exn_release(&na2);
    Exn_release(&sum1);
}

TEST(add_one_zero)
{
    srand(time(NULL));
    Exn na1 = atoExn("0.123456789", 20);
    Exn na2 = atoExn("0.", 20);
    Exn sum1 = Exn_add(na1, na2);
    Exn_show(sum1);
    char* sum1_s = Exn_fmt(sum1, EXN_FMT_NORMAL);
    assert(strcmp(sum1_s, "0.123456789") == 0);
    free(sum1_s);
    Exn_release(&na1);
    Exn_release(&na2);
    Exn_release(&sum1);
}

TEST(add_both_neg)
{
    Exn na1 = atoExn("-7.24", 20);
    Exn na2 = atoExn("-15.46E-1", 20);
    Exn sum1 = Exn_add(na1, na2);
    Exn_show(sum1);
    char* sum1_s = Exn_fmt(sum1, EXN_FMT_NORMAL);
    assert(strcmp(sum1_s, "-8.786") == 0);
    free(sum1_s);
    Exn_release(&na1);
    Exn_release(&na2);
    Exn_release(&sum1);
}

TEST(release_nullptr)
{
    Exn num = NULL;
    Exn_release(&num);
    assert(num == NULL);
}

// TEST(mul_fft_basic)
// {
//     Exn num1 = atoExn("123456", 100);
//     Exn num2 = atoExn("654321", 100);
//     Exn num3 = Exn_mul(num1, num2);
//     char* num3_s = Exn_fmt(num3, EXN_FMT_NORMAL);
//     Exn_show(num3);
//     fprintf(stdout, "Formatted number: %s\n", num3_s);
//     assert(strcmp(num3_s, "80779853376") == 0);
//     free(num3_s);
//     Exn_release(&num1);
//     Exn_release(&num2);
//     Exn_release(&num3);
// }

// TEST(mul_fft_sci)
// {
//     Exn num1 = atoExn("123456E2", 100);
//     Exn num2 = atoExn("6.54321E-4", 100);
//     Exn num3 = Exn_mul(num1, num2);
//     char* num3_s = Exn_fmt(num3, EXN_FMT_SCIENTIFIC);
//     Exn_show(num3);
//     fprintf(stdout, "Formatted number: %s\n", num3_s);
//     assert(strcmp(num3_s, "8.0779853376E3") == 0);
//     free(num3_s);
//     Exn_release(&num1);
//     Exn_release(&num2);
//     Exn_release(&num3);
// }

// TEST(mul_fft_no_carry)
// {
//     Exn num1 = atoExn("123456", 100);
//     Exn num2 = atoExn("111111", 100);
//     Exn num3 = Exn_mul(num1, num2);
//     char* num3_s = Exn_fmt(num3, EXN_FMT_NORMAL);
//     Exn_show(num3);
//     fprintf(stdout, "Formatted number: %s\n", num3_s);
//     assert(strcmp(num3_s, "13717319616") == 0);
//     free(num3_s);
//     Exn_release(&num1);
//     Exn_release(&num2);
//     Exn_release(&num3);
// }

TEST(karatsuba_basic)
{
    Exn num1 = atoExn("12345", 100);
    Exn num2 = atoExn("65432", 100);
    Exn num3 = __Exn_mul_karatsuba(num1, num2, num1->length + num2->length, 
        num1->sign * num2->sign);
    char* num3_s = Exn_fmt(num3, EXN_FMT_NORMAL);
    Exn_show(num3);
    fprintf(stdout, "Formatted number: %s\n", num3_s);
    // assert(strcmp(num3_s, "80779853376") == 0);
    free(num3_s);
    Exn_release(&num1);
    Exn_release(&num2);
    Exn_release(&num3);
}

TEST(karatsuba_speed)
{
    Exn num1 = Exn_rand(100000);
    Exn num2 = Exn_rand(100000);
    TIME_START
    Exn num3 = __Exn_mul_karatsuba(num1, num2, num1->length + num2->length, 
        num1->sign * num2->sign);
    TIME_END
    Exn_release(&num1);
    Exn_release(&num2);
    Exn_release(&num3);
}

// TEST(mul_rand_speed)
// {
//     Exn num1 = Exn_rand(100000);
//     Exn num2 = Exn_rand(100000);
//     TIME_START
//     Exn num3 = Exn_mul(num1, num2);
//     TIME_END
//     Exn_release(&num1);
//     Exn_release(&num2);
//     Exn_release(&num3);
// }

// TEST(binopexpr_build)
// {
//     Exn num1 = atoExn("123456", 100);
//     Exn num2 = atoExn("654321", 100);
//     BinOprExpr expr;
//     BinOprExpr_build(&expr, '/', num1, num2);
//     printf("num1: ");
//     Exn_show(expr.num1);
//     printf("num2: ");
//     Exn_show(expr.num2);
//     printf("op: %c\n", expr.binop);
//     // clean
//     Exn_release(&num1);
//     Exn_release(&num2);
// }

// TEST(binop_expr_parse)
// {
//     char in[] = "123456 + 654321";
//     MathExpr expr;
//     MathExpr_build(in, &expr, 100);
//     BinOprExpr* bin_op = (BinOprExpr*)expr.expr.binop;
//     printf("num1: ");
//     Exn_show(bin_op->num1);
//     printf("num2: ");
//     Exn_show(bin_op->num2);
//     // clean
//     MathExpr* ptr = &expr;
//     MathExpr_release(ptr);
// }

// TEST(func_expr_parse)
// {
//     char in[] = "fact(123456E19, 16)";
//     MathExpr expr;
//     if (MathExpr_build(in, &expr, 100) == 0){
//     MathFunc* func = (MathFunc*)expr.expr.func;
//     printf("func: %s\n", func->alias);
//     printf("argc: %d\n", func->argc);
//     printf("num1: ");
//     Exn_show(func->args[1]);}
//     // clean
//     MathExpr* ptr = &expr;
//     MathExpr_release(ptr);
// }

// TEST(expr_eval)
// {
//     char in[] = "3.24 / 4.56";
//     MathExpr expr;
//     if (MathExpr_build(in, &expr, 100) == 0) {
//         Exn res = MathExpr_eval(&expr);
//         if (res != NULL) {
//             Exn_show(res);
//             char* res_s = Exn_fmt(res, EXN_FMT_NORMAL);
//             printf("Formatted number: %s\n", res_s);
//             free(res_s);
//             Exn_release(&res);
//         } else {
//             fprintf(stderr, "Failed to evaluate expression\n");
//         }
//         MathExpr_release(&expr);
//     }
//     else {
//         fprintf(stderr, "Failed to build expression\n");
//     }
// }

TEST(div_basic)
{
    Exn divend = atoExn("25565", 100);
    Exn div = atoExn("2", 100);
    Exn div_res = Exn_div(divend, div);
    Exn_show(div_res);
    char* div_res_s = Exn_fmt(div_res, EXN_FMT_NORMAL);
    printf("Formatted number: %s\n", div_res_s);
    Exn_release(&divend);
    Exn_release(&div);
    Exn_release(&div_res);

    free(div_res_s);
}
// TEST(copy)
// {
//     Exn num1 = atoExn("70.45", 4);
//     Exn num2 = NULL;
//     Exn_cpy(&num2, num1);
//     char* num_n = Exn_fmt(num2, EXN_FMT_NORMAL);


//     fprintf(stdout, "Formatted number: %s\n", num_n);
//     assert(strcmp(num_n, "70.45") == 0);


//     Exn_release(&num1);
//     Exn_release(&num2);

// }

TEST(divmod_basic)
{
    Exn divend = atoExn("99", 100);
    Exn div = atoExn("3", 100);
    Exn mod = NULL;
    Exn div_res = Exn_divmod(divend, div, &mod);
    Exn_show(div_res);
    char* div_res_s = Exn_fmt(div_res, EXN_FMT_NORMAL);
    printf("Formatted number: %s\n", div_res_s);
    Exn_show(mod);
    char* mod_s = Exn_fmt(mod, EXN_FMT_NORMAL);
    printf("Formatted number: %s\n", mod_s);

    Exn_release(&divend);
    Exn_release(&div);
    Exn_release(&div_res);
    Exn_release(&mod);

    free(div_res_s);
    free(mod_s);
}

TEST(sqrt_basic)
{
    Exn num = atoExn("2", 100);
    Exn sqrt_res = Exn_sqrt(num);
    Exn_show(sqrt_res);
    char* sqrt_res_s = Exn_fmt(sqrt_res, EXN_FMT_NORMAL);
    printf("Formatted number: %s\n", sqrt_res_s);

    Exn_release(&num);
    Exn_release(&sqrt_res);

    free(sqrt_res_s);
}

TEST(ksm_basic) {
    Exn num = atoExn("2", 1000);
    Exn ksm_res = ksm(num, 10);
    Exn_show(ksm_res);
    char* ksm_res_s = Exn_fmt(ksm_res, EXN_FMT_NORMAL);
    printf("Formatted number: %s\n", ksm_res_s);

    Exn_release(&num);
    Exn_release(&ksm_res);

    free(ksm_res_s);
}

// TEST(ksm_speed) {
//     Exn num = atoExn("2", 100000000);
//     TIME_START
//     Exn ksm_res = ksm(num, 1000000);
//     TIME_END
//     Exn_release(&num);
//     Exn_release(&ksm_res);
// }

TEST(Exn_toInt_basic) 
{
    Exn num = atoExn("123456789", 100);
    int int_res = Exn_toInt(num);
    printf("Integer result: %d\n", int_res);
    assert(int_res == 123456789);

    Exn_release(&num);
}

TEST(Exn_toInt_neg) 
{
    Exn num = atoExn("-123456789", 100);
    int int_res = Exn_toInt(num);
    printf("Integer result: %d\n", int_res);
    assert(int_res == -123456789);

    Exn_release(&num);
}

TEST(fact_basic)
{
    Exn num = atoExn("10", 100);
    Exn fact_res = fact(Exn_toInt(num));
    Exn_show(fact_res);
    char* fact_res_s = Exn_fmt(fact_res, EXN_FMT_NORMAL);
    printf("Formatted number: %s\n", fact_res_s);

    Exn_release(&num);
    Exn_release(&fact_res);

    free(fact_res_s);
}

int main()
{
    run_all_tests();
    return 0;
}