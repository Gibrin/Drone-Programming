#include "matrix.h"
#include <iostream>
#include <stdexcept>

namespace math {
    real& Matrix::operator()(int row, int col) {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
            throw std::out_of_range("Matrix index out of range");
        return data_[cols_ * row + col];
    }

    real Matrix::operator()(int row, int col) const {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
            throw std::out_of_range("Matrix index out of range");
        return data_[cols_ * row + col];
    }

    void Matrix::print() const {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j)
                std::cout << data_[cols_ * i + j] << " ";
            std::cout << "\n";
        }
    }

    Matrix operator+(const Matrix& A, const Matrix& B) {
        if (A.rows_ != B.rows_ || A.cols_ != B.cols_)
            throw std::invalid_argument("Dimensions mismatch for addition");
        Matrix R(A.rows_, A.cols_);
        for (int i = 0; i < A.rows_; ++i)
            for (int j = 0; j < A.cols_; ++j)
                R(i,j) = A(i,j) + B(i,j);
        return R;
    }

    Matrix operator-(const Matrix& A, const Matrix& B) {
        if (A.rows_ != B.rows_ || A.cols_ != B.cols_)
            throw std::invalid_argument("Dimensions mismatch for subtraction");
        Matrix R(A.rows_, A.cols_);
        for (int i = 0; i < A.rows_; ++i)
            for (int j = 0; j < A.cols_; ++j)
                R(i,j) = A(i,j) - B(i,j);
        return R;
    }

    Matrix operator*(const Matrix& A, const Matrix& B) {
        if (A.cols_ != B.rows_)
            throw std::invalid_argument("Dimensions mismatch for multiplication");
        Matrix R(A.rows_, B.cols_);
        for (int i = 0; i < A.rows_; ++i) {
            for (int j = 0; j < B.cols_; ++j) {
                real sum = 0;
                for (int k = 0; k < A.cols_; ++k)
                    sum += A(i,k) * B(k,j);
                R(i,j) = sum;
            }
        }
        return R;
    }
}