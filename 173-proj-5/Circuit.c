/*
 * Circuit.c
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include "strdup.h"
#include "Circuit.h"

/**
 * A Connection in a Circuit is a directed link from a src Boolean to
 * a dst Boolean.
 */
typedef struct Connection *Connection;
struct Connection {
    Boolean src;
    Boolean dst;
    Connection next;
};

static Connection new_Connection(Boolean src, Boolean dst) {
    Connection this = (Connection)malloc(sizeof(struct Connection));
    this->src = src;
    this->dst = dst;
    this->next = NULL;
    return this;
}

/**
 * A ConnectionList is a linked list of Connections.
 * This implementation uses a ``last'' pointer for quick append.
 */
typedef struct ConnectionList *ConnectionList;
struct ConnectionList {
    Connection first;
    Connection last;
};

static ConnectionList new_ConnectionList() {
    ConnectionList this = (ConnectionList)malloc(sizeof(struct ConnectionList));
    this->first = NULL;
    this->last = NULL;
    return this;
}

/**
 * Free the given ConnectionList and all of its Connections (but not
 * their constituent Booleans).
 */
static void free_ConnectionList(ConnectionList this) {
    Connection c = this->first;
    while (c != NULL) {
	Connection next = c->next;
	free(c);
	c = next;
    }
    free(this);
}
		
/**
 * Append the given Connection to this ConnectionList (at the end).
 */
static void ConnectionList_add(ConnectionList this, Connection conn) {
    if (this->first == NULL) {
	this->first = conn;
	this->last = conn;
    } else {
	this->last->next = conn;
	this->last = conn;
    }
}

/**
 * Add a new Connection between src and dst to this ConnectionList (at the end).
 */
static void ConnectionList_addConnection(ConnectionList list, Boolean src, Boolean dst) {
    ConnectionList_add(list, new_Connection(src, dst));
}

/**
 * Return true if there is a (direct) connection between src and dst in this
 * ConnectionList.
 * This is not the fastest implementation. You could probably do better.
 */
static bool ConnectionList_find(ConnectionList list, Boolean src, Boolean dst) {
    for (Connection c=list->first; c != NULL; c=c->next) {
	if (c->src == src && c->dst == dst) {
	    return true;
	}
    }
    return false;
}

/**
 * A Circuit is a graph of Gates connected by Connections.
 */
struct Circuit {
    char *title;
    int numInputs;
    Boolean* inputs;
    int numOutputs;
    Boolean* outputs;
    int numGates;
    Gate* gates;
    ConnectionList connections;
};

/**
 * Allocate and return a new Circuit with the given title, inputs, outputs,
 * and gates.
 */
Circuit new_Circuit(char *title,
		    int numInputs, Boolean* inputs,
		    int numOutputs, Boolean* outputs,
		    int numGates, Gate* gates) {
    Circuit this = (Circuit)malloc(sizeof(struct Circuit));
    this->title = STRDUP(title); // strdup so we can free later
    this->numInputs = numInputs;
    this->inputs = inputs;
    this->numOutputs = numOutputs;
    this->outputs = outputs;
    this->numGates = numGates;
    this->gates = gates;
    this->connections = new_ConnectionList();
    return this;
}

/**
 * Free the given Circuit, including its inputs, outputs, and gates.
 */
void free_Circuit(Circuit this) {
    if (this->title != NULL) {
	free(this->title);
    }
    for (int i=0; i < this->numInputs; i++) {
	free_Boolean(this->inputs[i]);
    }
    free(this->inputs);
    for (int i=0; i < this->numOutputs; i++) {
	free_Boolean(this->outputs[i]);
    }
    free(this->outputs);
    for (int i=0; i < this->numGates; i++) {
	free_Gate(this->gates[i]);
    }
    free(this->gates);
    free_ConnectionList(this->connections);
    free(this);
}

/**
 * Return the title of the given Circuit.
 */
char* Circuit_getTitle(Circuit this) {
    return this->title;
}

/**
 * Return the number of inputs for the given Circuit.
 */
int Circuit_numInputs(Circuit this) {
    return this->numInputs;
}

/**
 * Return the index'th Boolean input of the given Circuit.
 */
Boolean Circuit_getInput(Circuit this, int index) {
    return this->inputs[index];
}

/**
 * Set the value of the index'th Boolean input of the given Circuit to the
 * given bool value.
 */
void Circuit_setInput(Circuit this, int index, bool value) {
    Boolean_setValue(this->inputs[index], value);
}

/**
 * Return the number of outputs for the given Circuit.
 */
int Circuit_numOutputs(Circuit this) {
    return this->numOutputs;
}

/**
 * Return the index'th Boolean output of the given Circuit.
 */
Boolean Circuit_getOutput(Circuit this, int index) {
    return this->outputs[index];
}

/**
 * Return the number of Gates in the given Circuit.
 */
int Circuit_numGates(Circuit this) {
    return this->numGates;
}

/**
 * Return the index'th Gate in the given Circuit.
 */
Gate Circuit_getGate(Circuit this, int index) {
    return this->gates[index];
}

/**
 * Print the given Circuit (probably only useful for debugging).
 */
void Circuit_dump(Circuit this) {
    printf("%s\n", this->title != NULL ? this->title : "(untitled)");
    printf("Inputs:\n");
    for (int i=0; i < this->numInputs; i++) {
	printf("  %d: ", i);
	Boolean_dump(this->inputs[i]);
	printf("\n");
    }
    printf("Outputs:\n");
    for (int i=0; i < this->numOutputs; i++) {
	printf("  %d: ", i);
	Boolean_dump(this->outputs[i]);
	printf("\n");
    }
    printf("Gates:\n");
    for (int i=0; i < this->numGates; i++) {
	printf("  %d: ", i);
	Gate_dump(this->gates[i]);
	printf("\n");
    }
    printf("Connections:\n");
    for (Connection c=this->connections->first; c != NULL; c=c->next) {
	printf("  %p -> %p\n", c->src, c->dst);
    }
}

/**
 * Connect the src Boolean to the dst Boolean in the given Circuit.
 */
void Circuit_connect(Circuit this, Boolean src, Boolean dst) {
    ConnectionList_addConnection(this->connections, src, dst);
}

/**
 * Gate g1 ``feeds into'' Gate g2 (directly) iff the output of g1 is
 * connected to one (or more) of the inputs of g2 in this Circuit.
 * This is not the fastest implementation. You could probably do better.
 */
static bool feedsInto(Circuit this, Gate g1, Gate g2) {
    Boolean out = Gate_getOutput(g1);
    for (int i=0; i < Gate_numInputs(g2); i++) {
	Boolean in = Gate_getInput(g2, i);
	if (ConnectionList_find(this->connections, out, in)) {
	    return true;
	}
    }
    return false;
}

/**
 * Topological sort using DFS per FOCS Ch. 9.
 */

// Doubly-linked list
typedef struct GateListNode *GateListNode;
struct GateListNode {
    Gate gate;
    GateListNode next;
    GateListNode prev;
};

static GateListNode new_GateListNode(Gate gate) {
    GateListNode this = (GateListNode)malloc(sizeof(struct GateListNode));
    this->gate = gate;
    this->next = NULL;
    this->prev = NULL;
    return this;
}

// Free the node but not its gate
static void free_GateListNode(GateListNode this) {
    free(this);
}

// Head and tail pointers
typedef struct GateList* GateList;
struct GateList {
    GateListNode first;
    GateListNode last;
};

static GateList new_GateList() {
    GateList this = (GateList)malloc(sizeof(struct GateList));
    this->first = NULL;
    this->last = NULL;
    return this;
}

// Free the nodes as well as the list (but not the gates)
static void free_GateList(GateList this) {
    GateListNode node = this->first;
    while (node != NULL) {
	GateListNode next = node->next;
	free_GateListNode(node);
	node = next;
    }
    free(this);
}

static bool GateList_isEmpty(GateList this) {
    return this->first == NULL;
}

static void GateList_push(GateList this, Gate gate) {
    GateListNode node = new_GateListNode(gate);
    node->next = this->first;
    if (this->first != NULL) {
	this->first->prev = node;
    }
    this->first = node;
}

static Gate GateList_pop(GateList this) {
    if (this->first == NULL) {
	return NULL;
    } else {
	GateListNode node = this->first;
	Gate gate = node->gate;
	this->first = node->next;
	if (this->first == NULL) {
	    this->last = NULL;
	} else {
	    this->first->prev = NULL;
	}
	free_GateListNode(node);
	return gate;
    }
}

// Add at end
static void GateList_add(GateList this, Gate gate) {
    GateListNode node = new_GateListNode(gate);
    if (this->first == NULL) {
	this->first = node;
	this->last = node;
    } else {
	this->last->next = node;
	node->prev = this->last;
	this->last = node;
    }
}

// Remove from end
static Gate GateList_remove(GateList this) {
    if (this->last == NULL) {
	return NULL;
    } else {
	GateListNode node = this->last;
	Gate gate = node->gate;
	this->last = this->last->prev;
	if (this->last == NULL) {
	    this->first = NULL;
	} else {
	    this->last->next = NULL;
	}
	free_GateListNode(node);
	return gate;
    }
}

static bool GateList_contains(GateList this, Gate gate) {
    for (GateListNode p=this->first; p != NULL; p=p->next) {
	if (p->gate == gate) {
	    return true;
	}
    }
    return false;
}

/**
 * FOCS Sec. 9.7 p. 497: ``As we are about to give a node its postorder number, that is, as we complete the call to dfs on that node, we push the node onto a stack. When we are done, the stack is a list in which the nodes appear in postorder, with the highest at the top (front).''
 */
static void dfs(Circuit circuit, GateList visited, GateList stack, Gate u) {
    GateList_add(visited, u);
    for (int i=0; i < Circuit_numGates(circuit); i++) {
	Gate v = Circuit_getGate(circuit, i);
	if (feedsInto(circuit, u, v) && !GateList_contains(visited, v)) {
	    dfs(circuit, visited, stack, v);
	}
    }
    GateList_push(stack, u);
}

/**
 * FOCS Sec. 9.6 p. 489: ``the general method of exploring a graph is to construct a sequence of trees. We start at some node u and call dfs(u). If there are nodes not yet visited, we pick one, say v, and call dfs(v). We repeat this process as long as there are nodes not yet assigned to any tree.''
 * We use a visited list rather than a ``mark'' in the graph nodes.
 */
static GateList topsortGates(Circuit circuit) {
    GateList visited = new_GateList();
    GateList stack = new_GateList();
    for (int i=0; i < Circuit_numGates(circuit); i++) {
	Gate gate = Circuit_getGate(circuit, i);
	if (!GateList_contains(visited, gate)) {
	    dfs(circuit, visited, stack, gate);
	}
    }
    free_GateList(visited);
    return stack;
}

/**
 * Propagate the given Boolean value through any connections in this Circuit.
 */
static void propagate(Circuit this, Boolean b) {
    for (Connection c=this->connections->first; c != NULL; c=c->next) {
	if (c->src == b) {
	    Boolean_setValue(c->dst, Boolean_getValue(b));
	}
    }
}

/**
 * Update all the values in this Circuit by first propagating its inputs
 * to any connected gates, then updating the Gates in topological
 * order and propagating their outputs to any connected Gates.
 */
void Circuit_update(Circuit this) {
    // Propagate circuit inputs initially
    for (int i=0; i < Circuit_numInputs(this); i++) {
	propagate(this, Circuit_getInput(this, i));
    }
    // Update gates in topological order
    GateList gatesInTopOrder = topsortGates(this);
    for (GateListNode node = gatesInTopOrder->first; node != NULL; node=node->next) {
	Gate gate = node->gate;
	Gate_update(gate);
	// Propagate output value to any connected inputs
	propagate(this, Gate_getOutput(gate));
    }
    free_GateList(gatesInTopOrder);
}
