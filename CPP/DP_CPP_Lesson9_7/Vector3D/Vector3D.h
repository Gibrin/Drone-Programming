#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <memory>
#include <iostream>

class Vector3D {
public:
    // Конструкторы
    Vector3D();                               // по умолчанию (0,0,0)
    Vector3D(double x, double y, double z);   // с параметрами

    // Правило пяти
    ~Vector3D() = default;                    // unique_ptr сам удалит массив
    Vector3D(const Vector3D& other);          // копирование
    Vector3D& operator=(const Vector3D& other); // копирующее присваивание
    Vector3D(Vector3D&& other) noexcept;      // перемещение
    Vector3D& operator=(Vector3D&& other) noexcept; // перемещающее присваивание

    // Доступ к координатам
    double x() const { return coords[0]; }
    double y() const { return coords[1]; }
    double z() const { return coords[2]; }

    void setX(double v) { coords[0] = v; }
    void setY(double v) { coords[1] = v; }
    void setZ(double v) { coords[2] = v; }

    // Операции
    double length() const;
    void normalize();

    // Вывод
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v);

private:
    std::unique_ptr<double[]> coords; // умный указатель на массив из 3 элементов
};

// Функция, принимающая shared_ptr (для демонстрации)
void processVector(std::shared_ptr<Vector3D> vec);

#endif // VECTOR3D_H