#ifndef AEROFLOT_H
#define AEROFLOT_H

#include <iostream>

class AEROFLOT {
private:
    char* destination;    // пункт назначени€
    int flightNumber;     // номер рейса
    char* planeType;      // тип самолЄта

    static char* duplicate_cstr(const char* src);

public:
    //  онструкторы
    AEROFLOT();                                   // без параметров
    AEROFLOT(const char* dest, int num, const char* type); // с параметрами
    AEROFLOT(const AEROFLOT& other);              // конструктор копировани€

    // ќператор присваивани€
    AEROFLOT& operator=(const AEROFLOT& other);

    // ƒеструктор
    ~AEROFLOT();

    // set/get
    void setDestination(const char* dest);
    void setFlightNumber(int num);
    void setPlaneType(const char* type);

    const char* getDestination() const;
    int getFlightNumber() const;
    const char* getPlaneType() const;

    // ѕерегрузки потоковых операторов
    friend std::ostream& operator<<(std::ostream& os, const AEROFLOT& a);
    friend std::istream& operator>>(std::istream& is, AEROFLOT& a);
};

#endif // AEROFLOT_H
