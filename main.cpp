#include <iostream>
#include <bitset>
#include <vector>
#include <limits>
#include <numeric>
#include <string>
#include <windows.h>

using namespace std;

void setConsoleColor(const int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}

constexpr int RED_COLOR = FOREGROUND_RED | FOREGROUND_INTENSITY;
constexpr int CYAN_COLOR = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
constexpr int HIGHLIGHT_COLOR = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

string decimalToBinary(const int number) {
    return bitset<8>(number).to_string();
}

void color(const string &str, const int colorCode) {
    setConsoleColor(colorCode);
    cout << str;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

vector<int> getUserInput() {
    vector<int> bytes(2);
    for (int i = 0; i < 2; ++i) {
        int input;
        cout << "Enter byte " << (i + 1) << " (0-255): ";
        while (true) {
            if (cin >> input) {
                if (input < 0 || input > 255) {
                    input %= 256; // Обробка перевищення 255
                }
                bytes[i] = input == 130 ? 130 : input;
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bytes[i] = 130; // Значення за умовчанням при некоректному вводі
            break;
        }
    }
    return bytes;
}

int getHighlightIndex() {
    int index;
    cout << "Enter index to highlight (0-15): ";
    while (true) {
        if (cin >> index && index >= 0 && index < 16) {
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between 0 and 15: ";
    }
    return index;
}

vector<int> combineBytesToBinaryArray(const vector<int> &bytes) {
    vector<int> binaryArray(16);
    for (int i = 0; i < 8; ++i) {
        binaryArray[i] = (bytes[0] >> (7 - i)) & 1;
        binaryArray[i + 8] = (bytes[1] >> (7 - i)) & 1;
    }
    return binaryArray;
}

void printHighlightedArray(const vector<int> &array, int highlightIndex) {
    for (int i = 0; i < array.size(); ++i) {
        // Пропускаємо виділений індекс, перший і останній елементи
        if (i == highlightIndex) {
            color(to_string(array[i]), HIGHLIGHT_COLOR); // Выделяем указанный бит
        } else if (i == 0 || i == array.size() - 1) {
            color(to_string(array[i]), RED_COLOR); // Виділяємо крайні елементи
        } else {
            cout << array[i];
        }
        cout << " ";
    }
    cout << endl;
}

vector<int> cyclicShiftArray(const vector<int> &binaryArray, int highlightIndex) {
    vector<int> shiftedArray(16);
    int shiftCount = 0;

    for (int i = 0; i < 16; ++i) {
        int sourceIndex = i;
        while (shiftCount < 2) {
            sourceIndex = (sourceIndex - 1 + 16) % 16;
            if (sourceIndex != highlightIndex && sourceIndex != 0 && sourceIndex != 15) {
                shiftCount++;
            }
        }
        shiftCount = 0;

        if (i == highlightIndex || i == 0 || i == 15) {
            shiftedArray[i] = binaryArray[i];
        } else {
            shiftedArray[i] = binaryArray[sourceIndex];
        }
    }

    return shiftedArray;
}

void splitAndOutputBytes(const vector<int> &shiftedArray) {
    vector<int> firstByte(shiftedArray.begin(), shiftedArray.begin() + 8);
    vector<int> secondByte(shiftedArray.begin() + 8, shiftedArray.end());

    color("First byte: ", CYAN_COLOR);
    for (const auto &bit: firstByte) cout << bit;
    cout << " (" << accumulate(firstByte.begin(), firstByte.end(), 0,
                               [](const int acc, const int bit) { return (acc << 1) | bit; })
            << ")" << endl;

    color("Second byte: ", CYAN_COLOR);
    for (const auto &bit: secondByte) cout << bit;
    cout << " (" << accumulate(secondByte.begin(), secondByte.end(), 0,
                               [](const int acc, const int bit) { return (acc << 1) | bit; }) << ")" << endl;
}

void outputProductOfBytes(const vector<int> &firstByte, const vector<int> &secondByte) {
    const int firstVal = accumulate(firstByte.begin(), firstByte.end(), 0,
                                    [](const int acc, const int bit) { return (acc << 1) | bit; });
    const int secondVal = accumulate(secondByte.begin(), secondByte.end(), 0,
                                     [](const int acc, const int bit) { return (acc << 1) | bit; });
    const int product = firstVal * secondVal;

    color("Product of bytes: ", CYAN_COLOR);
    cout << product << " (" << bitset<16>(product).to_string().substr(0, 8) << " "
            << bitset<16>(product).to_string().substr(8) << ")" << endl;
}

int main() {
    const vector<int> bytes = getUserInput();

    cout << "Entered bytes: " << bytes[0] << " (" << decimalToBinary(bytes[0]) << "), "
            << bytes[1] << " (" << decimalToBinary(bytes[1]) << ")" << endl;

    const vector<int> binaryArray = combineBytesToBinaryArray(bytes);
    cout << "Combined binary array: ";
    int highlightIndex = getHighlightIndex();
    printHighlightedArray(binaryArray, highlightIndex);

    const vector<int> shiftedArray = cyclicShiftArray(binaryArray, highlightIndex);
    cout << "Array after cyclic shift right by 2: ";
    printHighlightedArray(shiftedArray, highlightIndex);

    splitAndOutputBytes(shiftedArray);
    outputProductOfBytes({shiftedArray.begin(), shiftedArray.begin() + 8},
                         {shiftedArray.begin() + 8, shiftedArray.end()});

    return 0;
}
