#pragma once
#include <vector>

namespace math {
    // Фиксируем тип double для простоты (можно параметризовать через макрос, если нужно)
    typedef double real;

    class Matrix {
    private:
        int rows_, cols_;
        std::vector<real> data_;
    public:
        Matrix() = default;
        Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows * cols) {}

        real& operator()(int row, int col);
        real  operator()(int row, int col) const;
        void print() const;

        friend Matrix operator+(const Matrix&, const Matrix&);
        friend Matrix operator-(const Matrix&, const Matrix&);
        friend Matrix operator*(const Matrix&, const Matrix&);
    };
}