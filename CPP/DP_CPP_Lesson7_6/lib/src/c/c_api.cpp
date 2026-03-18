#include "c_api.h"
#include "../matrix.h"
#include <iostream>

struct Matrix : public math::Matrix {
    Matrix(const math::Matrix& m) : math::Matrix(m) {}
};

// Макросы безопасного вызова (обработка исключений)
#define SAFE_CALL(expr) \
    try { return expr; } \
    catch(const std::exception& e) { \
        std::cerr << "Error in C API: " << e.what() << "\n"; \
        return {}; \
    } \
    catch(...) { \
        std::cerr << "Unknown error in C API\n"; \
        return {}; \
    }

#define SAFE_CALL_PTR(expr) \
    try { return new Matrix(expr); } \
    catch(const std::exception& e) { \
        std::cerr << "Error in C API: " << e.what() << "\n"; \
        return nullptr; \
    } \
    catch(...) { \
        std::cerr << "Unknown error in C API\n"; \
        return nullptr; \
    }

extern "C" {

Matrix* math_createMatrix(int rows, int cols) {
    try {
        return new Matrix(math::Matrix(rows, cols));
    } catch(...) {
        std::cerr << "Error: cannot create matrix\n";
        return nullptr;
    }
}

void math_deleteMatrix(Matrix* M) {
    try { delete M; } catch(...) {}
}

real math_get(const Matrix* M, int row, int col) {
    SAFE_CALL(M->operator()(row, col))
}

void math_set(Matrix* M, int row, int col, real val) {
    try { (*M)(row, col) = val; } catch(...) {}
}

Matrix* math_add(const Matrix* A, const Matrix* B) {
    SAFE_CALL_PTR(*A + *B)
}

Matrix* math_subtract(const Matrix* A, const Matrix* B) {
    SAFE_CALL_PTR(*A - *B)
}

Matrix* math_dot(const Matrix* A, const Matrix* B) {
    SAFE_CALL_PTR(*A * *B)
}

void math_print(const Matrix* M) {
    try { M->print(); } catch(...) {}
}

} // extern "C"