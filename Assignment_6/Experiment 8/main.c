#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define SIZE 20

char rules[SIZE][SIZE];
char firstSet[SIZE][SIZE];
char followSet[SIZE][SIZE];
char parseTable[SIZE][SIZE][SIZE];
char nonTermList[SIZE];
char termList[SIZE];

int ntCount=0, tCount=0, ruleCount;

int checkNonTerminal(char ch){
    return (ch>='A' && ch<='Z');
}

void insertSet(char *set, char ch){
    if(!strchr(set,ch)){
        int l = strlen(set);
        set[l]=ch;
        set[l+1]='\0';
    }
}

int getNTPos(char ch){
    for(int i=0;i<ntCount;i++)
        if(nonTermList[i]==ch) return i;
    return -1;
}

int getTPos(char ch){
    for(int i=0;i<tCount;i++)
        if(termList[i]==ch) return i;
    return -1;
}

void findFirst(char ch,int idx){
    for(int i=0;i<ruleCount;i++){
        if(rules[i][0]==ch){
            char rhs = rules[i][2];

            if(rhs=='$')
                insertSet(firstSet[idx],'$');
            else if(!checkNonTerminal(rhs))
                insertSet(firstSet[idx],rhs);
            else{
                int id = getNTPos(rhs);
                for(int k=0;k<strlen(firstSet[id]);k++)
                    insertSet(firstSet[idx],firstSet[id][k]);
            }
        }
    }
}

void findFollow(char ch,int idx){
    if(idx==0)
        insertSet(followSet[idx],'#');

    for(int i=0;i<ruleCount;i++){
        for(int j=2;j<strlen(rules[i]);j++){
            if(rules[i][j]==ch){
                if(rules[i][j+1]!='\0'){
                    char next = rules[i][j+1];

                    if(!checkNonTerminal(next))
                        insertSet(followSet[idx],next);
                    else{
                        int id = getNTPos(next);
                        for(int k=0;k<strlen(firstSet[id]);k++){
                            if(firstSet[id][k]!='$')
                                insertSet(followSet[idx],firstSet[id][k]);
                        }
                    }
                }
            }
        }
    }
}

int main(){

    printf("Enter number of productions: ");
    scanf("%d",&ruleCount);

    printf("Enter productions (Example: S=a):\n");
    for(int i=0;i<ruleCount;i++)
        scanf("%s",rules[i]);

    for(int i=0;i<ruleCount;i++){
        if(!strchr(nonTermList,rules[i][0])){
            nonTermList[ntCount++]=rules[i][0];
        }
    }

    for(int i=0;i<ruleCount;i++){
        for(int j=2;j<strlen(rules[i]);j++){
            char ch = rules[i][j];
            if(!checkNonTerminal(ch) && ch!='$' && !strchr(termList,ch)){
                termList[tCount++]=ch;
            }
        }
    }
    termList[tCount++]='#';

    for(int i=0;i<ntCount;i++)
        findFirst(nonTermList[i],i);

    for(int i=0;i<ntCount;i++)
        findFollow(nonTermList[i],i);

    printf("\nFIRST Sets:\n");
    for(int i=0;i<ntCount;i++)
        printf("FIRST(%c) = { %s }\n",nonTermList[i],firstSet[i]);

    printf("\nFOLLOW Sets:\n");
    for(int i=0;i<ntCount;i++)
        printf("FOLLOW(%c) = { %s }\n",nonTermList[i],followSet[i]);

    return 0;
}