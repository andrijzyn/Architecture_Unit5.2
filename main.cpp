#include <iostream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <limits>  // for numeric_limits
#include <algorithm>  // for find()

using namespace std;

// Helper function to ensure valid byte input
int getValidByteInput() {
    int num;
    cin >> num;
    if (num > 255) {
        num = num % 256;  // Modulo 256 for numbers larger than 255
    }
    if (cin.fail()) {
        num = 130;  // Default value for invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return num;
}

// Helper function to print binary representation with fixed size (8 bits)
void printBinary(int num, bool highlight = false) {
    bitset<8> binary(num);
    cout << (highlight ? "\033[31m" : "")  // Red color if highlight is true
         << binary << "\033[0m";           // Reset color after
}

// Function to perform right cyclic shift while keeping specified elements fixed
void cyclicRightShift(vector<int>& binaryArray, int shiftAmount, vector<int> fixedPositions) {
    std::vector<int>::size_type size = binaryArray.size();
    vector<int> tempArray(size);

    // Copy elements to new positions with cyclic shift
    for (std::vector<int>::size_type i = 0; i < size; ++i) {
        if (find(fixedPositions.begin(), fixedPositions.end(), i + 1) == fixedPositions.end()) {
            tempArray[(i + shiftAmount) % size] = binaryArray[i];
        } else {
            tempArray[i] = binaryArray[i];  // Fixed elements stay in place
        }
    }
    binaryArray = tempArray;
}

int main() {
    // Get user input for two bytes
    cout << "Enter two numbers between 0 and 255:" << endl;
    int byte1 = getValidByteInput();
    int byte2 = getValidByteInput();

    // Output decimal and binary values of the bytes
    cout << "Decimal input:" << endl;
    cout << "Byte 1: " << byte1 << " | Byte 2: " << byte2 << endl;

    cout << "Binary input:" << endl;
    cout << "Byte 1: "; printBinary(byte1); cout << endl;
    cout << "Byte 2: "; printBinary(byte2); cout << endl;

    // Combine both bytes into a 16-bit array
    vector<int> binaryArray(16);
    bitset<8> bin1(byte1), bin2(byte2);
    for (std::vector<int>::size_type i = 0; i < 8; ++i) {
        binaryArray[i] = bin1[i];
        binaryArray[8 + i] = bin2[i];
    }

    // Print combined array, highlight positions 14 and 3
    cout << "Combined 16-bit binary array:" << endl;
    for (std::vector<int>::size_type i = 15; i < 16; --i) {
        if (i == 13 || i == 2) {
            cout << "\033[31m";  // Red for highlighted bits
        }
        cout << binaryArray[i];
        cout << "\033[0m";
        if (i % 8 == 0) cout << " ";  // Separate bytes for readability
    }
    cout << endl;

    // Perform cyclic right shift by 2, keeping elements 14 and 3 in place
    vector<int> fixedPositions = {14, 3};
    cyclicRightShift(binaryArray, 2, fixedPositions);

    // Split into two 8-bit segments and display
    cout << "After cyclic shift (keeping bits 14 and 3 fixed):" << endl;
    cout << "Byte 1: ";
    for (std::vector<int>::size_type i = 7; i < 8; --i) {
        if (i == 6 || i == 1) {
            cout << "\033[31m";  // Red for bits 14 and 3
        }
        cout << binaryArray[i];
        cout << "\033[0m";
    }
    cout << endl;

    cout << "Byte 2: ";
    for (std::vector<int>::size_type i = 15; i < 16; --i) {
        if (i == 13 || i == 2) {
            cout << "\033[31m";  // Red for bits 14 and 3
        }
        cout << binaryArray[i];
        cout << "\033[0m";
    }
    cout << endl;

    // Convert back to decimal
    int byte1Shifted = 0, byte2Shifted = 0;
    for (std::vector<int>::size_type i = 0; i < 8; ++i) {
        byte1Shifted |= (binaryArray[i] << i);
        byte2Shifted |= (binaryArray[8 + i] << i);
    }

    // Sum the bytes and display
    int result = byte1Shifted + byte2Shifted;
    cout << "Sum of shifted bytes:" << endl;
    cout << "Decimal: " << result << endl;

    // Output result in binary with spaces between bytes
    cout << "Binary: " << bitset<16>(result).to_string().substr(0, 8) << " "
         << bitset<16>(result).to_string().substr(8) << endl;

    return 0;
}
