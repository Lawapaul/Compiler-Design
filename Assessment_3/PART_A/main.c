
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_STACK 100
#define MAX_INPUT 100

enum Symbol {
    T_ID = 0, T_PLUS, T_STAR, T_LPAREN, T_RPAREN, T_DOLLAR,
    NT_E, NT_EP, NT_T, NT_TP, NT_F,
    SYM_EPS,   
    SYM_ERR   
};

static const char *sym_name[] = {
    "id", "+", "*", "(", ")", "$",
    "E", "E'", "T", "T'", "F",
    "eps", "ERR"
};

#define NUM_NT  5   
#define NUM_T   6   



typedef struct {
    const char *display;          /* human-readable string               */
    int         rhs[5];           /* RHS symbols, -1 = end of list        */
} Production;

static Production prods[] = {
     { "E  -> T E'",    { NT_T, NT_EP, -1 } },
    /* 1 */ { "E' -> + T E'",  { T_PLUS, NT_T, NT_EP, -1 } },
    /* 2 */ { "E' -> eps",     { SYM_EPS, -1 } },
    /* 3 */ { "T  -> F T'",    { NT_F, NT_TP, -1 } },
    /* 4 */ { "T' -> * F T'",  { T_STAR, NT_F, NT_TP, -1 } },
    /* 5 */ { "T' -> eps",     { SYM_EPS, -1 } },
    /* 6 */ { "F  -> id",      { T_ID, -1 } },
    /* 7 */ { "F  -> ( E )",   { T_LPAREN, NT_E, T_RPAREN, -1 } }
};

/* ─── Parse table M[NT][Terminal] ─────────────────────────────────────────
 *
 * Rows  : NT_E(0) NT_EP(1) NT_T(2) NT_TP(3) NT_F(4)
 * Cols  : T_ID(0) T_PLUS(1) T_STAR(2) T_LPAREN(3) T_RPAREN(4) T_DOLLAR(5)
 *
 * Cell value = production index (0-7), or -1 for error.
 */
static int parse_table[NUM_NT][NUM_T] = {
/*            id    +     *     (     )    $   */
/* E   */  {  0,   -1,   -1,    0,   -1,  -1 },
/* E'  */  { -1,    1,   -1,   -1,    2,   2 },
/* T   */  {  3,   -1,   -1,    3,   -1,  -1 },
/* T'  */  { -1,    5,    4,   -1,    5,   5 },
/* F   */  {  6,   -1,   -1,    7,   -1,  -1 }
};

/* ─── Stack helpers ──────────────────────────────────────────────────────── */

static int stack[MAX_STACK];
static int sp = -1;

static void push(int sym) { stack[++sp] = sym; }
static int  pop(void)     { return stack[sp--]; }
static int  top(void)     { return stack[sp]; }

static void print_stack(void) {
    /* Print stack from bottom to top */
    printf("  Stack : [ ");
    for (int i = 0; i <= sp; i++)
        printf("%s ", sym_name[stack[i]]);
    printf("]");
}

/* ─── Tokenizer ──────────────────────────────────────────────────────────── */

/*
 * Very simple tokenizer.
 * Accepts strings like "id+id*id" or "id + id * id".
 * Recognised: id, +, *, (, )
 */

static int tokens[MAX_INPUT];
static int num_tokens;

static int tokenize(const char *s) {
    num_tokens = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == ' ' || s[i] == '\t') { i++; continue; }
        if (s[i] == 'i' && s[i+1] == 'd') {
            tokens[num_tokens++] = T_ID; i += 2; continue;
        }
        switch (s[i]) {
            case '+': tokens[num_tokens++] = T_PLUS;   break;
            case '*': tokens[num_tokens++] = T_STAR;   break;
            case '(': tokens[num_tokens++] = T_LPAREN; break;
            case ')': tokens[num_tokens++] = T_RPAREN; break;
            default:
                fprintf(stderr, "Unknown character '%c'\n", s[i]);
                return 0;
        }
        i++;
    }
    tokens[num_tokens++] = T_DOLLAR;
    return 1;
}

/* ─── Print parse table ──────────────────────────────────────────────────── */

static void print_parse_table(void) {
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                   LL(1) PARSE TABLE                         ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");

    /* Header */
    printf("  %-5s |", "NT");
    for (int t = 0; t < NUM_T; t++) printf(" %-12s|", sym_name[t]);
    printf("\n");
    printf("  ------+");
    for (int t = 0; t < NUM_T; t++) printf("--------------+");
    printf("\n");

    const char *nt_names[] = { "E", "E'", "T", "T'", "F" };
    for (int nt = 0; nt < NUM_NT; nt++) {
        printf("  %-5s |", nt_names[nt]);
        for (int t = 0; t < NUM_T; t++) {
            int p = parse_table[nt][t];
            if (p == -1)
                printf(" %-12s|", "—");
            else
                printf(" %-12s|", prods[p].display);
        }
        printf("\n");
    }
    printf("\n");
}

/* ─── Predictive parse ───────────────────────────────────────────────────── */

static void parse(void) {
    /* Initialise stack with $ and start symbol E */
    sp = -1;
    push(T_DOLLAR);
    push(NT_E);

    int ip = 0;   /* input pointer */
    int step = 1;

    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    PARSE TRACE                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    printf("  %-4s  %-28s  %-18s  %s\n",
           "Step", "Stack (bottom→top)", "Input", "Action");
    printf("  ----  ----------------------------  ------------------  --------------------\n");

    while (1) {
        int X = top();
        int a = tokens[ip];

        /* Build input display string */
        char inp_buf[64] = {0};
        for (int k = ip; k < num_tokens; k++) {
            strcat(inp_buf, sym_name[tokens[k]]);
            if (k < num_tokens - 1) strcat(inp_buf, " ");
        }

        /* Print step line */
        printf("  %-4d  ", step++);

        /* Stack (compact) */
        char stk_buf[64] = {0};
        for (int k = 0; k <= sp; k++) {
            strcat(stk_buf, sym_name[stack[k]]);
            if (k < sp) strcat(stk_buf, " ");
        }
        printf("%-28s  %-18s  ", stk_buf, inp_buf);

        /* ── Accept ── */
        if (X == T_DOLLAR && a == T_DOLLAR) {
            printf("ACCEPT\n");
            printf("\n  ✓  String ACCEPTED\n\n");
            return;
        }

        /* ── Terminal match ── */
        if (X < NUM_T) {          /* X is a terminal */
            if (X == a) {
                printf("Match '%s'\n", sym_name[a]);
                pop();
                ip++;
            } else {
                printf("ERROR: expected '%s', got '%s'\n",
                       sym_name[X], sym_name[a]);
                printf("\n  ✗  String REJECTED\n\n");
                return;
            }
            continue;
        }

        /* ── Non-terminal: consult parse table ── */
        int nt = X - NT_E;           /* row index 0..4 */
        int col = a;                 /* col index 0..5 */
        int prod_idx = parse_table[nt][col];

        if (prod_idx == -1) {
            printf("ERROR: no entry for M[%s, %s]\n",
                   sym_name[X], sym_name[a]);
            printf("\n  ✗  String REJECTED\n\n");
            return;
        }

        printf("Output: %s\n", prods[prod_idx].display);
        pop();

        /* Push RHS in reverse order (skip if ε) */
        const int *rhs = prods[prod_idx].rhs;
        if (rhs[0] == SYM_EPS) continue;   /* ε production, push nothing */

        int len = 0;
        while (rhs[len] != -1) len++;
        for (int k = len - 1; k >= 0; k--)
            push(rhs[k]);
    }
}

/* ─── Main ───────────────────────────────────────────────────────────────── */

int main(void) {
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║             PREDICTIVE PARSER  (LL(1))                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");

    printf("\nGrammar:\n");
    printf("  E  -> T E'\n");
    printf("  E' -> + T E' | eps\n");
    printf("  T  -> F T'\n");
    printf("  T' -> * F T' | eps\n");
    printf("  F  -> ( E ) | id\n");

    print_parse_table();

    /* Test multiple inputs */
    const char *test_cases[] = {
        "id + id * id",
        "(id + id) * id",
        "id * id",
        "id + + id",       /* should be rejected */
        "id",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        printf("══════════════════════════════════════════════════════════════\n");
        printf("Input: \"%s\"\n", test_cases[i]);

        if (!tokenize(test_cases[i])) {
            printf("  ✗  Tokenization failed\n");
            continue;
        }
        parse();
    }

    /* Interactive mode */
    printf("══════════════════════════════════════════════════════════════\n");
    printf("Interactive mode — enter a string (or 'quit' to exit):\n");
    printf("Terminals: id  +  *  (  )\n\n");

    char buf[256];
    while (1) {
        printf("Input> ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) break;

        /* strip newline */
        buf[strcspn(buf, "\n")] = 0;
        if (strcmp(buf, "quit") == 0 || strcmp(buf, "exit") == 0) break;
        if (buf[0] == '\0') continue;

        if (!tokenize(buf)) continue;
        parse();
    }

    return 0;
}