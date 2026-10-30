%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int maxLen=0;
char maxWord[100];
%}


%%
[a-zA-Z]+       {if(strlen(yytext) > maxLen){
                    maxLen = strlen(yytext);
                    strcpy(maxWord,yytext);
                };}
[ \t\n]       {;}
.           {;}
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
    printf("Max Word: %s\nMax Len: %d",maxWord,maxLen);
    fclose(fp);
    return 0;
}
int yywrap(){
    return 1;
}