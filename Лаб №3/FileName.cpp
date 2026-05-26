#include <iostream>
#include <iomanip>
#include <cmath>
#include <clocale>

using namespace std;

// Функция для вычисления факториала
long long factorial(int n) {
    long long fact = 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

double exactFunction(double x) {
    return exp(2 * x);
}

double sumForN(double x, int n) {
    double sum = 1.0;

    for (int i = 1; i <= n; i++) {
        double term = pow(2 * x, i) / factorial(i);
        sum += term;
    }

    return sum;
}

double sumForEpsilon(double x, double epsilon) {
    double sum = 1.0; 
    double term;
    int i = 1;

    do {
        term = pow(2 * x, i) / factorial(i);
        sum += term;
        i++;
    } while (abs(term) > epsilon);

    return sum;
}

int main() {
    setlocale(LC_ALL, "RU");
    double a = 0.1;
    double b = 1.0;
    int k = 10; 
    double epsilon = 0.0001;
    int n = 20;

    double step = (b - a) / k;

    cout << "-----------------------------------------------------------------\n";
    cout << "|   x  |  Точное значение | Сумма (n=20) | Сумма (eps) | Членов |\n";
    cout << "-----------------------------------------------------------------\n";

    for (int i = 0; i <= k; i++) {
        double x = a + i * step;

        double exact = exactFunction(x);        // точное значение
        double sumN = sumForN(x, n);            // значение для заданного n

        double sumEps = 1.0;
        double term;
        int termsCount = 0;
        int j = 1;

        do {
            term = pow(2 * x, j) / factorial(j);
            sumEps += term;
            j++;
            termsCount++;
        } while (abs(term) > epsilon);

        cout << "| " << fixed << setprecision(2) << x << " | "
            << setprecision(6) << setw(16) << exact << " | "
            << setw(12) << sumN << " | "
            << setw(11) << sumEps << " | "
            << setw(6) << termsCount << " |\n";
    }

    cout << "-----------------------------------------------------------------\n";

    return 0;
}