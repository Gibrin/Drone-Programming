#include "vector3d.h"
#include <cmath>
#include <pybind11/pybind11.h>

// Реализация методов
double Vector3D::length() const {
    return std::sqrt(coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2]);
}

void Vector3D::normalize() {
    double len = length();
    if (len > 1e-9) {
        coords[0] /= len; coords[1] /= len; coords[2] /= len;
    }
}

double Vector3D::dot(const Vector3D& other) const {
    return coords[0]*other.coords[0] + coords[1]*other.coords[1] + coords[2]*other.coords[2];
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(coords[0]+other.coords[0], coords[1]+other.coords[1], coords[2]+other.coords[2]);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(coords[0]-other.coords[0], coords[1]-other.coords[1], coords[2]-other.coords[2]);
}

// Свободные функции
Vector3D add_vectors(const Vector3D& a, const Vector3D& b) { return a + b; }
Vector3D subtract_vectors(const Vector3D& a, const Vector3D& b) { return a - b; }
double dot_product(const Vector3D& a, const Vector3D& b) { return a.dot(b); }

// ========== Привязки Pybind11 ==========
namespace py = pybind11;

PYBIND11_MODULE(vector_math, m) {
    m.doc() = "Vector algebra module (3D)";
    
    py::class_<Vector3D>(m, "Vector3D")
        .def(py::init<>())
        .def(py::init<double, double, double>())
        .def("x", &Vector3D::x)
        .def("y", &Vector3D::y)
        .def("z", &Vector3D::z)
        .def("length", &Vector3D::length)
        .def("normalize", &Vector3D::normalize)
        .def("dot", &Vector3D::dot)
        .def("__add__", &Vector3D::operator+)
        .def("__sub__", &Vector3D::operator-)
        .def("__repr__", [](const Vector3D& v) {
            char buf[64];
            snprintf(buf, sizeof(buf), "Vector3D(%.2f, %.2f, %.2f)", v.x(), v.y(), v.z());
            return std::string(buf);
        });
    
    m.def("add", &add_vectors, "Add two vectors");
    m.def("subtract", &subtract_vectors, "Subtract two vectors");
    m.def("dot", &dot_product, "Dot product of two vectors");
}