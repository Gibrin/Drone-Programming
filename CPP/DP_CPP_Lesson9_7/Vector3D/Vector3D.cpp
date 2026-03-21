#include "Vector3D.h"
#include <cmath>

// Конструктор по умолчанию
Vector3D::Vector3D() : coords(std::make_unique<double[]>(3)) {
    coords[0] = coords[1] = coords[2] = 0.0;
}

// Конструктор с параметрами
Vector3D::Vector3D(double x, double y, double z) : coords(std::make_unique<double[]>(3)) {
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
}

// Конструктор копирования
Vector3D::Vector3D(const Vector3D& other) : coords(std::make_unique<double[]>(3)) {
    coords[0] = other.coords[0];
    coords[1] = other.coords[1];
    coords[2] = other.coords[2];
    std::cout << "Vector3D copy constructor\n";
}

// Оператор присваивания копированием
Vector3D& Vector3D::operator=(const Vector3D& other) {
    if (this != &other) {
        coords[0] = other.coords[0];
        coords[1] = other.coords[1];
        coords[2] = other.coords[2];
        std::cout << "Vector3D copy assignment\n";
    }
    return *this;
}

// Конструктор перемещения
Vector3D::Vector3D(Vector3D&& other) noexcept
    : coords(std::move(other.coords))
{
    // Оставляем other в валидном состоянии (нулевой вектор)
    other.coords = std::make_unique<double[]>(3);
    other.coords[0] = other.coords[1] = other.coords[2] = 0.0;
    std::cout << "Vector3D move constructor\n";
}

// Оператор присваивания перемещением
Vector3D& Vector3D::operator=(Vector3D&& other) noexcept {
    if (this != &other) {
        coords = std::move(other.coords);
        other.coords = std::make_unique<double[]>(3);
        other.coords[0] = other.coords[1] = other.coords[2] = 0.0;
        std::cout << "Vector3D move assignment\n";
    }
    return *this;
}

double Vector3D::length() const {
    return std::sqrt(coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2]);
}

void Vector3D::normalize() {
    double len = length();
    if (len > 1e-9) {
        coords[0] /= len;
        coords[1] /= len;
        coords[2] /= len;
    }
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}

void processVector(std::shared_ptr<Vector3D> vec) {
    if (vec) {
        std::cout << "processVector: use_count = " << vec.use_count() << '\n';
        vec->normalize();
        std::cout << "Normalized: " << *vec << '\n';
    } else {
        std::cout << "processVector: received null pointer\n";
    }
}