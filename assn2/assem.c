#include <stdio.h>
#include <string.h>

unsigned short binstr2num(char *binary) {
    unsigned short num = 0;
    for (int i = 0; i < 16; i++) {
        if (binary[i] == '1') {
            num = num + (1 << (15 - i));
        }
    }
    return num;
}

int decode(unsigned short instr, unsigned short *opcode, char *mnem, unsigned short *opn) {
    
    *opcode = (instr >> 12) & 0xF; // first 4 bits
    int m = (instr >> 11) & 0x1; //5th bit
    *opn = instr & 0x7FF; //last 11 bits

    if (*opcode == 0x0) {
        strcpy(mnem, "EOC   ");
    } else if (*opcode == 0x1) {
        strcpy(mnem, "LOAD  ");
    } else if (*opcode == 0x2) {
        strcpy(mnem, "STORE ");
    } else if (*opcode == 0x3) {
        strcpy(mnem, "READ  ");
    } else if (*opcode == 0x4) {
        strcpy(mnem, "WRITE ");
    } else if (*opcode == 0x5) {
        strcpy(mnem, "ADD   ");
    } else if (*opcode == 0x6) {
        strcpy(mnem, "SUB   ");
    } else if (*opcode == 0x7) {
        strcpy(mnem, "MUL   ");
    } else if (*opcode == 0x8) {
        strcpy(mnem, "DIV   ");
    } else if (*opcode == 0x9) {
        strcpy(mnem, "MOD   ");
    } else if (*opcode == 0xA) {
        strcpy(mnem, "NEG   ");
    } else if (*opcode == 0xB) {
        strcpy(mnem, "NOP   ");
    } else if (*opcode == 0xC) {
        strcpy(mnem, "JUMP  ");
    } else if (*opcode == 0xD) {
        strcpy(mnem, "JNEG  ");
    } else if (*opcode == 0xE) {
        strcpy(mnem, "JZERO ");
    } else if (*opcode == 0xF) {
        strcpy(mnem, "HALT  ");
    } else {
        strcpy(mnem, "UNKNOWN");
    }

    return m;
}

int main() {
    char temp[17];
    char mnem[8];
    unsigned short opcode, opn, instruction;

    while (scanf("%16s", temp)!= EOF){
        unsigned short number = binstr2num(temp);
        decode(number, &opcode, mnem, &opn);
        printf("%04X %s %04d\n", number, mnem, opn);
    }
    return 0;
}
