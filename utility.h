//
// Created by Jonathon Racz on 5/3/18.
//

#include <stdint.h>
#include <stdio.h>

#ifndef COS_BYTESWAP_H
#define COS_BYTESWAP_H

void swap_bytes(void* bytes, int size);

void swap_uint32(uint32_t* value);

int read_bytes(void* dest, int num, FILE* src);

int read_uint32_le(uint32_t* dst, FILE* src);

int read_uint32_be(uint32_t* dst, FILE* src);

#endif //COS_BYTESWAP_H
