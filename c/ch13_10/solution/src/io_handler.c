#include "io_handler.h"

enum file_status try_open_file(const char* filename, const char* mode, FILE** out) {
    *out = fopen(filename, mode);
    if (!*out) return FILE_ERROR;
    return FILE_OK;
}
