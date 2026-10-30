%{
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int yylex();
int yyerror(const char *s);
%}

%union{
    float val;
}
%token <val> NUMBER
%type <val> expr

%left '+' '-'
%left '*' '/' '%'

%%
stmt : expr '\n' {printf("\nResult: %.2f",$1);} ;

expr: expr '+' expr {$$ = $1 + $3;}
      | expr '-' expr {$$ = $1 - $3;}
      | expr '*' expr {$$ = $1 * $3;}
      | expr '/' expr {if($3==0){
        printf("\nError Division by zero");
        return 0;
        }else{
            $$ = $1 / $3;
        };}
      | expr '%' expr {$$ = (int)$1 % (int)$3;}
      | '(' expr ')' {$$ = $2;}
      | NUMBER {$$ = $1;}
      ;

%%

int main(){
    printf("\nEnter Expression: ");
    yyparse();
    return 0;
}
int yyerror(const char* s){
    printf("\nError");
    return 0;
}

