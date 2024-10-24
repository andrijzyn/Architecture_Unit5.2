#include <iostream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;

constexpr int MAX_BYTE_VALUE = 255;
const string RESET_COLOR = "\033[0m";
const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string LIGHT_BLUE_COLOR = "\033[34m";

// Printing text in color
void printColored(const string &text, const string &colorCode) {
    cout << colorCode << text << RESET_COLOR;
}

// Checking and getting a valid byte
int getValidByteInput() {
    int num;
    while (true) {
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            num = 130;
            break;
        } else {
            num = (num >= 0) ? (num > MAX_BYTE_VALUE ? MAX_BYTE_VALUE : num) : 130;
            break;
        }
    }
    return num;
}

// Printing binary representation of a number
void printBinary(const int num) {
    cout << bitset<8>(num);
}

// Cyclic right shift while keeping fixed elements
void cyclicRightShift(vector<int> &binaryArray, const int shiftAmount, const vector<int> &fixedPositions) {
    const size_t size = binaryArray.size();
    vector<int> tempArray(size);

    // Save fixed positions
    vector<int> fixedValues(fixedPositions.size());
    for (size_t i = 0; i < fixedPositions.size(); ++i) {
        fixedValues[i] = binaryArray[fixedPositions[i] - 1]; // -1 for indexing
    }

    // Perform cyclic shift
    for (size_t i = 0; i < size; ++i) {
        // If the position is fixed, keep its value
        if (find(fixedPositions.begin(), fixedPositions.end(), i + 1) != fixedPositions.end()) {
            tempArray[i] = binaryArray[i];
        } else {
            // Calculate new position for the shift
            tempArray[(i + shiftAmount) % size] = binaryArray[i];
        }
    }

    // Insert fixed values back
    for (size_t i = 0; i < fixedPositions.size(); ++i) {
        tempArray[fixedPositions[i] - 1] = fixedValues[i]; // -1 for indexing
    }

    // Update the original array
    binaryArray = move(tempArray);
}

// Collecting user input
void collectInput(int &byte1, int &byte2) {
    printColored("Enter two numbers between 0 and 255:\n", LIGHT_BLUE_COLOR);
    byte1 = getValidByteInput();
    byte2 = getValidByteInput();
}

// Printing decimal values
void printDecimalValues(const int byte1, const int byte2) {
    printColored("Decimal input:\n", GREEN_COLOR);
    cout << "Byte 1: " << byte1 << " | Byte 2: " << byte2 << endl;
}

// Printing binary values
void printBinaryValues(const int byte1, const int byte2) {
    printColored("Binary input:\n", GREEN_COLOR);
    cout << "Byte 1: ";
    printBinary(byte1);
    cout << endl;
    cout << "Byte 2: ";
    printBinary(byte2);
    cout << endl;
}

// Creating combined 16-bit binary array
vector<int> createBinaryArray(const int byte1, const int byte2) {
    vector<int> binaryArray(16);
    const bitset<8> bin1(byte1);
    const bitset<8> bin2(byte2);
    for (size_t i = 0; i < 8; ++i) {
        binaryArray[i] = bin1[i];
        binaryArray[8 + i] = bin2[i];
    }
    return binaryArray;
}

// Printing combined binary array
void printCombinedBinaryArray(const vector<int> &binaryArray) {
    printColored("Combined 16-bit binary array:\n", GREEN_COLOR);
    for (size_t i = 0; i < binaryArray.size(); ++i) {
        if (i == 13 || i == 2) {
            cout << RED_COLOR;
        }
        cout << binaryArray[i] << RESET_COLOR;
        if (i == 7) cout << " "; // Byte separator
    }
    cout << endl;
}

// Printing shifted binary array
void printShiftedBinaryArray(const vector<int> &binaryArray) {
    printColored("After cyclic shift (keeping bits 14 and 3 fixed):\n", GREEN_COLOR);
    for (size_t i = 0; i < binaryArray.size(); ++i) {
        if (i == 13 || i == 2) {
            cout << RED_COLOR;
        }
        cout << binaryArray[i] << RESET_COLOR;
        if (i == 7) cout << " "; // Byte separator
    }
    cout << endl;
}

// Printing binary and decimal representation of individual bytes
void printByteValues(const vector<int> &binaryArray) {
    vector<int> byte1(binaryArray.begin(), binaryArray.begin() + 8);
    vector<int> byte2(binaryArray.begin() + 8, binaryArray.end());

    auto printWithRedFixedBits = [](const vector<int> &byte) {
        for (size_t i = 0; i < byte.size(); ++i) {
            if (i == 5 || i == 6) {
                // Fixed bits in the original 16-bit array are 3rd and 14th, translating to 5th and 6th in the first byte
                cout << RED_COLOR;
            }
            cout << byte[i] << RESET_COLOR;
        }
    };

    printColored("New Byte 1:\n", GREEN_COLOR);
    printWithRedFixedBits(byte1);
    cout << " Decimal: " << accumulate(byte1.begin(), byte1.end(), 0, [](const int acc, const int bit) {
        return (acc << 1) | bit; // Left shift and append current bit
    }) << endl;

    printColored("New Byte 2:\n", GREEN_COLOR);
    printWithRedFixedBits(byte2);
    cout << " Decimal: " << accumulate(byte2.begin(), byte2.end(), 0, [](const int acc, const int bit) {
        return (acc << 1) | bit; // Left shift and append current bit
    }) << endl;
}

// Summing and printing the result
void printSumOfShiftedBytes(const vector<int> &binaryArray) {
    const int result = accumulate(binaryArray.begin(), binaryArray.end(), 0, [](const int acc, const int bit) {
        return (acc << 1) | bit; // Left shift and append current bit
    });

    printColored("Sum of shifted bytes:\n", GREEN_COLOR);
    cout << "Decimal: " << result << endl;
    cout << "Binary: " << bitset<16>(result) << endl;

    if (bitset<16>(result).count() > 8) {
        cout << bitset<16>(result).to_string().substr(0, 8) << " " << bitset<16>(result).to_string().substr(8) << endl;
    } else {
        cout << bitset<16>(result) << endl;
    }
}


int main() {
    int byte1, byte2;
    collectInput(byte1, byte2);
    printDecimalValues(byte1, byte2);
    printBinaryValues(byte1, byte2);

    vector<int> binaryArray = createBinaryArray(byte1, byte2);
    printCombinedBinaryArray(binaryArray);

    const vector<int> fixedPositions = {14, 3};
    cyclicRightShift(binaryArray, 2, fixedPositions);
    printShiftedBinaryArray(binaryArray);

    printByteValues(binaryArray);
    printSumOfShiftedBytes(binaryArray);

    return 0;
}
