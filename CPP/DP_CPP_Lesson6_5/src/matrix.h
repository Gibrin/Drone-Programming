#pragma once
#include <vector>
#include <iostream>

namespace math {
    typedef double real;

    class Matrix {
    private:
        int rows_, cols_;
        std::vector<real> data_;

    public:
        Matrix() : rows_(0), cols_(0) {}
        Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows * cols) {}

        // Доступ к элементам
        real& operator()(int row, int col);
        real operator()(int row, int col) const;

        // Составные операторы присваивания (методы класса)
        Matrix& operator+=(const Matrix& other);
        Matrix& operator-=(const Matrix& other);
        Matrix& operator*=(real scalar);

        // Дружественные арифметические операторы
        friend Matrix operator+(const Matrix& A, const Matrix& B);
        friend Matrix operator-(const Matrix& A, const Matrix& B);
        friend Matrix operator*(const Matrix& A, const Matrix& B);

        // Дружественные операторы ввода/вывода (без подсказок)
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
        friend std::istream& operator>>(std::istream& is, Matrix& m);
    };
}