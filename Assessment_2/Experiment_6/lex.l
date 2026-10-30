%{
#include <stdlib.h>
#include <stdio.h>
%}

%%
main|scanf|while|if|else|for|switch|break     {printf("\n%s: Keyword",yytext);}
int|float   {printf("\n%s: Data Type",yytext);}
"("|"{"|"[" {printf("\n%s: Open Parenthesis",yytext);}
")"|"}"|"]" {printf("\n%s: Close Parenthesis",yytext);}
"+"|"-"|"*"|"/"|"%" {printf("\n%s: Operator",yytext);}
\"    {printf("\n%s: Quotes",yytext);}
[a-zA-Z_][a-zA-Z0-9_]* {printf("\n%s: Identifier",yytext);}
[0-9]+\.[0-9]+      {printf("\n%s: Float Number",yytext);}
[0-9]+              {printf("\n%s: Number",yytext);}
[;]           {printf("\n%s: Seperator",yytext);}
[ \t]           {;}
.           {printf("\n%s: Special Symbol",yytext);}
%%

int main(){
    char buffer[1000];
    printf("Enter full code:\n");
    fread(buffer, 1, sizeof(buffer), stdin);
    yy_scan_string(buffer);
    yylex();
    return 0;
}
int yywrap(){
    return 1;
}