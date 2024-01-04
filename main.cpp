//
//  main.cpp
//  MipsMicroprocessor
//
//  Created by Jovan Ford and Julius Jones on 11/10/23.
//
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <limits>

using namespace std;

// MIPS Formats
void Rformat(string);
void Iformat(string);

// MIPS INSTRUCTIONS

void AND(int, int, int);
void OR(int, int, int);
void add(int, int, int);
void sub(int, int, int);
void lw(int, int, int);
void sw(int, int, int);
void bridgeIfEqual(int, int, int);
void bridgeIfNotEqual(int, int, int);
void addi(int, int, int);

// CONVERSION FUNCTIONS

string binInstruction(string);
string decimalToHex(int);
string hexInstruction;
string decimalToBinary(int);
string decimalToBinaryString(int);
string binaryToHex(int);
string addZeroes(int, string);
int binaryToDecimal(string);
int intDecimal(int, int);
int pc = 0;

// Extras
string userinput;
string getFormat(const string &hexInstruction);

string decimalToBinaryString(int decimal){
    const int bitSize = 32;
    return bitset<bitSize>(decimal).to_string();
}

// integer arrays to hold the values of the registers and memory
int registers[16];
int memory[16];

string registersStr[16] = {"0000", "0001", "0002", "0003", "0004", "0005", "0006", "0007",
                           "0008", "0009", "000A", "000B", "000C", "000D", "000E", "000F"};

string memoryStr[16] = {"0000", "0001", "0002", "0003", "0004", "0005", "0006", "0007",
                        "0008", "0009", "000A", "000B", "000C", "000D", "000E", "000F"};

bool displayUserMenu() {
    string userInput;
    cout << "==== MIPS Microprocessor Simulation ====\n\n"
         << "This program is a Mips processor. It takes in a hexadecimal instruction, decodes it, "
         << "and prints out the contents for the user.\n\n"
         << "Do you want to run the program? (Y/N): ";

    while (true) {
        cin >> userInput;
        // Convert user input to lowercase for easier comparison
        for (auto &c : userInput) {
            c = tolower(c);
        }

        if (userInput == "yes" || userInput == "y") {
            return true;
        } else if (userInput == "no" || userInput == "n") {
            return false;
        } else {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No: ";
        }
    }
}


string getFormat(const string& hexInstruction) {
    // Check if the instruction is long enough
    if (hexInstruction.length() < 8) {
        return "Invalid"; // Return invalid if the instruction is too short
    }

    string format = "";

    try {
        // Convert the first two characters of the hex string to a number
        unsigned int opcode = stoi(hexInstruction.substr(0, 2), nullptr, 16);

        // Check if the opcode is 0, indicating an R-format
        if (opcode == 0x0) {
            format = "R";
        } else {
            format = "I"; // Defaulting to I-format
        }
    } catch (const std::invalid_argument& e) {
        // Handle invalid hex string
        format = "Invalid";
    } catch (const std::out_of_range& e) {
        // Handle out of range error
        format = "Invalid";
    }

    return format;
}


// Display function
void displayState(const int* array, int size, const string& label) {
    cout << label << "\n------------------\n";
    for (int i = 0; i < size; i += 2) {
        cout << "$" << i << " = " << decimalToHex(array[i]);
        cout << "    $" << i + 1 << " = " << decimalToHex(array[i + 1]);
        cout << "\n";
    }
    cout << "\n";
}

// Main Function
int main()
{

    displayState(registers, 16, "Register Values");
    displayState(memory, 16, "Memory Values");

    bool userWantsToRun = displayUserMenu();

    for (int i = 0; i < 16; i++)
    {
        registers[i] = i;
    }
    for (int j = 0; j < 16; j++)
    {
        memory[j] = j;
    }

    if (userWantsToRun)
    {
        cout << "\n";
        cout << "Registers\n";
        cout << "---------------------------------\n";
        for (int i = 0; i < 16; i += 2)
        {
            cout << "$" << i << " = " << registersStr[i];
            cout << "    "
                 << "$" << i + 1 << " = " << registersStr[i + 1];

            cout << "\n";
        }
        cout << "\n";
        cout << "Memory\n";
        cout << "---------------------------------\n";
        for (int i = 0; i < 16; i += 2)
        {
            cout << "mem[" << i << "] = " << memoryStr[i];
            cout << "     "
                 << "mem[" << i + 1 << "] = " << memoryStr[i + 1];

            cout << "\n";
        }
                int num;
                cout << "\nHow many hexadecimal instructions would you like to execute?: ";
                while (!(cin >> num) || num < 0) {
                    cout << "Invalid input. Please enter a positive number: ";
                    cin.clear(); // Clear the error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the incorrect input
                }

                int currentNum = 0; // Declare and initialize currentNum variable
                while (currentNum < num)
        {
            cout << "\n";
            cout << "\n";
            cout << "Enter Hex Instruction: ";
            cin >> hexInstruction;
            string binIn = binInstruction(hexInstruction);
            cout << "\n";
            cout << "\n";
            cout << "Format: " << getFormat(hexInstruction) << "\n";
            cout << "Your hex instruction in binary: " << binIn << "\n";
            // If the first six digits for the MIPS instruction in binary are all '0', its an R-Format instruction
            if (binIn[0] == '0' && binIn[1] == '0' && binIn[2] == '0' && binIn[3] == '0' && binIn[4] == '0' &&
                binIn[5] == '0')
            {
                Rformat(binIn);
            }
            else
            {
                Iformat(binIn);
            }
            for (int i = 0; i < 16; i++)
            {
                string regHex = addZeroes(4 - decimalToHex(registers[i]).length(), decimalToHex(registers[i]));
                registersStr[i] = regHex;
            }
            for (int j = 0; j < 16; j++)
            {
                string memHex = addZeroes(4 - decimalToHex(memory[j]).length(), decimalToHex(memory[j]));
                memoryStr[j] = memHex;
            }

            cout << "\n";
            cout << "Register Values\n";
            cout << "------------------\n";
            for (int i = 0; i < 16; i += 2)
            {
                cout << "$" << i << " = " << registersStr[i];
                cout << "    "
                     << "$" << i + 1 << " = " << registersStr[i + 1];

                cout << "\n";
            }
            cout << "\n";
            cout << "\n";
            cout << "Memory Values\n";
            cout << "----------------\n";
            for (int i = 0; i < 16; i += 2)
            {
                cout << "mem[" << i << "] = " << memoryStr[i];
                cout << "     "
                     << "mem[" << i + 1 << "] = " << memoryStr[i + 1];

                cout << "\n";
            }
            currentNum++;
        }
        cout << "All instruction have been run, have a great day!";
      
    }
    else
    {
        cout << "Exiting Program."
             << "\n";
    }
}

// Function for parsing R format instruction
void Rformat(string instructionArr) {
    string destreg, firstreg, secreg, shamt, instruction;

    // Extracting different parts of the instruction
    destreg = instructionArr.substr(6, 5);
    firstreg = instructionArr.substr(11, 5);
    secreg = instructionArr.substr(16, 5);
    shamt = instructionArr.substr(21, 5);
    instruction = instructionArr.substr(26, 6);

    // Converting binary strings to integers
    int rdtemp = stoi(destreg, nullptr, 2);
    int rstemp = stoi(firstreg, nullptr, 2);
    int rttemp = stoi(secreg, nullptr, 2);
    int funct = stoi(instruction, nullptr, 2);

    // Displaying the extracted parts
    cout << "Opcode = 0\n";
    cout << "Rd (Destination Register) = $" << rdtemp << "\n";
    cout << "Rs (Source Register) = $" << rstemp << "\n";
    cout << "Rt (Target Register) = $" << rttemp << "\n";
    cout << "Shift Amount = " << stoi(shamt, nullptr, 2) << "\n";
    cout << "Function = " << funct << "\n";

    // Executing the instruction based on the function code
    switch (funct) {
        case 32:  // 'add'
            cout << "Executing: add $" << rdtemp << ", $" << rstemp << ", $" << rttemp << endl;
            add(rdtemp, rstemp, rttemp);
            cout << "Result stored in $" << rdtemp << endl;
            break;
        case 34:  // 'sub'
            cout << "Executing: sub $" << rdtemp << ", $" << rstemp << ", $" << rttemp << endl;
            sub(rdtemp, rstemp, rttemp);
            cout << "Result stored in $" << rdtemp << endl;
            break;
        case 36:  // 'and'
            cout << "Executing: and $" << rdtemp << ", $" << rstemp << ", $" << rttemp << endl;
            AND(rdtemp, rstemp, rttemp);
            cout << "Result stored in $" << rdtemp << endl;
            break;
        case 37:  // 'or'
            cout << "Executing: or $" << rdtemp << ", $" << rstemp << ", $" << rttemp << endl;
            OR(rdtemp, rstemp, rttemp);
            cout << "Result stored in $" << rdtemp << endl;
            break;
        default:
            cout << "Unknown function code.\n";
            break;
    }
}

// Function for parsing I format instruction
void Iformat(string instructArr)
{
    int instructIdent = 0, opIdent = 0, rsIdent = 0, rtIdent = 0;
    string optemp, firstreg, secreg, address;

        opIdent = binaryToDecimal(optemp);
rsIdent = binaryToDecimal(firstreg);
rtIdent = binaryToDecimal(secreg);
instructIdent = binaryToDecimal(address);

    char iform[32];
    // parse the instruction into respective pieces

        for (int i = 0; i < 32; i++) {
        iform[i] = instructArr[i];
    }

    // Extract the opcode
    for (int i = 0; i < 6; i++) {
        optemp += iform[i];
    }

    // Extract the source register
    for (int i = 6; i < 11; i++) {
        firstreg += iform[i];
    }

    // Extract the target register
    for (int i = 11; i < 16; i++) {
        secreg += iform[i];
    }

    // Extract the immediate value or offset
    for (int i = 16; i < 32; i++) {
        address += iform[i];
    }

opIdent = binaryToDecimal(optemp);
rsIdent = binaryToDecimal(firstreg);
rtIdent = binaryToDecimal(secreg);
instructIdent = binaryToDecimal(address);


    cout << "Opcode = ";
int opcode = binaryToDecimal(optemp);
opIdent = opcode;  // storing the opcode
cout << opcode << "\n";

cout << "Rs = $";
int rs = binaryToDecimal(firstreg);
rsIdent = rs;  // Store the source register identifier
cout << rs << "\n";

cout << "Rt = $";
int rt = binaryToDecimal(secreg);
rtIdent = rt;  // Store the target register identifier
cout << rt << "\n";

cout << "Immediate/Offset = ";
int immediate = binaryToDecimal(address);
instructIdent = immediate;  // Store the immediate value or offset
cout << immediate << "\n";

    // if statements controlling which instructions are run
    switch (opIdent) {
        case 35: // Opcode for 'lw'
            cout << "Executing: lw $" << rtIdent << ", " << instructIdent << "($" << rsIdent << ")\n";
            lw(rtIdent, rsIdent, instructIdent);
            break;
        case 43: // Opcode for 'sw'
            sw(rtIdent, rsIdent, instructIdent);
            cout << "Executing: sw $" << rtIdent << ", " << instructIdent << "($" << rsIdent << ")\n";
            break;
        case 8: // Opcode for 'addi'
            addi(rtIdent, rsIdent, instructIdent);
            cout << "Executing: addi $" << rtIdent << ", $" << rsIdent << ", " << instructIdent << endl;
            break;
        case 4: // Opcode for 'beq'
            bridgeIfEqual(rsIdent, rtIdent, instructIdent);
            cout << "Executing: beq $" << rsIdent << ", $" << rtIdent << ", " << instructIdent << endl;
            break;
        case 5: // Opcode for 'bne'
            bridgeIfNotEqual(rsIdent, rtIdent, instructIdent);
            cout << "Executing: bne $" << rsIdent << ", $" << rtIdent << ", " << instructIdent << endl;
            break;
        default:
            cout << "Unknown opcode: " << opIdent << endl;
            break;
    }
}

// MIPS instruction functions
// Function for bitwise OR of two registers
void OR(int rs, int rt, int rd) {
    cout << "Or $" << rd << " = $" << rs << " || $" << rt << "\n";

    int reg = registers[rs];
    int reg2 = registers[rt];

    string binOne = decimalToBinaryString(reg);
    string binTwo = decimalToBinaryString(reg2); // Convert to binary string

    // Pad strings to ensure equal length
    while (binOne.length() < binTwo.length()) binOne = "0" + binOne;
    while (binTwo.length() < binOne.length()) binTwo = "0" + binTwo;

    string res = "";
    for (int i = 0; i < binOne.length(); i++) {
        res += (binOne[i] == '1' || binTwo[i] == '1') ? '1' : '0';
    }

    int binDec = binaryToDecimal(res);  // Convert back to decimal
    registers[rd] = binDec;
    cout << "$" << rd << " = " << decimalToHex(binDec) << "\n"; // Print hex value
}

// Function for bitwise AND of two registers
void AND(int rs, int rt, int rd) {
    cout << "And $" << rd << " = $" << rs << " && $" << rt << "\n";

    string binOne = decimalToBinaryString(registers[rs]);
    string binTwo = decimalToBinaryString(registers[rt]);

    // Pad strings to ensure equal length
    while (binOne.length() < binTwo.length()) binOne = "0" + binOne;
    while (binTwo.length() < binOne.length()) binTwo = "0" + binTwo;

    string res = "";
    for (int i = 0; i < binOne.length(); i++) {
        res += (binOne[i] == '1' && binTwo[i] == '1') ? '1' : '0';
    }

    int binDec = binaryToDecimal(res);  // Convert back to decimal
    registers[rd] = binDec;
    cout << "$" << rd << " = " << decimalToHex(binDec) << "\n"; // Print hex value

}

// Function for adding two registers
void add(int rs, int rt, int rd) {
    cout << "Add   $" << rd << " = $" << rs << " + $" << rt << "\n";

    long long sum = static_cast<long long>(registers[rs]) + static_cast<long long>(registers[rt]);

    // Overflow detection
    if (sum > std::numeric_limits<int>::max() || sum < std::numeric_limits<int>::min()) {
        cout << "Error: Arithmetic overflow" << endl;
    } else {
        registers[rd] = static_cast<int>(sum);
        cout << "$" << rd << " = " << registers[rd] << "\n\n";
    }
}

// Function for subtracting two registers
void sub(int rs, int rt, int rd) {
    cout << "Sub $" << rd << " = $" << rs << " - $" << rt << "\n";

    long long difference = static_cast<long long>(registers[rs]) - static_cast<long long>(registers[rt]);

    // Overflow detection
    if (difference > std::numeric_limits<int>::max() || difference < std::numeric_limits<int>::min()) {
        cout << "Error: Arithmetic overflow" << endl;
    } else {
        registers[rd] = static_cast<int>(difference);
        cout << "$" << rd << " = " << registers[rd] << "\n\n";
    }
}

// Function for loading a word from memory into a register
void lw(int tempRt, int tempRs, int addToMem) {
    // Fetching values from the registers
    int rt = tempRt;
    int rs = tempRs;
    int rsVal = registers[tempRs];
    int offset = addToMem;

    // Calculating the effective memory address
    int memAddress = rsVal + offset;

    // Boundary check for memory access
    if (memAddress < 0 || memAddress >= sizeof(memory)/sizeof(memory[0])) {
        cout << "Error: Memory access out of bounds at address " << memAddress << endl;
        return;
    }

    // Loading the word from memory into the register
    registers[rt] = memory[memAddress];

    // Debugging output for clarity
    cout << "lw  $" << rt << ", " << offset << "($" << rs << ")" << "\n";
    cout << "Effective Memory Address: " << memAddress << "\n";
    cout << "Loaded value: " << memory[memAddress] << " into register $" << rt << "\n\n";
}


// Function for storing a word from a register into memory
void sw(int tempRt, int tempRs, int part) {
    // Fetching values from the registers
    int rt = tempRt;
    int rs = tempRs;
    int offset = part;

    // Calculating the effective memory address
    int memAddress = rs + offset;

    // Boundary check for memory access
    if (memAddress < 0 || memAddress >= sizeof(memory)/sizeof(memory[0])) {
        cout << "Error: Memory access out of bounds at address " << memAddress << endl;
        return;
    }

    // Storing the value from the register into memory
    memory[memAddress] = registers[rt];

    // Debugging output for clarity
    cout << "sw  $" << rt << ", " << offset << "($" << rs << ")" << "\n";
    cout << "Stored value: " << registers[rt] << " from register $" << rt << " to memory address " << memAddress << "\n\n";
}

// Function for adding an immediate value to a register
void addi(int tempRt, int tempRs, int part) {
    // Fetching value from the source register
    int rsValue = registers[tempRs];
    int immediate = part;

    // Adding immediate value to the content of the source register
    int sum = rsValue + immediate;

    // Storing the result in the target register
    registers[tempRt] = sum;

    // Debugging output for clarity
    cout << "addi $" << tempRt << ", $" << tempRs << ", " << immediate << "\n";
    cout << "Result stored in $" << tempRt << ": " << sum << "\n\n";
}

// Function for branching if two registers are equal
void bridgeIfEqual(int first, int sec, int offset) {
    int pc = 0; // Declare and initialize 'pc' variable

    if (registers[first] == registers[sec]) {
        cout << "$" << first << " and $" << sec << " are equal.\n";
        pc += offset;  // Update the program counter
        cout << "Branch taken. New PC: " << pc << "\n";
    } else {
        cout << "Branch not taken. $" << first << " and $" << sec << " are not equal.\n";
    }
}


// Function for branching if two registers are not equal
void bridgeIfNotEqual(int first, int sec, int offset) {
    if (registers[first] != registers[sec]) {
        cout << "$" << first << " and $" << sec << " are not equal.\n";
         pc += offset;  // Update the program counter
        cout << "Branch taken. New PC: " << pc << "\n";
    } else {
        cout << "Branch not taken. $" << first << " and $" << sec << " are equal.\n";
    }
}

// Function for converting decimal values to hexadecimal
string decimalToHex(int decimal) {
    if (decimal == 0) {
        return "0"; // Handling zero case
    }

    char hexa[50]; // Array to hold hexadecimal digits
    int temp = decimal;
    int rem, j = 0;
    string answer = "";

    while (temp > 0) {
        rem = temp % 16;
        hexa[j++] = (rem < 10) ? (48 + rem) : (55 + rem); // Using ternary operator for conciseness
        temp = temp / 16;
    }

    for (int i = j - 1; i >= 0; i--) {
        answer += hexa[i];
    }

    return answer;
}

// Function for converting decimal values to binary
string decimalToBinary(int n) {
    if (n == 0) {
        return "0"; // Handling zero case
    }

    string bin = "";
    while (n != 0) {
        bin = to_string(n % 2) + bin; // Prepending the binary digit
        n /= 2;
    }

    return bin;
}

// function for converting binary values to decimal
int binaryToDecimal(string binaryStr) {
    int decimalNumber = 0;
    int base = 1;

    for (int i = binaryStr.length() - 1; i >= 0; i--) {
        if (binaryStr[i] == '1') {
            decimalNumber += base;
        }
        base *= 2;
    }

    return decimalNumber;
}

// function for converting decimal values to binary
int intDecimal(int n, int i) {
    if (n == 0) {
        return 0;
    }

    // Use bitwise shift instead of pow for efficiency
    return ((n % 10) << i) + intDecimal(n / 10, i + 1);
}

// Function to convert binary to hexadecimal
string binaryToHex(int n) {
    if (n == 0) {
        return "0";  // Handle zero case
    }

    // Recursively process each 4-bit chunk
    string hexaPart = binaryToHex(n / 10000);

    // Convert the last 4 binary digits to decimal
    int lastFourBits = intDecimal(n % 10000, 0);

    // Map decimal to hexadecimal character
    char hexChar = (lastFourBits < 10) ? ('0' + lastFourBits) : ('A' + lastFourBits - 10);

    // Combine and return the hexadecimal string
    return hexaPart + hexChar;
}

// Function to convert hexadecimal values to binary
string hexToBinary(string hexStr) {
    string binStr = "";
    for (int i = 0; i < hexStr.length(); i++) {
        char hexChar = hexStr[i];
        string bin = "";

        // Switch case for each hexadecimal character
        switch (hexChar) {
            case '0': bin = "0000"; break;
            case '1': bin = "0001"; break;
            case '2': bin = "0010"; break;
            case '3': bin = "0011"; break;
            case '4': bin = "0100"; break;
            case '5': bin = "0101"; break;
            case '6': bin = "0110"; break;
            case '7': bin = "0111"; break;
            case '8': bin = "1000"; break;
            case '9': bin = "1001"; break;
            case 'A': case 'a': bin = "1010"; break;
            case 'B': case 'b': bin = "1011"; break;
            case 'C': case 'c': bin = "1100"; break;
            case 'D': case 'd': bin = "1101"; break;
            case 'E': case 'e': bin = "1110"; break;
            case 'F': case 'f': bin = "1111"; break;
            default:
                // Handle invalid hexadecimal character
                bin = ""; // Or some error handling
                break;
        }

        // Append the binary equivalent to the result string
        binStr += bin;
    }
    return binStr;
}

// Function to convert hexadecimal values to binary
string binInstruction(string hexValue) {
    long int i = 0;
    string instruction;
    while (hexValue[i]) {
        switch (hexValue[i]) {
            // Conversion of each hex digit to binary
            case '0': instruction += "0000"; break;
            case '1': instruction += "0001"; break;
            case '2': instruction += "0010"; break;
            case '3': instruction += "0011"; break;
            case '4': instruction += "0100"; break;
            case '5': instruction += "0101"; break;
            case '6': instruction += "0110"; break;
            case '7': instruction += "0111"; break;
            case '8': instruction += "1000"; break;
            case '9': instruction += "1001"; break;
            case 'A': case 'a': instruction += "1010"; break;
            case 'B': case 'b': instruction += "1011"; break;
            case 'C': case 'c': instruction += "1100"; break;
            case 'D': case 'd': instruction += "1101"; break;
            case 'E': case 'e': instruction += "1110"; break;
            case 'F': case 'f': instruction += "1111"; break;
            default:
                // Handle invalid hexadecimal character
                cout << "ERROR, invalid hexadecimal MIPS instruction: " << hexValue[i] << endl;
                return ""; // Optionally return an error code
        }
        i++;
    }
    return instruction;
}

// Function to add leading zeroes to a string
string addZeroes(int zeroes, string str) {
    string newString = "";

    // Add the specified number of zeroes to the beginning of newString
    for (int i = 0; i < zeroes; i++) {
        newString += "0";
    }

    // Append the original string to the newString
    newString += str;

    return newString;
}
