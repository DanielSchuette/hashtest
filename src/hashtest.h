#ifndef _HASHTEST_H_
#define _HASHTEST_H_

#include <sys/types.h>

/* provide a string to hash and the table size to a hash function */
typedef size_t (*ht_func)(char *, unsigned int);

typedef struct {
    const char *testfile;       /* path to file with strings to hash */
    const char *outfile;        /* if NULL: out.ht */
    unsigned int total_runs;    /* number of runs through `testfile' */
    unsigned int max_per_run;   /* max number of strings hashed every run */
    unsigned int table_size;    /* size of hash table */
} ht_config;

typedef struct {
    /*
     * Metadata constituting the header of the output file.
     */
    ht_config *config;          /* configuration of this test */
    unsigned int zero_lists;    /* number of lists that have zero nodes */
    unsigned int max_list_len;  /* highest number of nodes in a single list */
    unsigned int min_list_len;  /* smallest number of nodes in a single list */
    unsigned long elapsed;      /* total microsecs elapsed (CPU time) */

    /*
     * `outtab' is a table of `size' integers. Every  element is the number of
     * list nodes associated with that particular hash, e.g. outtab[231] is the
     * number of input strings that produced the hash 231.
     */
    unsigned int *outtab;
} ht_result;

ht_result ht_run(ht_config *, ht_func);
void ht_dump_result(ht_result *);
void ht_save_result(ht_result *);

/*
 * The hash table used is a table of `node_t *' which build up singly-linked
 * lists as soon as >= 2 items produce the same hash. Resources are *not* freed
 * because the program terminates anyways as soon as all tests are run. The
 * kernel will clean up everything.
 */
typedef struct NODE {
    char *data;
    struct NODE *next;
} node_t;

node_t *add_node(node_t *, node_t *);
void ht_free(node_t **, unsigned int);

#endif /* _HASHTEST_H_ */
