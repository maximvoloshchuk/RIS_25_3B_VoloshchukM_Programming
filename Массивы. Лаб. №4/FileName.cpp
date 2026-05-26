#include <clocale>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	const int maxSize = 100;
	int arr[maxSize];
	int curSize;

	cout << "Введите кол-во элементов массива (не более " << maxSize << "): ";
	cin >> curSize;
	cout << endl;

	if (curSize > maxSize || curSize <= 0) {
		cout << "Неккоректный размер массива!";
		return 1;
	}

	for (int i = 0; i < curSize; i++) {
		arr[i] = rand() % 100 - 50;
	}

	cout << "Исходный массив: { ";
	for (int i = 0; i < curSize; i++) {
		cout << arr[i] << " ";
	}
	cout << "}" << endl << endl;

	for (int i = curSize - 1; i >= 0; i--) {
		if (i % 3 == 0) {
			for (int j = i; j < curSize - 1; j++) {
				arr[j] = arr[j + 1];
			}
			curSize--;
		}
	}

	cout << "Массив после удаления эл. с индексами, кратными 3: ";
	cout << "{ ";
	for (int i = 0; i < curSize; i++) {
		cout << arr[i] << " ";
	}
	cout << "}" << endl << endl;

	int negativeCount = 0;
	for (int i = 0; i < curSize; i++) {
		if (arr[i] < 0) {
			negativeCount++;
		}
	}

	if (curSize + negativeCount > maxSize) {
		cout << "Ошибка! Недостаточно места для вставки" << endl;
		return 1;
	}


	for (int i = curSize; i >= 0; i--) {
		int shift = 0;
		for (int j = 0; j < i; j++) {
			if (arr[j] < 0) {
				shift++;
			}
		}

		arr[i + shift] = arr[i];

		if (arr[i] < 0) {
			int prevValue;
			if (i > 0) {
				prevValue = arr[i - 1];
			}
			else {
				prevValue = 0;
			}
			arr[i + shift + 1] = abs(prevValue + 1);
		}
	}

	int finSize = curSize + negativeCount;

	cout << "Финальный массив: { ";
	for (int i = 0; i < finSize; i++) {
		cout << arr[i] << " ";
	}
	cout << "}" << endl;

	return 0;
}