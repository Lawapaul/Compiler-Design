%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char newWord[50];
char oldWord[50];
FILE* out;
%}


%%
[a-zA-Z]+       {if(strcmp(oldWord,yytext)==0){
                    fprintf(out,"%s",newWord);
                }else{
                    fprintf(out,"%s",yytext);
                };}
[ \t\n]       {fprintf(out,"%s",yytext);}
.           {fprintf(out,"%s",yytext);}
%%

int main(){
    FILE* fp;
    fp = fopen("input.txt","r");
    if(fp==NULL){
        printf("FILE NOT FOUND");
        return 0;
    }
    out = fopen("output.txt","w");
    printf("\nEnter Old Word: ");
    scanf("%s",oldWord);
    printf("\nEnter New Word: ");
    scanf("%s",newWord);
    yyin = fp;
    yylex();
    fclose(fp);
    fclose(out);
    remove("input.txt");
    rename("output.txt","input.txt");
    return 0;
}
int yywrap(){
    return 1;
}