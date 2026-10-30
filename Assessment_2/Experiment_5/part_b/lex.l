%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
if|else|while|for  {printf("\n%s: Keyword",yytext);}
int|float       {printf("\n%s: Data Type",yytext);}
"+"|"-"|"*"|"/"|"++"|"--"    {printf("\n%s: Operator",yytext);}
"("|"{"|"["|"]"|"}"|")" {printf("\n%s: Parenthesis",yytext);}
[a-zA-Z_][a-zA-Z0-9_]*  {printf("\n%s: Identifier",yytext);}
[0-9]+              {printf("\n%s: Number",yytext);}
[0-9]+\.[0-9]+       {printf("\n%s: Float Number",yytext);}
[;]           {printf("\n%s: Seperator",yytext);}
[ \t]           {;}
.           {printf("\n%s: Special Symbol",yytext);}
%%

int main(){
    FILE* fp;
    fp = fopen("input.txt","r");
    if(fp==NULL){
        printf("FILE NOT FOUND");
        return 0;
    }
    yyin = fp;
    yylex();
    fclose(fp);
    return 0;
}

int yywrap(){
    return 1;
}