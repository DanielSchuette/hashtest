#ifndef _CUSTOM_H_
#define _CUSTOM_H_

#include "hashtest.h"

size_t hash(char *str, unsigned int tbl_size)
{
    size_t h;
    char *ptr;

    h = 0;
    for (ptr = str; *ptr != '\0'; ptr++)
        h += *ptr;

    return h % tbl_size;
}

#endif /* _CUSTOM_H_ */
