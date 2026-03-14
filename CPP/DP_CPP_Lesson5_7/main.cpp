#include <iostream>
#include <string>
using namespace std;

class Car {
private:
    string brand;       // торговая марка
    int cylinders;       // число цилиндров
    int power;           // мощность
    static int total;    // счётчик всех объектов Car и производных

public:
    friend int count();  // дружественная функция доступа к total

    // Конструкторы
    Car() : brand(""), cylinders(0), power(0) { total++; }
    Car(string b, int c, int p) : brand(b), cylinders(c), power(p) { total++; }
    Car(const Car& other) : brand(other.brand), cylinders(other.cylinders), power(other.power) { total++; }

    // Деструктор
    virtual ~Car() { total--; }

    // Геттеры/сеттеры
    string getBrand() const { return brand; }
    void setBrand(string b) { brand = b; }
    int getCylinders() const { return cylinders; }
    void setCylinders(int c) { cylinders = c; }
    int getPower() const { return power; }
    void setPower(int p) { power = p; }

    // Перегрузка присваивания
    Car& operator=(const Car& other) {
        if (this != &other) {
            brand = other.brand;
            cylinders = other.cylinders;
            power = other.power;
        }
        return *this;
    }

    // Дружественные операторы ввода/вывода
    friend ostream& operator<<(ostream& os, const Car& car);
    friend istream& operator>>(istream& is, Car& car);
};

int Car::total = 0;

// Операторы для Car
ostream& operator<<(ostream& os, const Car& car) {
    os << car.brand << " (" << car.cylinders << " cyl, " << car.power << " hp)";
    return os;
}
istream& operator>>(istream& is, Car& car) {
    cout << "Brand: "; is >> car.brand;
    cout << "Cylinders: "; is >> car.cylinders;
    cout << "Power: "; is >> car.power;
    return is;
}

// Производный класс Lorry
class Lorry : public Car {
private:
    int capacity;       // грузоподъёмность

public:
    Lorry() : Car(), capacity(0) {}
    Lorry(string b, int c, int p, int cap) : Car(b, c, p), capacity(cap) {}
    Lorry(const Lorry& other) : Car(other), capacity(other.capacity) {}

    int getCapacity() const { return capacity; }
    void setCapacity(int cap) { capacity = cap; }

    Lorry& operator=(const Lorry& other) {
        if (this != &other) {
            Car::operator=(other);
            capacity = other.capacity;
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Lorry& l);
    friend istream& operator>>(istream& is, Lorry& l);
};

ostream& operator<<(ostream& os, const Lorry& l) {
    os << static_cast<const Car&>(l) << ", load: " << l.capacity << " t";
    return os;
}
istream& operator>>(istream& is, Lorry& l) {
    is >> static_cast<Car&>(l);
    cout << "Load capacity (t): "; is >> l.capacity;
    return is;
}

// Дружественная функция count()
int count() { return Car::total; }

// Демонстрация
int main() {
    cout << "Objects created: " << count() << endl;

    Car car1("Toyota", 6, 200);
    Car car2 = car1;                 // копирование
    Lorry lorry1("Volvo", 8, 400, 15);
    Lorry lorry2 = lorry1;           // копирование

    cout << "car1: " << car1 << "\ncar2: " << car2 << endl;
    cout << "lorry1: " << lorry1 << "\nlorry2: " << lorry2 << endl;
    cout << "Objects now: " << count() << endl;

    car1.setPower(250);
    lorry2.setCapacity(20);
    cout << "Modified car1: " << car1 << "\nModified lorry2: " << lorry2 << endl;

    return 0;
}