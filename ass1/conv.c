/*
 * Program: conv.c 
 * Purpose: Converts decimal to binary and hex representation and prints it
 * Author:  Anupam Khargharia
 * Date:    10/13/2023
 */

#include <stdio.h>

/*
 * Function: dec2bin
 * Purpose:  Prints the binary representation of a decimal number by storing the number's remainder (by 2) in an array.
 *
 * Parameters:
 *    n: a positive number
 *
 * Returns: void (does not return anything but prints the binary representation).
 */
void dec2bin(int n) {
    int binary[sizeof(n) * 8]; //store binary according to system
    int i = 0;

    while (n != 0) { // till recurring n divided by 2 is not completed
        binary[i] = n % 2; //remainder
        i++;
        n = n / 2;
    }

    printf("0b");

    // print the result in reverse order
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binary[j]);
    }
}

/*
 * Function: dec2hex
 * Purpose:  Prints the hex representation of a decimal number by storing the number's remainder (by 16) in an array.
 *
 * Parameters:
 *    n: a positive number
 *
 * Returns: void (does not return anything but prints the hex representation).
 */
void dec2hex(int n) {
    char hexadecimal[sizeof(n) * 8];  //store hex according to system
    int i = 0;
    while (n != 0) { // till recurring n divided by 16 is not completed
        int temp = n % 16; //remainder
        n = n / 16;
        if (temp < 10) {
            hexadecimal[i] = '0' + temp; //save num as char
        } else {
            hexadecimal[i] = 'a' + (temp - 10); //convert num to corresponding char
        }
        i++;
    }

    printf("0x");
    
    // print the result in reverse order
    for (int j = i - 1; j >= 0; j--) {
        printf("%c", hexadecimal[j]);
    }
}

//execute code
int main() {
    int n;
    scanf("%d", &n);
    if (n<0){
        printf("Negative decimals not accepted. \n");
        return 0;
    }
    printf("%d = ", n);
    dec2bin(n);
    printf(" = ");
    dec2hex(n);
    printf("\n");
}
