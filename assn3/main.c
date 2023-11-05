#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 2048
#define MAX_INSTRUCTION_SIZE 16
#define DATA_SECTION_START 1024

unsigned short memory[MEMORY_SIZE/2];

#define EOC 0x0
#define READ 0x3

// VSM Registers
unsigned short accumulator = 0;
unsigned short instructionCounter = 0;
unsigned short instructionRegister = 0;
unsigned short opCode = 0;
unsigned short operand = 0;


// Function to convert binary string to number
unsigned short bin2num(const char *binaryString) {
    unsigned short value = 0;
    for (int i = 0; i < MAX_INSTRUCTION_SIZE; i++) {
        value = value * 2 + (binaryString[i] - '0');
    }
    return value;
}

// Function to load the VSML program into memory
void loadVSMLProgram() {
    char line[MAX_INSTRUCTION_SIZE + 1];
    int memoryIndex = 0;
    int charIndex = 0;
    char ch;

    // Read each character from standard input
    while ((ch = getchar()) != EOF && memoryIndex < DATA_SECTION_START / 2) {
        if (ch == '\n') { // Handle newline
            line[charIndex] = '\0'; // Null terminate the line

            // Convert binary string to an unsigned short and store in memory
            memory[memoryIndex++] = bin2num(line);

            charIndex = 0; // Reset the line index

            // If EOC is encountered, break the loop
            if (memory[memoryIndex - 1] == EOC) {
                break;
            }
        } else {
            if (charIndex < MAX_INSTRUCTION_SIZE) {
                line[charIndex++] = ch; // Add the char to the line
            }
        }
    }

    // Process the last line if it didn't end with a newline
    if (charIndex == MAX_INSTRUCTION_SIZE) {
        line[charIndex] = '\0'; // Null terminate the line
        
        // Convert binary string to an unsigned short and store in memory
        memory[memoryIndex++] = bin2num(line);
    }
}

void executeVSMLProgram() {
    int running = 1;
    while (running) {
        // Fetch the next instruction
        instructionRegister = memory[instructionCounter];
        opCode = (instructionRegister >> 12) & 0xF;
        operand = instructionRegister & 0xFFF;
        printf("%d rand", operand);
        printf("%d CODE", opCode);
        
        switch (opCode) {
            case READ: {
                // Handle the READ operation
                printf("READ\n");
                int inputNumber;
                scanf("%d", &inputNumber);
                memory[operand/2] = (unsigned short)inputNumber;
                break;
            }
            case EOC: {
                // Handle the EOC operation
                running = 0;
                break;
            }
            default: {
                // Handle unknown operation
                printf("Unknown operation code: %04x\n", opCode);
                running = 0;
                break;
            }
        }

        // Move to the next instruction
        instructionCounter += 1;
    }
}

// Function to print the memory contents
void printMemory() {
    printf("REGISTERS:\n");
    printf("accumulator          0x%04x\n", accumulator);
    printf("instructionCounter   0x%04x\n", instructionCounter);
    printf("instructionRegister  0x%04x\n", instructionRegister);
    printf("opCode               0x%x\n", opCode);
    printf("operand              0x%04x\n\n", operand);

    printf("CODE:\n");
    for (int i = 0; i < DATA_SECTION_START / 2; i++) {
        if (i % 10 == 0) {
            if (i > 0) {
                printf("\n");
            }
            printf("%04d ", i * 2);
        }
        printf("%04x ", memory[i]);
    }
    printf("\n\nDATA:\n");
    for (int i = DATA_SECTION_START / 2; i < MEMORY_SIZE / 2; i++) {
        if (i % 10 == 0) {
            if (i > DATA_SECTION_START / 2) {
                printf("\n");
            }
            printf("%04d ", i * 2);
        }
        printf("%04x ", memory[i]);
    }
    printf("\n");
}

int main() {
    // Initialize memory with zeros
    memset(memory, 0, sizeof(memory));

    // Load the VSML program into memory from standard input
    loadVSMLProgram();

    // Execute the VSML program
    executeVSMLProgram();

    // Print memory contents for verification
    printMemory();

    return 0;
}
