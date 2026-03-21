#include "Vector3D.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Семантика перемещения ===\n";
    Vector3D a(1, 2, 3);
    Vector3D b = std::move(a);            // перемещающий конструктор
    std::cout << "a after move: " << a << "\n";   // валидный нулевой вектор
    std::cout << "b: " << b << "\n";

    Vector3D c(4, 5, 6);
    c = std::move(b);                     // перемещающее присваивание
    std::cout << "b after move: " << b << "\n";
    std::cout << "c: " << c << "\n";

    std::cout << "\n=== std::unique_ptr ===\n";
    auto uptr = std::make_unique<Vector3D>(7, 8, 9);
    std::cout << "uptr -> " << *uptr << '\n';
    auto uptr2 = std::move(uptr);          // перемещение владения
    if (!uptr) std::cout << "uptr is now null\n";
    std::cout << "uptr2 -> " << *uptr2 << '\n';

    std::cout << "\n=== std::shared_ptr ===\n";
    auto sptr1 = std::make_shared<Vector3D>(10, 0, 0);
    std::cout << "use_count: " << sptr1.use_count() << '\n';
    {
        auto sptr2 = sptr1;
        std::cout << "use_count inside block: " << sptr1.use_count() << '\n';
        processVector(sptr1);
    }
    std::cout << "use_count after block: " << sptr1.use_count() << '\n';
    std::cout << "sptr1: " << *sptr1 << '\n';

    return 0;
}