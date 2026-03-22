import vector_math
import math

def test_all():
    v1 = vector_math.Vector3D(1, 2, 3)
    v2 = vector_math.Vector3D(4, 5, 6)
    
    # Сложение
    v3 = v1 + v2
    assert v3.x() == 5 and v3.y() == 7 and v3.z() == 9
    
    # Вычитание
    v4 = v1 - v2
    assert v4.x() == -3
    
    # Скалярное произведение
    assert v1.dot(v2) == 32
    assert vector_math.dot(v1, v2) == 32
    
    # Длина
    v5 = vector_math.Vector3D(3, 4, 0)
    assert abs(v5.length() - 5.0) < 1e-9
    
    # Нормализация
    v5.normalize()
    assert abs(v5.length() - 1.0) < 1e-9
    
    # Нормализация нулевого вектора (краевой случай)
    zero = vector_math.Vector3D(0, 0, 0)
    zero.normalize()
    assert zero.length() == 0
    
    # Свободные функции
    v6 = vector_math.add(v1, vector_math.Vector3D(0, 0, 0))
    assert v6.x() == 1
    
    print("✅ All tests passed!")

if __name__ == "__main__":
    test_all()