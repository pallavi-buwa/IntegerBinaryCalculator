#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "DLL.h"
#include <string.h>

#define PI 3.14159

//initializing a list
void init(list* l) {

    l->n = NULL;
    l->tail = NULL;
    l->length = 0;
    l->sign = '+';      //default sign set to + if none is entered
    return;
}


//accepts a number as a string and converts it to type "list"
list extract(char* s) {

    list l;
    init(&l);

    int x = 0, i = 0;
    char ch = s[0];

    if(ch == '-') {
        l.sign = '-';
    }
    else {
        l.sign = '+';
    }
    while(s[i] != '\0') {
        if(isdigit(s[i])) {
            x = s[i] - '0';     //extracting a character and converting to 'int'
            append(&l, x);
        }
        i++;
    }

    return l;
}

//add a node at the end of the list
void append(list *l, int data) {

    node *newnode;
    newnode = (node*)malloc(sizeof(node));      //creating a new node

    if(newnode) {
        newnode->next = NULL;
        newnode->d = data;
        newnode->prev = NULL;
    }
    else {
        return;
    }

    if(l->n == NULL) {                  //if the node to be added is the first node
        l->n = newnode;
        l->tail = newnode;
        l->length = 1;
        return;
    }

    newnode->prev = l->tail;
    l->tail->next = newnode;
    l->tail = newnode;
    l->length = l->length + 1;

    return;
}


//add a node to the beginning of the list
void add_ahead(list* l, int data) {

    node *p, *newnode;
    p = l->n;
    newnode = (node*)malloc(sizeof(node));          //creating a new node

    if(newnode) {
        newnode->d = data;
        newnode->next = NULL;
        newnode->prev = NULL;
    }
    else {
        return;
    }

    if(l->n == NULL) {                         //if the node to be added is the first node
        l->n = newnode;
        l->tail = newnode;                     //treated as a separate case since tail is modified
        l->length = 1;
        return;
    }

    newnode->next = p;
    p->prev = newnode;
    l->n = newnode;
    l->length = l->length + 1;

    return;

}

//adding two lists
list add(list l1, list l2) {

    list l3;
    init(&l3);

    change_sign(&l3, &l1, &l2);

    //considering the cases when either of the numbers is negative
    if(l3.sign == '-') {
        if(l1.sign == '-') {   //calling sub in num2 - num1 format
            l1.sign = '+';
            l3 = sub(l2, l1);
            return l3;
        }
        else {
            l2.sign = '+';
            l3 = sub(l1, l2);   //calling sub in num1 - num2 format
            return l3;
        }
    }

    int s = 0, c = 0;           //sum and carry initialized to zero

    node *end1, *end2;          //pointers to the last digit of l1 and l2 respectively
    end1 = l1.tail;
    end2 = l2.tail;

    //adding digit by digit starting from the rightmost digit
    while (end1 != NULL || end2 != NULL) {
        if (end1 != NULL && end2 != NULL) {
            s = ((end1->d) + (end2->d) + c) % 10;
            c = ((end1->d) + (end2->d) + c) / 10;
            end1 = end1->prev;
            end2 = end2->prev;
        }
        else if (end1 == NULL && end2 != NULL) {
            s = ((end2->d) + c) % 10;
            c = ((end2->d) + c) / 10;
            end2 = end2->prev;
        }
        else if (end1 != NULL && end2 == NULL) {
            s = ((end1->d) + c) % 10;
            c = ((end1->d) + c) / 10;
            end1 = end1->prev;
        }

        // inserting the sum digit
        add_ahead(&l3, s);
    }

    // inserting last carry
    if (c != 0) {
        add_ahead(&l3, c);
    }

    if(l1.sign == '-' && l2.sign == '-') {      //if both the numbers are negative
        l3.sign = '-';
    }

    return l3;

}

//subtracting two lists
list sub(list l1, list l2) {

    list l3;
    init(&l3);

    node *end1, *end2;                          //pointers to the last digit of l1 and l2 respectively

    rm_zero(&l1);                               //removing preceding zeros if any
    rm_zero(&l2);                               //returns NULL if the number is zero

    if(l1.n == NULL && l2.n == NULL) {          //case:  0 - 0
        append(&l3, 0);
        return l3;
    }
    else if(l1.n != NULL && l2.n == NULL) {     //case: num1 - 0
        return l1;
    }
    else if(l1.n == NULL && l2.n != NULL) {     //case: 0 - num2
        if(l2.sign == '-'){
            l2.sign = '+';
        }
        else {
            l2.sign = '-';
        }
        return l2;
    }

    if(l1.sign == '-' && l2.sign == '+') {   //-num1 - (+num2) implies addition of two negative numbers
        l2.sign = '-';
        l3 = add(l1, l2);
        return l3;
    }
    else if(l1.sign == '+' && l2.sign == '-') {  //+num1 - (-num2) implies addition of two positive numbers
        l2.sign = '+';
        l3 = add(l1, l2);
        return l3;
    }
    else if(l1.sign == '+' && l2.sign == '+') {  //+num1 - (num2) standard case
        int check = compare(l1, l2);
        if(check == INT_MIN) {                   //if l2 is zero
            return l1;
        }
        if(check < 0) {                         //if the negative number is greater in magnitude
            l3.sign = '-';
            end2 = l1.tail;
            end1 = l2.tail;

        }
        if(check >= 0) {
            end1 = l1.tail;
            end2 = l2.tail;
        }
    }
    else {                                      //-num1 - (-num2) reverse case
        int check = compare(l1, l2);
        if(check > 0) {                         //if the negative number is greater in magnitude
            l3.sign = '-';
            end1 = l1.tail;
            end2 = l2.tail;
        }
        if(check <= 0) {
            end1 = l2.tail;
            end2 = l1.tail;
        }
    }


    int s = 0, c = 0;                       //sum and carry(borrow) initialized to zero


    //subtracting digit by digit starting from the rightmost digit
    while (end1 != NULL || end2 != NULL) {
        if (end1 != NULL && end2 != NULL) {
            if ((end1->d) + c >= (end2->d)) {
                s = ((end1->d) + c - (end2->d));
                c = 0;
            }
            else {
                s = ((end1->d) + c + 10 - (end2->d));
                c = -1;
            }
            end1 = end1->prev;
            end2 = end2->prev;
        }
        else if (end1 != NULL && end2 == NULL) {
            if (end1->d >= 1) {
                s = ((end1->d) + c);
                c = 0;
            }
            else {                                  //special case when the l1 has zeros
                if (c != 0) {
                    s = ((end1->d) + 10 + c);       //case when borrow has taken place
                    c = -1;
                }
                else
                    s = end1->d;                    //case when borrow has not taken place
            }
            end1 = end1->prev;
        }
        add_ahead(&l3, s);
    }

    return l3;
}

//multiplying two lists
list mul(list l1, list l2) {

    list ans, f_answer, temp;

    init(&f_answer);
    init(&ans);
    init(&temp);

    int k = 0, i = 0;                   //k keeps a count of the pass to adjust trailing zeros
    int s = 0, c = 0;                   //initialising sum and carry to zero
    node *end2, *endp;                  //pointers to the last digit of l2 and l1 respectively

    end2 = l2.tail;

    while(end2 != NULL) {

        init(&ans);
        init(&temp);

        endp = l1.tail;                 //resetting endp to point to the end of l1 after each pass

        c = 0;
        s = 0;

        while(endp != NULL) {           //multiplying one digit of l2 with all the digits of l1
            s = ((endp->d) * (end2->d) + c) % 10;
            c = ((endp->d) * (end2->d) + c) / 10;
            add_ahead(&ans, s);
            endp = endp->prev;
        }

        if (c != 0) {
            add_ahead(&ans, c);
        }

        for (i = 0; i < k; i++) {  //appending zeros to the end of the obtained result corresponding to shift in each pass
            append(&ans, 0);
        }

        if(k == 0) {
            f_answer = ans;
        }
        else {
            temp = add(f_answer, ans);
            f_answer = temp;
        }
        k++;
        end2 = end2->prev;
    }

    change_sign(&f_answer, &l1, &l2);
    return f_answer;
}

//dividing two lists
list division(list l1, list l2) {

    int x = compare(l1, l2);
    int i = 0;

    list l3;
    init(&l3);

    change_sign(&l3, &l1, &l2);

    if(x == INT_MIN) {              //if l2 is zero
        printf("Divide by zero error\n");
        exit(1);
    }

    if(x < 0) {                     //if the first number is smaller than the second
        append(&l3, 0);
        return l3;
    }
    else if(x == 0) {               //if the two numbers are equal
        append(&l3, 1);
        return l3;
    }
    else {

        int length1 = l1.length;
        int length2 = l2.length;

        node *p = l1.n;         //pointer to the head of l1
        node *t = l2.n;         //pointer to the head of l2

        while(p->d == 0) {      //ignoring preceding zeros
            p = p->next;
        }

        while(t->d == 0) {
            t = t->next;
        }
        int digits = 0;         //counting digits in the divisor
        while(t != NULL) {
            digits++;
            t = t->next;
        }

        list temp, num, l22, q, j, quo;

        init(&q);
        init(&j);
        init(&num);
        init(&temp);
        init(&l22);
        init(&quo);

        append(&q, 0);
        append(&j, 1);
        l22 = l2;

        int pass = 0;

        //for the first pass, the same number of digits as in the divisor are extracted from the dividend
        //then onwards, a single digit is extracted

        while(p != NULL) {
            if(pass == 0) {
                for(i = 0; i < digits; i++) {
                    if(p != NULL) {
                        append(&num, p->d);   //num stores the extracted digits of the dividend
                        p = p->next;
                    }
                    else {
                        break;
                    }
                }
                pass++;
            }
            else {
                append(&num, p->d);
                p = p->next;
            }

            int r = compare(l2, num);    //compare divisor with the extracted number

            if(r == INT_MIN) {          //if the extracted number is zero
                append(&quo, 0);
                continue;
            }

            int check = 0;
            l22 = l2;                       //temporary storage for the divisor

            while(r < 0) {                  //multiply the divisor till it is greater than or equal to the extracted number
                q = add(q, j);
                l22 = mul(l2, q);
                r = compare(l22, num);
                check++;
            }

            if(check == 0 && r != 0) {      //if r == 1 ie the divisor is greater than the extracted number
                append(&quo, 0);
                temp = num;
            }
            else if(check == 0 && r == 0){  //if r == 0 ie the divisor is equal to the extracted number
                append(&quo, 1);            //separate case as q is set to zero
                temp = sub(num, l22);
            }

            else if(r == 0) {               //if r == 0 ie the multiplied divisor is equal to the extracted number
                append(&quo, q.tail->d);
                temp = sub(num, l22);
            }
            else {                          //if the multiplied divisor is greater than the extracted number
                q = sub(q, j);              //subtracting 1 from q
                append(&quo, (q.tail->d));
                l22 = sub(l22, l2);
                temp = sub(num, l22);
            }

            init(&q);
            append(&q, 0);
            num = temp;
            init(&l22);
        }

        change_sign(&quo, &l1, &l2);
        return (quo);
    }

}

//compares two positive lists
int compare(list l1, list l2) {

    int c1 = 0, c2 = 0, count_zero = 0, c11 = 0, c22 = 0;
    node *p = l1.n;
    node *q = l2.n;

    c1 = l1.length;
    c2 = l2.length;

    while(q != NULL) {
        if(q->d == 0) {
            count_zero++;
            c22++;      //counting leading zeros
        }
        else {
            break;
        }
        q = q->next;
    }
    if(count_zero == c2) {  //if all the digits are zero
        return INT_MIN;
    }
    while(p != NULL) {
        if(p->d == 0) {
            p = p->next;
            c11++;
        }
        else {
            break;
        }
    }

    int comp = (c1 - c11) - (c2 - c22); //comparing the lengths of the numbers(taking preceding zeros into account)

    if(comp < 0) {
        return -1;
    }
    else if(comp > 0) {
        return 1;
    }
    else if(comp == 0) {            //test cases for numbers of the same length
        while((p->d - q->d == 0) && (p->next != NULL && q ->next != NULL)) {
            p = p->next;
            q = q -> next;
        }
        if(p->d - q->d > 0) {
            return 1;
        }
        else if(p->d - q->d < 0) {
            return -1;
        }
        else {
            return 0;
        }
    }
    return 0;
}


//displays a list
void display(list l) {

    if(l.sign == '-') {
        printf("-");
    }

    node *p = l.n;

    while(p != NULL) {
        printf("%d", p->d);
        p = p->next;
    }

    printf("\n");
    return;
}

//changes sign for the answer for multiplication and division
void change_sign(list *l3, list *l1, list *l2) {

    if((l1->sign == '-' && l2->sign == '-') || (l1->sign == '+' && l2->sign == '+')) {
            l3->sign = '+';
    }
    else {
            l3->sign = '-';
    }
}



void init1(stack_operands *s, int len){
    s->a = (list*)malloc(sizeof(list)*len);
    s->size = len;
    s->top = -1;
}
int isFull1(stack_operands s) {
    if(s.top == s.size-1)
        return 1;
    else
        return 0;
}

int isEmpty1(stack_operands s) {
    if(s.top == -1)
        return 1;
    else
        return 0;
}
void push1(stack_operands *s, list d){
    if(isFull1(*s))
        return ;
    else {
        s->top++;
        s->a[s->top] = d;
        return;
    }
}

list pop1(stack_operands *s) {
    list l;
    init(&l);
    if(isEmpty1(*s)) {
        return l;
    }
    else {

        l = s->a[s->top];
        s->top--;
        return l;
    }
}


void init2(stack_operators *s, int len){
    s->a = (char*)malloc(sizeof(char)*len);
    s->size = len;
    s->top = -1;
}
int isFull2(stack_operators s) {
    if(s.top == s.size-1)
        return 1;
    else
        return 0;
}

int isEmpty2(stack_operators s) {
    if(s.top == -1)
        return 1;
    else
        return 0;
}
void push2(stack_operators *s, char d){
    if(isFull2(*s))
        return ;
    else {
        s->top++;
        s->a[s->top] = d;
        return;
    }
}

char pop2(stack_operators *s) {
    char ch = ' ';
    if(isEmpty2(*s)) {
        return ch;
    }
    else {

        ch = s->a[s->top];
        s->top--;
        return ch;
    }
}

//performs operation on lists according to the passed operator
list calc(list l1, list l2, char op) {

    switch(op){
        case '+': return add(l1, l2);
        case '-': return sub(l1, l2);
        case '*': return mul(l1, l2);
        case '/': return division(l1, l2);
    }
}

//calculates the precedence of operators
int precedence(char op) {
    if(op == '+'||op == '-')
        return 1;
    if(op == '*'||op == '/')
        return 2;
    return 0;
}

//extracts numbers and operators from the input string.
//evaluates the expression according to the infix evaluation algorithm
//displays the result
void infix_eval(char *s) {

    stack_operands s1;
    init1(&s1, 50);


    stack_operators s2;
    init2(&s2, 50);


    list l;
    init(&l);

    list op1, op2, ans;
    init(&op1);
    init(&op2);
    init(&ans);

    char number[10000];
    char ch, op;
    int i = 0;
    int cb_op = 0, cb_cl = 0;
    while(s[i] != '\0') {

        ch = s[i];
        if(isdigit(ch)) {       //reads a number without sign
            strncat(number, &ch, 1);
            i++;
            ch = s[i];
            while(isdigit(ch) && s[i] != '\0') {  //reads a number till digits are encountered
                strncat(number, &ch, 1);
                i++;
                ch = s[i];

            }
            if(ch == '\0') {
                l = extract(number);
                push1(&s1, l);
                strcpy(number, "");
                break;
            }
            else if(ch == ' ') {
                l = extract(number);
                push1(&s1, l);
                strcpy(number, "");
                i++;
                ch = s[i];
                if(isdigit(ch) || ch =='(' || ((ch == '+' || ch == '-') && isdigit(s[i+1]))) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else if(ch == ')'){
                l = extract(number);
                push1(&s1, l);
                strcpy(number, "");
            }

            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == '+' || ch == '-'){
            strncat(number, &ch, 1);
            i++;
            ch = s[i];
            if(isdigit(ch)) {               //reads number with a sign
                strncat(number, &ch, 1);
                i++;
                ch = s[i];
                while(isdigit(ch)) {
                    strncat(number, &ch, 1);
                    i++;
                    ch = s[i];
                }
                if(ch == ' ') {
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                    i++;
                    if(isdigit(ch) || ch =='(' || ((ch == '+' || ch == '-') && isdigit(s[i+1]))) {
                        printf("Error in evaluation\n");
                        exit(1);
                    }

                    continue;
                }
                else if(ch == '\0') {
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                    break;
                }
                else if(ch == ')'){
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                }
                else {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else if(ch == ' '){         //reads an operator
                if(i == 1) {
                    printf("Error in evaluation\n");  //if the expression begins with an operator
                    exit(1);
                }
                strcpy(number, "");
                i--;
                ch = s[i];
                i++;
                if(isEmpty2(s2)) {              //infix algorithm
                    push2(&s2, ch);
                }
                else if(!isEmpty2(s2)) {

                    if(precedence(ch) > precedence(s2.a[s2.top])) {
                        push2(&s2, ch);
                    }
                    else {
                        while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {
                            init(&op1);
                            init(&op2);
                            init(&ans);
                            op = pop2(&s2);
                            if(op == '(') {
                               push2(&s2, op);
                               continue;
                            }
                            op2 = pop1(&s1);
                            op1 = pop1(&s1);

                            ans = calc(op1, op2, op);
                            push1(&s1, ans);
                        }
                        push2(&s2, ch);
                    }
                }
                i++;
                ch = s[i];

                if(ch == '\0' || ch == '*' || ch == '/' || ch == ')') {
                    printf("Error in evaluation\n");
                    exit(1);
                }
                else if(ch == '+' || ch == '-' )  {
                    if(!isdigit(s[i+1])) {
                        printf("Error in evaluation\n");
                        exit(1);
                    }
                }
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }

        }
        else if(ch == '*' || ch == '/') {                   //reads an operator
            if(i == 0) {
                printf("Error in evaluation\n");            //if the expression begins with an operator
                exit(1);
            }
            i++;
            ch = s[i];
            if(ch == ' '){                                   //infix algorithm
                ch = s[i-1];

                if(isEmpty2(s2)) {
                    push2(&s2, ch);
                }
                else if(!isEmpty2(s2)) {
                    if(precedence(ch) > precedence(s2.a[s2.top])) {
                        push2(&s2, ch);
                    }
                    else {
                        while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {
                            init(&op1);
                            init(&op2);
                            init(&ans);
                            op = pop2(&s2);
                            if(op == '(') {
                               push2(&s2, op);
                               continue;
                            }

                            op2 = pop1(&s1);
                            op1 = pop1(&s1);

                            ans = calc(op1, op2, op);
                            push1(&s1, ans);
                        }
                        push2(&s2, ch);
                    }
                }
                i++;
                ch = s[i];
                if(ch == '\0' || ch == '*' || ch == '/' || ch == ')') {
                    printf("Error in evaluation\n");
                    exit(1);
                }
                else if(ch == '+' || ch == '-' )  {
                    if(!isdigit(s[i+1])) {
                        printf("Error in evaluation\n");
                        exit(1);
                    }
                }
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == '(') {
            cb_op++;
            push2(&s2, ch);
            i++;
            ch = s[i];
            if(isdigit(ch) || ch == '(') {
                continue;
            }
            else if(ch == '+' || ch == '-') {
                if(!isdigit(s[i+1])) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == ')') {
            cb_cl++;
            if(i == 0) {
                printf("Error in evaluation\n");
                exit(1);
            }
            while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {   //infix algorithm
                init(&op1);
                init(&op2);
                init(&ans);
                op = pop2(&s2);
                if(op == '(') {
                    break;
                }
                op2 = pop1(&s1);
                op1 = pop1(&s1);
                ans = calc(op1, op2, op);
                push1(&s1, ans);
            }
            i++;
            ch = s[i];
            if(ch == '\0'){
                break;
            }
            if(ch == ' ') {
                i++;
                ch = s[i];
                if(ch == '\0' || isdigit(ch)) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
        }
        else {
            printf("Error in evaluation\n");
            exit(1);
        }

    }

    while(!isEmpty2(s2)) {
        init(&op1);
        init(&op2);
        init(&ans);
        op2 = pop1(&s1);
        op1 = pop1(&s1);
        op = pop2(&s2);
        ans = calc(op1, op2, op);
        push1(&s1, ans);
    }
    if(cb_cl != cb_op) {                    //if brackets are not balanced
        printf("Error in evaluation\n");
        exit(1);
    }
    printf("Final answer: ");
    display(s1.a[s1.top]);
    return;
}

//to change from decimal to any base between 2 and 9
void change_base(list l, int b) {

    list l1, l2, l3, l4, l5, ans;
    init(&l1);
    init(&l2);
    init(&l3);
    init(&l4);
    init(&l5);
    init(&ans);
    l1 = l;
    append(&l3, b);                     //list with required base
    while(1) {
        l2 = division(l1, l3);          //dividing the number by the required base
        l5 = mul(l2, l3);
        l4 = sub(l1, l5);               //remainder
        add_ahead(&ans, l4.tail->d);    //appending the obtained remainder
        l1 = l2;
        if(l1.n->d == 0 && l1.length == 1)
            break;
        init(&l4);
        init(&l5);
        init(&l2);
    }
    printf("Final answer:");
    if(l.sign == '-') {
        printf("-");
    }
    display(ans);
    return;
}


//removes preceding zeros
void rm_zero(list *l) {

    node *p = l->n;
    while(p != NULL && p->d == 0) {     //checking nullity condition before zero condition

        l->length = l->length - 1;
        l->n = p->next;
        p = p->next;
        if(p != NULL) {
            free(p->prev);
            p->prev = NULL;
        }
    }
    return;
}



//to calculate sin and cosine of given number
void trigo_fn(list l, int choice) {

    list l1, l2, d, m, s;
    init(&l1);
    init(&l2);
    init(&d);
    init(&m);
    init(&s);


    l1 = l;
    char t[] = "360";
    l2 = extract(t);

    int sign_change = 0;

    if(l1.sign == '-') {        //if the inputted angle is negative
        l1.sign = '+';
        sign_change = 1;
    }

    double angle_deg, ans, angle_rad;


    int r = compare(l1, l2);

    if(r <= 0) {                //if the inputted angle is <= 360 degrees
        node* p = l1.n;
        while(p != NULL) {
            angle_deg = ((angle_deg * 10.0) + (p->d));      //storing the angle in a variable of type double
            p = p->next;
        }

        angle_rad = ((angle_deg * PI) / 180.0);             //converting the angle to radians


        if(choice == 1) {                       //sin
            ans = sin(angle_rad);
            if(sign_change == 1) {
                ans = ans * (-1);               //sin(-theta) = -sin(theta)
            }
        }
        else if(choice == 2) {                  //cos
            ans = cos(angle_rad);               //cos(-theta) = cos(theta)
        }

    }
    else {

        d = division(l1, l2);
        m = mul(d, l2);
        s = sub(l1, m);                 //the remainder obtained by dividing the angle by 360

        node* p = s.n;
        while(p != NULL) {
            angle_deg = ((angle_deg * 10.0) + (p->d));      //storing the angle in a variable of type double
            p = p->next;
        }

        angle_rad = ((angle_deg * PI) / 180.0);         //converting the angle to radians

        if(choice == 1) {                               //sin
            ans = sin(angle_rad);
            if(sign_change == 1) {                      //sin(-theta) = -sin(theta)
                ans = ans * (-1);
            }
        }
        else if(choice == 2) {                          //cos
            ans = cos(angle_rad);                       //cos(-theta) = cos(theta)
        }
    }
    printf("Answer = %lf \n", ans);
    return;
}







