#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 2048
#define MAX_INSTRUCTION_SIZE 16
#define DATA_SECTION_START 1024

// Define the op-codes
#define EOC 0x0
#define LOAD 0x1
#define STORE 0x2
#define READ 0x3
#define WRITE 0x4
#define ADD 0x5
#define SUB 0x6
#define MUL 0x7
#define DIV 0x8
#define MOD 0x9
#define NEG 0xA
#define NOP 0xB
#define JUMP 0xC
#define JNEG 0xD
#define JZERO 0xE
#define HALT 0xF

// The VSM memory
unsigned short memory[MEMORY_SIZE / 2]; // each instruction 2 bytes

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

// Function to execute the VSML program
void executeVSMLProgram() {
    int running = 1;
    while (running) {
        // Fetch the next instruction
        instructionRegister = memory[instructionCounter];
        opCode = (instructionRegister >> 12) & 0xF;
        operand = instructionRegister & 0xFFF;
        operand = operand / 2;
        
        switch (opCode) {
            case EOC: {
                // Handle the EOC operation
                running = 0;
                break;
            }
            case LOAD: {
                // Handle the LOAD operation
                accumulator = memory[operand];
                break;
            }
            case STORE: {
                // Handle the STORE operation
                memory[operand] = accumulator;
                break;
            }
            case READ: {
                // Handle the READ operation
                char binaryInput[17]; // Buffer for binary input
                scanf("%16s", binaryInput); // Read 16-bit binary number as string
                memory[operand] = bin2num(binaryInput);
                break;
            }
            case WRITE: {
                printf("%d\n", memory[operand]);
                break;
            }
            case ADD: {
                // Handle the ADD operation
                accumulator += memory[operand];
                break;
            }
            case SUB: {
                // Handle the SUB operation
                accumulator -= memory[operand];
                break;
            }
            case MUL: {
                // Handle the MUL operation
                accumulator *= memory[operand];
                break;
            }
            case DIV: {
                // Handle the DIV operation
                accumulator /= memory[operand];
                break;
            }
            case MOD: {
                // Handle the MOD operation
                unsigned short remainder = accumulator % memory[operand];
                accumulator = remainder;
                break;
            }
            case NEG: {
                accumulator *= -1;
                break;
            }
            case NOP: {
                break;
            }
            case JUMP: {
                instructionCounter = operand;
                break;
                instructionCounter--;
            }
            case JNEG: {
                if (accumulator & 0x8000){
                    instructionCounter = operand;
                }
                instructionCounter--;
                break;
            }
            case JZERO: {
                if (accumulator == 0){
                    instructionCounter = operand;
                }
                instructionCounter--;
                break;
            }
            case HALT: {
                // Handle the HALT operation
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
    for (int i = 0; i < 10; i++) {
        printf("    %x", i);
    }
    printf("\n");

    for (int i = 0; i < 100; i++) {
        if (i % 10 == 0) {
            printf("%04d ", i);
        }
        printf("%04x ", memory[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }

    printf("\n\nDATA:\n");
    for (int i = 0; i < 10; i++) {
        printf("    %x", i);
    }
    printf("\n");

    for (int i = 512; i < 562; i++){
        if ((i * 2 - 1024) % 10 == 0){
            printf("%d ", i * 2);
        }
        if (((i + 1) * 2 - 1024) % 10 == 0){
            printf("\n");
        }
    }

    for (int i = DATA_SECTION_START / 2; i < (DATA_SECTION_START / 2) + 100; i++) {
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
