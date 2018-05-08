//
// Created by Jonathon Racz on 5/3/18.
//

#ifndef COS_BYTESWAP_H
#define COS_BYTESWAP_H

#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define mkdir(dir, mode) _mkdir(dir)
#else
#include <sys/stat.h>
#endif

void swap_bytes(void* bytes, int size);

void swap_uint32(uint32_t* value);

long read_bytes(void* dst, FILE* src, long num);

int read_uint32_le(uint32_t* dst, FILE* src);

int read_uint32_be(uint32_t* dst, FILE* src);

long write_bytes(FILE* dst, const void* src, long num);

int replace_character(char* string, char char_to_replace, char replacement_char);
int replace_characters(char* string, char* chars_to_replace, char replacement_char);

int mkdir_recursive(char* dir, int mode);

#endif //COS_BYTESWAP_H
