#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT 256
#define EPSILON "\u03B5"
#define MAX_STACK 100
#define NUM_NON_TERMINALS 9
#define ASCII_MAX 130
#define CHAR_POSITIVE_NUMBER '+'
#define CHAR_NEGATIVE_NUMBER '-'
#define MAX_PRINT_DEPTH 1000

typedef struct NODE *TREE;
struct NODE
{
    char label;
    int int_value;
    TREE leftmostChild, rightSibling;
};

typedef enum
{
    NT_E,
    NT_T,
    NT_U,
    NT_G,
    NT_F,
    NT_A,
    NT_N,
    NT_M,
    NT_D
} NonTerminal;

typedef enum
{
    T_PLUS = '+',
    T_MINUS = '-',
    T_STAR = '*',
    T_SLASH = '/',
    T_LPAREN = '(',
    T_RPAREN = ')',
    T_POSITIVE_NUMBER = 128,
    T_NEGATIVE_NUMBER = 129,
    T_EOI = '\0',
    T_EPSILON = 'e'
} Terminal;

typedef struct
{
    bool is_terminal;
    union
    {
        Terminal terminal;
        NonTerminal non_terminal;
    } value;
    int int_value; // To store the actual number for T_POSITIVE_NUMBER and T_NEGATIVE_NUMBER
} Symbol;

// Production structure
typedef struct
{
    NonTerminal lhs;
    Symbol rhs[10];
    int rhs_length;
} Production;

void initialize_parsing_table(Production *parsing_table[NUM_NON_TERMINALS][ASCII_MAX]);
// Parsing table
Production *parsing_table[NUM_NON_TERMINALS][ASCII_MAX];

// Function prototypes
TREE E();
TREE U();
TREE T();
TREE G();
TREE F();
TREE A();
TREE N();
TREE M();
TREE D();

// Utility functions
TREE makeNode0(char x);
TREE makeNode1(char x, TREE t);
TREE makeNode2(char x, TREE t1, TREE t2);

// Parsing utilities
char *nextTerminal;
bool lookahead(char c);
bool match(char c);

TREE makeNode0(char x)
{
    TREE root;
    root = (TREE)malloc(sizeof(struct NODE));
    root->label = x;
    root->int_value = 0; // Initialize int_value to 0 for all nodes
    root->leftmostChild = NULL;
    root->rightSibling = NULL;
    return root;
}

TREE makeNode1(char x, TREE t)
{
    TREE root;
    root = makeNode0(x);
    root->leftmostChild = t;
    return root;
}

TREE makeNode2(char x, TREE t1, TREE t2)
{
    TREE root = makeNode1(x, t1);
    t1->rightSibling = t2;
    return root;
}

bool lookahead(char c)
{
    return *nextTerminal == c;
}

bool match(char c)
{
    if (lookahead(c))
    {
        nextTerminal++;
        return true;
    }
    return false;
}

TREE E()
{
    TREE t = T();
    if (t == NULL)
    {
        return NULL;
    }
    TREE u = U();
    if (u == NULL)
    {
        return NULL;
    }
    return makeNode2('E', t, u);
}

TREE U()
{
    if (lookahead('+'))
    {
        match('+');
        TREE e = E();
        if (e == NULL)
            return NULL; // Error in E
        return makeNode2('U', makeNode0('+'), e);
    }
    if (lookahead('-'))
    {
        match('-');
        TREE e = E();
        if (e == NULL)
            return NULL; // Error in E
        return makeNode2('U', makeNode0('-'), e);
    }
    else
    {
        return makeNode1('U', makeNode0(EPSILON[0]));
    }
}

TREE T()
{
    TREE f = F();
    if (f == NULL)
    {
        return NULL;
    }
    TREE g = G();
    if (g == NULL)
    {
        // Don't forget to free f if you're managing memory manually
        return NULL;
    }
    return makeNode2('T', f, g);
}

TREE G()
{
    if (lookahead('*'))
    {
        match('*');
        TREE t = T();
        if (t == NULL)
            return NULL;
        return makeNode2('G', makeNode0('*'), t);
    }
    if (lookahead('/'))
    {
        match('/');
        TREE t = T();
        if (t == NULL)
            return NULL;
        return makeNode2('G', makeNode0('/'), t);
    }
    else
    {
        return makeNode1('G', makeNode0(EPSILON[0]));
    }
}

TREE F()
{
    if (lookahead('+'))
    {
        match('+');
        TREE f = F();
        if (f == NULL)
            return NULL;
        return makeNode2('F', makeNode0('+'), f);
    }
    else if (lookahead('-'))
    {
        match('-');
        TREE f = F();
        if (f == NULL)
            return NULL;
        return makeNode2('F', makeNode0('-'), f);
    }
    else
    {
        return makeNode1('F', A());
    }
}

TREE A()
{
    if (lookahead('('))
    {
        match('(');
        TREE e = E(); // Parse E and store the result
        if (!match(')'))
        {
            return NULL;
        }
        return makeNode1('A', e); // Use the stored result
    }
    else
    {
        return makeNode1('A', N());
    }
}

TREE N()
{
    TREE d = D();
    if (d == NULL)
        return NULL;
    TREE m = M();
    if (m == NULL)
    {
        return NULL;
    }
    return makeNode2('N', d, m);
}

TREE M()
{
    if (isdigit(*nextTerminal))
    {
        return makeNode1('M', N());
    }
    else
    {
        return makeNode1('M', makeNode0(EPSILON[0]));
    }
}

TREE D()
{
    if (isdigit(*nextTerminal) || *nextTerminal == '-')
    {
        int value = 0;
        bool is_negative = false;
        if (*nextTerminal == '-')
        {
            is_negative = true;
            match('-');
        }
        while (isdigit(*nextTerminal))
        {
            value = value * 10 + (*nextTerminal - '0');
            match(*nextTerminal);
        }
        if (is_negative)
            value = -value;
        TREE node = makeNode0('D');
        node->int_value = value;
        return node;
    }
    else
    {
        return NULL;
    }
}

// Main parsing function
TREE recursive_descent_parse(char *input)
{
    nextTerminal = input;
    if (*input == '\0')
    {
        // Empty input
        return makeNode1('E', makeNode0(EPSILON[0]));
    }
    TREE parseTree = E();
    if (parseTree != NULL && *nextTerminal == '\0')
    {
        return parseTree;
    }
    printf("Error: Invalid input\n");
    return NULL;
}

void printParseTree(TREE root, int depth)
{
    if (depth > MAX_PRINT_DEPTH)
    {
        printf("Maximum print depth exceeded\n");
        return;
    }

    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }

    if (root->label == EPSILON[0])
    {
        printf("%s\n", EPSILON);
    }
    else if (root->label == 'D')
    {
        printf("D\n");
        for (int i = 0; i <= depth; i++)
        {
            printf("  ");
        }
        printf("%d\n", root->int_value);
    }
    else
    {
        printf("%c\n", root->label);
    }

    printParseTree(root->leftmostChild, depth + 1);
    printParseTree(root->rightSibling, depth);
}

// Part 2
// Stack for parsing
Symbol stack[MAX_STACK];
int top = -1;

// Function prototypes
Production *create_production(NonTerminal lhs, Symbol *rhs, int rhs_length);
void push(Symbol symbol);
Symbol pop();
Terminal get_next_token(const char *input, int *pos, int *value);

// Initialize the parsing table
void initialize_parsing_table(Production *parsing_table[NUM_NON_TERMINALS][ASCII_MAX])
{
    printf("Initializing parsing table...\n");
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
    {
        for (int j = 0; j < ASCII_MAX; j++)
        {
            parsing_table[i][j] = NULL;
        }
    }

// Helper macros to create terminal and non-terminal symbols
#define T(x) (Symbol){true, {.terminal = x}} // creates Symbol struct representing terminal
#define NT(x) (Symbol){false, {.non_terminal = x}} // creates Symbol struct representing NON-terminal

    // E -> TU
    Symbol rhs_E[] = {NT(NT_T), NT(NT_U)};
    parsing_table[NT_E][T_POSITIVE_NUMBER] = create_production(NT_E, rhs_E, 2);
    parsing_table[NT_E][T_NEGATIVE_NUMBER] = create_production(NT_E, rhs_E, 2);
    parsing_table[NT_E][T_PLUS] = create_production(NT_E, rhs_E, 2);
    parsing_table[NT_E][T_MINUS] = create_production(NT_E, rhs_E, 2);
    parsing_table[NT_E][T_LPAREN] = create_production(NT_E, rhs_E, 2);

    // U -> +E | -E | ε
    Symbol rhs_U_plus[] = {T(T_PLUS), NT(NT_E)};
    Symbol rhs_U_minus[] = {T(T_MINUS), NT(NT_E)};
    Symbol rhs_U_epsilon[] = {T(T_EPSILON)};
    parsing_table[NT_U][T_PLUS] = create_production(NT_U, rhs_U_plus, 2);
    parsing_table[NT_U][T_MINUS] = create_production(NT_U, rhs_U_minus, 2);
    parsing_table[NT_U][T_RPAREN] = create_production(NT_U, rhs_U_epsilon, 1);
    parsing_table[NT_U][T_EOI] = create_production(NT_U, rhs_U_epsilon, 1);

    // T -> FG
    Symbol rhs_T_FG[] = {NT(NT_F), NT(NT_G)};
    parsing_table[NT_T][T_POSITIVE_NUMBER] = create_production(NT_T, rhs_T_FG, 2);
    parsing_table[NT_T][T_NEGATIVE_NUMBER] = create_production(NT_T, rhs_T_FG, 2);
    parsing_table[NT_T][T_LPAREN] = create_production(NT_T, rhs_T_FG, 2);
    parsing_table[NT_T][T_PLUS] = create_production(NT_T, rhs_T_FG, 2);
    parsing_table[NT_T][T_MINUS] = create_production(NT_T, rhs_T_FG, 2);

    // G -> *T | /T | ε
    Symbol rhs_G_mult[] = {T(T_STAR), NT(NT_T)};
    Symbol rhs_G_div[] = {T(T_SLASH), NT(NT_T)};
    Symbol rhs_G_epsilon[] = {T(T_EPSILON)};
    parsing_table[NT_G][T_STAR] = create_production(NT_G, rhs_G_mult, 2);
    parsing_table[NT_G][T_SLASH] = create_production(NT_G, rhs_G_div, 2);
    parsing_table[NT_G][T_PLUS] = create_production(NT_G, rhs_G_epsilon, 1);
    parsing_table[NT_G][T_MINUS] = create_production(NT_G, rhs_G_epsilon, 1);
    parsing_table[NT_G][T_RPAREN] = create_production(NT_G, rhs_G_epsilon, 1);
    parsing_table[NT_G][T_EOI] = create_production(NT_G, rhs_G_epsilon, 1);

    // F -> +F | -F | A
    Symbol rhs_F_plus[] = {T(T_PLUS), NT(NT_F)};
    Symbol rhs_F_minus[] = {T(T_MINUS), NT(NT_F)};
    Symbol rhs_F_A[] = {NT(NT_A)};
    parsing_table[NT_F][T_PLUS] = create_production(NT_F, rhs_F_plus, 2);
    parsing_table[NT_F][T_MINUS] = create_production(NT_F, rhs_F_minus, 2);
    parsing_table[NT_F][T_POSITIVE_NUMBER] = create_production(NT_F, rhs_F_A, 1);
    parsing_table[NT_F][T_NEGATIVE_NUMBER] = create_production(NT_F, rhs_F_A, 1);
    parsing_table[NT_F][T_LPAREN] = create_production(NT_F, rhs_F_A, 1);

    // A -> (E) | N
    Symbol rhs_A_paren[] = {T(T_LPAREN), NT(NT_E), T(T_RPAREN)};
    Symbol rhs_A_N[] = {NT(NT_N)};
    parsing_table[NT_A][T_LPAREN] = create_production(NT_A, rhs_A_paren, 3);
    parsing_table[NT_A][T_POSITIVE_NUMBER] = create_production(NT_A, rhs_A_N, 1);
    parsing_table[NT_A][T_NEGATIVE_NUMBER] = create_production(NT_A, rhs_A_N, 1);

    // N -> DM
    Symbol rhs_N[] = {NT(NT_D), NT(NT_M)};
    parsing_table[NT_N][T_POSITIVE_NUMBER] = create_production(NT_N, rhs_N, 2);
    parsing_table[NT_N][T_NEGATIVE_NUMBER] = create_production(NT_N, rhs_N, 2);

    // M -> N | ε
    Symbol rhs_M_N[] = {NT(NT_N)};
    Symbol rhs_M_epsilon[] = {T(T_EPSILON)};
    parsing_table[NT_M][T_POSITIVE_NUMBER] = create_production(NT_M, rhs_M_N, 1);
    parsing_table[NT_M][T_NEGATIVE_NUMBER] = create_production(NT_M, rhs_M_N, 1);
    parsing_table[NT_M][T_PLUS] = create_production(NT_M, rhs_M_epsilon, 1);
    parsing_table[NT_M][T_MINUS] = create_production(NT_M, rhs_M_epsilon, 1);
    parsing_table[NT_M][T_STAR] = create_production(NT_M, rhs_M_epsilon, 1);
    parsing_table[NT_M][T_SLASH] = create_production(NT_M, rhs_M_epsilon, 1);
    parsing_table[NT_M][T_RPAREN] = create_production(NT_M, rhs_M_epsilon, 1);
    parsing_table[NT_M][T_EOI] = create_production(NT_M, rhs_M_epsilon, 1);

    // D -> POSITIVE_NUMBER | NEGATIVE_NUMBER
    Symbol rhs_D_positive[] = {T(T_POSITIVE_NUMBER)};
    Symbol rhs_D_negative[] = {T(T_NEGATIVE_NUMBER)};
    parsing_table[NT_D][T_POSITIVE_NUMBER] = create_production(NT_D, rhs_D_positive, 1);
    parsing_table[NT_D][T_NEGATIVE_NUMBER] = create_production(NT_D, rhs_D_negative, 1);

    printf("Parsing table initialized.\n");
}

Production *create_production(NonTerminal lhs, Symbol *rhs, int rhs_length)
{
    Production *p = (Production *)malloc(sizeof(Production));
    p->lhs = lhs;
    memcpy(p->rhs, rhs, rhs_length * sizeof(Symbol));
    for (int i = 0; i < rhs_length; i++)
    {
        p->rhs[i].int_value = 0; 
    }
    p->rhs_length = rhs_length;
    return p;
}

void push(Symbol symbol)
{
    if (top < MAX_STACK - 1)
    {
        stack[++top] = symbol;
    }
    else
    {
        printf("Stack overflow\n");
        exit(1);
    }
}

Symbol pop()
{
    if (top >= 0)
    {
        return stack[top--];
    }
    else
    {
        printf("Stack underflow\n");
        exit(1);
    }
}

Terminal get_next_token(const char *input, int *pos, int *value)
{
    char c = input[*pos];
    while (isspace(c))
    {
        (*pos)++;
        c = input[*pos];
    }

    if (c == '-')
    {
        (*pos)++;
        if (isdigit(input[*pos]))
        {
            *value = 0;
            while (isdigit(input[*pos]))
            {
                *value = *value * 10 + (input[*pos] - '0');
                (*pos)++;
            }
            return T_NEGATIVE_NUMBER;
        }
        return T_MINUS;
    }
    else if (c == '+')
    {
        (*pos)++;
        return T_PLUS;
    }
    else if (isdigit(c))
    {
        *value = 0;
        while (isdigit(c))
        {
            *value = *value * 10 + (c - '0');
            (*pos)++;
            c = input[*pos];
        }
        return T_POSITIVE_NUMBER;
    }
    else if (c == '*' || c == '/' || c == '(' || c == ')')
    {
        (*pos)++;
        return (Terminal)c;
    }
    else if (c == '\0')
    {
        return T_EOI;
    }

    // Unrecognized token
    printf("Lexer Error: Unrecognized token '%c'\n", c);
    (*pos)++;
    return T_EOI;
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

#define MAX_PARSE_DEPTH 1000

TREE table_driven_parse(Production *parsing_table[NUM_NON_TERMINALS][ASCII_MAX], NonTerminal start_symbol, const char *input)
{
    int input_pos = 0;
    int value;
    Terminal current_token = get_next_token(input, &input_pos, &value);

    Symbol stack[MAX_STACK];
    int top = -1;

    stack[++top] = (Symbol){true, {.terminal = T_EOI}};
    stack[++top] = (Symbol){false, {.non_terminal = start_symbol}};

    TREE parse_tree = makeNode0('E');
    TREE current_node = parse_tree;
    TREE *node_stack[MAX_STACK];
    int node_top = 0;
    node_stack[node_top++] = &parse_tree;
    int parse_depth = 0;

    while (top >= 0 && current_token != T_EOI && parse_depth < MAX_PARSE_DEPTH)
    {
        parse_depth++;
        Symbol top_symbol = stack[top--];
        if (node_top > 0)
        {
            current_node = *node_stack[--node_top];
        }
        if (top_symbol.is_terminal)
        {
            if (top_symbol.value.terminal == T_EPSILON)
            {
                current_node->leftmostChild = makeNode0(EPSILON[0]);
            }
            else if (top_symbol.value.terminal == current_token)
            {
                if (current_token == T_POSITIVE_NUMBER)
                {
                    TREE number_node = makeNode0('D');
                    number_node->int_value = value;
                    current_node->leftmostChild = number_node;
                }
                else
                {
                    current_node->leftmostChild = makeNode0(current_token);
                }
                current_token = get_next_token(input, &input_pos, &value);
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            Production *prod = parsing_table[top_symbol.value.non_terminal][current_token];
            if (prod == NULL)
            {
                return NULL;
            }
            if (prod->lhs == NT_F && current_token == T_NEGATIVE_NUMBER)
            {
                TREE minus_node = makeNode0('-');
                TREE f_node = makeNode0('F');
                TREE a_node = makeNode0('A');
                TREE n_node = makeNode0('N');
                TREE d_node = makeNode0('D');
                d_node->int_value = value;
                TREE m_node = makeNode0('M');
                m_node->leftmostChild = makeNode0(EPSILON[0]);

                n_node->leftmostChild = d_node;
                d_node->rightSibling = m_node;
                a_node->leftmostChild = n_node;
                f_node->leftmostChild = a_node;
                minus_node->rightSibling = f_node;
                current_node->leftmostChild = minus_node;

                current_token = get_next_token(input, &input_pos, &value);
                continue;
            }

            TREE first_child = NULL;
            for (int i = prod->rhs_length - 1; i >= 0; i--)
            {
                TREE new_node;
                if (prod->rhs[i].is_terminal)
                {
                    if (prod->rhs[i].value.terminal == T_EPSILON)
                    {
                        new_node = makeNode0(EPSILON[0]);
                        // printf("DEBUG: Created epsilon node in production\n");
                    }
                    else
                    {
                        new_node = makeNode0(prod->rhs[i].value.terminal);
                        // printf("DEBUG: Created terminal node %c in production\n", prod->rhs[i].value.terminal);
                    }
                }
                else
                {
                    new_node = makeNode0("ETUGFANMD"[prod->rhs[i].value.non_terminal]);
                    // printf("DEBUG: Created non-terminal node %c in production\n", "ETUGFANMD"[prod->rhs[i].value.non_terminal]);
                }
                if (first_child == NULL)
                {
                    first_child = new_node;
                }
                else
                {
                    new_node->rightSibling = first_child;
                }
                first_child = new_node;
                stack[++top] = prod->rhs[i];
                node_stack[node_top++] = &new_node;
            }
            current_node->leftmostChild = first_child;
        }
    }

    if (parse_depth >= MAX_PARSE_DEPTH)
    {
        printf("Parse error: Maximum parse depth exceeded\n");
        return NULL;
    }

    TREE t_node = parse_tree->leftmostChild;
    if (t_node)
    {
        TREE f_node = t_node->leftmostChild;
        if (f_node)
        {
            TREE g_node = f_node->rightSibling;
            if (g_node && !g_node->leftmostChild)
            {
                g_node->leftmostChild = makeNode0(EPSILON[0]);
            }
        }
        TREE u_node = t_node->rightSibling;
        if (u_node && !u_node->leftmostChild)
        {
            u_node->leftmostChild = makeNode0(EPSILON[0]);
        }
    }

    TREE f_node = parse_tree->leftmostChild->leftmostChild;
    if (f_node)
    {
        TREE a_node = f_node->leftmostChild;
        if (f_node->leftmostChild && f_node->leftmostChild->label == '-')
        {
            a_node = f_node->leftmostChild->rightSibling->leftmostChild;
        }
        if (a_node && a_node->label == 'A')
        {
            TREE n_node = a_node->leftmostChild;
            if (n_node && n_node->label == 'N')
            {
                TREE d_node = n_node->leftmostChild;
                if (d_node)
                {
                    TREE m_node = d_node->rightSibling;
                    if (m_node && !m_node->leftmostChild)
                    {
                        m_node->leftmostChild = makeNode0(EPSILON[0]);
                    }
                }
            }
        }
    }

    if (current_token == T_EOI)
    {
        printf("Parse successful. Reached end of input.\n");
        return parse_tree;
    }
    else
    {
        printf("Parse error: Unexpected end of input\n");
        return NULL;
    }
}

int main()
{
    char input[MAX_INPUT];
    Production *parsing_table[NUM_NON_TERMINALS][ASCII_MAX];
    initialize_parsing_table(parsing_table);

    while (1)
    {
        printf("Enter input to test (or 'quit' to exit): ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
            len--;
        }

        if (len == MAX_INPUT - 1 && input[len] != '\n')
        {
            printf("Error: Input too long (max 255 characters)\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }

        if (len == 0)
        {
            printf("Empty input is valid, but skipped.\n");
            continue;
        }

        if (strcmp(input, "quit") == 0)
        {
            break;
        }

        printf("Testing \"%s\"\n", input);

        printf("Choose parsing method:\n");
        printf("1. Recursive Descent\n");
        printf("2. Table-Driven\n");
        int choice;
        scanf("%d", &choice);
        getchar(); // Consume newline

        TREE parseTree = NULL;
        if (choice == 1)
        {
            parseTree = recursive_descent_parse(input);
            if (parseTree != NULL)
            {
                printf("Recursive Descent Parse successful. Parse tree:\n");
                printParseTree(parseTree, 0);
            }
            else
            {
                printf("Recursive Descent Parse failed\n");
            }
        }
        else if (choice == 2)
        {
            parseTree = table_driven_parse(parsing_table, NT_E, input);
            if (parseTree != NULL)
            {
                printf("Table-Driven Parse successful. Parse tree:\n");
                printParseTree(parseTree, 0);
            }
            else
            {
                printf("Table-Driven Parse failed\n");
            }
        }
        else
        {
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }
    return 0;
}