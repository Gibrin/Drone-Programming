#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D {
public:
    Vector3D() : coords{0, 0, 0} {}
    Vector3D(double x, double y, double z) : coords{x, y, z} {}
    
    double x() const { return coords[0]; }
    double y() const { return coords[1]; }
    double z() const { return coords[2]; }
    
    double length() const;
    void normalize();
    double dot(const Vector3D& other) const;
    
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;

private:
    double coords[3];
};

// Свободные функции
Vector3D add_vectors(const Vector3D& a, const Vector3D& b);
Vector3D subtract_vectors(const Vector3D& a, const Vector3D& b);
double dot_product(const Vector3D& a, const Vector3D& b);

#endif