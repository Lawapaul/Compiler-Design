%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(const char* s);
%}

%token A B C

%%
S : X C '\n' { printf("Accepted\n"); }
  | '\n' { printf("Rejected\n"); }
  ;

/* Equal number of a and b */
X : A X B
  | /* empty */
  ;
%%

int main(){
    printf("Enter String: ");
    yyparse();
    return 0;
}

int yyerror(const char* s){
    printf("Error\n");
    return 0;
}