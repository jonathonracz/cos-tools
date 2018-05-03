#include <stdio.h>
#include "bigf_reader.h"

int main()
{
    printf("Hello, World!\n");

    struct bigf_file_table* table;
    bigf_file_table_read("../../../Downloads/HPCOS.iso01/BIG/DATA.BIG", &table);
    bigf_file_table_print(table);
    bigf_file_table_free(&table);

    return 0;
}
