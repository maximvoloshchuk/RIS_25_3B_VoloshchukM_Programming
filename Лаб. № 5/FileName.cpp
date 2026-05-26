#include <iostream>
#include <iomanip>
using namespace std;


void deleteRow(int**& arr, int& rows, int cols, int rowIndex) {
    if (rowIndex < 0 || rowIndex >= rows) {
        cout << "Ошибка: неверный индекс строки!" << endl;
        return;
    }


    int** newArr = new int* [rows - 1];


    for (int i = 0; i < rowIndex; i++) {
        newArr[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            newArr[i][j] = arr[i][j];
        }
    }


    for (int i = rowIndex + 1; i < rows; i++) {
        newArr[i - 1] = new int[cols];
        for (int j = 0; j < cols; j++) {
            newArr[i - 1][j] = arr[i][j];
        }
    }


    for (int i = 0; i < rows; i++) {
        delete[] arr[i];
    }
    delete[] arr;


    arr = newArr;
    rows--;
}


void deleteRowsFromAtoB(int**& arr, int& rows, int cols, int A, int B) {

    if (A < 0 || B >= rows || A > B) {
        cout << "Ошибка: неверный диапазон строк!" << endl;
        return;
    }


    for (int i = B; i >= A; i--) {
        deleteRow(arr, rows, cols, i);
    }
}


void inputArray(int** arr, int rows, int cols) {
    cout << "Введите элементы массива (" << rows << " строк по " << cols << " элементов):" << endl;
    for (int i = 0; i < rows; i++) {
        cout << "Строка " << i + 1 << ": ";
        for (int j = 0; j < cols; j++) {
            cin >> arr[i][j];
        }
    }
}


void printArray(int** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(4) << arr[i][j];
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    int rows, cols;
    int A, B;


    cout << "Введите количество строк: ";
    cin >> rows;
    cout << "Введите количество столбцов: ";
    cin >> cols;


    if (rows <= 0 || cols <= 0) {
        cout << "Ошибка: размеры массива должны быть положительными числами!" << endl;
        return 1;
    }


    int** arr = new int* [rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
    }


    inputArray(arr, rows, cols);

    cout << "\nИсходный массив:" << endl;
    printArray(arr, rows, cols);


    cout << "\nВведите номера строк для удаления (от A до B):" << endl;
    cout << "A (начиная с 0): ";
    cin >> A;
    cout << "B (начиная с 0): ";
    cin >> B;


    if (A < 0 || B >= rows || A > B) {
        cout << "Ошибка: неверный диапазон строк! Строки должны быть от 0 до " << rows - 1 << endl;


        for (int i = 0; i < rows; i++) {
            delete[] arr[i];
        }
        delete[] arr;

        return 1;
    }


    cout << "\nУдаляем строки с " << A << " по " << B << ":" << endl;
    deleteRowsFromAtoB(arr, rows, cols, A, B);

    cout << "Результирующий массив:" << endl;
    printArray(arr, rows, cols);


    for (int i = 0; i < rows; i++) {
        delete[] arr[i];
    }
    delete[] arr;

    return 0;
}