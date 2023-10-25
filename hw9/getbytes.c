#include <stdio.h>

int main() {
    unsigned int x = 0x12345678;

    // Print the first byte (0x12)
    printf("0x%02x\n", (x >> 24) & 0xFF);

    // Print the second byte (0x34)
    printf("0x%02x\n", (x >> 16) & 0xFF);

    // Print the third byte (0x56)
    printf("0x%02x\n", (x >> 8) & 0xFF);

    // Print the fourth byte (0x78)
    printf("0x%02x\n", x & 0xFF);

    return 0;
}