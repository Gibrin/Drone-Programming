#pragma once
#include "../export.h"

// Тип real должен совпадать с определением в матрице. Фиксируем double.
typedef double real;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Matrix Matrix;   // непрозрачный указатель

MATRIXLIB_EXPORT Matrix* math_createMatrix(int rows, int cols);
MATRIXLIB_EXPORT void    math_deleteMatrix(Matrix* M);
MATRIXLIB_EXPORT real    math_get(const Matrix* M, int row, int col);
MATRIXLIB_EXPORT void    math_set(Matrix* M, int row, int col, real val);
MATRIXLIB_EXPORT Matrix* math_add(const Matrix* A, const Matrix* B);
MATRIXLIB_EXPORT Matrix* math_subtract(const Matrix* A, const Matrix* B);
MATRIXLIB_EXPORT Matrix* math_dot(const Matrix* A, const Matrix* B);
MATRIXLIB_EXPORT void    math_print(const Matrix* M);

#ifdef __cplusplus
}
#endif