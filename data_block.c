//
// Created by Jonathon Racz on 5/3/18.
//

#include "data_block.h"
#include "utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct data_block* data_block_alloc(int size)
{
    struct data_block* new_block = malloc(sizeof(struct data_block));
    new_block->size = size;
    new_block->data = malloc((size_t)size);
    return new_block;
}

void data_block_realloc(struct data_block* data_block, int new_size)
{
    data_block->size = new_size;
    data_block->data = realloc(data_block->data, (size_t)new_size);
}

void data_block_free(struct data_block** data_block)
{
    free((*data_block)->data);
    free(*data_block);
}

long data_block_dump_to_file(struct data_block* data_block, const char* filename)
{
    assert(data_block != NULL);
    FILE *output = fopen(filename, "wb");
    if (output == NULL)
        return 0;

    long bytes_written = write_bytes(output, data_block->data, data_block->size);
    fclose(output);
    return bytes_written;
}
