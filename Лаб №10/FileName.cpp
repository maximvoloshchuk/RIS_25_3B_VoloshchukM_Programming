#include <iostream>
#include <clocale>
using namespace std;

int main() {
	setlocale(LC_ALL, "RU");
	int n;
	int num;
	cout << "Введите длину массива: ";
	cin >> n;
	int* arr = new int[n];

	cout << "Введите " << n << " элементов массива: ";
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}

	cout << "Введённый вами массив: ";
	for (int i = 0; i < n; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	cout << "Введите номер элемента, который необходимо удалить: ";
	cin >> num;
    int ind = num - 1;
	for (int i = ind; i < n; i++) {
		arr[i] = arr[i + 1];
	}
	n--;
	cout << endl;
	
	int* arr2 = new int[n];
	for (int i = 0; i < n; i++) {
		arr2[i] = arr[i];
	}

	delete[] arr;
	arr = arr2;

	cout << "Массив после удаление элемента под номером " << num << ": ";
	for (int i = 0; i < n; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

    int position;
    cout << "\nВведите номер элемента, ПОСЛЕ которого добавить K элементов (от 1 до " << n << "): ";
    cin >> position;

    if (position < 0 || position >= n) {
        cout << "Ошибка: неверный номер элемента!" << endl;
        delete[] arr;
        return 1;
    }

    int K;
    cout << "Введите количество элементов для добавления (K): ";
    cin >> K;

    if (K <= 0) {
        cout << "Ошибка: K должно быть положительным!" << endl;
        delete[] arr;
        return 1;
    }

    int new_n = n + K;
    int* newArr = new int[new_n];

    for (int i = 0; i <= position - 1; i++) {
        newArr[i] = arr[i];
    }

    cout << "\nВведите " << K << " новых элементов:" << endl;
    for (int i = 0; i < K; i++) {
        cout << "Новый элемент " << i + 1 << ": ";
        cin >> newArr[position + i];
    }

    for (int i = position; i < n; i++) {
        newArr[i + K] = arr[i];
    }

    delete[] arr;

    arr = newArr;
    n = new_n;

    cout << "\nМассив после добавления " << K << " элементов после позиции " << position << ":" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

	cout << endl << endl;

	delete[] arr;
	return 0;
}