%{
#include <stdio.h>
#include <stdlib.h>
FILE* out;
%}

%%
if|else|while|for  {fprintf(out,"\n%s: Keyword",yytext);}
int|float       {fprintf(out,"\n%s: Data Type",yytext);}
"+"|"-"|"*"|"/"|"++"|"--"    {fprintf(out,"\n%s: Operator",yytext);}
"("|"{"|"["|"]"|"}"|")" {fprintf(out,"\n%s: Parenthesis",yytext);}
[a-zA-Z_][a-zA-Z0-9_]*  {fprintf(out,"\n%s: Identifier",yytext);}
[0-9]+\.[0-9]+       {fprintf(out,"\n%s: Float Number",yytext);}
[0-9]+              {fprintf(out,"\n%s: Number",yytext);}
[;]           {fprintf(out,"\n%s: Seperator",yytext);}
[ \t]           {;}
.           {fprintf(out,"\n%s: Special Symbol",yytext);}
%%

int main(){
    FILE* fp;
    fp = fopen("input.txt","r");
    out = fopen("output.txt","w");
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