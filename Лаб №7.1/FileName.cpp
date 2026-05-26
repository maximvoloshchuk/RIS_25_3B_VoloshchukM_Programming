#include <iostream>
#include <clocale>
using namespace std;

struct Complex {
    double real;
    double imag;
};

// Функция для создания комплексного числа
Complex createComplex(double r, double i) {
    Complex c;
    c.real = r;
    c.imag = i;
    return c;
}

// Функция для вывода комплексного числа
void printComplex(Complex c) {
    if (c.imag >= 0)
        cout << c.real << " + " << c.imag << "i";
    else
        cout << c.real << " - " << -c.imag << "i";
}


//Вычитание целых чисел
int subtract(int a, int b) {
    cout << "Вычитание целых чисел: ";
    return a - b;
}

//Вычитание комплексных чисел
Complex subtract(Complex a, Complex b) {
    cout << "Вычитание комплексных чисел: ";
    Complex result;
    result.real = a.real - b.real;
    result.imag = a.imag - b.imag;
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== Вычитание целых чисел ===" << endl;
    int x, y;
    cout << "Введите x: ";
    cin >> x;
    cout << "Введите y: ";
    cin >> y;
    cout << x << " - " << y << " = " << subtract(x, y) << endl;

    cout << "\n=== Вычитание комплексных чисел ===" << endl;

    // Создаем комплексные числа
    int a, b, a1, b1;
    cout << "\nВведите целую часть комплексного числа a: ";
    cin >> a;
    cout << "Введите коэффициент мнимой части числа а: ";
    cin >> a1;
    cout << "Введите целую часть комплексного числа b: ";
    cin >> b;
    cout << "Введите коэффициент мнимой части числа b: ";
    cin >> b1;
    Complex num1 = createComplex(a, a1);
    Complex num2 = createComplex(b, b1);

    cout << "\nПервое число: ";
    printComplex(num1);
    cout << endl;

    cout << "Второе число: ";
    printComplex(num2);
    cout << endl;

    Complex result = subtract(num1, num2);

    cout << "Результат: ";
    printComplex(result);
    cout << endl;

    return 0;
}