#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "hashtest.h"

/* default configuration */
int max_per_run = 20000;
char *outfile = NULL;
char *testfile = "data/dict.ht";
int total_runs = 3;

/*
 * A function `hash' with the signature below is expected by the test runner.
 * Usually, this is the only place users need to modify. A pointer to `hash' is
 * passed to `ht_run' in `main.c'.
 */
size_t hash(char *str, unsigned int tbl_size)
{
    size_t h;
    char *ptr;

    h = 0;
    for (ptr = str; *ptr != '\0'; ptr++)
        h += *ptr; /* better: h = h*31 + *ptr; */

    return h % tbl_size;
}

#endif /* _CONFIG_H_ */
