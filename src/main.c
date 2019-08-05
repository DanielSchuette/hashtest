/* main.c: the program entry point. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtest.h"
#include "config.h"

#define TABLE_SIZE 4096

char *progname;

const char *help_msg =
"Test a hashing function that was compiled into the source via config.h.\n"
"Options for running test are listed below, a user interface can be opened\n"
"via ui/index.html.\n"
"\n"
"Author: Daniel Schuette <d.schuette@online.de>\n"
"Date: 2019/08/05\n"
"Sources: https://github.com/DanielSchuette/hashtest\n"
"License: GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)\n"
"\n"
"USAGE:\n"
"\thashtest [FLAGS] [OPTIONS]\n"
"\n"
"FLAGS:\n"
"\t-h, --help\t\tDisplay this help message\n"
"\n"
"OPTIONS:\n"
"\t-m, --max-per-run <INT>\tThe number of total test iterations.\n"
"\t-r, --total-runs <INT>\tThe number of hashes calculated per test iteration\n"
"\t-f, --outfile <FILE>\tJSON file where output is saved (default: out.json).\n"
"\t-t, --testfile <FILE>\tStrings for hashing (default: data/dict.ht).\n";

void usage(void)
{
    fprintf(stderr, "%s", help_msg);
}

int main(int argc, char **argv)
{
    ht_config config;
    ht_result result;

    progname = *argv;

    /* Parse cli args if any were provided.
     * FIXME: invalid input likely leads to a segfault.
     */
    argc--;
    while (argc > 0) {
        argv++;    /* advance to next option */
        argc -= 2; /* it's assumed that there are only options, no flags */
        if (!strcmp(*argv, "--max-per-run") ||
            !strcmp(*argv, "-m")) {
            max_per_run = atoi(*(++argv));
            continue;
        }
        if (!strcmp(*argv, "--outfile") ||
            !strcmp(*argv, "-f")) {
            outfile = *(++argv);
            continue;
        }
        if (!strcmp(*argv, "--testfile") ||
            !strcmp(*argv, "-t")) {
            testfile = *(++argv);
            continue;
        }
        if (!strcmp(*argv, "--total-runs") ||
            !strcmp(*argv, "-r")) {
            total_runs = atoi(*(++argv));
            continue;
        }
        if (!strcmp(*argv, "--help") ||
            !strcmp(*argv, "-h")) {
            usage();
            exit(0);
        }
    }

    /* create test configuration */
    config.max_per_run = max_per_run;
    config.outfile = outfile;
    config.testfile = testfile;
    config.total_runs = total_runs;
    config.table_size = TABLE_SIZE;

    /* run tests and save the results */
    result = ht_run(&config, hash);
    ht_dump_result(&result);
    ht_save_result(&result);

    free(result.outtab);
    fclose(result.outfile); /* ignore errors, the kernel will clean up */

    return 0;
}
