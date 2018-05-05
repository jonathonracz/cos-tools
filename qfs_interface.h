//
// Created by Jonathon Racz on 5/3/18.
//

#ifndef COS_QFS_INTERFACE_H
#define COS_QFS_INTERFACE_H

#include <stdio.h>
#include "data_block.h"

enum qfs_decompress_status
{
    QFS_DECOMPRESS_STATUS_SUCCESS,
    QFS_DECOMPRESS_STATUS_READ_ERROR,
    QFS_DECOMPRESS_STATUS_BAD_COPY_OFFSET
};

// Note that stream should be pointing to byte 3 within the file (i.e. the first
// byte after magic), and qfs_file_size refers to the size of the entire
// compressed file (including magic)
enum qfs_decompress_status qfs_decompress_stream(FILE* stream,
        long qfs_file_size, struct data_block** dst);

#endif //COS_QFS_INTERFACE_H
