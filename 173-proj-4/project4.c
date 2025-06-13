#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct TUPLE *TUPLELIST;
struct TUPLE {
    char *components[10];  
    TUPLELIST next;
};

typedef TUPLELIST HASHTABLE[1009]; 

// Database relations
typedef struct {
    HASHTABLE pIdIndex;
} PNCZ;

typedef struct {
    HASHTABLE raceIndex;
} RPT;

typedef struct {
    HASHTABLE raceIndex;
} RD;

typedef struct {
    HASHTABLE raceIndex;
} RS;

typedef struct {
    HASHTABLE raceIndex;
} RQ;

typedef struct {
    PNCZ pncz;
    RPT rpt;
    RD rd;
    RS rs;
    RQ rq;
} Database;

// Function prototypes
void init_database(Database* db);
void cleanup_database(Database* db);
void print_all_relations(Database* db);
void free_tuple_list(TUPLELIST list);

// Hash function
int hash(const char* str) {
    int sum = 0;
    while (*str) {
        sum = (sum * 127 + *str) % 1009;
        str++;
    }
    return sum;
}

static char* my_strdup(const char* s) {
    if (!s) return NULL;
    size_t size = strlen(s) + 1;    // +1 for the null terminator
    char* p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

// Tuple creation helper
TUPLELIST create_tuple(int num_components, ...) {
    va_list args;
    va_start(args, num_components);
    
    TUPLELIST new_tuple = malloc(sizeof(struct TUPLE));
    if (!new_tuple) {
        va_end(args);
        return NULL;
    }
    memset(new_tuple, 0, sizeof(struct TUPLE));
    
    for (int i = 0; i < num_components; i++) {
        const char* value = va_arg(args, const char*);
        new_tuple->components[i] = value ? my_strdup(value) : NULL;
    }
    
    new_tuple->next = NULL;
    va_end(args);
    return new_tuple;
}

bool tuples_equal(TUPLELIST t1, TUPLELIST t2, int num_components) {
    for (int i = 0; i < num_components; i++) {
        if (strcmp(t1->components[i], t2->components[i]) != 0) {
            return false;
        }
    }
    return true;
}

// PNCZ Operations
void insert_PNCZ(PNCZ* rel, const char* pId, const char* name, const char* city, const char* zip) {
    // Create temporary tuple for comparison
    TUPLELIST new_tuple = create_tuple(4, pId, name, city, zip);
    
    // Check if exists in appropriate bucket
    int bucket = hash(pId);
    TUPLELIST current = rel->pIdIndex[bucket];
    while (current) {
        if (tuples_equal(current, new_tuple, 4)) {
            // Duplicate found, free the new tuple and return
            free_tuple_list(new_tuple);
            return;
        }
        current = current->next;
    }
    
    // No duplicate found, insert the new tuple
    new_tuple->next = rel->pIdIndex[bucket];
    rel->pIdIndex[bucket] = new_tuple;
}

TUPLELIST lookup_PNCZ(PNCZ* rel, const char* pId, const char* name, const char* city, const char* zip) {
    TUPLELIST results = NULL;
    
    // If PId is specified (not *), we can use the hash index
    if (pId && strcmp(pId, "*") != 0) {
        int bucket = hash(pId);
        TUPLELIST current = rel->pIdIndex[bucket];
        while (current) {
            if ((!name || strcmp(name, "*") == 0 || strcmp(current->components[1], name) == 0) &&
                (!city || strcmp(city, "*") == 0 || strcmp(current->components[2], city) == 0) &&
                (!zip || strcmp(zip, "*") == 0 || strcmp(current->components[3], zip) == 0)) {
                TUPLELIST new_result = create_tuple(4,
                    current->components[0], current->components[1],
                    current->components[2], current->components[3]);
                new_result->next = results;
                results = new_result;
            }
            current = current->next;
        }
    } else {
        // Full table scan when PId is wildcard
        for (int i = 0; i < 1009; i++) {
            TUPLELIST current = rel->pIdIndex[i];
            while (current) {
                // Check if this tuple matches all specified conditions
                if ((!name || strcmp(name, "*") == 0 || strcmp(current->components[1], name) == 0) &&
                    (!city || strcmp(city, "*") == 0 || strcmp(current->components[2], city) == 0) &&
                    (!zip || strcmp(zip, "*") == 0 || strcmp(current->components[3], zip) == 0)) {
                    // Create new result tuple
                    TUPLELIST new_result = create_tuple(4,
                        current->components[0], current->components[1],
                        current->components[2], current->components[3]);
                    // Add to results list
                    new_result->next = results;
                    results = new_result;
                }
                current = current->next;
            }
        }
    }
    
    // If no results found, return NULL
    if (!results) {
        return NULL;
    }
    
    return results;
}

void delete_PNCZ(PNCZ* rel, const char* pId, const char* name, const char* city, const char* zip) {
    if (pId && strcmp(pId, "*") != 0) {
        int bucket = hash(pId);
        TUPLELIST* current = &(rel->pIdIndex[bucket]);
        while (*current) {
            if ((!name || strcmp(name, "*") == 0 || strcmp((*current)->components[1], name) == 0) &&
                (!city || strcmp(city, "*") == 0 || strcmp((*current)->components[2], city) == 0) &&
                (!zip || strcmp(zip, "*") == 0 || strcmp((*current)->components[3], zip) == 0)) {
                TUPLELIST to_delete = *current;
                *current = (*current)->next;
                free_tuple_list(to_delete);
            } else {
                current = &((*current)->next);
            }
        }
    } else {
        // Handle wildcard PId by checking all buckets
        for (int i = 0; i < 1009; i++) {
            TUPLELIST* current = &(rel->pIdIndex[i]);
            while (*current) {
                if ((!name || strcmp(name, "*") == 0 || strcmp((*current)->components[1], name) == 0) &&
                    (!city || strcmp(city, "*") == 0 || strcmp((*current)->components[2], city) == 0) &&
                    (!zip || strcmp(zip, "*") == 0 || strcmp((*current)->components[3], zip) == 0)) {
                    TUPLELIST to_delete = *current;
                    *current = (*current)->next;
                    free_tuple_list(to_delete);
                } else {
                    current = &((*current)->next);
                }
            }
        }
    }
}
// RPT Operations
void insert_RPT(RPT* rel, const char* race, const char* pId, const char* time) {
    // Create temporary tuple for comparison
    TUPLELIST new_tuple = create_tuple(3, race, pId, time);
    
    int bucket = hash(race);
    TUPLELIST current = rel->raceIndex[bucket];
    while (current) {
        if (tuples_equal(current, new_tuple, 3)) {
            // Duplicate found, free the new tuple and return
            free_tuple_list(new_tuple);
            return;
        }
        current = current->next;
    }
    
    // No duplicate found, insert the new tuple
    new_tuple->next = rel->raceIndex[bucket];
    rel->raceIndex[bucket] = new_tuple;
}

TUPLELIST lookup_RPT(RPT* rel, const char* race, const char* pId, const char* time) {
    TUPLELIST results = NULL;
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST current = rel->raceIndex[bucket];
        while (current) {
            if ((!pId || strcmp(pId, "*") == 0 || strcmp(current->components[1], pId) == 0) &&
                (!time || strcmp(time, "*") == 0 || strcmp(current->components[2], time) == 0)) {
                TUPLELIST new_result = create_tuple(3,
                    current->components[0], current->components[1],
                    current->components[2]);
                new_result->next = results;
                results = new_result;
            }
            current = current->next;
        }
    } else {
        // Add full table scan for wildcard race
        for (int i = 0; i < 1009; i++) {
            TUPLELIST current = rel->raceIndex[i];
            while (current) {
                if ((!pId || strcmp(pId, "*") == 0 || strcmp(current->components[1], pId) == 0) &&
                    (!time || strcmp(time, "*") == 0 || strcmp(current->components[2], time) == 0)) {
                    TUPLELIST new_result = create_tuple(3,
                        current->components[0], current->components[1],
                        current->components[2]);
                    new_result->next = results;
                    results = new_result;
                }
                current = current->next;
            }
        }
    }
    return results;
}

void delete_RPT(RPT* rel, const char* race, const char* pId, const char* time) {
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST* current = &(rel->raceIndex[bucket]);
        while (*current) {
            if ((!pId || strcmp(pId, "*") == 0 || strcmp((*current)->components[1], pId) == 0) &&
                (!time || strcmp(time, "*") == 0 || strcmp((*current)->components[2], time) == 0)) {
                TUPLELIST to_delete = *current;
                *current = (*current)->next;
                free_tuple_list(to_delete);
            } else {
                current = &((*current)->next);
            }
        }
    } else {
        // Handle wildcard race by checking all buckets
        for (int i = 0; i < 1009; i++) {
            TUPLELIST* current = &(rel->raceIndex[i]);
            while (*current) {
                if ((!pId || strcmp(pId, "*") == 0 || strcmp((*current)->components[1], pId) == 0) &&
                    (!time || strcmp(time, "*") == 0 || strcmp((*current)->components[2], time) == 0)) {
                    TUPLELIST to_delete = *current;
                    *current = (*current)->next;
                    free_tuple_list(to_delete);
                } else {
                    current = &((*current)->next);
                }
            }
        }
    }
}
// RD Operations
void insert_RD(RD* rel, const char* race, const char* date) {
    // Create temporary tuple for comparison
    TUPLELIST new_tuple = create_tuple(2, race, date);
    
    int bucket = hash(race);
    TUPLELIST current = rel->raceIndex[bucket];
    while (current) {
        if (tuples_equal(current, new_tuple, 2)) {
            // Duplicate found, free the new tuple and return
            free_tuple_list(new_tuple);
            return;
        }
        current = current->next;
    }
    
    // No duplicate found, insert the new tuple
    new_tuple->next = rel->raceIndex[bucket];
    rel->raceIndex[bucket] = new_tuple;
}

TUPLELIST lookup_RD(RD* rel, const char* race, const char* date) {
    TUPLELIST results = NULL;
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST current = rel->raceIndex[bucket];
        while (current) {
            if (!date || strcmp(date, "*") == 0 || strcmp(current->components[1], date) == 0) {
                TUPLELIST new_result = create_tuple(2,
                    current->components[0], current->components[1]);
                new_result->next = results;
                results = new_result;
            }
            current = current->next;
        }
    } else {
        // Full table scan for wildcard race
        for (int i = 0; i < 1009; i++) {
            TUPLELIST current = rel->raceIndex[i];
            while (current) {
                if (!date || strcmp(date, "*") == 0 || strcmp(current->components[1], date) == 0) {
                    TUPLELIST new_result = create_tuple(2,
                        current->components[0], current->components[1]);
                    new_result->next = results;
                    results = new_result;
                }
                current = current->next;
            }
        }
    }
    return results;
}

void delete_RD(RD* rel, const char* race, const char* date) {
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST* current = &(rel->raceIndex[bucket]);
        while (*current) {
            if (!date || strcmp(date, "*") == 0 || strcmp((*current)->components[1], date) == 0) {
                TUPLELIST to_delete = *current;
                *current = (*current)->next;
                free_tuple_list(to_delete);
            } else {
                current = &((*current)->next);
            }
        }
    } else {
        // Add handling for wildcard race
        for (int i = 0; i < 1009; i++) {
            TUPLELIST* current = &(rel->raceIndex[i]);
            while (*current) {
                if (!date || strcmp(date, "*") == 0 || strcmp((*current)->components[1], date) == 0) {
                    TUPLELIST to_delete = *current;
                    *current = (*current)->next;
                    free_tuple_list(to_delete);
                } else {
                    current = &((*current)->next);
                }
            }
        }
    }
}

// RS Operations
void insert_RS(RS* rel, const char* race, const char* sponsor) {
    // Create temporary tuple for comparison
    TUPLELIST new_tuple = create_tuple(2, race, sponsor);
    
    int bucket = hash(race);
    TUPLELIST current = rel->raceIndex[bucket];
    while (current) {
        if (tuples_equal(current, new_tuple, 2)) {
            // Duplicate found, free the new tuple and return
            free_tuple_list(new_tuple);
            return;
        }
        current = current->next;
    }
    
    // No duplicate found, insert the new tuple
    new_tuple->next = rel->raceIndex[bucket];
    rel->raceIndex[bucket] = new_tuple;
}


TUPLELIST lookup_RS(RS* rel, const char* race, const char* sponsor) {
    TUPLELIST results = NULL;
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST current = rel->raceIndex[bucket];
        while (current) {
            if (!sponsor || strcmp(sponsor, "*") == 0 || strcmp(current->components[1], sponsor) == 0) {
                TUPLELIST new_result = create_tuple(2,
                    current->components[0], current->components[1]);
                new_result->next = results;
                results = new_result;
            }
            current = current->next;
        }
    } else {
        // Full table scan for wildcard race
        for (int i = 0; i < 1009; i++) {
            TUPLELIST current = rel->raceIndex[i];
            while (current) {
                if (!sponsor || strcmp(sponsor, "*") == 0 || strcmp(current->components[1], sponsor) == 0) {
                    TUPLELIST new_result = create_tuple(2,
                        current->components[0], current->components[1]);
                    new_result->next = results;
                    results = new_result;
                }
                current = current->next;
            }
        }
    }
    return results;
}

void delete_RS(RS* rel, const char* race, const char* sponsor) {
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST* current = &(rel->raceIndex[bucket]);
        while (*current) {
            if (!sponsor || strcmp(sponsor, "*") == 0 || 
                 strcmp((*current)->components[1], sponsor) == 0) {
                TUPLELIST to_delete = *current;
                *current = (*current)->next;
                free_tuple_list(to_delete);
            } else {
                current = &((*current)->next);
            }
        }
    } else {
        for (int i = 0; i < 1009; i++) {
            TUPLELIST* current = &(rel->raceIndex[i]);
            while (*current) {
                if (!sponsor || strcmp(sponsor, "*") == 0 || 
                    strcmp((*current)->components[1], sponsor) == 0) {
                    TUPLELIST to_delete = *current;
                    *current = (*current)->next;
                    free_tuple_list(to_delete);
                } else {
                    current = &((*current)->next);
                }
            }
        }
    }
}

// RQ Operations
void insert_RQ(RQ* rel, const char* race, const char* qualifier) {
    // Create temporary tuple for comparison
    TUPLELIST new_tuple = create_tuple(2, race, qualifier);
    
    int bucket = hash(race);
    TUPLELIST current = rel->raceIndex[bucket];
    while (current) {
        if (tuples_equal(current, new_tuple, 2)) {
            // Duplicate found, free the new tuple and return
            free_tuple_list(new_tuple);
            return;
        }
        current = current->next;
    }
    
    // No duplicate found, insert the new tuple
    new_tuple->next = rel->raceIndex[bucket];
    rel->raceIndex[bucket] = new_tuple;
}

TUPLELIST lookup_RQ(RQ* rel, const char* race, const char* qualifier) {
    TUPLELIST results = NULL;
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST current = rel->raceIndex[bucket];
        while (current) {
            if (!qualifier || strcmp(qualifier, "*") == 0 || 
                strcmp(current->components[1], qualifier) == 0) {
                TUPLELIST new_result = create_tuple(2,
                    current->components[0], current->components[1]);
                new_result->next = results;
                results = new_result;
            }
            current = current->next;
        }
    } else {
        for (int i = 0; i < 1009; i++) {
            TUPLELIST current = rel->raceIndex[i];
            while (current) {
                if (!qualifier || strcmp(qualifier, "*") == 0 || 
                    strcmp(current->components[1], qualifier) == 0) {
                    TUPLELIST new_result = create_tuple(2,
                        current->components[0], current->components[1]);
                    new_result->next = results;
                    results = new_result;
                }
                current = current->next;
            }
        }
    }
    return results;
}

void delete_RQ(RQ* rel, const char* race, const char* qualifier) {
    if (race && strcmp(race, "*") != 0) {
        int bucket = hash(race);
        TUPLELIST* current = &(rel->raceIndex[bucket]);
        while (*current) {
            if (!qualifier || strcmp(qualifier, "*") == 0 || 
                strcmp((*current)->components[1], qualifier) == 0) {
                TUPLELIST to_delete = *current;
                *current = (*current)->next;
                free_tuple_list(to_delete);
            } else {
                current = &((*current)->next);
            }
        }
    } else {
        // Handle wildcard race by checking all buckets
        for (int i = 0; i < 1009; i++) {
            TUPLELIST* current = &(rel->raceIndex[i]);
            while (*current) {
                if (!qualifier || strcmp(qualifier, "*") == 0 || 
                    strcmp((*current)->components[1], qualifier) == 0) {
                    TUPLELIST to_delete = *current;
                    *current = (*current)->next;
                    free_tuple_list(to_delete);
                } else {
                    current = &((*current)->next);
                }
            }
        }
    }
}
// Print functions
void print_PNCZ(PNCZ* rel) {
    printf("\nPNCZ Table:\n");
    printf("%-8s %-15s %-15s %s\n", "PId", "Name", "City", "Zip");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 1009; i++) {
        TUPLELIST current = rel->pIdIndex[i];
        while (current) {
            printf("%-8s %-15s %-15s %s\n",
                   current->components[0], current->components[1],
                   current->components[2], current->components[3]);
            current = current->next;
        }
    }
}

void print_RPT(RPT* rel) {
    printf("\nRPT Table:\n");
    printf("%-20s %-8s %s\n", "Race", "PId", "Time");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 1009; i++) {
        TUPLELIST current = rel->raceIndex[i];
        while (current) {
            printf("%-20s %-8s %s\n",
                   current->components[0], current->components[1],
                   current->components[2]);
            current = current->next;
        }
    }
}

void print_RD(RD* rel) {
    printf("\nRD Table:\n");
    printf("%-20s %s\n", "Race", "Date");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 1009; i++) {
        TUPLELIST current = rel->raceIndex[i];
        while (current) {
            printf("%-20s %s\n",
                   current->components[0], current->components[1]);
            current = current->next;
        }
    }
}

void print_RS(RS* rel) {
    printf("\nRS Table:\n");
    printf("%-20s %s\n", "Race", "Sponsor");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 1009; i++) {
        TUPLELIST current = rel->raceIndex[i];
        while (current) {
            printf("%-20s %s\n",
                   current->components[0], current->components[1]);
            current = current->next;
        }
    }
}

void print_RQ(RQ* rel) {
    printf("\nRQ Table:\n");
    printf("%-20s %s\n", "Race", "Qualifier");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 1009; i++) {
        TUPLELIST current = rel->raceIndex[i];
        while (current) {
            printf("%-20s %s\n",
                   current->components[0], current->components[1]);
            current = current->next;
        }
    }
}

void print_all_relations(Database* db) {
    print_PNCZ(&db->pncz);
    print_RPT(&db->rpt);
    print_RD(&db->rd);
    print_RS(&db->rs);
    print_RQ(&db->rq);
}

// Memory management
void free_tuple_list(TUPLELIST list) {
    while (list) {
        TUPLELIST next = list->next;
        for (int i = 0; i < 10; i++) {
            if (list->components[i]) {
                free(list->components[i]);
            }
        }
        free(list);
        list = next;
    }
}

void cleanup_database(Database* db) {
    for (int i = 0; i < 1009; i++) {
        if (db->pncz.pIdIndex[i]) {
            free_tuple_list(db->pncz.pIdIndex[i]);
            db->pncz.pIdIndex[i] = NULL;
        }
        if (db->rpt.raceIndex[i]) {
            free_tuple_list(db->rpt.raceIndex[i]);
            db->rpt.raceIndex[i] = NULL;
        }
        if (db->rd.raceIndex[i]) {
            free_tuple_list(db->rd.raceIndex[i]);
            db->rd.raceIndex[i] = NULL;
        }
        if (db->rs.raceIndex[i]) {
            free_tuple_list(db->rs.raceIndex[i]);
            db->rs.raceIndex[i] = NULL;
        }
        if (db->rq.raceIndex[i]) {
            free_tuple_list(db->rq.raceIndex[i]);
            db->rq.raceIndex[i] = NULL;
        }
    }
}

void init_database(Database* db) {
    memset(db, 0, sizeof(Database));
    // Initialize PNCZ
    insert_PNCZ(&db->pncz, "67815", "E. Allen", "Clarkson", "14642");
    insert_PNCZ(&db->pncz, "37435", "J. Thomas", "Churchville", "14643");
    insert_PNCZ(&db->pncz, "41222", "H. Evans", "North Gates", "14621");
    insert_PNCZ(&db->pncz, "31444", "E. Allen", "Pittsford", "14559");
    insert_PNCZ(&db->pncz, "89602", "J. Thomas", "Hamlin", "14651");
    insert_PNCZ(&db->pncz, "69441", "O. Price", "Hamlin", "14506");

    // Initialize RPT
    insert_RPT(&db->rpt, "UTri", "69441", "01:45:12");
    insert_RPT(&db->rpt, "CSC Marathon", "31444", "01:22:22");
    insert_RPT(&db->rpt, "GVP Classic", "89602", "01:52:26");
    insert_RPT(&db->rpt, "CSC Marathon", "67815", "00:32:32");
    insert_RPT(&db->rpt, "Buffalo Stampede", "41222", "01:53:57");
    insert_RPT(&db->rpt, "GVP Classic", "37435", "01:20:23");

    // Initialize RD
    insert_RD(&db->rd, "RocRace", "2023-07-27");
    insert_RD(&db->rd, "Buffalo Stampede", "2023-05-16");
    insert_RD(&db->rd, "UTri", "2023-11-02");
    insert_RD(&db->rd, "GVP Classic", "2023-05-16");
    insert_RD(&db->rd, "CSC Marathon", "2023-11-11");

    // Initialize RS
    insert_RS(&db->rs, "Buffalo Stampede", "Sadida");
    insert_RS(&db->rs, "CSC Marathon", "Psyke");
    insert_RS(&db->rs, "CSC Marathon", "URMC Ortho");
    insert_RS(&db->rs, "GVP Classic", "LizardAde");
    insert_RS(&db->rs, "Buffalo Stampede", "Run Club");
    insert_RS(&db->rs, "RocRace", "Crooks");

    // Initialize RQ
    insert_RQ(&db->rq, "RocRace", "Buffalo Stampede");
    insert_RQ(&db->rq, "GVP Classic", "CSC Marathon");
    insert_RQ(&db->rq, "GVP Classic", "RocRace");
}

void reset_database(Database* db) {
    cleanup_database(db);
    memset(db, 0, sizeof(Database));

    // PNCZ initial data
    insert_PNCZ(&db->pncz, "67815", "E. Allen", "Clarkson", "14642");
    insert_PNCZ(&db->pncz, "37435", "J. Thomas", "Churchville", "14643");
    insert_PNCZ(&db->pncz, "41222", "H. Evans", "North Gates", "14621");
    insert_PNCZ(&db->pncz, "31444", "E. Allen", "Pittsford", "14559");
    insert_PNCZ(&db->pncz, "89602", "J. Thomas", "Hamlin", "14651");
    insert_PNCZ(&db->pncz, "69441", "O. Price", "Hamlin", "14506");

    // RPT initial data
    insert_RPT(&db->rpt, "UTri", "69441", "01:45:12");
    insert_RPT(&db->rpt, "CSC Marathon", "31444", "01:22:22");
    insert_RPT(&db->rpt, "GVP Classic", "89602", "01:52:26");
    insert_RPT(&db->rpt, "CSC Marathon", "67815", "00:32:32");
    insert_RPT(&db->rpt, "Buffalo Stampede", "41222", "01:53:57");
    insert_RPT(&db->rpt, "GVP Classic", "37435", "01:20:23");

    // RD initial data
    insert_RD(&db->rd, "RocRace", "2023-07-27");
    insert_RD(&db->rd, "Buffalo Stampede", "2023-05-16");
    insert_RD(&db->rd, "UTri", "2023-11-02");
    insert_RD(&db->rd, "GVP Classic", "2023-05-16");
    insert_RD(&db->rd, "CSC Marathon", "2023-11-11");

    // RS initial data
    insert_RS(&db->rs, "Buffalo Stampede", "Sadida");
    insert_RS(&db->rs, "CSC Marathon", "Psyke");
    insert_RS(&db->rs, "CSC Marathon", "URMC Ortho");
    insert_RS(&db->rs, "GVP Classic", "LizardAde");
    insert_RS(&db->rs, "Buffalo Stampede", "Run Club");
    insert_RS(&db->rs, "RocRace", "Crooks");

    // RQ initial data
    insert_RQ(&db->rq, "RocRace", "Buffalo Stampede");
    insert_RQ(&db->rq, "GVP Classic", "CSC Marathon");
    insert_RQ(&db->rq, "GVP Classic", "RocRace");
}

void print_lookup_results(TUPLELIST results, int num_components) {
    if (!results) {
        printf("No results found.\n");
        return;
    }

    TUPLELIST current = results;
    while (current) {
        printf("(");
        for (int i = 0; i < num_components; i++) {
            if (current->components[i]) {
                printf("%s", current->components[i]);
            } else {
                printf("null");
            }
            if (i < num_components - 1) {
                printf(", ");
            }
        }
        printf(")\n");
        current = current->next;
    }
}

// Function to get the time a person got in a race
void find_participant_time(Database* db, const char* name, const char* race) {
    TUPLELIST name_matches = lookup_PNCZ(&db->pncz, "*", name, "*", "*");
    
    if (!name_matches) {
        printf("No participant found with name: %s\n", name);
        return;
    }

    bool found = false;
    // (2-6) For each matching person, check if they ran the specified race
    TUPLELIST current_person = name_matches;
    while (current_person) {
        // Get ID
        const char* pid = current_person->components[0];
        
        // Look up this person's time in the specified race
        TUPLELIST race_result = lookup_RPT(&db->rpt, race, pid, "*");
        
        if (race_result) {
            printf("Time for %s (ID: %s) in %s: %s\n", 
                   name, pid, race, race_result->components[2]);
            found = true;
            free_tuple_list(race_result);
        }
        
        current_person = current_person->next;
    }
    
    if (!found) {
        printf("%s did not participate in %s\n", name, race);
    }

    free_tuple_list(name_matches);
}

// REPL implementation for Part 2.1
void time_query_repl(Database* db) {
    char name[100];
    char race[100];
    char input[10];
    
    printf("\nTime Query REPL (Enter 'quit' when done)\n");
    printf("This REPL finds the time a participant achieved in a specific race\n");
    
    while (1) {
        printf("\nEnter participant name (or 'quit'): ");
        if (fgets(name, sizeof(name), stdin) == NULL) break;
        name[strcspn(name, "\n")] = 0;  // Remove newline
        
        if (strcmp(name, "quit") == 0) break;
        
        printf("Enter race name: ");
        if (fgets(race, sizeof(race), stdin) == NULL) break;
        race[strcspn(race, "\n")] = 0;  // Remove newline
        
        printf("\nExecuting query: What time did %s get in %s?\n", name, race);
        printf("----------------------------------------\n");
        
        find_participant_time(db, name, race);
        
        printf("\nContinue? (yes/no): ");
        if (fgets(input, sizeof(input), stdin) == NULL || 
            input[0] == 'n' || input[0] == 'N') break;
    }
}

bool has_completed_qualifier(Database* db, const char* pid, const char* race, const char* race_date) {
    // Get qualifiers for this race
    TUPLELIST qualifiers = lookup_RQ(&db->rq, race, "*");
    if (!qualifiers) {
        return true;  // No qualifiers needed
    }

    // Check each qualifier
    TUPLELIST current_qualifier = qualifiers;
    while (current_qualifier) {
        const char* qualifier_race = current_qualifier->components[1];
        
        // Get participant's result in qualifier race
        TUPLELIST qualifier_result = lookup_RPT(&db->rpt, qualifier_race, pid, "*");
        if (!qualifier_result) {
            free_tuple_list(qualifiers);
            return false;  // Didn't complete this qualifier
        }

        // Check if qualifier was completed before this race
        TUPLELIST qualifier_date = lookup_RD(&db->rd, qualifier_race, "*");
        TUPLELIST race_date_info = lookup_RD(&db->rd, race, "*");
        
        if (qualifier_date && race_date_info) {
            // Compare dates (assuming YYYY-MM-DD format)
            if (strcmp(qualifier_date->components[1], race_date_info->components[1]) >= 0) {
                // Qualifier wasn't completed before the race
                free_tuple_list(qualifier_date);
                free_tuple_list(race_date_info);
                free_tuple_list(qualifier_result);
                free_tuple_list(qualifiers);
                return false;
            }
            free_tuple_list(qualifier_date);
            free_tuple_list(race_date_info);
        }
        
        free_tuple_list(qualifier_result);
        current_qualifier = current_qualifier->next;
    }
    
    free_tuple_list(qualifiers);
    return true;
}

void find_sponsored_race_dates(Database* db, const char* name, const char* sponsor) {
    TUPLELIST name_matches = lookup_PNCZ(&db->pncz, "*", name, "*", "*");
    if (!name_matches) {
        printf("No participant found with name: %s\n", name);
        return;
    }

    bool found = false;
    TUPLELIST current_person = name_matches;
    while (current_person) {
        const char* pid = current_person->components[0];
        
        // 1) Find all races sponsored by the target sponsor
        TUPLELIST sponsor_races = lookup_RS(&db->rs, "*", sponsor);
        TUPLELIST current_sponsored_race = sponsor_races;
        
        while (current_sponsored_race) {
            const char* sponsored_race = current_sponsored_race->components[0];
            
            // Check if participant directly ran this sponsored race
            TUPLELIST ran_race = lookup_RPT(&db->rpt, sponsored_race, pid, "*");
            if (ran_race) {
                TUPLELIST date_info = lookup_RD(&db->rd, sponsored_race, "*");
                if (date_info) {
                    printf("%s (ID: %s) ran %s on %s, sponsored by %s\n",
                           name, pid, sponsored_race, date_info->components[1], sponsor);
                    found = true;
                    free_tuple_list(date_info);
                }
                free_tuple_list(ran_race);
            }
            
            // 2) Check if this sponsored race has qualifiers or is a qualifier
            // First: Is it a race that has qualifiers?
            TUPLELIST has_qualifiers = lookup_RQ(&db->rq, sponsored_race, "*");
            if (has_qualifiers) {
                TUPLELIST current_q = has_qualifiers;
                while (current_q) {
                    const char* qualifier = current_q->components[1];
                    // 3) Check if the qualifier is sponsored by our target sponsor
                    TUPLELIST qualifier_sponsor = lookup_RS(&db->rs, qualifier, sponsor);
                    if (qualifier_sponsor) {
                        // Check if participant ran this qualifier
                        TUPLELIST ran_qualifier = lookup_RPT(&db->rpt, qualifier, pid, "*");
                        if (ran_qualifier) {
                            TUPLELIST date_info = lookup_RD(&db->rd, qualifier, "*");
                            if (date_info) {
                                printf("%s (ID: %s) ran %s on %s, sponsored by %s (a qualifier for %s)\n",
                                       name, pid, qualifier, date_info->components[1], sponsor, sponsored_race);
                                found = true;
                                free_tuple_list(date_info);
                            }
                            free_tuple_list(ran_qualifier);
                        }
                        free_tuple_list(qualifier_sponsor);
                    }
                    current_q = current_q->next;
                }
                free_tuple_list(has_qualifiers);
            }
            
            // Second: Is it a qualifier for other races?
            TUPLELIST qualifies_for = lookup_RQ(&db->rq, "*", sponsored_race);
            if (qualifies_for) {
                TUPLELIST current_main = qualifies_for;
                while (current_main) {
                    const char* main_race = current_main->components[0];
                    // 4) Check if participant ran the main race that required this qualifier
                    TUPLELIST ran_main = lookup_RPT(&db->rpt, main_race, pid, "*");
                    if (ran_main) {
                        TUPLELIST date_info = lookup_RD(&db->rd, sponsored_race, "*");
                        if (date_info) {
                            printf("%s (ID: %s) ran %s on %s, sponsored by %s (required to run %s)\n",
                                   name, pid, sponsored_race, date_info->components[1], sponsor, main_race);
                            found = true;
                            free_tuple_list(date_info);
                        }
                        free_tuple_list(ran_main);
                    }
                    current_main = current_main->next;
                }
                free_tuple_list(qualifies_for);
            }
            
            current_sponsored_race = current_sponsored_race->next;
        }
        
        if (sponsor_races) {
            free_tuple_list(sponsor_races);
        }
        
        current_person = current_person->next;
    }
    
    if (!found) {
        printf("%s did not participate in any races sponsored by %s or related qualifying races\n", 
               name, sponsor);
    }

    free_tuple_list(name_matches);
}
// REPL for this query
void sponsored_races_repl(Database* db) {
    char name[100];
    char sponsor[100];
    char input[10];
    
    printf("\nSponsored Races Query REPL (Enter 'quit' when done)\n");
    printf("This REPL finds dates when a participant ran races sponsored by a specific sponsor\n");
    
    while (1) {
        printf("\nEnter participant name (or 'quit'): ");
        if (fgets(name, sizeof(name), stdin) == NULL) break;
        name[strcspn(name, "\n")] = 0;  // Remove newline
        
        if (strcmp(name, "quit") == 0) break;
        
        printf("Enter sponsor name: ");
        if (fgets(sponsor, sizeof(sponsor), stdin) == NULL) break;
        sponsor[strcspn(sponsor, "\n")] = 0;  // Remove newline
        
        printf("\nExecuting query: On what date(s) did %s run a race sponsored by %s?\n", 
               name, sponsor);
        printf("----------------------------------------\n");
        
        find_sponsored_race_dates(db, name, sponsor);
        
        printf("\nContinue? (yes/no): ");
        if (fgets(input, sizeof(input), stdin) == NULL || 
            input[0] == 'n' || input[0] == 'N') break;
    }
}

// Selection operation for RPT relation by Race
TUPLELIST select_RPT_Race(RPT* rel, const char* race_value) {
    TUPLELIST result = NULL;
    
    // We'll reuse our lookup function since it already implements selection
    result = lookup_RPT(rel, race_value, "*", "*");
    
    return result;
}

// Projection operation to get just PId from RPT relation
TUPLELIST project_RPT_PId(TUPLELIST relation) {
    TUPLELIST result = NULL;
    TUPLELIST current = relation;
    
    while (current) {
        // Create new tuple with just the PId component
        TUPLELIST new_tuple = create_tuple(1, current->components[1]);
        new_tuple->next = result;
        result = new_tuple;
        current = current->next;
    }
    
    return result;
}

// Join operation for RD and RS relations
TUPLELIST join_RD_RS(RD* rd, RS* rs) {
    TUPLELIST result = NULL;
    
    // For each bucket in RD
    for (int i = 0; i < 1009; i++) {
        TUPLELIST rd_current = rd->raceIndex[i];
        
        while (rd_current) {
            const char* race = rd_current->components[0];
            const char* date = rd_current->components[1];
            
            // Find matching tuples in RS
            TUPLELIST rs_matches = lookup_RS(rs, race, "*");
            TUPLELIST rs_current = rs_matches;
            
            while (rs_current) {
                // Create joined tuple with all components
                TUPLELIST new_tuple = create_tuple(3, 
                    race,
                    date,
                    rs_current->components[1]  // sponsor
                );
                new_tuple->next = result;
                result = new_tuple;
                rs_current = rs_current->next;
            }
            
            if (rs_matches) free_tuple_list(rs_matches);
            rd_current = rd_current->next;
        }
    }
    
    return result;
}

// Project just Sponsor from a relation
TUPLELIST project_Sponsor(TUPLELIST relation) {
    TUPLELIST result = NULL;
    TUPLELIST current = relation;
    
    while (current) {
        // Assuming sponsor is the last component
        TUPLELIST new_tuple = create_tuple(1, current->components[2]);
        new_tuple->next = result;
        result = new_tuple;
        current = current->next;
    }
    
    return result;
}

// Helper function to print relation contents
void print_relation(TUPLELIST relation, int num_components) {
    if (!relation) {
        printf("Empty relation\n");
        return;
    }
    
    TUPLELIST current = relation;
    while (current) {
        printf("(");
        for (int i = 0; i < num_components; i++) {
            printf("%s%s", current->components[i], 
                   (i < num_components - 1) ? ", " : "");
        }
        printf(")\n");
        current = current->next;
    }
}

// relational algrebra part 3
void demonstrate_relational_algebra(Database* db) {
    printf("\n1. Selection: σ_Race=\"GVP Classic\"(RPT)\n");
    printf("Finding all RPT tuples where Race is \"GVP Classic\"\n");
    TUPLELIST result1 = select_RPT_Race(&db->rpt, "GVP Classic");
    print_relation(result1, 3);

    printf("\n2. Projection: π_PId(σ_Race=\"GVP Classic\"(RPT))\n");
    printf("Getting just the PIds from the previous result\n");
    TUPLELIST result2 = project_RPT_PId(result1);
    print_relation(result2, 1);

    free_tuple_list(result1);
    free_tuple_list(result2);

    printf("\n3. Join: RD ▷◁ RS\n");
    printf("Joining Race-Date with Race-Sponsor on Race\n");
    TUPLELIST result3 = join_RD_RS(&db->rd, &db->rs);
    print_relation(result3, 3);

    printf("\n4. Composite: π_Sponsor(σ_Date=\"2023-10-02\"(RD ▷◁ RS))\n");
    printf("Finding sponsors of races on 2023-10-02\n");
    TUPLELIST joined = join_RD_RS(&db->rd, &db->rs);
    // Filter for date (we could make this more efficient but following the expression order)
    TUPLELIST filtered = NULL;
    TUPLELIST current = joined;
    while (current) {
        if (strcmp(current->components[1], "2023-10-02") == 0) {
            TUPLELIST new_tuple = create_tuple(3, 
                current->components[0],
                current->components[1],
                current->components[2]);
            new_tuple->next = filtered;
            filtered = new_tuple;
        }
        current = current->next;
    }
    TUPLELIST result4 = project_Sponsor(filtered);
    print_relation(result4, 1);
    
    free_tuple_list(joined);
    free_tuple_list(filtered);
    free_tuple_list(result4);
    free_tuple_list(result3);
}

int main() {
    Database db;
    init_database(&db);

    printf("Initial database state:\n");
    print_all_relations(&db);

    // Part 1.4 demonstrations
    printf("\n=== Demonstrating Required Operations ===\n");

    // (a) lookup(("CSC Marathon", 67815, *), Race-PId-Time)
    printf("\n(a) lookup((\"CSC Marathon\", 67815, *), Race-PId-Time)\n");
    TUPLELIST result = lookup_RPT(&db.rpt, "CSC Marathon", "67815", "*");
    print_lookup_results(result, 3);
    free_tuple_list(result);

    // (b) lookup((*, "J. Thomas", *, *), PId-Name-City-Zip)
    printf("\n(b) lookup((*, \"J. Thomas\", *, *), PId-Name-City-Zip)\n");
    result = lookup_PNCZ(&db.pncz, "*", "J. Thomas", "*", "*");
    print_lookup_results(result, 4);
    free_tuple_list(result);


    // (c) lookup(("CSC Marathon", "GVP Classic"), Race-Qualifier) Professor changed query
    printf("\n(c) lookup((\"CSC Marathon\", \"GVP Classic\"), Race-Qualifier)\n");
    result = lookup_RQ(&db.rq, "CSC Marathon", "GVP Classic");
    print_lookup_results(result, 2);
    free_tuple_list(result);

    // (d) delete(("UTri", "2023-11-02"), Race-Date)
    printf("\n(d) delete((\"UTri\", \"2023-11-02\"), Race-Date)\n");
    printf("Before deletion:\n");
    print_RD(&db.rd);
    delete_RD(&db.rd, "UTri", "2023-11-02");
    printf("\nAfter deletion:\n");
    print_RD(&db.rd);

    // (e) delete(("GVP Classic", "McDavid's"), Race-Sponsor)
    printf("\n(e) delete((\"GVP Classic\", \"McDavid's\"), Race-Sponsor)\n");
    printf("Before deletion:\n");
    print_RS(&db.rs);
    delete_RS(&db.rs, "GVP Classic", "McDavid's");
    printf("\nAfter deletion:\n");
    print_RS(&db.rs);

    // (f) delete((*, "LizardAde"), Race-Sponsor)
    printf("\n(f) delete((*, \"LizardAde\"), Race-Sponsor)\n");
    printf("Before deletion:\n");
    print_RS(&db.rs);
    delete_RS(&db.rs, "*", "LizardAde");
    printf("\nAfter deletion:\n");
    print_RS(&db.rs);

    // (g) insert(("GVP Classic", "Newbie Classic"), Race-Qualifier)
    printf("\n(g) insert((\"GVP Classic\", \"Newbie Classic\"), Race-Qualifier)\n");
    printf("Before insertion:\n");
    print_RQ(&db.rq);
    insert_RQ(&db.rq, "GVP Classic", "Newbie Classic");
    printf("\nAfter insertion:\n");
    print_RQ(&db.rq);

    // (h) insert(("GVP Classic", "CSC Marathon"), Race-Qualifier)
    printf("\n(h) insert((\"GVP Classic\", \"CSC Marathon\"), Race-Qualifier)\n");
    printf("Before insertion:\n");
    print_RQ(&db.rq);
    insert_RQ(&db.rq, "GVP Classic", "CSC Marathon");
    printf("\nAfter insertion:\n");
    print_RQ(&db.rq);

    printf("\n===== RESETTING DATABASE: =====\n");
    reset_database(&db);
    print_all_relations(&db);

    time_query_repl(&db);
    sponsored_races_repl(&db);

    printf("\n===== RELATIONAL ALGREBRA: =====\n");
    cleanup_database(&db);
    demonstrate_relational_algebra(&db);
    cleanup_database(&db);

    return 0;
}