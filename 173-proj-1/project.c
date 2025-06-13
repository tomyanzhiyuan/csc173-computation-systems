#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 128
#define MAX_INPUT_LENGTH 1000

typedef struct
{
    int numStates;
    int numFinalStates;
    int finalStates[MAX_STATES];
    int transitionTable[MAX_STATES][MAX_ALPHABET];
    int currentState;
} DFA;

DFA *createDFA(int numStates, int numFinalStates, int *finalStates)
{
    DFA *dfa = malloc(sizeof(DFA));
    dfa->numStates = numStates;
    dfa->numFinalStates = numFinalStates;
    memcpy(dfa->finalStates, finalStates, numFinalStates * sizeof(int));

    for (int s = 0; s < MAX_STATES; s++)
    {
        for (int x = 0; x < MAX_ALPHABET; x++)
        {
            dfa->transitionTable[s][x] = -1; // all transitions are invalid by default until defined
        }
    }

    dfa->currentState = 0;
    return dfa;
}

void addTransitionDFA(DFA *dfa, int fromState, char input, int toState)
{
    dfa->transitionTable[fromState][(unsigned char)input] = toState;
}

bool runDFA(DFA *dfa, const char *input)
{
    dfa->currentState = 0;
    printf("Starting state: %d\n", dfa->currentState);
    while (*input)
    {
        int nextState = dfa->transitionTable[dfa->currentState][(unsigned char)*input];
        // printf("Input: %c, Current state: %d, Next state: %d\n", *input, dfa->currentState, nextState);
        if (nextState == -1)
        {
            printf("No valid transition. Rejecting.\n");
            return false;
        }
        dfa->currentState = nextState;
        input++;
    }

    printf("Final state: %d\n", dfa->currentState);
    for (int s = 0; s < dfa->numFinalStates; s++)
    {
        if (dfa->currentState == dfa->finalStates[s])
        {
            printf("Accepted: Final state %d is in the set of final states.\n", dfa->currentState);
            return true;
        }
    }
    printf("Rejected: Final state %d is not in the set of final states.\n", dfa->currentState);
    return false;
}

void dfaREPL(DFA *dfa)
{
    char input[MAX_INPUT_LENGTH];
    printf("Enter strings to test (or 'quit' to exit):\n");
    while (1)
    {
        printf("Enter a string: ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        // Remove newline character if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
            len--;
        }

        // Echo back the user's input
        printf("You entered: %s\n", input);

        if (strcmp(input, "quit") == 0)
            break;

        bool result = runDFA(dfa, input);
        printf("Input '%s' is %s\n", input, result ? "accepted" : "rejected");
        printf("\n"); // Add a blank line for better readability
    }
    printf("\n");
}

DFA *DFAForContainsXYZZY()
{
    int finalStates[] = {5};
    DFA *dfa = createDFA(6, 1, finalStates);
    addTransitionDFA(dfa, 0, 'x', 1);
    addTransitionDFA(dfa, 1, 'y', 2);
    addTransitionDFA(dfa, 2, 'z', 3);
    addTransitionDFA(dfa, 3, 'z', 4);
    addTransitionDFA(dfa, 4, 'y', 5);
    for (int s = 0; s < 6; s++)
    {
        for (int x = 0; x < 128; x++)
        {
            if (s == 0 && x != 'x')
                addTransitionDFA(dfa, s, x, 0);
            else if (s == 1 && x != 'y')
                addTransitionDFA(dfa, s, x, (x == 'x' ? 1 : 0));
            else if (s == 2 && x != 'z')
                addTransitionDFA(dfa, s, x, (x == 'x' ? 1 : 0));
            else if (s == 3 && x != 'z')
                addTransitionDFA(dfa, s, x, (x == 'x' ? 1 : 0));
            else if (s == 4 && x != 'y')
                addTransitionDFA(dfa, s, x, (x == 'x' ? 1 : 0));
            else if (s == 5)
                addTransitionDFA(dfa, s, x, s);
        }
    }
    return dfa;
}

DFA *DFASubsequence987()
{
    int finalStates[] = {3};
    DFA *dfa = createDFA(4, 1, finalStates);
    addTransitionDFA(dfa, 0, '9', 1);
    addTransitionDFA(dfa, 1, '8', 2);
    addTransitionDFA(dfa, 2, '7', 3);
    for (int s = 0; s < 4; s++)
    {
        for (int x = 0; x < 128; x++)
        {
            if (s == 0 && x != '9')
                addTransitionDFA(dfa, s, x, 0);
            else if (s == 1 && x != '8')
                addTransitionDFA(dfa, s, x, s);
            else if (s == 2 && x != '7')
                addTransitionDFA(dfa, s, x, s);
            else if (s == 3)
                addTransitionDFA(dfa, s, x, s);
        }
    }
    return dfa;
}

DFA *DFA4s()
{
    int finalStates[] = {2, 3};
    DFA *dfa = createDFA(5, 2, finalStates); // continue working from here 22:38 sept 14// either 4/5
    addTransitionDFA(dfa, 0, '4', 1);
    addTransitionDFA(dfa, 1, '4', 2);
    addTransitionDFA(dfa, 2, '4', 3);
    addTransitionDFA(dfa, 3, '4', 4);
    for (int s = 0; s < 5; s++)
    {
        for (int x = 0; x < 128; x++)
        {
            if (s == 0 && x != '4')
                addTransitionDFA(dfa, s, x, 0);
            else if (s == 1 && x != '4')
                addTransitionDFA(dfa, s, x, s);
            else if (s == 2 && x != '4')
                addTransitionDFA(dfa, s, x, s);
            else if (s == 3 && x != '4')
                addTransitionDFA(dfa, s, x, s);
            else if (s == 4)
                addTransitionDFA(dfa, s, x, s);
        }
    }
    return dfa;
}

DFA *DFAForBinaryParity()
{
    int finalStates[] = {3};
    DFA *dfa = createDFA(4, 1, finalStates);

    // State 0: Even 0s, Even 1s
    // State 1: Odd 0s, Even 1s
    // State 2: Even 0s, Odd 1s
    // State 3: Odd 0s, Odd 1s

    // Transitions for '0'
    addTransitionDFA(dfa, 0, '0', 1);
    addTransitionDFA(dfa, 1, '0', 0);
    addTransitionDFA(dfa, 2, '0', 3);
    addTransitionDFA(dfa, 3, '0', 2);

    // Transitions for '1'
    addTransitionDFA(dfa, 0, '1', 2);
    addTransitionDFA(dfa, 1, '1', 3);
    addTransitionDFA(dfa, 2, '1', 0);
    addTransitionDFA(dfa, 3, '1', 1);

    return dfa;
}

typedef struct
{
    int numStates;
    int numFinalStates;
    int finalStates[MAX_STATES];
    int transitionTable[MAX_STATES][MAX_ALPHABET][MAX_STATES];
    int numTransitions[MAX_STATES][MAX_ALPHABET];
    int epsilonTransitions[MAX_STATES][MAX_STATES];
    int numEpsilonTransitions[MAX_STATES];
} NFA;

NFA *createNFA(int numStates, int numFinalStates, int *finalStates)
{
    NFA *nfa = malloc(sizeof(NFA));
    nfa->numStates = numStates;
    nfa->numFinalStates = numFinalStates;
    memcpy(nfa->finalStates, finalStates, numFinalStates * sizeof(int));

    for (int s = 0; s < MAX_STATES; s++)
    {
        for (int x = 0; x < MAX_ALPHABET; x++)
        {
            nfa->numTransitions[s][x] = 0;
        }
        nfa->numEpsilonTransitions[s] = 0;
    }

    return nfa;
}

void addTransitionNFA(NFA *nfa, int fromState, char input, int toState)
{
    nfa->transitionTable[fromState][(unsigned char)input][nfa->numTransitions[fromState][(unsigned char)input]++] = toState;
}

void addEpsilonTransition(NFA *nfa, int fromState, int toState)
{
    nfa->epsilonTransitions[fromState][nfa->numEpsilonTransitions[fromState]++] = toState;
}

bool runNFA(NFA *nfa, const char *input)
{
    bool currentStates[MAX_STATES] = {false};
    bool nextStates[MAX_STATES] = {false};

    // Initialize with start state
    currentStates[0] = true;

    // Process each input character
    while (*input)
    {
        memset(nextStates, false, sizeof(nextStates));

        for (int s = 0; s < nfa->numStates; s++)
        {
            if (currentStates[s])
            {
                // Handle transitions for the current input character
                for (int i = 0; i < nfa->numTransitions[s][(unsigned char)*input]; i++)
                {
                    int nextState = nfa->transitionTable[s][(unsigned char)*input][i];
                    nextStates[nextState] = true;
                }
            }
        }

        memcpy(currentStates, nextStates, sizeof(currentStates));
        input++;
    }

    // Check if final state
    for (int i = 0; i < nfa->numFinalStates; i++)
    {
        if (currentStates[nfa->finalStates[i]])
        {
            return true;
        }
    }

    return false;
}

void nfaREPL(NFA *nfa)
{
    char input[MAX_INPUT_LENGTH];
    printf("Enter strings to test (or 'quit' to exit):\n");
    while (1)
    {
        printf("Enter a string: ");
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

        if (strcmp(input, "quit") == 0)
            break;

        bool result = runNFA(nfa, input);
        printf("Input '%s' is %s\n", input, result ? "accepted" : "rejected");
        printf("\n");
    }
    printf("\n");
}

NFA *NFAStringsEndingInGH()
{
    int finalStates[] = {2};
    NFA *nfa = createNFA(3, 1, finalStates);

    addTransitionNFA(nfa, 0, 'g', 1);

    addTransitionNFA(nfa, 1, 'h', 2);

    for (int c = 0; c < MAX_ALPHABET; c++)
    {
        if (c != 'g')
        {
            addTransitionNFA(nfa, 0, c, 0);
        }
        if (c != 'h')
        {
            addTransitionNFA(nfa, 1, c, 0); // Changed from 1 to 0
        }
        if (c != 'g')
        {
            addTransitionNFA(nfa, 2, c, 0); // Changed from 2 to 0
        }
        else
        {
            addTransitionNFA(nfa, 2, 'g', 1);
        }
    }

    return nfa;
}

NFA *NFAStringsContainingMoo()
{
    int finalStates[] = {3};
    NFA *nfa = createNFA(4, 1, finalStates);

    // State 0: Initial state
    // State 1: Seen 'm'
    // State 2: Seen 'mo'
    // State 3: Seen 'moo' (accepting state)

    for (int c = 0; c < 128; c++)
    {
        addTransitionNFA(nfa, 0, c, 0);
    }
    addTransitionNFA(nfa, 0, 'm', 1);

    for (int c = 0; c < 128; c++)
    {
        addTransitionNFA(nfa, 1, c, 0);
    }
    addTransitionNFA(nfa, 1, 'o', 2);

    for (int c = 0; c < 128; c++)
    {
        addTransitionNFA(nfa, 2, c, 0);
    }
    addTransitionNFA(nfa, 2, 'o', 3);

    for (int c = 0; c < 128; c++)
    {
        addTransitionNFA(nfa, 3, c, 3);
    }

    return nfa;
}

NFA *NFASpecialString()
{                                          // >1 'a' or 'i' || >2 'y' || >3 'c' or 'l'
    int finalStates[] = {2, 4, 7, 11, 15}; // Accept states for each condition
    NFA *nfa = createNFA(16, 5, finalStates);

    // States: 0 (start), 1-2 (a), 3-4 (i), 5-7 (y), 8-11 (c), 12-15 (l)

    // Transitions for counting 'a'
    addTransitionNFA(nfa, 0, 'a', 1);
    addTransitionNFA(nfa, 1, 'a', 2);

    // Transitions for counting 'i'
    addTransitionNFA(nfa, 0, 'i', 3);
    addTransitionNFA(nfa, 3, 'i', 4);

    // Transitions for counting 'y'
    addTransitionNFA(nfa, 0, 'y', 5);
    addTransitionNFA(nfa, 5, 'y', 6);
    addTransitionNFA(nfa, 6, 'y', 7);

    // Transitions for counting 'c'
    addTransitionNFA(nfa, 0, 'c', 8);
    addTransitionNFA(nfa, 8, 'c', 9);
    addTransitionNFA(nfa, 9, 'c', 10);
    addTransitionNFA(nfa, 10, 'c', 11);

    // Transitions for counting 'l'
    addTransitionNFA(nfa, 0, 'l', 12);
    addTransitionNFA(nfa, 12, 'l', 13);
    addTransitionNFA(nfa, 13, 'l', 14);
    addTransitionNFA(nfa, 14, 'l', 15);

    // Self-loops for all states
    for (int s = 0; s < 16; s++)
    {
        for (int c = 0; c < 128; c++)
        {
            addTransitionNFA(nfa, s, c, s);
        }
    }

    return nfa;
}

typedef struct
{
    int states[MAX_STATES];
    int count;
} StateSet;

bool contains(StateSet *set, int state)
{
    for (int i = 0; i < set->count; i++)
    {
        if (set->states[i] == state)
            return true;
    }
    return false;
}

bool equalSets(StateSet *set1, StateSet *set2)
{
    if (set1->count != set2->count)
        return false;
    for (int i = 0; i < set1->count; i++)
    {
        if (!contains(set2, set1->states[i]))
            return false;
    }
    return true;
}

int findDFAState(StateSet *workList, int workListSize, StateSet *set)
{
    for (int i = 0; i < workListSize; i++)
    {
        if (equalSets(&workList[i], set))
            return i;
    }
    return -1;
}

bool isNFAFinalState(NFA *nfa, int state)
{
    for (int i = 0; i < nfa->numFinalStates; i++)
    {
        if (nfa->finalStates[i] == state)
            return true;
    }
    return false;
}

void epsilonClosure(NFA *nfa, StateSet *set)
{
    bool changed;
    do
    {
        changed = false;
        for (int i = 0; i < set->count; i++)
        {
            int state = set->states[i];
            for (int j = 0; j < nfa->numEpsilonTransitions[state]; j++)
            {
                int epsilonState = nfa->epsilonTransitions[state][j];
                if (!contains(set, epsilonState))
                {
                    set->states[set->count++] = epsilonState;
                    changed = true;
                }
            }
        }
    } while (changed);
}

DFA *createDFAForStringsEndingInGH()
{
    printf("Creating DFA for strings ending in 'gh'...\n");
    int finalStates[] = {2};
    DFA *dfa = createDFA(3, 1, finalStates);

    // State 0: Initial state
    // State 1: Seen 'g'
    // State 2: Seen 'gh' (accepting state)

    // Add transitions
    for (int c = 0; c < MAX_ALPHABET; c++)
    {
        addTransitionDFA(dfa, 0, c, 0); // Stay in state 0 for all inputs except 'g'
        addTransitionDFA(dfa, 1, c, 0); // Go back to state 0 for all inputs except 'h'
        addTransitionDFA(dfa, 2, c, 0); // Go back to state 0 for all inputs
    }

    addTransitionDFA(dfa, 0, 'g', 1); // Go to state 1 when 'g' is seen
    addTransitionDFA(dfa, 1, 'h', 2); // Go to state 2 when 'h' is seen after 'g'
    addTransitionDFA(dfa, 2, 'g', 1); // Allow 'g' to start a new potential match

    printf("DFA created with %d states.\n", dfa->numStates);
    return dfa;
}

DFA *NFA_to_DFA(NFA *nfa)
{
    printf("Starting NFA to DFA conversion...\n");
    fflush(stdout);

    StateSet *dfaStates = malloc(sizeof(StateSet) * (1 << nfa->numStates));
    int dfaStateCount = 0;

    StateSet initialSet = {{0}, 1};
    epsilonClosure(nfa, &initialSet);
    dfaStates[dfaStateCount++] = initialSet;

    DFA *dfa = createDFA(1, 0, NULL);

    fflush(stdout);
    // int iterationCount = 0;
    int processingState = 0;
    while (processingState < dfaStateCount)
    {
        // iterationCount++;
        fflush(stdout);

        StateSet currentSet = dfaStates[processingState];

        for (int c = 0; c < MAX_ALPHABET; c++)
        {
            StateSet nextSet = {0};

            // Compute next state set
            for (int i = 0; i < currentSet.count; i++)
            {
                int state = currentSet.states[i];
                for (int j = 0; j < nfa->numTransitions[state][c]; j++)
                {
                    int nextState = nfa->transitionTable[state][c][j];
                    if (!contains(&nextSet, nextState))
                    {
                        nextSet.states[nextSet.count++] = nextState;
                    }
                }
            }

            epsilonClosure(nfa, &nextSet);

            if (nextSet.count > 0)
            {
                int dfaState = findDFAState(dfaStates, dfaStateCount, &nextSet);
                if (dfaState == -1)
                {
                    dfaState = dfaStateCount;
                    dfaStates[dfaStateCount++] = nextSet;
                }
                addTransitionDFA(dfa, processingState, c, dfaState);
            }
        }
        processingState++;
    }

    // Set final states
    int finalStates[MAX_STATES];
    int numFinalStates = 0;
    for (int i = 0; i < dfaStateCount; i++)
    {
        StateSet set = dfaStates[i];
        for (int j = 0; j < set.count; j++)
        {
            if (isNFAFinalState(nfa, set.states[j]))
            {
                finalStates[numFinalStates++] = i;
                break;
            }
        }
    }

    dfa->numStates = dfaStateCount;
    dfa->numFinalStates = numFinalStates;
    memcpy(dfa->finalStates, finalStates, numFinalStates * sizeof(int));

    free(dfaStates);
    fflush(stdout);

    return dfa;
}

int main()
{
    DFA *dfaXYZZY = DFAForContainsXYZZY();
    printf("Testing DFA for strings containing 'xyzzy':\n");
    dfaREPL(dfaXYZZY);
    free(dfaXYZZY);

    DFA *dfa987 = DFASubsequence987();
    printf("Testing DFA for strings containing '987':\n");
    dfaREPL(dfa987);
    free(dfa987);

    DFA *dfa4s = DFA4s();
    printf("Testing DFA for strings containing two or three '4':\n");
    dfaREPL(dfa4s);
    free(dfa4s);

    DFA *dfaBinaryParity = DFAForBinaryParity();
    printf("Testing DFA for strings containing odd number of zeroes AND ones:\n");
    dfaREPL(dfaBinaryParity);
    free(dfaBinaryParity);

    // part 2 NFA
    NFA *nfaGH = NFAStringsEndingInGH();
    printf("Testing NFA for strings ending in 'gh':\n");
    nfaREPL(nfaGH);
    free(nfaGH);

    NFA *nfaMoo = NFAStringsContainingMoo();
    printf("Testing NFA for strings containing 'moo':\n");
    nfaREPL(nfaMoo);
    free(nfaMoo);

    NFA *nfaSpecialString = NFASpecialString();
    printf("Testing NFA for strings containing >1 'a' or 'i' || >2 'y' || >3 'c' or 'l':\n");
    nfaREPL(nfaSpecialString);
    free(nfaSpecialString);

    // Convert NFA for strings ending in 'gh' to DFA
    NFA *nfaGHConverted = NFAStringsEndingInGH();
    DFA *dfaGHConverted = NFA_to_DFA(nfaGHConverted);
    // printf("DFA for strings ending in 'gh' has %d states.\n", dfaGHConverted->numStates);
    printf("Testing converted DFA for strings ending in 'gh':\n");
    dfaREPL(dfaGHConverted);
    free(nfaGHConverted);
    free(dfaGHConverted);

    // Convert NFA for strings containing 'moo' to DFA
    NFA *nfaMooConverted = NFAStringsContainingMoo();
    DFA *dfaMooConverted = NFA_to_DFA(nfaMooConverted);
    // printf("DFA for strings containing 'moo' has %d states.\n", dfaMooConverted->numStates);
    printf("Testing converted DFA for strings containing 'moo':\n");
    dfaREPL(dfaMooConverted);
    free(nfaMooConverted);
    free(dfaMooConverted);

    printf("Program completed.\n");

    return 0;
}
