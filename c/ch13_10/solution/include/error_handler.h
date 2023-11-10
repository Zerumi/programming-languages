#ifndef IMAGE_TRANSFORMER_ERROR_HANDLER_H
#define IMAGE_TRANSFORMER_ERROR_HANDLER_H

#include "image_bmp_worker.h"
#include "io_handler.h"

#include <errno.h>
#include <stdbool.h>
#include <string.h>

void print_error(const char* error);

bool console_handle_read_error(enum bmp_read_status read_status);
bool console_handle_write_error(enum bmp_write_status write_status);
bool console_handle_file_error(enum file_status file_status, const char* filename);

#endif //IMAGE_TRANSFORMER_ERROR_HANDLER_H
