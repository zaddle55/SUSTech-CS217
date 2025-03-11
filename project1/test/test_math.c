#include "../math.h"
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

// TEST(fromstring_zeros)
// {
//     Exn num = atoExn("0.000000", 10);
//     if (num == NULL) {
//         fprintf(stderr, "Failed to convert string to extended number\n");
//         return;
//     }
//     Exn_show(num);
//     Exn_release(&num);
// }


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

TEST(mul_rand_speed)
{
    Exn num1 = Exn_rand(1000);
    Exn num2 = Exn_rand(1000);
    TIME_START
    Exn num3 = Exn_mul(num1, num2);
    TIME_END
    Exn_release(&num1);
    Exn_release(&num2);
    Exn_release(&num3);
}

TEST(div_basic)
{
    Exn divend = atoExn("2", 100);
    Exn div = atoExn("3", 100);
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

int main()
{
    run_all_tests();
    return 0;
}