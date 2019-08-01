#include <stdio.h>
#include "custom.h"
#include "hashtest.h"

#define TABLE_SIZE 4096

int main(void)
{
    size_t h;

    h = hash("hash me!", TABLE_SIZE);
    printf("hashed: %lu\n", h);

    return 0;
}
