typedef struct node {
    int d;
    struct node *next, *prev;
}node;

typedef struct list {
    int length;
    char sign;
    node *tail, *n;
}list;

typedef struct stack_operands{
    list *a;
    int size;
    int top;
}stack_operands;

typedef struct stack_operators{
    char *a;
    int size;
    int top;
}stack_operators;

// list functions
void init(list *l);
void append(list *l, int data);
void add_ahead(list* l, int data);
list extract(char* s);
list add(list l1, list l2);
list sub(list l1, list l2);
list mul(list l1, list l2);
list division(list l1, list l2);
int compare(list l1, list l2);
void display(list l);
void change_sign(list *l3, list *l1, list *l2);
list calc(list l1, list l2, char op);
int precedence(char op);
void infix_eval(char *s);

//stack_operands functions
void init1(stack_operands *s, int len);
int isFull1(stack_operands s) ;
int isEmpty1(stack_operands s);
void push1(stack_operands *s, list d);
list pop1(stack_operands *s);


//stack_operators function
void init2(stack_operators *s, int len);
int isFull2(stack_operators s) ;
int isEmpty2(stack_operators s);
void push2(stack_operators *s, char d);
char pop2(stack_operators *s);


void change_base(list l, int b);

