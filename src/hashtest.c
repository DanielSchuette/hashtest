#include <stdlib.h>
#include "hashtest.h"

ht_result ht_run(ht_config *config, ht_func hash_func)
{
    ht_result result;

    result = create_empty_result();

    return result;
}

ht_result create_empty_result(void)
{
    ht_result result;

    result.max_list_len = 0;
    result.min_list_len = 0;
    result.outtab = NULL;
    result.size = 0;
    result.zero_lists = 0;

    return result;
}
