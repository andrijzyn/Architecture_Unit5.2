#include <iostream>
#include <bitset>
#include <vector>
#include <limits>
#include <numeric>
#include <string>

using namespace std;

const string RED_COLOR = "\033[31m";
const string CYAN_COLOR = "\033[36m";
constexpr int HL_1 = 14 - 1;
constexpr int HL_2 = 3 - 1;

string decimalToBinary(const int number) {
    return bitset<8>(number).to_string();
}

string color(const string &str, const string &color) {
    return color + str + "\033[0m"; // ANSI escape code to reset color
}

vector<int> getUserInput() {
    vector<int> bytes(2);
    for (int i = 0; i < 2; ++i) {
        int input;
        cout << "Enter byte " << (i + 1) << " (0-255): ";
        while (true) {
            if (cin >> input) {
                if (input < 0 || input > 255) {
                    input = input % 256; // Handle overflow
                }
                bytes[i] = input == 130 ? 130 : input;
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bytes[i] = 130; // Default on incorrect input
            break;
        }
    }
    return bytes;
}

vector<int> combineBytesToBinaryArray(const vector<int> &bytes) {
    vector<int> binaryArray(16);
    for (int i = 0; i < 8; ++i) {
        binaryArray[i] = (bytes[0] >> (7 - i)) & 1;
        binaryArray[i + 8] = (bytes[1] >> (7 - i)) & 1;
    }
    return binaryArray;
}

string getHighlightedArray(const vector<int> &array, int highlightIndex1, int highlightIndex2) {
    string result;
    for (int i = 0; i < 16; ++i) {
        result += (i == highlightIndex1 || i == highlightIndex2)
                      ? color(to_string(array[i]), RED_COLOR)
                      : to_string(array[i]);
        result += " "; // Добавляем пробел между элементами
    }
    return result;
}

vector<int> cyclicShiftArray(const vector<int> &binaryArray) {
    vector<int> shiftedArray(16);
    for (int i = 0, j = 0; i < 16; ++i) {
        if (i == HL_1 || i == HL_2) {
            shiftedArray[i] = binaryArray[i];
        } else {
            shiftedArray[i] = binaryArray[(j - 2 + 16) % 16];
            ++j;
        }
    }
    return shiftedArray;
}

string outputShiftedArray(const vector<int> &shiftedArray) {
    string result;
    for (const auto &bit: shiftedArray) result += to_string(bit) + " ";
    result += "\n";
    return result;
}

pair<vector<int>, vector<int> > splitAndOutputBytes(const vector<int> &shiftedArray) {
    vector<int> firstByte(shiftedArray.begin(), shiftedArray.begin() + 8);
    vector<int> secondByte(shiftedArray.begin() + 8, shiftedArray.end());

    cout << color("First byte: ", CYAN_COLOR);
    for (const auto &bit: firstByte) cout << bit;
    cout << " (" << accumulate(firstByte.begin(), firstByte.end(), 0,
                               [](const int acc, const int bit) { return (acc << 1) | bit; })
            << ")" << endl;

    cout << color("Second byte: ", CYAN_COLOR);
    for (const auto &bit: secondByte) cout << bit;
    cout << " (" << accumulate(secondByte.begin(), secondByte.end(), 0,
                               [](const int acc, const int bit) { return (acc << 1) | bit; }) << ")" << endl;

    return {firstByte, secondByte};
}

void outputSumOfBytes(const vector<int> &firstByte, const vector<int> &secondByte) {
    const int sum = accumulate(firstByte.begin(), firstByte.end(), 0,
                               [](const int acc, const int bit) { return (acc << 1) | bit; }) +
                    accumulate(secondByte.begin(), secondByte.end(), 0, [](const int acc, const int bit) {
                        return (acc << 1) | bit;
                    });

    cout << "Sum of bytes: " << sum << " (" << decimalToBinary(sum) << ")" << endl;
}

int main() {
    const vector<int> bytes = getUserInput();

    cout << "Entered bytes: " << bytes[0] << " (" << decimalToBinary(bytes[0]) << "), "
            << bytes[1] << " (" << decimalToBinary(bytes[1]) << ")" << endl;

    const vector<int> binaryArray = combineBytesToBinaryArray(bytes);
    cout << "Combined binary array: " << getHighlightedArray(binaryArray, HL_1, HL_2) << endl;

    const vector<int> shiftedArray = cyclicShiftArray(binaryArray);
    cout << "Array after cyclic shift right by 2: " << getHighlightedArray(shiftedArray, HL_1, HL_2) << endl; // And here

    auto [firstByte, secondByte] = splitAndOutputBytes(shiftedArray);
    outputSumOfBytes(firstByte, secondByte);

    return 0;
}
