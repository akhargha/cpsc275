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
                running = 0; //STOP
                break;
            }
            case LOAD: {
                accumulator = memory[operand]; //put memory value to accumulator
                break;
            }
            case STORE: {
                memory[operand] = accumulator; //put accumulator value to memory
                break;
            }
            case READ: {
                char binaryInput[17]; //for binary input
                scanf("%16s", binaryInput); // Read 16-bit binary number as string
                memory[operand] = bin2num(binaryInput); //convert to num and put into memory
                break;
            }
            case WRITE: {
                printf("%d\n", memory[operand]); //print value
                break;
            }
            case ADD: {
                accumulator += memory[operand]; //add accumulator to mem value
                break;
            }
            case SUB: {
                accumulator -= memory[operand]; //sub mem value from accumulator
                break;
            }
            case MUL: {
                accumulator *= memory[operand]; //multiply mem value to accumulator
                break;
            }
            case DIV: {
                accumulator /= memory[operand]; //div accumulator by mem value
                break;
            }
            case MOD: {
                unsigned short remainder = accumulator % memory[operand]; //find remainder of accumulator/mem value
                accumulator = remainder; //save remainder to accumulator
                break;
            }
            case NEG: {
                accumulator *= -1; //negate the accumulator value
                break;
            }
            case NOP: {
                break; //do nothing
            }
            case JUMP: {
                instructionCounter = operand; //jump to operand
                instructionCounter--; //subl IC so that the operand stays the same when it increments after the switch cases.
                break;
            }
            case JNEG: {
                if (accumulator & 0x8000){ //if accumulator is negative (8000 = 1000 0000 0000 0000)
                    instructionCounter = operand;
                }
                instructionCounter--; //subl IC so that the operand stays the same when it increments after the switch cases.
                break;
            }
            case JZERO: {
                if (accumulator == 0){ //if accumulator is zero
                    instructionCounter = operand;
                }
                instructionCounter--; //subl IC so that the operand stays the same when it increments after the switch cases.
                break;
            }
            case HALT: {
                // Handle the HALT operation
                running = 0; //stop
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

    int print_index = 1024;
    for(int i = 512; i < 612; i += 10) {
        printf("%4d ", print_index);
        for(int j = i; j < i + 10 && j < 612; ++j) {
            printf("%04x ", memory[j]);
        }
        printf("\n");
        print_index += 10; // Increment print index for next line
    }
}

int main() {
    // Initialize memory with zeros
    memset(memory, 0, sizeof(memory));

    // Load the VSML program into memory from standard input
    loadVSMLProgram();

    // Execute the VSML program
    executeVSMLProgram();

    //dump
    printMemory();

    return 0;
}
