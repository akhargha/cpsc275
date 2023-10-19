#include <stdio.h>
#include <string.h>
#include <math.h>

unsigned short binstr2hex(char *tokens){
    char *p;
    int decimal = 0;
    unsigned short answer = 0;
    int num = 0;

    int len = strlen(tokens);
    
    for (p = tokens + len - 1; p > tokens - 1; p--){
        int multiply = 1;
        if (*p=='1'){
            for (int i = 0; i < num; i++){
                multiply*=2;
            }
            decimal += multiply;
        }    
        num++;
    }
    printf("%d result\n", decimal);
}

void main() {
    char temp[17];
    char tokens[17];
    while (scanf("%16s", temp)!= EOF){
        binstr2hex(temp);
        /**
        int len = strlen(temp);


        printf("%d\n",len);
        for (int i = 0; i < 13; i++){
            printf("%c", temp[i]);
        }
        printf("\n");
        */
    }
}