# Vector3D Module (Pybind11)

Модуль для работы с трёхмерными векторами на C++ с интеграцией в Python.

## 📋 Требования

- Python 3.8+
- C++ компилятор с поддержкой C++17
- CMake 3.14+
- Пакет заголовков Python: `python3-dev` (для Ubuntu/Debian)

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
# 💡 Если у вас несколько версий Python, укажите нужную:
cmake .. -DPython_EXECUTABLE=$(which python3.8)

# 5. Соберите проект
cmake --build .

# 6. Запустите тесты
export PYTHONPATH=$(pwd):$PYTHONPATH
python3 ../test_vector.py