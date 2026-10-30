#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char prod[MAX][MAX];
char firstvt[MAX][MAX], lastvt[MAX][MAX];
char nonterm[MAX];
char term[MAX];
char table[MAX][MAX];

int n, t = 0, nt = 0;

// Utility functions
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

int findIndex(char c, char *arr, int size) {
    for (int i = 0; i < size; i++)
        if (arr[i] == c) return i;
    return -1;
}

void addToSet(char set[MAX][MAX], int index, char val) {
    if (strchr(set[index], val) == NULL) {
        int len = strlen(set[index]);
        set[index][len] = val;
        set[index][len + 1] = '\0';
    }
}

// FIRSTVT
void computeFirstVT() {
    for (int i = 0; i < n; i++) {
        char A = prod[i][0];
        int Aidx = findIndex(A, nonterm, nt);

        if (!isNonTerminal(prod[i][3])) {
            addToSet(firstvt, Aidx, prod[i][3]);
        } else {
            char B = prod[i][3];
            int Bidx = findIndex(B, nonterm, nt);

            if (prod[i][4] != '\0' && !isNonTerminal(prod[i][4])) {
                addToSet(firstvt, Aidx, prod[i][4]);
            }

            for (int j = 0; j < strlen(firstvt[Bidx]); j++) {
                addToSet(firstvt, Aidx, firstvt[Bidx][j]);
            }
        }
    }
}

// LASTVT
void computeLastVT() {
    for (int i = 0; i < n; i++) {
        char A = prod[i][0];
        int Aidx = findIndex(A, nonterm, nt);

        int len = strlen(prod[i]);

        if (!isNonTerminal(prod[i][len - 1])) {
            addToSet(lastvt, Aidx, prod[i][len - 1]);
        } else {
            char B = prod[i][len - 1];
            int Bidx = findIndex(B, nonterm, nt);

            if (!isNonTerminal(prod[i][len - 2])) {
                addToSet(lastvt, Aidx, prod[i][len - 2]);
            }

            for (int j = 0; j < strlen(lastvt[Bidx]); j++) {
                addToSet(lastvt, Aidx, lastvt[Bidx][j]);
            }
        }
    }
}

// Build precedence table
void buildTable() {
    for (int i = 0; i < t; i++)
        for (int j = 0; j < t; j++)
            table[i][j] = ' ';

    for (int i = 0; i < n; i++) {
        char rhs[MAX];
        strcpy(rhs, &prod[i][3]);

        for (int j = 0; j < strlen(rhs) - 1; j++) {
            char a = rhs[j];
            char b = rhs[j + 1];

            if (!isNonTerminal(a) && !isNonTerminal(b)) {
                table[findIndex(a, term, t)][findIndex(b, term, t)] = '=';
            }

            if (!isNonTerminal(a) && isNonTerminal(b)) {
                int Bidx = findIndex(b, nonterm, nt);
                for (int k = 0; k < strlen(firstvt[Bidx]); k++) {
                    table[findIndex(a, term, t)][findIndex(firstvt[Bidx][k], term, t)] = '<';
                }
            }

            if (isNonTerminal(a) && !isNonTerminal(b)) {
                int Aidx = findIndex(a, nonterm, nt);
                for (int k = 0; k < strlen(lastvt[Aidx]); k++) {
                    table[findIndex(lastvt[Aidx][k], term, t)][findIndex(b, term, t)] = '>';
                }
            }
        }
    }
}

// Print table
void printTable() {
    printf("\nOperator Precedence Table:\n\n  ");
    for (int i = 0; i < t; i++)
        printf(" %c", term[i]);

    printf("\n");
    for (int i = 0; i < t; i++) {
        printf("%c ", term[i]);
        for (int j = 0; j < t; j++) {
            printf(" %c", table[i][j]);
        }
        printf("\n");
    }
}

// Parsing
void parseString(char input[]) {
    char stack[50];
    int top = 0;

    stack[top++] = '$';
    int i = 0;

    printf("\nStack\tInput\tAction\n");

    while (1) {
        char a = stack[top - 1];
        char b = input[i];

        printf("%s\t%s\t", stack, input + i);

        if (a == '$' && b == '$') {
            printf("Accepted\n");
            return;
        }

        int row = findIndex(a, term, t);
        int col = findIndex(b, term, t);

        if (table[row][col] == '<' || table[row][col] == '=') {
            stack[top++] = b;
            i++;
            printf("Shift\n");
        } else if (table[row][col] == '>') {
            top--;
            printf("Reduce\n");
        } else {
            printf("Error\n");
            return;
        }
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (E=E+T):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", prod[i]);
    }
    for (int i = 0; i < n; i++) {
        char A = prod[i][0];
        if (findIndex(A, nonterm, nt) == -1)
            nonterm[nt++] = A;

        for (int j = 3; j < strlen(prod[i]); j++) {
            char c = prod[i][j];
            if (!isNonTerminal(c) && c != '#' && findIndex(c, term, t) == -1) {
                term[t++] = c;
            }
        }
    }

    term[t++] = '$';

    computeFirstVT();
    computeLastVT();

    buildTable();
    printTable();

    char input[50];
    printf("\nEnter input string (end with $): ");
    scanf("%s", input);

    parseString(input);

    return 0;
}