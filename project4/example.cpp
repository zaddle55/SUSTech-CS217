#include <iostream>
#include <cassert>
#include "matrix.hpp"



using namespace std;

int main() {
    
    Mati32 mat(2, 2);
    assert(mat.rows() == 2);
    assert(mat.cols() == 2);
    assert(mat.channels() == 1);
    assert(mat.size() == 4);
    assert(mat.use_count() == 1);

    assert(mat(0, 0) == 0);
    assert(mat(0, 1) == 0);
    assert(mat(1, 0) == 0);
    assert(mat(1, 1) == 0);

    cout << mat << endl;

    Mati32 mat_cpy(mat);
    assert(mat_cpy.rows() == 2);
    assert(mat_cpy.cols() == 2);
    assert(mat_cpy.channels() == 1);
    assert(mat_cpy.size() == 4);
    assert(mat_cpy.use_count() == 2);

    cout << mat_cpy << endl;

    Matf matf(
        {
            {1.0f, 2.0f},
            {4.0f, 5.0f, 6.0f}
        }
    );

    assert(matf.rows() == 2);
    assert(matf.cols() == 3);
    assert(matf.channels() == 1);
    assert(matf.size() == 6);

    cout << matf << endl;

    Mati16 mat16(2, 2);
    cin >> mat16;
    
    return 0;
}