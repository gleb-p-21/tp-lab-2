#include "AEROFLOT.h"
#include <cstring>
#include <iomanip>
#include <stdexcept>
#include <string>

// Вспомогательная функция: дублирование C-style строки
char* AEROFLOT::duplicate_cstr(const char* src) {
    if (!src) {
        char* s = new char[1];
        s[0] = '\0';
        return s;
    }
    size_t len = std::strlen(src);
    char* dst = new char[len + 1];
    std::strcpy(dst, src);
    return dst;
}

// Конструктор без параметров
AEROFLOT::AEROFLOT() : destination(nullptr), flightNumber(0), planeType(nullptr) {
    destination = duplicate_cstr("");
    planeType = duplicate_cstr("");
    std::cout << "Создан объект AEROFLOT() (пустой)\n";
}

// Конструктор с параметрами
AEROFLOT::AEROFLOT(const char* dest, int num, const char* type) {
    destination = duplicate_cstr(dest ? dest : "");
    planeType = duplicate_cstr(type ? type : "");
    flightNumber = num;
    std::cout << "Создан объект AEROFLOT(dest, num, type) для рейса " << flightNumber << "\n";
}

// Конструктор копирования (глубокая копия)
AEROFLOT::AEROFLOT(const AEROFLOT& other) {
    destination = duplicate_cstr(other.destination ? other.destination : "");
    planeType = duplicate_cstr(other.planeType ? other.planeType : "");
    flightNumber = other.flightNumber;
    std::cout << "Скопирован объект AEROFLOT (рейс " << flightNumber << ")\n";
}

// Оператор присваивания
AEROFLOT& AEROFLOT::operator=(const AEROFLOT& other) {
    if (this == &other) return *this;
    delete[] destination;
    delete[] planeType;
    destination = duplicate_cstr(other.destination ? other.destination : "");
    planeType = duplicate_cstr(other.planeType ? other.planeType : "");
    flightNumber = other.flightNumber;
    std::cout << "Присвоение объекта AEROFLOT (рейс " << flightNumber << ")\n";
    return *this;
}

// Деструктор
AEROFLOT::~AEROFLOT() {
    std::cout << "Удалён объект AEROFLOT для рейса " << flightNumber << "\n";
    delete[] destination;
    delete[] planeType;
}

// set/get
void AEROFLOT::setDestination(const char* dest) {
    if (!dest) throw std::invalid_argument("Пустой пункт назначения");
    delete[] destination;
    destination = duplicate_cstr(dest);
}

void AEROFLOT::setFlightNumber(int num) {
    if (num < 0) throw std::invalid_argument("Номер рейса не может быть отрицательным");
    flightNumber = num;
}

void AEROFLOT::setPlaneType(const char* type) {
    if (!type) throw std::invalid_argument("Пустой тип самолёта");
    delete[] planeType;
    planeType = duplicate_cstr(type);
}

const char* AEROFLOT::getDestination() const { return destination ? destination : ""; }
int AEROFLOT::getFlightNumber() const { return flightNumber; }
const char* AEROFLOT::getPlaneType() const { return planeType ? planeType : ""; }

// Оператор вывода
std::ostream& operator<<(std::ostream& os, const AEROFLOT& a) {
    os << "Пункт назначения: " << (a.destination ? a.destination : "") << " | "
       << "Номер рейса: " << a.flightNumber << " | "
       << "Тип самолёта: " << (a.planeType ? a.planeType : "");
    return os;
}

// Оператор ввода: читаем строки с пробелами
std::istream& operator>>(std::istream& is, AEROFLOT& a) {
    std::string dest, type;
    int num;

    std::cout << "Введите пункт назначения: ";
    std::getline(is, dest);
    // если сразу был оставшийся \n, пытаемся снова
    if (dest.size() == 0 && !is.eof()) {
        // возможно предыдущий ввод оставил \n; но getline вернул пустую строку - всё равно ок
    }

    std::cout << "Введите номер рейса (целое неотрицательное число): ";
    while (!(is >> num)) {
        is.clear();
        std::string dummy;
        std::getline(is, dummy);
        std::cout << "Некорректный номер рейса, попробуйте снова: ";
    }
    std::string tmp;
    std::getline(is, tmp); // съесть остаток строки после числа

    std::cout << "Введите тип самолёта: ";
    std::getline(is, type);

    try {
        a.setDestination(dest.c_str());
        a.setFlightNumber(num);
        a.setPlaneType(type.c_str());
    } catch (const std::exception& e) {
        std::cerr << "Ошибка ввода: " << e.what() << "\n";
    }

    return is;
}
