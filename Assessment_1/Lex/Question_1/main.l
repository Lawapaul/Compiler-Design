%{
    #include <stdlib.h>
    #include <stdio.h>
    int state=0;
%}

%%
1 {
    int prev = state;
    if(state == 0) state=1;
    else if(state == 1) state = 1;
    else if(state == 2) state = 3;
    else if(state == 3) state = 1;
}

0 {
    int prev = state;
    if(state == 0 ) state=0;
    else if(state == 1) state =2;
    else if(state == 2) state = 0;
    else if(state == 3) state = 1;
}

\n {
    if(state==3){
        printf("\nAccepted");
    }else{
        printf("\nRejected");
    }
    state = 0;
}
. ;
%%

int main(){
    printf("\nEnter String: ");
    yylex();
    return 0;
}
int yywrap(){
    return 1;
}