//
// Created by Jonathon Racz on 5/4/18.
//

#include <string.h>
#include "stream_handler.h"
#include "qfs_interface.h"
#include "utility.h"

void stream_handler(struct data_block** result, FILE* stream, long length)
{
    uint8_t first_bytes[2] = { 0 };
    long bytes_read = read_bytes(first_bytes, stream, sizeof(first_bytes));

    uint8_t qfs_magic[2] = { 0x10, 0xFB };
    if (!memcmp(first_bytes, &qfs_magic, 2))
    {
        qfs_decompress_stream(stream, length, result);
    }
    else
    {
        data_block_alloc((int)length);
        memcpy((*result)->data, first_bytes, sizeof(first_bytes));
        read_bytes(&((uint8_t*)(*result)->data)[sizeof(first_bytes)], stream, length - sizeof(first_bytes));
    }
}
