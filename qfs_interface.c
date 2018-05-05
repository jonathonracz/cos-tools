//
// Created by Jonathon Racz on 5/3/18.
//

#include <stdint.h>
#include <assert.h>
#include "qfs_interface.h"
#include "utility.h"

enum qfs_decompress_status qfs_decompress_stream(FILE* stream,
        long qfs_file_size, struct data_block** dst)
{
    assert(stream != NULL);

    uint32_t uncompressed_size = 0;
    // Reads a 24-bit big endian uint into the second byte + swaps to make LE.
    if (!read_bytes(&((uint8_t*)&uncompressed_size)[1], stream, 3))
        return QFS_DECOMPRESS_STATUS_READ_ERROR;

    swap_uint32(&uncompressed_size);

    *dst = data_block_alloc((int)uncompressed_size);

    long destination_offset = ftell(stream) + qfs_file_size - 5;
    long decompressed_write_offset = 0;
    while (ftell(stream) != destination_offset)
    {
        int control_code = fgetc(stream);
        if (ferror(stream))
            return QFS_DECOMPRESS_STATUS_READ_ERROR;

        uint8_t control_codes[4];
        control_codes[0] = (uint8_t)control_code;
        long compressed_bytes_to_read = 0;
        long decompressed_bytes_to_read = 0;
        long decompressed_bytes_to_read_offset = 0;

        if (control_codes[0] <= 0x7F) // 0x00 - 0x7F
        {
            if (!read_bytes(&control_codes[1], stream, 1))
                return QFS_DECOMPRESS_STATUS_READ_ERROR;

            compressed_bytes_to_read = control_codes[0] & 0x03;
            decompressed_bytes_to_read = ((control_codes[0] & 0x1C) >> 2) + 3;
            decompressed_bytes_to_read_offset = ((control_codes[0] & 0x60) << 3) + control_codes[1] + 1;
        }
        else if (control_codes[0] <= 0xBF) // 0x80 - 0xBF
        {
            if (!read_bytes(&control_codes[1], stream, 2))
                return QFS_DECOMPRESS_STATUS_READ_ERROR;

            compressed_bytes_to_read = ((control_codes[1] & 0xC0) >> 6) & 0x03;
            decompressed_bytes_to_read = (control_codes[0] & 0x3F) + 4;
            decompressed_bytes_to_read_offset = ((control_codes[1] & 0x3F) << 8) + control_codes[2] + 1;
        }
        else if (control_codes[0] <= 0xDF) // 0xC0 - 0xDF
        {
            if (!read_bytes(&control_codes[1], stream, 3))
                return QFS_DECOMPRESS_STATUS_READ_ERROR;

            compressed_bytes_to_read = control_codes[0] & 0x03;
            decompressed_bytes_to_read = ((control_codes[0] & 0x0C) << 6) + control_codes[3] + 4;
            decompressed_bytes_to_read_offset = ((control_codes[0] & 0x10) << 12) + (control_codes[1] << 8) + 1;
        }
        else if (control_codes[0] <= 0xFC) // 0xE0 - 0xFC
        {
            compressed_bytes_to_read = ((control_codes[0] & 0x1F) << 2) + 4;
        }
        else // 0xFD - 0xFF
        {
            compressed_bytes_to_read = control_codes[0] & 0x03;
        }

        if (!read_bytes(&(*dst)->data[decompressed_write_offset], stream, compressed_bytes_to_read))
            return QFS_DECOMPRESS_STATUS_READ_ERROR;

        decompressed_write_offset += compressed_bytes_to_read;
        if (decompressed_bytes_to_read > 0 && decompressed_write_offset - decompressed_bytes_to_read_offset < 0)
            return QFS_DECOMPRESS_STATUS_BAD_COPY_OFFSET;

        for (long i = 0; i < decompressed_bytes_to_read; ++i)
        {
            ((uint8_t*)(*dst)->data)[decompressed_write_offset] = ((uint8_t*)(*dst)->data)[decompressed_write_offset - 1 - decompressed_bytes_to_read_offset];
            decompressed_write_offset++;
        }
    }

    return QFS_DECOMPRESS_STATUS_SUCCESS;
}
