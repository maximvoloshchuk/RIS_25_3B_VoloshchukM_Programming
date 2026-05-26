#include <iostream>
#include <clocale>
#include <cstdarg>
using namespace std;

int sum(int count, ...) {
    va_list args;
    va_start(args, count);

    int s = 0;

    for (int i = 0; i < count; i += 2) {
        int a = va_arg(args, int);
        int b = va_arg(args, int);

        s += a * b;

        cout << "Умножаем " << a << " * " << b << " = " << a * b << endl;
        cout << "Текущая сумма: " << s << endl << endl;
    }

    va_end(args);
    return s;
}

int main() {
    setlocale(LC_ALL, "RU");

    cout << "=== Первый вызов (8 параметров) ===" << endl;
    int result1 = sum(8, 1, 2, 3, 4, 5, 6, 7, 8);
    cout << "ИТОГО: " << result1 << endl << endl;

    cout << "=== Второй вызов (10 параметров) ===" << endl;
    int result2 = sum(10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    cout << "ИТОГО: " << result2 << endl << endl;

    cout << "=== Третий вызов (12 параметров) ===" << endl;
    int result3 = sum(12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
    cout << "ИТОГО: " << result3 << endl;

    return 0;
}