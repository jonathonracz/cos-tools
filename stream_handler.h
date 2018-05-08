//
// Created by Jonathon Racz on 5/4/18.
//

#ifndef COS_STREAM_HANDLER_H
#define COS_STREAM_HANDLER_H

#include <stdio.h>
#include "data_block.h"
#include "bigf_reader.h"

enum bigf_stream_handler_status stream_handler(struct data_block** result, FILE* stream, long length);

#endif //COS_STREAM_HANDLER_H
