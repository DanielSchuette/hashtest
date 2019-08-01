# hashtest
Test a hashing function.

# To-Do
- Run a hashing function pointer multiple times -> test runner api
    - mock data must be saved into a mock hash tables to ensure
      that the timing results are realistic
- Save output of every iteration (i.e. the resulting hash):
    - hash table with a single node (i.e. only NULL tests)
        ```c
        #define MAX_HASH_NUM 4096 /* might want to set this dynamicall*/
        hash_info_tab[MAX_HASH_NUM];
        struct hash_info {
            count *
            /* is more meta required? */
        }
        /*
         * Enforce that every hash is (hash % MAX_HASH_NUM)
         * in case the user did not comply with provided
         * params.
         */
        ```
    - calculate statistics (see below)
    - save this table out to a temporary file:
        ```
        HEADER_START            /* write header func */
        table_len: 4096
        total_runs: 10000
        time_elapsed: 3912      /* in ms */
        HEADER_END
        BODY_START              /* write body func */
        1
        9
        3
        12
        0
        0
        7
        ...
        BODY_END
        ```
    - are there actual statistics to evaluate hash functions?
