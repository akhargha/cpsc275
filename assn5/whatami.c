/*
 * Program: whatami.c 
 * Purpose: Checks if a machine is big-endian or little-endian depending on the byte-ordering convention adopted by the machine.
 * Author:  Anupam Khargharia
 * Date:    10/13/2023
 */

#include <stdio.h>

int main() {
    int n = 0x11223344; //assign so that it is obvious that 11 is the most significant byte.
    char *pointer = (char *) &n; //point to first byte
    if (*pointer != 0x11){ //if most significant byte is not first
        printf("I'm on a little-endian machine.\n");
    }
    else{
        printf("I'm on a big-endian machine.\n");
    }
}