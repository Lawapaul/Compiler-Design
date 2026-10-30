#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 20

typedef struct {
    char lhs[10];
    char a[10];
    char b[10];
    char op;
} TAC;

TAC code[MAX];
int n;

/* Check if number */
int isNumber(char s[]) {
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i])) return 0;
    return 1;
}

/* 1️⃣ Constant Folding */
void constantFolding() {
    for (int i = 0; i < n; i++) {
        if (isNumber(code[i].a) && isNumber(code[i].b)) {
            int x = atoi(code[i].a);
            int y = atoi(code[i].b);
            int res;

            switch (code[i].op) {
                case '+': res = x + y; break;
                case '-': res = x - y; break;
                case '*': res = x * y; break;
                case '/': if (y != 0) res = x / y; else continue;
            }

            sprintf(code[i].a, "%d", res);
            strcpy(code[i].b, "");
            code[i].op = '=';
        }
    }
}

/* 2️⃣ Strength Reduction */
void strengthReduction() {
    for (int i = 0; i < n; i++) {

        // x * 2 → x + x
        if (code[i].op == '*' && strcmp(code[i].b, "2") == 0) {
            strcpy(code[i].b, code[i].a);
            code[i].op = '+';
        }

        // x / 2 → x >> 1
        if (code[i].op == '/' && strcmp(code[i].b, "2") == 0) {
            strcpy(code[i].b, "1");
            code[i].op = '>'; // represent >>
        }
    }
}

void algebraicTransformation() {
    for (int i = 0; i < n; i++) {

        // x + 0 → x
        if (code[i].op == '+' && strcmp(code[i].b, "0") == 0) {
            code[i].op = '=';
            strcpy(code[i].b, "");
        }

        // x * 1 → x
        if (code[i].op == '*' && strcmp(code[i].b, "1") == 0) {
            code[i].op = '=';
            strcpy(code[i].b, "");
        }

        // x * 0 → 0
        if (code[i].op == '*' && strcmp(code[i].b, "0") == 0) {
            strcpy(code[i].a, "0");
            code[i].op = '=';
            strcpy(code[i].b, "");
        }
    }
}

/* Print TAC */
void printCode() {
    for (int i = 0; i < n; i++) {
        if (code[i].op == '=')
            printf("%s = %s\n", code[i].lhs, code[i].a);
        else if (code[i].op == '>')
            printf("%s = %s >> %s\n", code[i].lhs, code[i].a, code[i].b);
        else
            printf("%s = %s %c %s\n",
                   code[i].lhs,
                   code[i].a,
                   code[i].op,
                   code[i].b);
    }
}

/* MAIN */
int main() {
    printf("Enter number of statements: ");
    scanf("%d", &n);

    printf("Enter TAC (format: x a + b):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s %s %c %s",
              code[i].lhs,
              code[i].a,
              &code[i].op,
              code[i].b);
    }

    printf("\n--- Original Code ---\n");
    printCode();

    constantFolding();
    strengthReduction();
    algebraicTransformation();

    printf("\n--- Optimized Code ---\n");
    printCode();

    return 0;
}