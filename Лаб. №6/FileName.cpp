#include <iostream>
#include <cstring>
#include <cctype>
#include <clocale>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    char s[256];

    cout << "Введите строку (максимум 255 символов, в конце точка):" << endl;
    fgets(s, sizeof(s), stdin);

    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }

    cout << "\nСлова, содержащие ровно одну цифру:" << endl;

    int length = strlen(s);
    bool inWord = false;
    int digitCount = 0;
    char currentWord[256] = { 0 };
    int wordIndex = 0;

    for (int i = 0; i <= length; i++) {
        char c = s[i];


        if (c != ' ' && c != '.' && c != '\0') {
            if (!inWord) {

                inWord = true;
                digitCount = 0;
                wordIndex = 0;
                memset(currentWord, 0, sizeof(currentWord));
            }


            currentWord[wordIndex++] = c;


            if (isdigit(static_cast<unsigned char>(c))) {
                digitCount++;
            }
        }
        else {

            if (inWord) {
                currentWord[wordIndex] = '\0';


                if (digitCount == 1) {
                    cout << currentWord << endl;
                }

                inWord = false;
            }


            if (c == '.') {
                break;
            }
        }
    }

    return 0;
}