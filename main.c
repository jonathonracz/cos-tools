#include <stdio.h>
#include <assert.h>
#include "bigf_reader.h"
#include "data_block.h"
#include "stream_handler.h"

int main()
{
    printf("Hello, World!\n");

    bigf_unpack("../../../Downloads/HPCOS.iso01/BIG/DATA.BIG", "./extract", &stream_handler);

    /*
    struct bigf_file_table* table;
    bigf_file_table_read("../../../Downloads/HPCOS.iso01/BIG/DATA.BIG", &table);
    bigf_file_table_print(table);

    struct data_block* extracted_file;
    enum bigf_stream_status stream_status = bigf_stream_file(
            "../../../Downloads/HPCOS.iso01/BIG/DATA.BIG",
            "generateddata\\chapters.txt", table, &stream_handler, &extracted_file);

    assert(stream_status == BIGF_STREAM_STATUS_SUCCESS);

    data_block_dump_to_file(extracted_file, "chapters.txt");

    bigf_file_table_free(&table);
*/

    return 0;
}
