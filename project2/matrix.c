#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

Mat* initMat( int m, int n )
{
    Mat* mat = (Mat*)malloc( sizeof(Mat) );
    mat->m = m;
    mat->n = n;
    mat->data = (float**)malloc( m * sizeof(float*) );

    for ( int i = 0; i < m; i++ )
    {
        mat->data[i] = (float*)malloc( n * sizeof(float) );
    }

    return mat;
}

Mat* randMat( int m, int n )
{
    Mat* mat = initMat( m, n );

    for ( int i = 0; i < m; i++ )
    {
        for ( int j = 0; j < n; j++ )
        {
            srand(time(NULL));
            mat->data[i][j] = (float)rand() / RAND_MAX;
        }
    }

    return mat;
}

int cpyMat( Mat* dest, const Mat* src )
{
    if ( dest->m != src->m || dest->n != src->n )
    {
        printf("Warning: matrix dimensions do not match\n");
        return 0;
    }

    for ( int i = 0; i < src->m; i++ )
    {
        for ( int j = 0; j < src->n; j++ )
        {
            dest->data[i][j] = src->data[i][j];
        }
    }

    return 1;
}

void freeMat( Mat* mat )
{
    for ( int i = 0; i < mat->m; i++ )
    {
        free( mat->data[i] );
    }

    free( mat->data );
    free( mat );
}

int readMat( Mat* mat )
{
    for ( int i = 0; i < mat->m; i++ )
    {
        for ( int j = 0; j < mat->n; j++ )
        {
            if ( scanf( "%f", &mat->data[i][j] ) != 1 )
            {
                printf( "Invalid input at (%d, %d)\n", i, j );
                return 0;
            }
        }
    }

    printf( "Matrix read successfully\n" );
    return 1;
}

const float peekMat( const Mat* mat, int i, int j )
{
    return mat->data[i][j];
}

int setMat( Mat* mat, int i, int j, float value )
{
    if ( i < 0 || i >= mat->m || j < 0 || j >= mat->n )
    {
        printf( "Invalid index (%d, %d)\n", i, j );
        return 0;
    }

    mat->data[i][j] = value;
    return 1;
}

void printMat( const Mat* mat )
{
    for ( int i = 0; i < mat->m; i++ )
    {
        for ( int j = 0; j < mat->n; j++ )
        {
            printf( "%f ", mat->data[i][j] );
        }

        printf( "\n" );
    }
}

int cmpMat( const Mat* mat1, const Mat* mat2 )
{
    if ( mat1->m != mat2->m || mat1->n != mat2->n )
    {
        printf( "Matrix dimensions do not match\n" );
        return -1;
    }

    for ( int i = 0; i < mat1->m; i++ )
    {
        for ( int j = 0; j < mat1->n; j++ )
        {
            if ( mat1->data[i][j] != mat2->data[i][j] )
            {
                return mat1->data[i][j] - mat2->data[i][j];
            }
        }
    }

    return 0;
}

int mulMat( Mat* result, const Mat* mat1, const Mat* mat2 )
{
    // check mat1 and mat2 matrix dimensions
    if ( mat1->n != mat2->m )
    {
        printf( "Invalid matrix dimensions\n" );
        return 0;
    }

    // check result matrix dimensions
    if ( result->m != mat1->m || result->n != mat2->n )
    {
        freeMat( result );
        result = initMat( mat1->m, mat2->n );
    }

    for ( int i = 0; i < mat1->m; i++ )
    {
        for ( int j = 0; j < mat2->n; j++ )
        {
            result->data[i][j] = 0;

            for ( int k = 0; k < mat1->n; k++ )
            {
                result->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
            }
        }
    }

    return 1;
}

// int main()
// {
//     int m, n;
//     printf( "Enter the number of rows and columns for the first matrix: " );
//     scanf( "%d %d", &m, &n );

//     Mat* mat1 = initMat( m, n );
//     printf( "Enter the first matrix:\n" );
//     readMat( mat1 );

//     printf( "Enter the number of rows and columns for the second matrix: " );
//     scanf( "%d %d", &m, &n );

//     Mat* mat2 = initMat( m, n );
//     printf( "Enter the second matrix:\n" );
//     readMat( mat2 );

//     Mat* result = initMat( mat1->m, mat2->n );
//     if ( mulMat( result, mat1, mat2 ) )
//     {
//         printf( "The result of the multiplication is:\n" );
//         printMat( result );
//     }

//     freeMat( mat1 );
//     freeMat( mat2 );
//     freeMat( result );

//     return 0;
// }