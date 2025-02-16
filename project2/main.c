#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

void test_multiply_correctness()
{
    Mat* mat1 = initMat( 2, 3 );
    mat1->data[0][0] = 1;
    mat1->data[0][1] = 2;
    mat1->data[0][2] = 3;
    mat1->data[1][0] = 4;
    mat1->data[1][1] = 5;
    mat1->data[1][2] = 6;

    printf( "mat1:\n" );
    printMat( mat1 );

    Mat* mat2 = initMat( 3, 2 );
    mat2->data[0][0] = 7;
    mat2->data[0][1] = 8;
    mat2->data[1][0] = 9;
    mat2->data[1][1] = 10;
    mat2->data[2][0] = 11;
    mat2->data[2][1] = 12;
    printf( "mat2:\n" );
    printMat( mat2 );

    Mat* result = initMat( 2, 2 );

    mulMat( result, mat1, mat2 );
    printf( "result:\n" );
    printMat( result );

    Mat* expected = initMat( 2, 2 );
    expected->data[0][0] = 58;
    expected->data[0][1] = 64;
    expected->data[1][0] = 139;
    expected->data[1][1] = 154;

    printf( "expected:\n" );
    printMat( expected );

    if ( cmpMat( result, expected ) == 0 )
    {
        printf( "test_multiply_correctness ends successfully\n" );
    }
    else
    {
        printf( "test_multiply_correctness failed\n" );
    }

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_128()
{
    Mat* mat1 = randMat( 128, 128 );
    Mat* mat2 = randMat( 128, 128 );
    Mat* result = initMat( 128, 128 );

    clock_t start = clock();
    mulMat( result, mat1, mat2 );
    clock_t end = clock();

    printf( "Time elapsed: %f seconds\n", (double)( end - start ) / CLOCKS_PER_SEC );
    printf( "test_multiply_speed_128 ends successfully\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_512()
{
    Mat* mat1 = randMat( 512, 512 );
    Mat* mat2 = randMat( 512, 512 );
    Mat* result = initMat( 512, 512 );

    clock_t start = clock();
    mulMat( result, mat1, mat2 );
    clock_t end = clock();

    printf( "Time elapsed: %f seconds\n", (double)( end - start ) / CLOCKS_PER_SEC );
    printf( "test_multiply_speed_512 ends successfully\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_1024()
{
    Mat* mat1 = randMat( 1024, 1024 );
    Mat* mat2 = randMat( 1024, 1024 );
    Mat* result = initMat( 1024, 1024 );

    clock_t start = clock();
    mulMat( result, mat1, mat2 );
    clock_t end = clock();

    printf( "Time elapsed: %f seconds\n", (double)( end - start ) / CLOCKS_PER_SEC );
    printf( "test_multiply_speed_1024 ends successfully\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_8k()
{
    Mat* mat1 = randMat( 8000, 8000 );
    Mat* mat2 = randMat( 8000, 8000 );
    Mat* result = initMat( 8000, 8000 );

    clock_t start = clock();
    mulMat( result, mat1, mat2 );
    clock_t end = clock();

    printf( "Time elapsed: %f seconds\n", (double)( end - start ) / CLOCKS_PER_SEC );
    printf( "test_multiply_speed_8k ends successfully\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_64k()
{
    Mat* mat1 = randMat( 64000, 64000 );
    Mat* mat2 = randMat( 64000, 64000 );
    Mat* result = initMat( 64000, 64000 );

    clock_t start = clock();
    mulMat( result, mat1, mat2 );
    clock_t end = clock();

    printf( "Time elapsed: %f seconds\n", (double)( end - start ) / CLOCKS_PER_SEC );
    printf( "test_multiply_speed_64k ends successfully\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_100t()
{
    double time_sum_ = 0;
    int test_n_ = 100;
    for (int i = 0; i < test_n_; i++)
    {
        Mat* mat1 = randMat( 100, 100 );
        Mat* mat2 = randMat( 100, 100 );
        Mat* result = initMat( 100, 100 );

        clock_t start = clock();
        mulMat( result, mat1, mat2 );
        clock_t end = clock();

        time_sum_ += (double)( end - start ) / CLOCKS_PER_SEC;

        freeMat( mat1 );
        freeMat( mat2 );
        freeMat( result );
    }
    
    printf( "Average time elapsed: %f seconds in %d turns\n", time_sum_ / 10, test_n_);
    printf( "test_multiply_speed2 ends successfully\n" );
}

int main()
{
    test_multiply_correctness();
    test_multiply_speed_128();
    test_multiply_speed_512();
    test_multiply_speed_1024();
    // test_multiply_speed_8k();
    test_multiply_speed_100t();
    printf( "All tests end\n" );

    return 0;
}