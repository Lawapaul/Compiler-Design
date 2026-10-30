%{
#include <stdlib.h>
#include <stdio.h>
%}

%%
if|else|while|for  {printf("\n%s: Keyword",yytext);}
int|float       {printf("\n%s: Data Type",yytext);}
"+"|"-"|"*"|"/"     {printf("\n%s: Operator",yytext);}
"("|"{"|"["|"]"|"}"|")" {printf("\n%s: Parenthesis",yytext);}
[a-zA-Z_][a-zA-Z0-9_]*  {printf("\n%s: Identifier",yytext);}
[0-9]+              {printf("\n%s: Number",yytext);}
[0-9]+\.[0-9]+       {printf("\n%s: Float Number",yytext);}
[ \t]           {;}
%%
int yywrap(){
    return 1;
}
int main(){
    printf("Enter Expression: ");
    yylex();
    return 0;
}