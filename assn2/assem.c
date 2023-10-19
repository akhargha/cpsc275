#include <stdio.h>
#include <string.h>

void main() {
    char tokens[17];
    char temp[17];
    while (scanf("%16s", tokens)!= EOF){
        int len = strlen(tokens);

        for (int i = 0; i < 16-len; i++){
            temp[i] = '0';
        }

        for (int i = 16-len; i < 16; i++){
            temp[i] = tokens[i - 16 + len];
        }

        temp[16] = '\0';

    }
}