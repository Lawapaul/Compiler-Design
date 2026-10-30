%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(const char* s);

int cat_count = 0;
int dog_count = 0;
%}

%token cat dog
%%
S : Cpart Dpart '\n' {
        if(cat_count >= 1 && dog_count >= 1 && dog_count > cat_count)
            printf("\nAccepted\n");
        else
            printf("\nRejected\n");
        cat_count = dog_count = 0;
    }
;

Cpart :
      cat Cpart { cat_count++; }
    | cat       { cat_count++; }
;

Dpart :
      dog Dpart { dog_count++; }
    | dog       { dog_count++; }
;
%%

int main(){
    printf("Enter String: ");
    yyparse();
    return 0;
}

int yyerror(const char* s){
    printf("\nError\n");
    return 0;
}