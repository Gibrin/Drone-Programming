# Vector3D Module (Pybind11)

Модуль для работы с трёхмерными векторами на C++ с интеграцией в Python.

## 📋 Требования

- Python 3.8+
- C++ компилятор с поддержкой C++17
- CMake 3.14+
- Пакет заголовков Python: python3-dev

## 🚀 Сборка

### Способ 1: Через CMake

1. Инициализируйте подмодуль pybind11:
   git submodule update --init --recursive

2. Создайте папку build:
   mkdir build && cd build

3. Запустите CMake:
   cmake .. -DPython_EXECUTABLE=$(which python3.8)

4. Соберите проект:
   cmake --build .

5. Запустите тесты:
   export PYTHONPATH=$(pwd):$PYTHONPATH
   python3 ../test_vector.py

### Способ 2: Через setup.py

1. Установите зависимости:
   pip install pybind11

2. Соберите модуль:
   python3 setup.py build_ext --inplace

3. Запустите тесты:
   python3 test_vector.py

## 📖 Пример использования

import vector_math

v1 = vector_math.Vector3D(1, 2, 3)
v2 = vector_math.Vector3D(4, 5, 6)

v_sum = v1 + v2
v_diff = v1 - v2
dot = v1.dot(v2)

length = v1.length()
v1.normalize()

print(v1)

## 📁 Структура проекта

vector_math/
├── CMakeLists.txt
├── setup.py
├── vector3d.h
├── vector3d.cpp
├── test_vector.py
├── README.md
├── .gitignore
├── .gitmodules
└── pybind11/

## ✅ Реализованный функционал

- [x] Сложение векторов
- [x] Вычитание векторов
- [x] Скалярное произведение
- [x] Длина вектора
- [x] Нормализация
- [x] Тесты

## 🔧 Устранение проблем

Ошибка: Could NOT find Python
- Решение: sudo apt install python3-dev
- Или: cmake .. -DPython_EXECUTABLE=$(which python3.8)

Ошибка: модуль не найден
- Решение: export PYTHONPATH=$(pwd):$PYTHONPATH

Ошибка: pybind11 не найден
- Решение: git submodule update --init --recursive