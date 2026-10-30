%{
#include "test.tab.h"
%}

%%
"a"     return A;
"b"     return B;
"c"     return C;
\n      return '\n';
[ \t]   ; 
.       return yytext[0];
%%

int yywrap(){
    return 1;
}