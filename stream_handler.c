//
// Created by Jonathon Racz on 5/4/18.
//

#include <string.h>
#include "stream_handler.h"
#include "qfs_interface.h"
#include "utility.h"

enum bigf_stream_handler_status stream_handler(struct data_block** result, FILE* stream, long length)
{
    uint8_t first_bytes[2] = { 0 };
    long bytes_read = read_bytes(first_bytes, stream, sizeof(first_bytes));

    uint8_t qfs_magic[2] = { 0x10, 0xFB };
    if (!memcmp(first_bytes, &qfs_magic, 2))
    {
        return (qfs_decompress_stream(stream, length, result) == QFS_DECOMPRESS_STATUS_SUCCESS) ? BIGF_STREAM_HANDLER_STATUS_SUCCESS : BIGF_STREAM_HANDLER_STATUS_FAILURE;
    }
    else
    {
        *result = data_block_alloc((int)length);
        memcpy((*result)->data, first_bytes, bytes_read);
        long remaining_bytes_read = read_bytes(&((uint8_t*)(*result)->data)[sizeof(first_bytes)], stream, length - sizeof(first_bytes));
        return (bytes_read + remaining_bytes_read == length) ? BIGF_STREAM_HANDLER_STATUS_SUCCESS : BIGF_STREAM_HANDLER_STATUS_FAILURE;
    }
}
