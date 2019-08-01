#ifndef _HASHTEST_H_
#define _HASHTEST_H_

#include <sys/types.h>

typedef size_t (*ht_func)(char *, unsigned int);

typedef struct {
    const char *testfile;       /* path to file with strings to hash */
    const char *outfile;        /* if NULL: out.ht */
    unsigned int total_runs;    /* number of runs through `testfile' */
    unsigned int max_per_run;   /* max number of strings hashed every run */
    unsigned int table_len;     /* length of hash table */
    int elapsed;                /* (elapsed/total_runs) = average per run */
    /* TODO: use clock_gettime, annotate the resolution here (ms/ns?) */
} ht_config;

typedef struct {
    /*
     * Metadata constituting the header of the output file.
     */
    unsigned int size;          /* `size'=`ht_config.table_len' */
    unsigned int zero_lists;    /* number of lists that have zero nodes */
    unsigned int max_list_len;  /* highest number of nodes in a single list */
    unsigned int min_list_len;  /* smallest number of nodes in a single list */

    /*
     * `outtab' is a table of `size' integers. Every  element is the number of
     * list nodes associated with that particular hash, e.g. outtab[231] is the
     * number of input strings that produced the hash 231.
     */
    unsigned int *outtab;
} ht_result;

ht_result ht_run(ht_config *, ht_func);
ht_result create_empty_result(void);

#endif /* _HASHTEST_H_ */
