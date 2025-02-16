
#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct
{
    int x;
    int y;
} Pos;

typedef struct
{
    int m;
    int n;
    float **data;
} Mat;

Mat* initMat( int m, int n ); // return a pointer to a new matrix with m rows and n columns
Mat* randMat( int m, int n ); // return a pointer to a new matrix with m rows and n columns, filled with random values
int cpyMat( Mat* dest, const Mat* src ); // copy the matrix src to dest
void freeMat( Mat* mat ); // free the memory allocated for the matrix
int readMat( Mat* mat ); // read the matrix from the standard input
// int readMat( Mat* mat, float** data, int m, int n ); // read the matrix from the standard input
const float peekMat( const Mat* mat, int i, int j ); // return the value at the i-th row and j-th column
int setMat( Mat* mat, int i, int j, float value ); // set the value at the i-th row and j-th column
void printMat( const Mat* mat ); // print the matrix to the standard output
int mulMat( Mat* result, const Mat* mat1, const Mat* mat2 ); // multiply two matrices (mat1 * mat2) and store the result in result
int cmpMat( const Mat* mat1, const Mat* mat2 ); // compare two matrices

#endif // MATRIX_H_