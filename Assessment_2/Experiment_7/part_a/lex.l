%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char target[50];
int count = 0;
%}


%%
[a-zA-Z]+       {if(strcmp(target,yytext)==0){
                    count++;
                };}
[ \t]       {;}
\n          {printf("\n");}
%%

int main(){
    FILE* fp;
    fp = fopen("input.txt","r");
    if(fp==NULL){
        printf("FILE NOT FOUND");
        return 0;
    }
    printf("Enter Target Word: ");
    scanf("%s",target);
    yyin = fp;
    yylex();
    printf("Count: %d",count);
    fclose(fp);
    return 0;
}
int yywrap(){
    return 1;
}