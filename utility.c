//
// Created by Jonathon Racz on 5/3/18.
//

#include <string.h>
#include "utility.h"

#if defined(__linux__) || defined(__APPLE__)
#include <zconf.h>
#endif

void swap_bytes(void* bytes, int size)
{
    int num_swaps = size / 2;
    for (int i = 0; i < num_swaps; ++i)
    {
        uint8_t swap = ((uint8_t*)bytes)[size - 1 - i];
        ((uint8_t*)bytes)[size - 1 - i] = ((uint8_t*)bytes)[i];
        ((uint8_t*)bytes)[i] = swap;
    }
}

void swap_uint32(uint32_t* value)
{
    swap_bytes(value, sizeof(uint32_t));
}

long read_bytes(void* dst, FILE* src, long num)
{
    int c;
    int i = 0;
    for (; i < num && ((c = fgetc(src)) != EOF) && !ferror(src); ++i)
        ((uint8_t*)dst)[i] = (uint8_t)c;

    return i;
}

int read_uint32_le(uint32_t* dst, FILE* src)
{
    uint32_t tmp_dst;
    long bytes_read = read_bytes(&tmp_dst, src, sizeof(uint32_t));
    if (bytes_read == sizeof(uint32_t))
    {
        *dst = tmp_dst;
        return 1;
    }

    return 0;
}

int read_uint32_be(uint32_t* dst, FILE* src)
{
    if (read_uint32_le(dst, src))
    {
        swap_uint32(dst);
        return 1;
    }

    return 0;
}

long write_bytes(FILE* dst, const void* src, long num)
{
    int c;
    int i = 0;
    for (; i < num; ++i)
        if (((c = fputc(((const uint8_t*)src)[i], dst)) == EOF))
            break;

    return i;
}

int replace_character(char* string, char char_to_replace, char replacement_char)
{
    return replace_characters(string, &char_to_replace, replacement_char);
}

int replace_characters(char* string, char* chars_to_replace, char replacement_char)
{
    int characters_replaced = 0;
    for (size_t i = 0; i < strlen(string); ++i)
    {
        if (strchr(chars_to_replace, string[i]) != NULL)
        {
            string[i] = replacement_char;
            characters_replaced++;
        }
    }

    return characters_replaced;
}

int mkdir_recursive(char* dir, int mode)
{
    return mkpath_np(dir, mode);
}
