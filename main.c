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

    printf("---------------------------------\n");
    printf("            CALCULATOR          \n");
    printf("---------------------------------\n");
    printf("RULES:\n");
    printf("1. All inputs must be separated by one blank space.\n");
    printf("2. There should be no space after '(' and before ')'.\n");
    printf("3. All braces must be balanced.\n");
    printf("4. For negative numbers, there should be no space between the sign and the number.\n");
    printf("5. An input in any other format will cause an error message to be displayed and the program will terminate.\n");

    while(1) {

        init(&L);

        printf("\n\n");
        printf("0. Exit\n");
        printf("1. Calculator\n");
        printf("2. Base Change\n");
        printf("3. Trigonometric functions\n");
        printf("Choice: ");
        scanf("%d", &ch);
        getchar();
        printf("\n");

        if(ch <= 0 || ch > 3) {
            exit(0);
        }
        else if(ch == 1) {
            gets(s);            //Accepts the expression to be evaluated
            infix_eval(s);      //Evaluates the expression and displays the answer
        }
        else if(ch == 2){
            printf("\n");
            printf("Enter the base to be converted to (2 - 9): ");
            scanf("%d", &base);
            getchar();

            printf("\n");
            printf("Enter the number in decimal (base 10): ");

            if(base >= 2 && base <= 9) {
                gets(s);
                L = extract(s);
                change_base(L, base);       //Changes the base of the number and displays the answer
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
                trigo_fn(L, 1);         //1 corresponds to sin
            }
            else if(choice == 2) {
                trigo_fn(L, 2);         //2 corresponds to cos
            }
            else {
                printf("Incorrect option\n");
                exit(1);
            }
        }
    }
    return 0;
}
