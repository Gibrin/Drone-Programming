#pragma once

// Простой экспорт для GCC (и других компиляторов с атрибутом visibility)
#if defined(__GNUC__) && __GNUC__ >= 4
    #define MATRIXLIB_EXPORT __attribute__((visibility("default")))
#else
    #define MATRIXLIB_EXPORT
#endif