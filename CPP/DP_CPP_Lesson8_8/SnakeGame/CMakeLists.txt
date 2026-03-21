# Минимальная версия CMake для поддержки C++17
cmake_minimum_required(VERSION 3.14)

# Название проекта
project(SnakeGame LANGUAGES CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Qt5 (требуются модули Core, Gui, Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Автоматическая обработка MOC (для сигналов/слотов Qt)
set(CMAKE_AUTOMOC ON)

# Исходные файлы проекта
add_executable(${PROJECT_NAME}
    main.cpp
    snake.cpp
    snake.h
)

# Подключение библиотек Qt
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)