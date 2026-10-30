%{
#include "calc.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+\.[0-9]+     {yylval.val = atof(yytext);
                    return NUMBER;}
[ \t]               ;
\n                  return '\n';
"+"     {return '+';}
"-"     {return '-';}
"*"     {return '*';}
"/"     {return '/';}
"("     {return '(';}
")"     {return ')';}
"%"     {return '%';}

%%

int yywrap(){
    return 1;
}