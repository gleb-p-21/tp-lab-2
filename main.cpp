#include <iostream>
#include <cstring>
#include <fstream>
#include <cctype>
#include <locale>
#include "AEROFLOT.h"

// Создание динамического массива AEROFLOT (size элементов)
AEROFLOT* allocate_array(int n) {
    if (n <= 0) return nullptr;
    AEROFLOT* arr = new AEROFLOT[n];
    return arr;
}

void deallocate_array(AEROFLOT* arr) {
    delete[] arr;
}

// Печать всех записей
void print_all(AEROFLOT* arr, int size) {
    if (size <= 0) {
        std::cout << "Записей нет.\n";
        return;
    }
    for (int i = 0; i < size; ++i) {
        std::cout << "[" << i << "] " << arr[i] << "\n";
    }
}

// Сортировка пузырьком по destination (алфавитно)
void sort_by_destination(AEROFLOT* arr, int size) {
    if (size < 2) return;
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1 - i; ++j) {
            const char* a = arr[j].getDestination();
            const char* b = arr[j + 1].getDestination();
            if (std::strcmp(a, b) > 0) {
                AEROFLOT tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    std::cout << "Записи отсортированы по пункту назначения (по алфавиту).\n";
}

// Поиск по типу самолёта (точное совпадение)
void find_by_plane_type(AEROFLOT* arr, int size, const char* planeType) {
    bool found = false;
    for (int i = 0; i < size; ++i) {
        if (std::strcmp(arr[i].getPlaneType(), planeType) == 0) {
            std::cout << arr[i] << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "Рейсов с типом самолёта \"" << planeType << "\" не найдено.\n";
}

// Вставка в позицию (0..size). Возвращает новый массив и увеличивает size.
AEROFLOT* insert_at(AEROFLOT* arr, int& size, int pos, const AEROFLOT& obj) {
    if (pos < 0) pos = 0;
    if (pos > size) pos = size;
    AEROFLOT* newArr = allocate_array(size + 1);
    for (int i = 0; i < pos; ++i) newArr[i] = arr[i];
    newArr[pos] = obj;
    for (int i = pos; i < size; ++i) newArr[i + 1] = arr[i];
    deallocate_array(arr);
    ++size;
    return newArr;
}

// Удаление по позиции
AEROFLOT* delete_at(AEROFLOT* arr, int& size, int pos) {
    if (size == 0) {
        std::cout << "Массив пуст, удалять нечего.\n";
        return arr;
    }
    if (pos < 0 || pos >= size) {
        std::cout << "Неверная позиция для удаления.\n";
        return arr;
    }
    if (size == 1) {
        deallocate_array(arr);
        size = 0;
        return nullptr;
    }
    AEROFLOT* newArr = allocate_array(size - 1);
    for (int i = 0; i < pos; ++i) newArr[i] = arr[i];
    for (int i = pos + 1; i < size; ++i) newArr[i - 1] = arr[i];
    deallocate_array(arr);
    --size;
    std::cout << "Запись на позиции " << pos << " удалена.\n";
    return newArr;
}

// Редактирование записи: пользователь выбирает поле
void edit_record(AEROFLOT* arr, int size, int pos) {
    if (pos < 0 || pos >= size) {
        std::cout << "Неверный индекс записи.\n";
        return;
    }
    std::cout << "Редактирование записи #" << pos << ": " << arr[pos] << "\n";
    std::cout << "Выберите поле для редактирования:\n";
    std::cout << "1) Пункт назначения\n2) Номер рейса\n3) Тип самолёта\nВыбор: ";
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::string dummy; std::getline(std::cin, dummy);
        std::cout << "Ошибка ввода.\n"; return;
    }
    std::string tmp;
    std::getline(std::cin, tmp); // съесть остаток строки
    try {
        if (choice == 1) {
            std::cout << "Введите новый пункт назначения: ";
            std::string s; std::getline(std::cin, s);
            arr[pos].setDestination(s.c_str());
        } else if (choice == 2) {
            std::cout << "Введите новый номер рейса: ";
            int num; while (!(std::cin >> num)) { std::cin.clear(); std::string d; std::getline(std::cin, d); std::cout << "Попробуйте снова: "; }
            std::string d; std::getline(std::cin, d);
            arr[pos].setFlightNumber(num);
        } else if (choice == 3) {
            std::cout << "Введите новый тип самолёта: ";
            std::string s; std::getline(std::cin, s);
            arr[pos].setPlaneType(s.c_str());
        } else {
            std::cout << "Неверный выбор.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка при редактировании: " << e.what() << "\n";
    }
}

// Чтение трёх предложений из файла и вывод в обратном порядке
// Граничные символы: '.', '!', '?'
void reverse_three_sentences_from_file(const char* filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cout << "Не удалось открыть файл: " << filename << "\n";
        return;
    }
    std::string content;
    std::string line;
    while (std::getline(fin, line)) {
        if (!content.empty()) content += '\n';
        content += line;
    }
    fin.close();

    std::string sentences[3];
    int found = 0;
    size_t i = 0;
    while (i < content.size() && found < 3) {
        // пропуск пробелов
        while (i < content.size() && std::isspace(static_cast<unsigned char>(content[i]))) ++i;
        if (i >= content.size()) break;
        size_t start = i;
        while (i < content.size() && content[i] != '.' && content[i] != '!' && content[i] != '?') ++i;
        if (i < content.size() && (content[i] == '.' || content[i] == '!' || content[i] == '?')) {
            ++i; // включаем знак пунктуации
            size_t end = i;
            while (end > start && std::isspace(static_cast<unsigned char>(content[end - 1]))) --end;
            sentences[found] = content.substr(start, end - start);
            ++found;
        } else {
            // до конца файла без знаков — берем остаток
            sentences[found] = content.substr(start);
            ++found;
            break;
        }
    }

    if (found == 0) {
        std::cout << "В файле не найдено предложений.\n";
        return;
    }

    std::cout << "Найдено " << found << " предложение(й). Вывод в обратном порядке:\n";
    for (int k = found - 1; k >= 0; --k) {
        std::cout << sentences[k] << "\n";
    }
}

// Сохранение записей в файл (каждая запись — 3 строки)
void save_to_file(AEROFLOT* arr, int size, const char* filename) {
    std::ofstream fout(filename);
    if (!fout) { std::cout << "Не удалось открыть файл для записи.\n"; return; }
    for (int i = 0; i < size; ++i) {
        fout << arr[i].getDestination() << "\n";
        fout << arr[i].getFlightNumber() << "\n";
        fout << arr[i].getPlaneType() << "\n";
    }
    fout.close();
    std::cout << "Сохранено " << size << " записей в файл " << filename << "\n";
}

// Загрузка из файла (формат: каждые 3 строки — запись)
AEROFLOT* load_from_file(int& size, const char* filename) {
    std::ifstream fin(filename);
    if (!fin) { std::cout << "Не удалось открыть файл для чтения.\n"; size = 0; return nullptr; }
    // посчитаем строки
    int lines = 0;
    std::string tmp;
    while (std::getline(fin, tmp)) ++lines;
    fin.clear();
    fin.seekg(0);
    int records = lines / 3;
    if (records == 0) { std::cout << "В файле нет записей.\n"; size = 0; return nullptr; }
    AEROFLOT* arr = allocate_array(records);
    for (int i = 0; i < records; ++i) {
        std::string dest; std::string numS; std::string type;
        std::getline(fin, dest);
        std::getline(fin, numS);
        std::getline(fin, type);
        int num = std::atoi(numS.c_str());
        try {
            arr[i].setDestination(dest.c_str());
            arr[i].setFlightNumber(num);
            arr[i].setPlaneType(type.c_str());
        } catch (const std::exception& e) {
            std::cout << "Ошибка при загрузке записи " << i << ": " << e.what() << "\n";
        }
    }
    fin.close();
    size = records;
    std::cout << "Загружено " << size << " записей из файла " << filename << "\n";
    return arr;
}

int main() {
    // Локаль и кодировка для корректного вывода русских букв в Windows-консоли
    setlocale(LC_ALL, "Russian");
    system("chcp 1251 > nul");

    AEROFLOT* arr = nullptr;
    int size = 0;

    while (true) {
        std::cout << "\n==== МЕНЮ РАБОТЫ С РЕЙСАМИ АВИАКОМПАНИИ ====\n";
        std::cout << "1) Добавить запись в указанную позицию\n";
        std::cout << "2) Удалить запись по позиции\n";
        std::cout << "3) Редактировать запись\n";
        std::cout << "4) Показать все записи\n";
        std::cout << "5) Отсортировать записи по пункту назначения (по алфавиту)\n";
        std::cout << "6) Найти рейсы по типу самолёта\n";
        std::cout << "7) Сохранить записи в файл\n";
        std::cout << "8) Загрузить записи из файла\n";
        std::cout << "9) Часть 2: прочитать из файла три предложения и вывести их в обратном порядке\n";
        std::cout << "0) Выход\n";
        std::cout << "Выберите действие: ";

        int opt;
        if (!(std::cin >> opt)) {
            std::cin.clear();
            std::string d; 
            std::getline(std::cin, d);
            std::cout << "Ошибка: неверный ввод.\n";
            continue;
        }
        std::string rest;
        std::getline(std::cin, rest); // убрать \n

        if (opt == 0) break;

        if (opt == 1) {
            std::cout << "Введите позицию для вставки (0.." << size << "): ";
            int pos;
            if (!(std::cin >> pos)) {
                std::cin.clear();
                std::string d; std::getline(std::cin, d);
                std::cout << "Ошибка: неверная позиция.\n";
                continue;
            }
            std::string d; std::getline(std::cin, d);

            AEROFLOT tmp;
            std::cout << "Введите данные нового рейса:\n";
            std::cin >> tmp;

            arr = insert_at(arr, size, pos, tmp);
            std::cout << "Запись успешно вставлена.\n";

        } else if (opt == 2) {
            if (size == 0) {
                std::cout << "Массив записей пуст.\n";
                continue;
            }
            std::cout << "Введите позицию для удаления (0.." << size - 1 << "): ";
            int pos;
            if (!(std::cin >> pos)) {
                std::cin.clear();
                std::string d; std::getline(std::cin,d);
                std::cout << "Ошибка: неверная позиция.\n";
                continue;
            }
            std::string d; std::getline(std::cin, d);
            arr = delete_at(arr, size, pos);

        } else if (opt == 3) {
            if (size == 0) {
                std::cout << "Массив записей пуст.\n";
                continue;
            }
            std::cout << "Введите индекс записи для редактирования (0.." << size - 1 << "): ";
            int pos;
            if (!(std::cin >> pos)) {
                std::cin.clear();
                std::string d; std::getline(std::cin,d);
                std::cout << "Ошибка: неверный индекс.\n";
                continue;
            }
            std::string d;
            std::getline(std::cin, d);
            edit_record(arr, size, pos);

        } else if (opt == 4) {
            print_all(arr, size);

        } else if (opt == 5) {
            sort_by_destination(arr, size);

        } else if (opt == 6) {
            std::cout << "Введите тип самолёта для поиска: ";
            std::string type;
            std::getline(std::cin, type);
            find_by_plane_type(arr, size, type.c_str());

        } else if (opt == 7) {
            std::cout << "Введите имя файла для сохранения: ";
            std::string fname;
            std::getline(std::cin, fname);
            save_to_file(arr, size, fname.c_str());

        } else if (opt == 8) {
            std::cout << "Введите имя файла для загрузки: ";
            std::string fname;
            std::getline(std::cin, fname);

            if (arr) { deallocate_array(arr); arr = nullptr; size = 0; }
            arr = load_from_file(size, fname.c_str());

        } else if (opt == 9) {
            std::cout << "Введите имя файла для чтения предложений: ";
            std::string fname;
            std::getline(std::cin, fname);
            reverse_three_sentences_from_file(fname.c_str());

        } else {
            std::cout << "Неизвестный пункт.\n";
        }
    }

    if (arr) deallocate_array(arr);
    std::cout << "\nПрограмма завершена.\n";
    return 0;
}
