%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char *val;
    struct node *left, *right;
} node;

node* createNode(char *val, node* left, node* right) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->val = strdup(val);
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

void postfix(node* root) {
    if(root) {
        postfix(root->left);
        postfix(root->right);
        printf("%s ", root->val);
    }
}

int yylex();
void yyerror(const char *s);
%}

%union {
    char *str;
    struct node* nodeptr;
}

%token <str> NUMBER
%type <nodeptr> expr

%left '+' '-'
%left '*' '/'

%%

input:
    input line
    |
    ;

line:
    expr '\n' {
        printf("Postfix: ");
        postfix($1);
        printf("\n");
    }
    ;

expr:
    expr '+' expr { $$ = createNode("+", $1, $3); }
    | expr '-' expr { $$ = createNode("-", $1, $3); }
    | expr '*' expr { $$ = createNode("*", $1, $3); }
    | expr '/' expr { $$ = createNode("/", $1, $3); }
    | '(' expr ')'  { $$ = $2; }
    | NUMBER        { $$ = createNode($1, NULL, NULL); }
    ;

%%

void yyerror(const char *s){
    printf("Error: %s\n", s);
}

int main(){
    printf("Enter Infix Expression:\n");
    yyparse();
    return 0;
}