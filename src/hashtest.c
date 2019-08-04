#define _POSIX_C_SOURCE 200809L /* getline() */
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hashtest.h"

extern char *progname;

static inline void fail(const char *fmt, ...)
{
    va_list ap;
    char buf[BUFSIZ];

    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ-1, fmt, ap);
    va_end(ap);

    fprintf(stderr, "%s: error: %s.\n", progname, buf);
    exit(EXIT_FAILURE);
}

static inline void debug_log(const char *fmt, ...)
{
#ifdef DEBUG
    va_list ap;
    char buf[BUFSIZ];

    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ-1, fmt, ap);
    va_end(ap);

    fprintf(stderr, "%s: log: %s.\n", progname, buf);
#endif
}

static inline ht_result create_empty_result(void)
{
    ht_result result;

    result.config = NULL;
    result.outtab = NULL;
    result.max_list_len = 0;
    result.min_list_len = 0;
    result.zero_lists = 0;

    return result;
}

static inline void calc_results(ht_result *result, node_t **hashtab)
{
    unsigned int i, zeros, max, min;
    zeros = max = 0;
    min = INT_MAX;

    for (i = 0; i < result->config->table_size; i++) {
        unsigned int length = 0;
        node_t *nptr;

        for (nptr = hashtab[i]; nptr != NULL; nptr = nptr->next)
            length++;
        if (length == 0) zeros++;
        if (length > max) max = length;
        if (length < min) min = length;
    }

    result->zero_lists = zeros;
    result->max_list_len = max;
    result->min_list_len = min;

    /* TODO: implement */
    result->outtab = NULL;
}

ht_result ht_run(ht_config *config, ht_func hash_func)
{
    int i, err, hashes;
    FILE *file;
    struct timespec ts_pre, ts_post;
    ht_result result;
    node_t **hashtab = NULL;

    file = fopen(config->testfile, "r");
    if (!file)
        fail("failed to open file %s: %s", config->testfile, strerror(errno));

    /* timing starts here */
    err = clock_gettime(CLOCK_REALTIME, &ts_pre);
    if (err) fail("failed to get CPU time: %s", strerror(errno));

    for (i = 0; i < config->total_runs; i++) {
        int j;
        ssize_t nread = 1;

        /*
         * To remember only the last iteration, every existing hash table is
         * free'd and freshly allocated.
         */
        debug_log("iter=%d/%d", i+1, config->total_runs);
        hashes = 0; /* reset number of hashes in this particular run */
        if (hashtab) ht_free(hashtab, config->table_size);
        hashtab = (node_t **)malloc(sizeof(node_t *)*config->table_size);

        if (!hashtab)
            fail("failed to allocate hash table: %s", strerror(errno));
        else
            for (j = 0; j < config->table_size; j++)
                hashtab[j] = NULL;

        while (hashes < config->max_per_run) {
            unsigned int h;
            size_t len = 0;
            char *hash_val = NULL;
            node_t *node;

            /* hash another value from the input file */
            nread = getline(&hash_val, &len, file);
            if (nread < 1) break;
            hash_val[nread-1] = '\0'; /* remove '\n' from buffer */
            h = hash_func(hash_val, config->table_size);
            debug_log("read %s (%d bytes, hash %d)", hash_val, nread, h);

            /* add the hashed value to the hash table */
            node = (node_t *)malloc(sizeof(node_t));
            if (!node)
                fail("failed to allocate new node: %s", strerror(errno));
            node->data = (char *)malloc(sizeof(char)*BUFSIZ);
            if (!node->data)
                fail("failed to allocate data: %s", strerror(errno));
            strncpy(node->data, hash_val, BUFSIZ);
            node->data[BUFSIZ-1] = '\0';
            node->next = NULL;
            hashtab[h] = add_node(hashtab[h], node);

            free(hash_val);
            hashes++;
        }
        rewind(file);
    }

    /* stop timing after all hashes are calculated */
    err = clock_gettime(CLOCK_REALTIME, &ts_post);
    if (err) fail("failed to get CPU time: %s", strerror(errno));

    err = fclose(file); /* this is not strictly necessary, though */
    if (err)
        fail("failed to close file %s: %s", config->testfile, strerror(errno));

    /* populate a result struct */
    result = create_empty_result();
    result.elapsed = (ts_post.tv_nsec - ts_pre.tv_nsec)/1000000; /* nanosecs */
    result.elapsed += (ts_post.tv_sec - ts_pre.tv_sec)*1000;     /* secs */
    result.config = config;
    result.hashes_done = hashes;
    calc_results(&result, hashtab);

    return result;
}

void ht_dump_result(ht_result *result)
{
    fprintf(stderr, "time elapsed:\t\t\t%ld millisecs\n", result->elapsed);
    fprintf(stderr, "table size:\t\t\t%d\n", result->config->table_size);
    fprintf(stderr, "total runs:\t\t\t%d\n", result->config->total_runs);
    fprintf(stderr, "max hashes per run:\t\t%d\n", result->config->max_per_run);
    fprintf(stderr, "actual hashes per run:\t\t%d\n", result->hashes_done);
    fprintf(stderr, "longest list:\t\t\t%d\n", result->max_list_len);
    fprintf(stderr, "shortest list:\t\t\t%d\n", result->min_list_len);
    fprintf(stderr, "number of zero-length lists:\t%d (%.2f%%)\n",
            result->zero_lists,
            (float)result->zero_lists/(float)result->config->table_size*100);
}

void ht_save_result(ht_result *result)
{
    /* TODO: implement */
}

node_t *add_node(node_t *head, node_t *new)
{
    node_t *nptr;

    if (!head)
        return new;

    for (nptr = head; nptr->next != NULL; nptr = nptr->next)
        ;
    nptr->next = new;

    return head;
}

void ht_free(node_t **tbl, unsigned int size)
{
    int i;

    for (i = 0; i < size; i++) {
        node_t *nptr, *tmp;

        for (nptr = tbl[i]; nptr != NULL;) {
            tmp = nptr->next;
            free(nptr->data);
            free(nptr);
            nptr = tmp;
        }
    }
    free(tbl);
}
