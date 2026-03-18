cmake_minimum_required(VERSION 3.18)
project(MatrixLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Источники
set(SOURCES
    src/matrix.cpp
    src/c/c_api.cpp
)

# Динамическая библиотека
add_library(shared-matrixlib SHARED ${SOURCES})

# Убираем префикс "lib" – имя файла будет shared-matrixlib.so
set_target_properties(shared-matrixlib PROPERTIES PREFIX "")

# Генерация экспортного заголовка (для макросов экспорта)
include(GenerateExportHeader)
generate_export_header(shared-matrixlib
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/export_shared.h"
    BASE_NAME matrixlib
)

# Пути для include: папки src/ и src/c/, а также бинарная папка (для export_shared.h)
target_include_directories(shared-matrixlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/c>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

# Если нужна опция точности – передаём макрос (здесь для простоты тип double фиксирован)
# Но если хотите, можно добавить опцию и определение:
# option(MATH_USE_DOUBLE_PRECISION "Use double precision" ON)
# target_compile_definitions(shared-matrixlib PUBLIC
#     $<$<BOOL:${MATH_USE_DOUBLE_PRECISION}>:MATH_DOUBLE_PREC_DEFINE>
# )