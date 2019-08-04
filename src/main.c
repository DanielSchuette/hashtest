#include <stdio.h>
#include "hashtest.h"
#include "custom.h"

#define TABLE_SIZE 4096

char *progname;

int main(int argc, char **argv)
{
    ht_config config;
    ht_result result;

    progname = *argv;

    /* create test configuration */
    config.max_per_run = 200000;
    config.outfile = NULL;
    config.testfile = "data/emails.ht"; /* "data/dict.ht" */
    config.table_size = TABLE_SIZE;
    config.total_runs = 3;

    /* run tests and save the results */
    result = ht_run(&config, hash);
    ht_dump_result(&result);
    ht_save_result(&result);

    return 0;
}
