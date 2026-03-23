# Vector3D Module (Pybind11)

Модуль для работы с трёхмерными векторами на C++ с интеграцией в Python.

## 📋 Требования

- Python 3.6+
- C++ компилятор с поддержкой C++17
- CMake 3.14+
- pybind11 (устанавливается автоматически или через `pip install pybind11`)

## 🚀 Сборка и запуск

### Способ 1: Через CMake (рекомендуется)

```bash
# 1. Клонируйте репозиторий
git clone https://github.com/Gibrin/Drone-Programming.git
cd Drone-Programming/CPP/DP_CPP_Lesson10_5/vector_math

# 2. Инициализируйте подмодуль pybind11
git submodule update --init --recursive

# 3. Создайте папку build и перейдите в неё
mkdir build && cd build

# 4. Запустите CMake
cmake ..

# 5. Соберите проект
cmake --build .

# 6. Запустите тесты
export PYTHONPATH=$(pwd):$PYTHONPATH
python3 ../test_vector.py