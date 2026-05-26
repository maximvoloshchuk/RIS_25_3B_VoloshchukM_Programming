#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <clocale>

using namespace std;

// Структура "Человек"
struct Person {
    char surname[50];      // фамилия
    char name[50];         // имя
    char patronymic[50];   // отчество
    char address[100];     // домашний адрес
    char phone[20];        // номер телефона
    int age;               // возраст
};

// Функция для ввода информации о человеке с клавиатуры
Person inputPerson() {
    Person p;
    cout << "Введите фамилию: ";
    cin >> p.surname;
    cout << "Введите имя: ";
    cin >> p.name;
    cout << "Введите отчество: ";
    cin >> p.patronymic;
    cout << "Введите адрес: ";
    cin.ignore(); // очищаем буфер
    cin.getline(p.address, 100);
    cout << "Введите номер телефона: ";
    cin >> p.phone;
    cout << "Введите возраст: ";
    cin >> p.age;
    return p;
}

// Функция для вывода информации о человеке на экран
void printPerson(const Person& p) {
    cout << p.surname << " " << p.name << " " << p.patronymic
        << ", адрес: " << p.address
        << ", тел: " << p.phone
        << ", возраст: " << p.age << endl;
}

// Функция для проверки существования файла
bool fileExists(const char* filename) {
    ifstream file(filename);
    return file.good(); // good() возвращает true если файл существует и доступен
}

// Функция для удаления файла
void deleteFile(const char* filename) {
    if (remove(filename) == 0) {
        cout << "Файл успешно удален!" << endl;
    }
    else {
        cout << "Ошибка: не удалось удалить файл (возможно, он не существует)" << endl;
    }
}

// Функция для формирования двоичного файла
void createFile(const char* filename) {
    // Проверяем, существует ли уже файл
    if (fileExists(filename)) {
        char choice;
        cout << "Файл уже существует. Хотите перезаписать его? (y/n): ";
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            cout << "Операция отменена." << endl;
            return;
        }
    }

    ofstream file(filename, ios::binary);
    if (!file) {
        cout << "Ошибка: не удалось создать файл!" << endl;
        return;
    }

    int n;
    cout << "Сколько человек добавить в файл? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\nВведите данные для человека " << i + 1 << ":" << endl;
        Person p = inputPerson();
        file.write((char*)&p, sizeof(Person));
    }

    file.close();
    cout << "Файл успешно создан!" << endl;
}

// Функция для печати содержимого файла
void printFile(const char* filename) {
    if (!fileExists(filename)) {
        cout << "Файл не существует. Сначала создайте его (пункт 1)." << endl;
        return;
    }

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Ошибка: не удалось открыть файл для чтения!" << endl;
        return;
    }

    Person p;
    int count = 0;
    cout << "\nСодержимое файла:" << endl;

    while (file.read((char*)&p, sizeof(Person))) {
        cout << count + 1 << ". ";
        printPerson(p);
        count++;
    }

    if (count == 0) {
        cout << "Файл пуст!" << endl;
    }

    file.close();
}

// Функция для удаления элементов с заданным возрастом
void deleteByAge(const char* filename, int targetAge) {
    if (!fileExists(filename)) {
        cout << "Файл не существует. Сначала создайте его (пункт 1)." << endl;
        return;
    }

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Ошибка: не удалось открыть исходный файл!" << endl;
        return;
    }

    // Временный файл для записи
    ofstream tempFile("temp.bin", ios::binary);
    if (!tempFile) {
        cout << "Ошибка: не удалось создать временный файл!" << endl;
        file.close();
        return;
    }

    Person p;
    int deleted = 0;

    // Копируем все записи, кроме тех, у которых возраст совпадает с targetAge
    while (file.read((char*)&p, sizeof(Person))) {
        if (p.age != targetAge) {
            tempFile.write((char*)&p, sizeof(Person));
        }
        else {
            deleted++;
        }
    }

    file.close();
    tempFile.close();

    // Заменяем исходный файл временным
    remove(filename);
    rename("temp.bin", filename);

    cout << "Удалено записей: " << deleted << endl;
}

// Функция для добавления элемента после элемента с заданным номером
void addAfterNumber(const char* filename, int number) {
    if (!fileExists(filename)) {
        cout << "Файл не существует. Сначала создайте его (пункт 1)." << endl;
        return;
    }

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Ошибка: не удалось открыть исходный файл!" << endl;
        return;
    }

    // Временный файл
    ofstream tempFile("temp.bin", ios::binary);
    if (!tempFile) {
        cout << "Ошибка: не удалось создать временный файл!" << endl;
        file.close();
        return;
    }

    Person p;
    int current = 0;
    bool found = false;

    // Копируем все записи до указанного номера
    while (file.read((char*)&p, sizeof(Person))) {
        current++;
        tempFile.write((char*)&p, sizeof(Person));

        // Если достигли нужного номера, добавляем новый элемент после него
        if (current == number) {
            found = true;
            cout << "\nВведите данные для нового человека:" << endl;
            Person newPerson = inputPerson();
            tempFile.write((char*)&newPerson, sizeof(Person));
            cout << "Элемент добавлен после записи №" << number << endl;
        }
    }

    file.close();
    tempFile.close();

    if (!found) {
        cout << "Элемент с номером " << number << " не найден!" << endl;
        remove("temp.bin"); // удаляем временный файл
        return;
    }

    // Заменяем исходный файл временным
    remove(filename);
    rename("temp.bin", filename);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian"); // для поддержки русского языка
    const char* filename = "people.dat";
    int choice, age, number;

    do {
        cout << "=========================== МЕНЮ ===========================" << endl;
        cout << "\n1. Создать файл" << endl;
        cout << "2. Просмотреть содержимое файла" << endl;
        cout << "3. Удалить записи по возрасту" << endl;
        cout << "4. Добавить запись после указанного номера" << endl;
        cout << "5. Удалить файл" << endl;  // НОВЫЙ ПУНКТ
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            createFile(filename);
            break;

        case 2:
            printFile(filename);
            break;

        case 3:
            cout << "Введите возраст для удаления: ";
            cin >> age;
            deleteByAge(filename, age);
            cout << "Обновленное содержимое файла:" << endl;
            printFile(filename);
            break;

        case 4:
            cout << "Введите номер записи, после которой нужно добавить новую: ";
            cin >> number;
            addAfterNumber(filename, number);
            cout << "Обновленное содержимое файла:" << endl;
            printFile(filename);
            break;

        case 5:  // НОВЫЙ ПУНКТ
            deleteFile(filename);
            break;

        case 0:
            cout << "Программа завершена." << endl;
            break;

        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}