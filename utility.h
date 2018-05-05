//
// Created by Jonathon Racz on 5/3/18.
//

#include <stdint.h>
#include <stdio.h>

#ifndef COS_BYTESWAP_H
#define COS_BYTESWAP_H

void swap_bytes(void* bytes, int size);

void swap_uint32(uint32_t* value);

long read_bytes(void* dst, FILE* src, long num);

int read_uint32_le(uint32_t* dst, FILE* src);

int read_uint32_be(uint32_t* dst, FILE* src);

long write_bytes(FILE* dst, const void* src, long num);

#endif //COS_BYTESWAP_H
