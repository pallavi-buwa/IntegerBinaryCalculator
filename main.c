#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "DLL.h"

int main()
{
    int ch = 0, base = 0, choice = 0;
    char s[50] = {};
    list L;
    init(&L);

    while(1) {

        init(&L);

        printf("\n\n");
        printf("0. Exit\n");
        printf("1. Calculator\n");
        printf("2. Base Change\n");
        printf("3. Trignometric functions\n");
        printf("Choice: ");
        scanf("%d", &ch);
        getchar();
        printf("\n");

        if(ch <= 0 || ch > 3) {
            exit(0);
        }
        else if(ch == 1) {
            gets(s);
            infix_eval(s);
        }
        else if(ch == 2){
            printf("\n\n");
            printf("Enter the base to be converted to (2 - 9)\n");
            scanf("%d", &base);
            getchar();
            printf("\n");

            printf("Enter the number in decimal: ");

            if(base >= 2 && base <= 9) {
                gets(s);
                L = extract(s);
                change_base(L, base);
            }
            else {
                printf("Incorrect base\n");
                exit(1);
            }
        }
        else {
            printf("1. sin\n");
            printf("2. cos\n");
            printf("Choice :");
            scanf("%d", &choice);
            getchar();
            printf("\n");

            printf("Enter the angle in degrees: ");
            gets(s);
            L = extract(s);
            if(choice == 1) {
                trigo_fn(L, 1);
            }
            else if(choice == 2) {
                trigo_fn(L, 2);
            }
            else {
                printf("Incorrect option\n");
                exit(1);
            }
        }
    }
    return 0;
}
