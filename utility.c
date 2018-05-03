//
// Created by Jonathon Racz on 5/3/18.
//

#include "utility.h"

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

int read_bytes(void* dest, int num, FILE* src)
{
    int c;
    int i = 0;
    for (; i < num && ((c = fgetc(src)) != EOF); ++i)
        ((uint8_t*)dest)[i] = (uint8_t)c;

    return i;
}

int read_uint32_le(uint32_t* dst, FILE* src)
{
    uint32_t tmp_dst;
    int bytes_read = read_bytes(&tmp_dst, sizeof(uint32_t), src);
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
