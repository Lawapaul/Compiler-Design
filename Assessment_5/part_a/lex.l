%{
#include "tac.tab.h"
%}

%%
while   {return while;}
if   {return if;}
else   {return else;}
[0-9]+      {yylval.str = atof(yytext); return NUM;}
[a-zA-Z]+   {yylval.str = strdup(yytext); return ID;}
[+\-*/<>=();{}] return yytext[0];
'\n'        {printf("\n");}
[ \t]       {;}
.           {;}
%%

int yywrap(){
    return 1;
}