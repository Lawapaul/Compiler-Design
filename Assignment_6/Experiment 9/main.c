#include<stdio.h>
#include<string.h>

#define MAX 20

char operators[MAX];
char precedenceTable[MAX][MAX];
int opCount = 0;

/* Function to check if character is operator */
int isOperator(char ch){
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='$')
        return 1;
    return 0;
}

/* Function to get index of operator */
int getIndex(char ch){
    for(int i=0;i<opCount;i++)
        if(operators[i]==ch)
            return i;
    return -1;
}

/* Function to assign precedence values */
void buildPrecedenceTable(){

    for(int i=0;i<opCount;i++){
        for(int j=0;j<opCount;j++){

            char a = operators[i];
            char b = operators[j];

            if(a=='$' && b!='$')
                precedenceTable[i][j] = '<';
            else if(a!='$', b=='$')
                precedenceTable[i][j] = '>';
            else if((a=='+' || a=='-') && (b=='*' || b=='/'))
                precedenceTable[i][j] = '<';
            else if((a=='*' || a=='/') && (b=='+' || b=='-'))
                precedenceTable[i][j] = '>';
            else
                precedenceTable[i][j] = '>';
        }
    }
}

/* Display Table */
void displayTable(){

    printf("\nOperator Precedence Table:\n\t");
    for(int i=0;i<opCount;i++)
        printf("%c\t",operators[i]);

    printf("\n");

    for(int i=0;i<opCount;i++){
        printf("%c\t",operators[i]);
        for(int j=0;j<opCount;j++)
            printf("%c\t",precedenceTable[i][j]);
        printf("\n");
    }
}

int main(){

    printf("Enter operators used (example: +-*$/): ");
    scanf("%s",operators);

    opCount = strlen(operators);

    buildPrecedenceTable();
    displayTable();

    return 0;
}