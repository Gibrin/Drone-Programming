#include "matrix.h"
#include <iostream>
#include <cmath>

using namespace math;

// ---------- Доступ к элементам ----------
real& Matrix::operator()(int row, int col) {
    if (row >= rows_ || col >= cols_)
        std::cerr << "Index out of bounds\n";
    return data_[cols_ * row + col];
}

real Matrix::operator()(int row, int col) const {
    if (row >= rows_ || col >= cols_)
        std::cerr << "Index out of bounds\n";
    return data_[cols_ * row + col];
}

// ---------- Операторы с присваиванием ----------
Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        std::cerr << "Size mismatch in +=\n";
        return *this;
    }
    for (size_t i = 0; i < data_.size(); ++i)
        data_[i] += other.data_[i];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        std::cerr << "Size mismatch in -=\n";
        return *this;
    }
    for (size_t i = 0; i < data_.size(); ++i)
        data_[i] -= other.data_[i];
    return *this;
}

Matrix& Matrix::operator*=(real scalar) {
    for (size_t i = 0; i < data_.size(); ++i)
        data_[i] *= scalar;
    return *this;
}

// ---------- Арифметические операторы (дружественные) ----------
Matrix math::operator+(const Matrix& A, const Matrix& B) {
    if (A.rows_ != B.rows_ || A.cols_ != B.cols_) {
        std::cerr << "Size mismatch in +\n";
        return Matrix();
    }
    Matrix res(A.rows_, A.cols_);
    for (size_t i = 0; i < res.data_.size(); ++i)
        res.data_[i] = A.data_[i] + B.data_[i];
    return res;
}

Matrix math::operator-(const Matrix& A, const Matrix& B) {
    if (A.rows_ != B.rows_ || A.cols_ != B.cols_) {
        std::cerr << "Size mismatch in -\n";
        return Matrix();
    }
    Matrix res(A.rows_, A.cols_);
    for (size_t i = 0; i < res.data_.size(); ++i)
        res.data_[i] = A.data_[i] - B.data_[i];
    return res;
}

Matrix math::operator*(const Matrix& A, const Matrix& B) {
    if (A.cols_ != B.rows_) {
        std::cerr << "Invalid dimensions for *\n";
        return Matrix();
    }
    Matrix res(A.rows_, B.cols_);
    for (int i = 0; i < A.rows_; ++i)
        for (int j = 0; j < B.cols_; ++j)
            for (int k = 0; k < A.cols_; ++k)
                res(i, j) += A(i, k) * B(k, j);
    return res;
}

// ---------- Операторы ввода/вывода (без подсказок) ----------
std::ostream& math::operator<<(std::ostream& os, const Matrix& m) {
    for (int i = 0; i < m.rows_; ++i) {
        for (int j = 0; j < m.cols_; ++j)
            os << m(i, j) << ' ';
        os << '\n';
    }
    return os;
}

std::istream& math::operator>>(std::istream& is, Matrix& m) {
    for (int i = 0; i < m.rows_; ++i)
        for (int j = 0; j < m.cols_; ++j)
            is >> m(i, j);
    return is;
}