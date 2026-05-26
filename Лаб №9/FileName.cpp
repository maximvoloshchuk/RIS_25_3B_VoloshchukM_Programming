#include <iostream>
#include <fstream>
#include <string>
#include <clocale>
#include <windows.h>
using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "RU");
    ofstream f1("F1.txt");
    string line;
    int count = 0;

    cout << "Введите не менее 10 строк (для окончания введите 'stop'):" << endl;

    while (true) {
        cout << "Строка " << count + 1 << ": ";
        getline(cin, line);

        if (line == "stop") {
            if (count < 10) {
                cout << "Нужно ввести минимум 10 строк! Продолжайте ввод." << endl;
                continue;
            }
            else {
                break;
            }
        }

        f1 << line << endl;
        count++;
    }

    f1.close();
    cout << "\nФайл F1 создан. Всего строк: " << count << endl;

    //Копируем строки из F1 в F2, начиная с 4-й
    ifstream fin("F1.txt");
    ofstream f2("F2.txt");

    int lineNumber = 0;

    while (getline(fin, line)) {
        lineNumber++;
        if (lineNumber >= 4) {
            f2 << line << endl;
        }
    }

    fin.close();
    f2.close();

    cout << "Строки с 4-й по последнюю скопированы в F2" << endl;

    ifstream fin2("F2.txt");
    string lastLine;

    while (getline(fin2, line)) {
        lastLine = line;
    }
    fin2.close();

    //Проверяем, не пустой ли файл
    if (lastLine.empty()) {
        cout << "Файл F2 пуст" << endl;
        return 0;
    }

    string lastWord = "";
    int length = lastLine.length();
    int i = length - 1;

    while (i >= 0 && lastLine[i] != ' ') {
        i--;
    }

    if (i >= 0) {
        for (int j = i + 1; j < length; j++) {
            lastWord += lastLine[j];
        }
    }
    else {
        lastWord = lastLine;
    }

    cout << "\nПоследняя строка в файле F2: \"" << lastLine << "\"" << endl;
    cout << "Последнее слово: \"" << lastWord << "\"" << endl;
    cout << "Количество символов в последнем слове: " << lastWord.length() << endl;

    return 0;
}