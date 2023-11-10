#ifndef IMAGE_TRANSFORMER_IO_HANDLER_H
#define IMAGE_TRANSFORMER_IO_HANDLER_H

#include <stdio.h>

enum file_status {
    FILE_OK = 0,
    FILE_ERROR
};

enum file_status try_open_file(const char* filename, const char* mode, FILE** out);

#endif //IMAGE_TRANSFORMER_IO_HANDLER_H
