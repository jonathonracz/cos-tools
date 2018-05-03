//
// Created by Jonathon Racz on 5/2/18.
//

#include <stdio.h>
#include <stdint.h>

#ifndef COS_BIGF_READER_H
#define COS_BIGF_READER_H

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
    BIGF_STATUS_FAILURE,
    BIGF_STATUS_COULD_NOT_OPEN_FILE,
    BIGF_STATUS_INVALID_CONTENTS,
    BIGF_STATUS_INVALID_FILENAME_IN_TABLE
};

enum bigf_status bigf_file_table_read(const char* filename, struct bigf_file_table** table);
void bigf_file_table_free(struct bigf_file_table** table);

void bigf_file_table_print(struct bigf_file_table* table);
void bigf_file_table_entry_print(struct bigf_file_table_entry* entry);

#endif //COS_BIGF_READER_H
