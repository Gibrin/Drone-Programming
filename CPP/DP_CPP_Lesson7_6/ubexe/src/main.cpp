#include <iostream>

typedef double real;
struct Matrix;

#ifdef _WIN32
    #define IMPORT __declspec(dllimport)
#else
    #define IMPORT
#endif

extern "C" {
    IMPORT Matrix* math_createMatrix(int rows, int cols);
    IMPORT void    math_deleteMatrix(Matrix* M);
    IMPORT real    math_get(const Matrix* M, int row, int col);
    IMPORT void    math_set(Matrix* M, int row, int col, real val);
    IMPORT Matrix* math_add(const Matrix* A, const Matrix* B);
    IMPORT Matrix* math_subtract(const Matrix* A, const Matrix* B);
    IMPORT Matrix* math_dot(const Matrix* A, const Matrix* B);
    IMPORT void    math_print(const Matrix* M);
}

int main() {
    Matrix* A = math_createMatrix(2, 2);
    Matrix* B = math_createMatrix(2, 2);

    math_set(A, 0,0,1); math_set(A,0,1,2);
    math_set(A, 1,0,3); math_set(A,1,1,4);
    math_set(B, 0,0,5); math_set(B,0,1,6);
    math_set(B, 1,0,7); math_set(B,1,1,8);

    std::cout << "A:\n"; math_print(A);
    std::cout << "B:\n"; math_print(B);

    Matrix* C = math_add(A, B);
    std::cout << "A+B:\n"; math_print(C);

    Matrix* D = math_dot(A, B);
    std::cout << "A*B:\n"; math_print(D);

    math_deleteMatrix(A); math_deleteMatrix(B);
    math_deleteMatrix(C); math_deleteMatrix(D);
    return 0;
}