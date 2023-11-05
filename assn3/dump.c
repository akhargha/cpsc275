#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 2048
#define MAX_INSTRUCTION_SIZE 16
#define DATA_SECTION_START 1024

// The VSM memory
unsigned short memory[MEMORY_SIZE / 2]; // each instruction 2 byte

unsigned short bin2num(const char *binaryString) {
    unsigned short value = 0;
    for (int i = 0; i < MAX_INSTRUCTION_SIZE; i++) {
        value = value * 2 + (binaryString[i] - '0');
    }
    return value;
}

void loadVSMLProgram() {
    char line[MAX_INSTRUCTION_SIZE + 1];
    int memoryIndex = 0;
    int charIndex = 0;
    char ch;

    // Read each character from standard input
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') { // Handle newline
                line[charIndex] = '\0'; // Null terminate the line

                // Convert binary string to an unsigned short and store in memory
                memory[memoryIndex++] = bin2num(line);

                charIndex = 0; // Reset the line index
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


void printMemory() {
    printf("Code Section:\n");
    for (int i = 0; i < DATA_SECTION_START / 2; i++) {
        printf("%04d\t%04x\n", i * 2, memory[i]);
    }

    printf("\nData Section:\n");
    for (int i = DATA_SECTION_START / 2; i < MEMORY_SIZE / 2; i++) {
        printf("%04d\t%04x\n", i * 2, memory[i]);
    }

}

int main() {
    // Initialize memory with zeros
    for (int i = 0; i < MEMORY_SIZE / 2; i++) {
        memory[i] = 0;
    }

    // Load the VSML program into memory from standard input
    loadVSMLProgram();

    // Print memory contents for verification
    printMemory();

    return 0;
}
