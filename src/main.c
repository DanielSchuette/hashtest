/* main.c: the program entry point. */
#include <stdio.h>
#include <stdlib.h>
#include "hashtest.h"
#include "config.h"

#define TABLE_SIZE 4096

char *progname;

int main(int argc, char **argv)
{
    ht_config config;
    ht_result result;

    progname = *argv;

    /* create test configuration */
    config.max_per_run = 20000;
    config.outfile = NULL; /* out.json by default */
    config.testfile = "data/dict.ht";
    config.table_size = TABLE_SIZE;
    config.total_runs = 3;

    /* run tests and save the results */
    result = ht_run(&config, hash);
    ht_dump_result(&result);
    ht_save_result(&result);

    free(result.outtab);
    fclose(result.outfile); /* ignore errors, the kernel will clean up */

    return 0;
}
