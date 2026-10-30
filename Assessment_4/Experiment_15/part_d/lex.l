%{
#include "test.tab.h"
%}

%%
"cat"     return cat;
"dog"     return dog;
\n      return '\n';
[ \t]   ; 
%%

int yywrap(){
    return 1;
}