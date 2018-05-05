//
// Created by Jonathon Racz on 5/3/18.
//

#ifndef COS_DATA_BLOCK_H
#define COS_DATA_BLOCK_H

#include <stdint.h>

struct data_block
{
    int size;
    void* data;
};

struct data_block* data_block_alloc(int size);
void data_block_realloc(struct data_block* data_block, int new_size);
void data_block_free(struct data_block** data_block);

long data_block_dump_to_file(struct data_block* data_block, const char* filename);

#endif //COS_DATA_BLOCK_H
