%{
#include "tree.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%
[0-9]+     { yylval.str = strdup(yytext); return NUMBER; }

"+" return '+';
"-" return '-';
"*" return '*';
"/" return '/';
"(" return '(';
")" return ')';

[ \t] ;      /* ignore spaces */
\n return '\n';

. { printf("Invalid character: %s\n", yytext); }

%%

int yywrap(){
    return 1;
}