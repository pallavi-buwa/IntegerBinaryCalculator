#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "DLL.h"

int main()
{
    int ch = 0, ch1 = 0;
    char s[50] = {};
    list L;
    init(&L);

    while(1) {
        printf("\n\n");
        printf("0. Exit\n");
        printf("1. Calculator\n");
        printf("2. Base Change\n");
        printf("Choice: ");
        scanf("%d", &ch);
        getchar();
        printf("\n");

        if(ch <= 0 || ch > 2) {
            exit(0);
        }
        else if(ch == 1) {
            gets(s);
            infix_eval(s);
        }
        else {
            printf("\n\n");
            printf("1. decimal to binary\n");
            printf("2. decimal to octal\n");
           // printf("3. decimal to hexadecimal\n");
            printf("0. exit\n");
            printf("Choice: ");
            scanf("%d", &ch1);
            getchar();
            printf("\n");

            if(ch1 == 1) {
                gets(s);
                L = extract(s);
                change_base(L, 2);
            }
            else if(ch1 == 2) {
                gets(s);
                L = extract(s);
                change_base(L, 8);
            }
            /*else if(ch1 == 3) {
                gets(s);
                L = extract(s);
                change_base(L, 16);
            }*/
            else {
                exit(0);
            }
        }
    }


    return 0;
}
