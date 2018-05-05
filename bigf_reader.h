//
// Created by Jonathon Racz on 5/2/18.
//

#ifndef COS_BIGF_READER_H
#define COS_BIGF_READER_H

#include <stdio.h>
#include <stdint.h>
#include "data_block.h"

struct bigf_file_table_entry
{
    uint32_t offset;
    uint32_t size;
    char* name;
};

struct bigf_file_table
{
    uint32_t num_entries;
    struct bigf_file_table_entry* entries;
};

enum bigf_status
{
    BIGF_STATUS_SUCCESS,
    BIGF_STATUS_FAILURE_COULD_NOT_OPEN_FILE,
    BIGF_STATUS_FAILURE_INVALID_CONTENTS,
    BIGF_STATUS_FAILURE_INVALID_FILENAME_IN_TABLE
};

enum bigf_stream_status
{
    BIGF_STREAM_STATUS_SUCCESS,
    BIGF_STREAM_STATUS_FAILURE_COULD_NOT_OPEN_BIGF,
    BIGF_STREAM_STATUS_FAILURE_COULD_NOT_FIND_FILE_IN_BIGF,
    BIGF_STREAM_STATUS_FAILURE_TABLE_MISMATCH
};

enum bigf_status bigf_file_table_read(const char* bigf_filename, struct bigf_file_table** table);
void bigf_file_table_free(struct bigf_file_table** table);

struct bigf_file_table_entry* bigf_find_entry_for_file(const char* filename_in_bigf, struct bigf_file_table* table);

enum bigf_stream_status bigf_stream_file(const char* bigf_filename, const char* filename_in_bigf,
        struct bigf_file_table* table, void (*stream_callback)(struct data_block**, FILE*, long),
        struct data_block** result);

void bigf_file_table_print(struct bigf_file_table* table);
void bigf_file_table_entry_print(struct bigf_file_table_entry* entry);

#endif //COS_BIGF_READER_H
