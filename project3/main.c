#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrix.h"

void test_multiply_correctness()
{
    Mat* mat1 = initMat( 2, 3 );
    setMat( mat1, 0, 0, 1 );
    setMat( mat1, 0, 1, 2 );
    setMat( mat1, 0, 2, 3 );
    setMat( mat1, 1, 0, 4 );
    setMat( mat1, 1, 1, 5 );
    setMat( mat1, 1, 2, 6 );

    printf( "mat1:\n" );
    printMat( mat1 );

    Mat* mat2 = initMat( 3, 2 );
    setMat( mat2, 0, 0, 7 );
    setMat( mat2, 0, 1, 8 );
    setMat( mat2, 1, 0, 9 );
    setMat( mat2, 1, 1, 10 );
    setMat( mat2, 2, 0, 11 );
    setMat( mat2, 2, 1, 12 );

    printf( "mat2:\n" );
    printMat( mat2 );

    Mat* result = initMat( 2, 2 );

    mulMat( result, mat1, mat2 );
    printf( "result:\n" );
    printMat( result );

    Mat* expected = initMat( 2, 2 );
    setMat( expected, 0, 0, 58 );
    setMat( expected, 0, 1, 64 );
    setMat( expected, 1, 0, 139 );
    setMat( expected, 1, 1, 154 );

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
    printf( "\n" );

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
    printf( "\n" );

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
    printf( "\n" );

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
    printf( "\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

void test_multiply_speed_2048()
{
    Mat* mat1 = randMat( 2048, 2048 );
    Mat* mat2 = randMat( 2048, 2048 );
    Mat* result = initMat( 2048, 2048 );

    clock_t start = clock();
    mulMat( result, mat1, mat2 );
    clock_t end = clock();

    printf( "Time elapsed: %f seconds\n", (double)( end - start ) / CLOCKS_PER_SEC );
    printf( "test_multiply_speed_2048 ends successfully\n" );
    printf( "\n" );

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
    printf( "\n" );

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
    printf( "\n" );

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
    printf( "\n" );
}

void test_multiply_speed_1024_opti_versus_plain()
{
    Mat* mat1 = randMat( 1024, 1024 );
    Mat* mat2 = randMat( 1024, 1024 );
    Mat* result = initMat( 1024, 1024 );

    clock_t starto = clock();
    mulMat( result, mat1, mat2 );
    clock_t endo = clock();

    clock_t startp = clock();
    mulMatPlain( result, mat1, mat2 );
    clock_t endp = clock();

    double time_decr_ = ((double)( endp - startp ) - (double)( endo - starto )) / (double)( endp - startp );

    printf( "Time elapsed in optimized GEMM: %f seconds\n", (double)( endo - starto ) / CLOCKS_PER_SEC );
    printf( "Time elapsed in plain GEMM: %f seconds\n", (double)( endp - startp ) / CLOCKS_PER_SEC );
    printf( "Runtime reduced: %f%%\n", time_decr_ * 100 );
    printf( "test_multiply_speed_1024 ends successfully\n" );
    printf( "\n" );

    freeMat( mat1 );
    freeMat( mat2 );
    freeMat( result );
}

int main( int argc , char** argv )
{
    // get the yyyy-mm-dd hh:mm:ss format
    time_t now = time(0);
    struct tm tstruct;
    char buf[80], fname[100];
    tstruct = *localtime( &now );
    strftime( buf, sizeof( buf ), "%Y%m%d_%H%M%S", &tstruct );

    int otof = ( argc >= 2 ) && ( strcmp( argv[1], "-f") || strcmp( argv[1], "-F") );

    sprintf( fname, "%s.out", buf );

    FILE* fp;
    if ( otof && (fp = freopen( fname, "w", stdout )) == NULL )
    {
        perror( "freopen() failed" );
        fprintf( stderr, "freopen() failed\n" );
        return -1;
    }

    // test cases begin

    printf( "All tests begin. \n\n");

    test_multiply_correctness();
    test_multiply_speed_128();
    test_multiply_speed_512();
    // test_multiply_speed_1024();
    test_multiply_speed_1024_opti_versus_plain();
    test_multiply_speed_2048();
    test_multiply_speed_8k();
    test_multiply_speed_100t();
    printf( "All tests end\n" );

    // test cases end
    
    if ( otof ) {
        fclose( fp );
        freopen( "/dev/tty", "w", stdout );
        printf( "Output has been written to %s\n", fname );
    }

    return 0;
}