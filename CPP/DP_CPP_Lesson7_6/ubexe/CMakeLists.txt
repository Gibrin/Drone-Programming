cmake_minimum_required(VERSION 3.18)
project(MatrixApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(MATRIXLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/matrixlib)

add_executable(matrix_app src/main.cpp)

target_link_libraries(matrix_app PRIVATE ${MATRIXLIB_DIR}/shared-matrixlib.so)

add_custom_command(TARGET matrix_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MATRIXLIB_DIR}/shared-matrixlib.so
        $<TARGET_FILE_DIR:matrix_app>
)