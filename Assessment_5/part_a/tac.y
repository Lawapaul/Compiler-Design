%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int temp_count = 1;
int label_count = 1;

char* newTemp() {
    char *t = (char*)malloc(10);
    sprintf(t, "t%d", temp_count++);
    return t;
}

char* newLabel() {
    char *l = (char*)malloc(10);
    sprintf(l, "L%d", label_count++);
    return l;
}

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}
%}

%union {
    char *str;
}

%token <str> ID NUM
%token IF ELSE WHILE
%type <str> E C

%%

S : STMT
  ;

STMT
    : ID '=' E ';' {
        printf("%s = %s\n", $1, $3);
      }

    | IF '(' C ')' STMT {
        char *L1 = newLabel();
        printf("if %s goto %s\n", $3, L1);
        printf("goto END\n");
        printf("%s:\n", L1);
      }

    | IF '(' C ')' STMT ELSE STMT {
        char *L1 = newLabel();
        char *L2 = newLabel();

        printf("if %s goto %s\n", $3, L1);
        printf("goto %s\n", L2);

        printf("%s:\n", L1);
        // then part already printed

        printf("%s:\n", L2);
      }

    | WHILE '(' C ')' STMT {
        char *L1 = newLabel();
        char *L2 = newLabel();

        printf("%s:\n", L1);
        printf("if %s goto %s\n", $3, L2);
        printf("goto END\n");

        printf("%s:\n", L2);
        // loop body printed

        printf("goto %s\n", L1);
      }
    ;

C : ID '<' ID {
        char *t = newTemp();
        sprintf(t, "%s < %s", $1, $3);
        $$ = t;
    }
  | ID '>' ID {
        char *t = newTemp();
        sprintf(t, "%s > %s", $1, $3);
        $$ = t;
    }
  ;

E : E '+' E {
        char *t = newTemp();
        printf("%s = %s + %s\n", t, $1, $3);
        $$ = t;
    }
  | E '*' E {
        char *t = newTemp();
        printf("%s = %s * %s\n", t, $1, $3);
        $$ = t;
    }
  | ID { $$ = $1; }
  | NUM { $$ = $1; }
  ;

%%
int main() {
    yyparse();
    return 0;
}