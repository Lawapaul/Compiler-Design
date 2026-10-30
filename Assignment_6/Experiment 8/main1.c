#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char production[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
char table[MAX][MAX][MAX];

char nonTerminals[MAX];
char terminals[MAX];

int nt = 0, t = 0, count;

// Check Non-Terminal
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Add to set (no duplicates)
void addToSet(char *set, char c) {
    if (!strchr(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// Get index
int getNTIndex(char c) {
    for (int i = 0; i < nt; i++)
        if (nonTerminals[i] == c) return i;
    return -1;
}

int getTIndex(char c) {
    for (int i = 0; i < t; i++)
        if (terminals[i] == c) return i;
    return -1;
}

// ✅ FIXED FIRST (handles non-terminals)
void computeFirst(char c, int index) {
    for (int i = 0; i < count; i++) {
        if (production[i][0] == c) {

            char rhs = production[i][2];

            if (rhs == '$') {
                addToSet(first[index], '$');
            }
            else if (!isNonTerminal(rhs)) {
                addToSet(first[index], rhs);
            }
            else {
                int idx = getNTIndex(rhs);

                // add FIRST of RHS non-terminal
                for (int k = 0; k < strlen(first[idx]); k++) {
                    addToSet(first[index], first[idx][k]);
                }
            }
        }
    }
}

// FOLLOW
void computeFollow(char c, int index) {

    if (index == 0)
        addToSet(follow[index], '#');

    for (int i = 0; i < count; i++) {
        for (int j = 2; j < strlen(production[i]); j++) {

            if (production[i][j] == c) {

                if (production[i][j + 1] != '\0') {
                    char next = production[i][j + 1];

                    if (!isNonTerminal(next)) {
                        addToSet(follow[index], next);
                    }
                }
            }
        }
    }
}

// Display Stack
void displayStack(char stack[], int top) {
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);
}

// MAIN
int main() {

    printf("Enter number of productions: ");
    scanf("%d", &count);

    printf("Enter productions (Example: S=a):\n");
    for (int i = 0; i < count; i++)
        scanf("%s", production[i]);

    // Collect Non-Terminals
    for (int i = 0; i < count; i++) {
        if (!strchr(nonTerminals, production[i][0]))
            nonTerminals[nt++] = production[i][0];
    }

    // Collect Terminals
    for (int i = 0; i < count; i++) {
        for (int j = 2; j < strlen(production[i]); j++) {

            char c = production[i][j];

            if (!isNonTerminal(c) && c != '$' && !strchr(terminals, c))
                terminals[t++] = c;
        }
    }

    terminals[t++] = '#';

    // ✅ Compute FIRST twice (propagation fix)
    for (int i = 0; i < nt; i++)
        computeFirst(nonTerminals[i], i);

    for (int i = 0; i < nt; i++)
        computeFirst(nonTerminals[i], i);

    // Compute FOLLOW
    for (int i = 0; i < nt; i++)
        computeFollow(nonTerminals[i], i);

    // Build Parse Table
    for (int i = 0; i < count; i++) {

        char lhs = production[i][0];
        int row = getNTIndex(lhs);
        char rhs = production[i][2];

        if (rhs == '$') {
            for (int k = 0; k < strlen(follow[row]); k++) {
                int col = getTIndex(follow[row][k]);
                strcpy(table[row][col], production[i]);
            }
        }
        else {
            int col = getTIndex(rhs);
            strcpy(table[row][col], production[i]);
        }
    }

    // FIRST Output
    printf("\nFIRST Sets:\n");
    for (int i = 0; i < nt; i++)
        printf("FIRST(%c) = { %s }\n", nonTerminals[i], first[i]);

    // FOLLOW Output
    printf("\nFOLLOW Sets:\n");
    for (int i = 0; i < nt; i++)
        printf("FOLLOW(%c) = { %s }\n", nonTerminals[i], follow[i]);

    // Parse Table
    printf("\nPredictive Parse Table:\n\t");

    for (int i = 0; i < t; i++)
        printf("%c\t", terminals[i]);

    printf("\n");

    for (int i = 0; i < nt; i++) {

        printf("%c\t", nonTerminals[i]);

        for (int j = 0; j < t; j++) {
            if (strlen(table[i][j]) > 0)
                printf("%s\t", table[i][j]);
            else
                printf("-\t");
        }
        printf("\n");
    }

    // Parsing
    char input[50], stack[50];
    int top = -1, ip = 0;

    printf("\nEnter input string (end with #): ");
    scanf("%s", input);

    stack[++top] = '#';
    stack[++top] = nonTerminals[0];

    printf("\nStack\tInput\tAction\n");
    printf("--------------------------------------\n");

    while (top >= 0) {

        char X = stack[top];
        char a = input[ip];

        displayStack(stack, top);
        printf("\t%s\t", input + ip);

        if (X == a) {
            printf("Match %c\n", a);
            top--;
            ip++;
        }
        else if (!isNonTerminal(X)) {
            printf("Error\nString Rejected\n");
            return 0;
        }
        else {

            int row = getNTIndex(X);
            int col = getTIndex(a);

            if (strlen(table[row][col]) == 0) {
                printf("Error\nString Rejected\n");
                return 0;
            }

            printf("Replace %c → %s\n", X, table[row][col]);

            top--;

            if (table[row][col][2] != '$') {
                for (int k = strlen(table[row][col]) - 1; k >= 2; k--)
                    stack[++top] = table[row][col][k];
            }
        }
    }

    if (input[ip] == '\0')
        printf("\nString Accepted\n");
    else
        printf("\nString Rejected\n");

    return 0;
}