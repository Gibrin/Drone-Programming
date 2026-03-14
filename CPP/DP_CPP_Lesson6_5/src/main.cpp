#include "matrix.h"
#include <iostream>

int main()
{
    using namespace math;

    // Создаём матрицы как в исходном примере
    Matrix m(3, 3);
    m(0, 0) = 5;
    m(1, 1) = 5;

    std::cout << "Matrix m is:\n" << m << std::endl;

    Matrix m1(3, 3);
    m1(0, 0) = 5;
    m1(1, 1) = 5;

    std::cout << "Matrix m1 is:\n" << m1 << std::endl;

    // Демонстрация原有的 операторов +, -, *
    std::cout << "Sum of matrices m and m1 is:\n";
    Matrix m2 = m + m1;
    std::cout << m2 << std::endl;

    std::cout << "Subtract of matrices m and m1 is:\n";
    Matrix m3 = m - m1;
    std::cout << m3 << std::endl;

    std::cout << "Multiplication of matrices m and m1 is:\n";
    Matrix m4 = m * m1;
    std::cout << m4 << std::endl;

    // Демонстрация новых операторов +=, -=, *=
    std::cout << "\n--- Testing +=, -=, *= ---\n";
    Matrix A(2, 2);
    A(0,0) = 1; A(0,1) = 2;
    A(1,0) = 3; A(1,1) = 4;

    Matrix B(2, 2);
    B(0,0) = 5; B(0,1) = 6;
    B(1,0) = 7; B(1,1) = 8;

    std::cout << "A:\n" << A << "B:\n" << B;
    A += B;
    std::cout << "After A += B:\n" << A;
    A -= B;
    std::cout << "After A -= B (back to original):\n" << A;
    A *= 2.5;
    std::cout << "After A *= 2.5:\n" << A;

    // Демонстрация оператора >> (ввод)
    std::cout << "\n--- Testing input operator ---\n";
    Matrix C(2, 2);
    std::cout << "Enter 4 elements for a 2x2 matrix:\n";
    std::cin >> C;
    std::cout << "You entered:\n" << C;

    return 0;
}