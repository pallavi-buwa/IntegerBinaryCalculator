#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "DLL.h"
#include <string.h>



void init(list* l) {

    l->n = NULL;
    l->tail = NULL;
    l->length = 0;
    l->sign = '+';
    return;
}

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
            x = s[i] - '0';
            append(&l, x);
        }
        i++;
    }

    return l;
}


void append(list *l, int data) {

    node *newnode;
    newnode = (node*)malloc(sizeof(node));

    newnode->next = NULL;
    newnode->d = data;
    newnode->prev = NULL;

    if(l->n == NULL) {
        l->n = newnode;
        l->tail = newnode;
        l->length = 1;
        return;
    }
    /*p = l->n;
    while(p->next != NULL) {
        p = p->next;
    }*/

    newnode->prev = l->tail;
    l->tail->next = newnode;
    l->tail = newnode;
    l->length = l->length + 1;

    return;
}



void add_ahead(list* l, int data) {

    node *p, *newnode;
    p = l->n;
    newnode = (node*)malloc(sizeof(node));
    newnode->d = data;
    newnode->next = NULL;
    newnode->prev = NULL;
    if(l->n == NULL) {
        l->n = newnode;
        l->tail = newnode;
        l->length = 1;
        return;
    }
    newnode->next = p;
    p->prev = newnode;
    l->n = newnode;
    l->length = l->length + 1;

    return;

}


list add(list l1, list l2) {

    list l3;
    init(&l3);
    change_sign(&l3, &l1, &l2);
    if(l3.sign == '-') {
        if(l1.sign == '-') {   //calling sub in num1 - num2 format
            l1.sign = '+';
            l3 = sub(l2, l1);
            return l3;
        }
        else {
            l2.sign = '+';
            l3 = sub(l1, l2);
            return l3;
        }
    }
    int s = 0, c = 0;
    node *end1, *end2;
    end1 = l1.tail;
    end2 = l2.tail;

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

        // Inserting the sum digit
        add_ahead(&l3, s);
    }

    // Inserting last carry
    if (c != 0)
        add_ahead(&l3, c);

    if(l1.sign == '-' && l2.sign == '-') {
        l3.sign = '-';
    }

    return l3;

}


list sub(list l1, list l2) {
    list l3;
    init(&l3);
    node *end1, *end2;

    if(l1.sign == '-' && l2.sign == '+') {   //-num1 - (+num2)
        l2.sign = '-';
        l3 = add(l1, l2);
        return l3;
    }
    else if(l1.sign == '+' && l2.sign == '-') {  //+num1 - (-num2)
        l2.sign = '+';
        l3 = add(l1, l2);
        return l3;
    }
    else if(l1.sign == '+' && l2.sign == '+') {  //+num1 - (num2)
        int check = compare(l1, l2);
        if(check == INT_MIN) {
            return l1;
        }
        if(check < 0) {
            l3.sign = '-';
            end2 = l1.tail;
            end1 = l2.tail;

        }
        if(check >= 0) {
            end1 = l1.tail;
            end2 = l2.tail;
        }
    }
    else {                                      //-num1 - (-num2)
        int check = compare(l1, l2);
        if(check > 0) {
            l3.sign = '-';
            end1 = l1.tail;
            end2 = l2.tail;
        }
        if(check <= 0) {
            end1 = l2.tail;
            end2 = l1.tail;
        }
    }


    int s = 0, c = 0;


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
            else {
                if (c != 0) {
                    s = ((end1->d) + 10 + c);
                    c = -1;
                }
                else
                    s = end1->d;
            }
            end1 = end1->prev;
        }
        add_ahead(&l3, s);
    }

    return l3;
}


list mul(list l1, list l2) {

    list ans, f_answer, temp;
    init(&f_answer);
    init(&ans);
    init(&temp);

    //change_sign(&f_answer, &l1, &l2);

    int k = 0, i = 0, s = 0, c = 0;
    node *end2, *endp;

    end2 = l2.tail;

    while(end2 != NULL) {
        init(&ans);
        init(&temp);

        endp = l1.tail;

        c = 0;
        s = 0;
        while(endp != NULL) {
            s = ((endp->d) * (end2->d) + c) % 10;
            c = ((endp->d) * (end2->d) + c) / 10;
            add_ahead(&ans, s);
            endp = endp->prev;
        }
        if (c != 0)
            add_ahead(&ans, c);

        for (i = 0; i < k; i++)
            append(&ans, 0);

        if(k == 0) {
            f_answer = ans;
        }
        else {
            temp = add(f_answer, ans);   //CALL TO ADD
            f_answer = temp;
        }
        k++;
        end2 = end2->prev;
    }
    //free(temp);
    //free(ans);
    change_sign(&f_answer, &l1, &l2);
    return f_answer;
}


list division(list l1, list l2) {

    int x = compare(l1, l2);

    list l3;
    init(&l3);

    change_sign(&l3, &l1, &l2);
    if(x == INT_MIN) {
        printf("Divide by zero error\n");
        exit(1);
    }

    if(x < 0) {
        append(&l3, 0);
        return l3;
    }
    else if(x == 0) {
        append(&l3, 1);
        return l3;
    }
    else {

        list i;
        init(&i);
        append(&i, 2);

        list j;
        init(&j);
        append(&j, 1);

        l3 = mul(l2, i);

        int r;
        r = compare(l3, l1);
        while(r < 0) {
            i = add(i, j);
            l3 = mul(l2, i);
            r = compare(l3, l1);

        }

        if(r == 0) {
            change_sign(&i, &l1, &l2);
            return (i);
        }
        else {
            i = sub(i, j);
            change_sign(&i, &l1, &l2);
            return (i);
        }
    }

}

int compare(list l1, list l2) {

    int c1 = 0, c2 = 0, count_zero = 0;
    node *p = l1.n;
    node *q = l2.n;

    c1 = l1.length;
    c2 = l2.length;

    while(q != NULL) {
        if(q->d == 0) {
            count_zero++;
        }
        else {
            break;
        }
        q = q->next;
    }
    if(count_zero == c2) {
        return INT_MIN;
    }

    p = l1.n;
    q = l2.n;
    int comp = c1 - c2;

    if(comp < 0) {
        return -1;
    }
    else if(comp > 0) {
        return 1;
    }
    else if(comp == 0) {
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

list calc(list l1, list l2, char op) {

    switch(op){
        case '+': return add(l1, l2);
        case '-': return sub(l1, l2);
        case '*': return mul(l1, l2);
        case '/': return division(l1, l2);
    }
}

int precedence(char op) {
    if(op == '+'||op == '-')
        return 1;
    if(op == '*'||op == '/')
        return 2;
    return 0;
}

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
    while(s[i] != '\0') {

        ch = s[i];
        if(isdigit(ch)) {       //reads a number without sign
            strncat(number, &ch, 1);
            i++;
            ch = s[i];
            while(isdigit(ch) && s[i] != '\0') {
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
                if(isdigit(ch) || ((ch == '+' || ch == '-') && isdigit(s[i+1]))) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else if(ch == '\)'){
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
                    if(isdigit(ch) || ((ch == '+' || ch == '-') && isdigit(s[i+1]))) {
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
                else if(ch == '\)'){
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                }
                else {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else if(ch == ' '){
                strcpy(number, "");
                i--;
                ch = s[i];
                i++;
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
                            if(op == '\(') {
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

                if(ch == '\0' || ch == '*' || ch == '/') {
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
        else if(ch == '*' || ch == '/') {
            i++;
            ch = s[i];
            if(ch == ' '){
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
                            if(op == '\(') {
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
                if(ch == '\0' || ch == '*' || ch == '/') {
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
        else if(ch == '\(') {
            push2(&s2, ch);
            i++;
            ch = s[i];
            if(ch == '+' || ch == '-' || isdigit(ch)) {
                continue;
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == '\)') {
            while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {
                init(&op1);
                init(&op2);
                init(&ans);
                op = pop2(&s2);
                if(op == '\(') {
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
    printf("Final answer: ");
    display(s1.a[s1.top]);
    return;
}


void change_base(list l, int b) {

    list l1, l2, l3, l4, l5, ans;
    init(&l1);
    init(&l2);
    init(&l3);
    init(&l4);
    init(&l5);
    init(&ans);
    l1 = l;  //decimal number
    append(&l3, b); //list with required base
    while(1) {
        l2 = division(l1, l3);
        l5 = mul(l2, l3);
        l4 = sub(l1, l5);  //remainder
        add_ahead(&ans, l4.tail->d);
        l1 = l2;
        if(l1.n->d == 0 && l1.length == 1)
            break;
        init(&l4);
        init(&l5);
        init(&l2);
    }
    display(ans);
    return;
}



