//
// Created by Jonathon Racz on 5/3/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "bigf_reader.h"
#include "utility.h"

enum bigf_table_status bigf_file_table_read(const char* filename, struct bigf_file_table** table)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
        return BIGF_TABLE_STATUS_FAILURE_COULD_NOT_OPEN_FILE;

    char magic[4];
    if (read_bytes(magic, file, sizeof(magic)) != sizeof(magic) ||
        memcmp(magic, "BIGF", sizeof(magic)) != 0)
    {
        fclose(file);
        return BIGF_TABLE_STATUS_FAILURE_INVALID_CONTENTS;
    }

    uint32_t file_size;
    uint32_t num_files;
    uint32_t file_table_size;
    if (!read_uint32_le(&file_size, file) ||
        !read_uint32_be(&num_files, file) ||
        !read_uint32_be(&file_table_size, file))
    {
        fclose(file);
        return BIGF_TABLE_STATUS_FAILURE_INVALID_CONTENTS;
    }

    (*table) = malloc(sizeof(struct bigf_file_table));
    (*table)->num_entries = num_files;
    (*table)->entries = malloc(sizeof(struct bigf_file_table_entry) * num_files);

    char filename_tmp[1024];
    for (uint32_t i = 0; i < num_files; ++i)
    {
        if (!read_uint32_be(&(*table)->entries[i].offset, file) ||
            !read_uint32_be(&(*table)->entries[i].size, file))
        {
            fclose(file);
            return BIGF_TABLE_STATUS_FAILURE_INVALID_CONTENTS;
        }

        int name_byte;
        int name_length = 0;
        for (; name_length < sizeof(filename_tmp); ++name_length)
        {
            name_byte = fgetc(file);
            if (name_byte == EOF)
            {
                fclose(file);
                return BIGF_TABLE_STATUS_FAILURE_INVALID_CONTENTS;
            }

            filename_tmp[name_length] = (char)name_byte;

            if ((char)name_byte == '\0')
                break;
        }

        if (name_length == 0 || name_length == sizeof(filename_tmp))
        {
            fclose(file);
            return BIGF_TABLE_STATUS_FAILURE_INVALID_FILENAME_IN_TABLE;
        }

        (*table)->entries[i].name = malloc((size_t)name_length + 1);
        strncpy((*table)->entries[i].name, filename_tmp, name_length);
    }

    fclose(file);
    return BIGF_TABLE_STATUS_SUCCESS;
}

void bigf_file_table_free(struct bigf_file_table** table)
{
    assert(table);
    for (uint32_t i = 0; i < (*table)->num_entries; ++i)
        free((*table)->entries[i].name);

    free((*table)->entries);
    free(*table);
}

struct bigf_file_table_entry* bigf_find_entry_for_file(const char* filename_in_bigf, struct bigf_file_table* table)
{
    assert(table);
    for (uint32_t i = 0; i < table->num_entries; ++i)
    {
        if (!strcmp(table->entries[i].name, filename_in_bigf))
            return &table->entries[i];
    }

    return NULL;
}

enum bigf_stream_status bigf_stream_file(const char* bigf_filename, const char* filename_in_bigf,
         struct bigf_file_table* table, enum bigf_stream_handler_status (*stream_callback)(struct data_block**, FILE*, long),
         struct data_block** result)
{
    assert(table);
    struct bigf_file_table_entry* entry = bigf_find_entry_for_file(filename_in_bigf, table);
    if (entry == NULL)
        return BIGF_STREAM_STATUS_FAILURE_COULD_NOT_FIND_FILE_IN_BIGF;

    FILE* file = fopen(bigf_filename, "rb");
    if (!file)
        return BIGF_STREAM_STATUS_FAILURE_COULD_NOT_OPEN_BIGF;

    fseek(file, entry->offset, SEEK_SET);

    enum bigf_stream_handler_status stream_handler_status = stream_callback(result, file, entry->size);
    fclose(file);
    return (stream_handler_status == BIGF_STREAM_HANDLER_STATUS_SUCCESS) ? BIGF_STREAM_STATUS_SUCCESS : BIGF_STREAM_STATUS_FAILURE_IN_HANDLER;
}

enum bigf_unpack_status bigf_unpack(const char* bigf_filename, const char* destination_dir,
       enum bigf_stream_handler_status (*stream_callback)(struct data_block**, FILE*, long))
{
    struct bigf_file_table* table;
    enum bigf_table_status table_status = bigf_file_table_read(bigf_filename, &table);
    if (table_status != BIGF_TABLE_STATUS_SUCCESS)
        return BIGF_UNPACK_STATUS_FAILURE_TABLE_ERROR;

    size_t destination_dir_len = strlen(destination_dir);
    for (uint32_t i = 0; i < table->num_entries; ++i)
    {
        size_t extracted_filename_length = destination_dir_len + strlen(table->entries[i].name) + 2;
        char* extracted_filename = malloc(extracted_filename_length);
        strcpy(extracted_filename, destination_dir);
        extracted_filename[destination_dir_len] = '/';
        strcpy(&extracted_filename[destination_dir_len + 1], table->entries[i].name);
        replace_character(&extracted_filename[destination_dir_len + 1], '\\', '/');
        printf("Extracting %s to %s... ", table->entries[i].name, extracted_filename);

        struct data_block* result;
        enum bigf_stream_status stream_result = bigf_stream_file(bigf_filename, table->entries[i].name, table, stream_callback, &result);
        if (stream_result != BIGF_STREAM_STATUS_SUCCESS)
        {
            printf("Failed (stream error)\n");
            return BIGF_UNPACK_STATUS_FAILURE_STREAM_ERROR;
        }

        {
            size_t output_dir_size = strrchr(extracted_filename, '/') - extracted_filename + 1;
            char* output_directory = malloc(output_dir_size);
            memcpy(output_directory, extracted_filename, output_dir_size);
            output_directory[output_dir_size - 1] = '\0';
            mkdir_recursive(output_directory, 0755);
            free(output_directory);
        }

        long bytes_written = data_block_dump_to_file(result, extracted_filename);
        if (bytes_written != result->size)
        {
            printf("Failed (disk write error)\n");
            return BIGF_UNPACK_STATUS_FILE_WRITE_ERROR;
        }

        printf("Success");
        free(extracted_filename);
    }

    bigf_file_table_free(&table);

    return BIGF_UNPACK_STATUS_SUCCESS;
}

void bigf_file_table_print(struct bigf_file_table* table)
{
    assert(table);
    printf("Entries: %d\n", table->num_entries);
    for (uint32_t i = 0; i < table->num_entries; ++i)
        bigf_file_table_entry_print(&table->entries[i]);
}

void bigf_file_table_entry_print(struct bigf_file_table_entry* entry)
{
    assert(entry);
    printf("%s\n\tOffset:\t%x\n\tSize:\t%d\n", entry->name, entry->offset, entry->size);
}
