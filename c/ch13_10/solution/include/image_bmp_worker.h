#ifndef IMAGE_TRANSFORMER_IMAGE_BMP_WORKER_H
#define IMAGE_TRANSFORMER_IMAGE_BMP_WORKER_H

#include <stdint.h>
#include <stdio.h>
#include "image.h"

enum bmp_read_status {
    READ_OK = 0,
    READ_CORRUPTED,
    READ_INVALID_SIGNATURE,
    READ_UNSUPPORTED_BITS,
    READ_INVALID_HEADER
};

enum bmp_write_status {
    WRITE_OK = 0,
    WRITE_CORRUPTION
};

enum bmp_read_status from_bmp(FILE* opened_binary_file, struct image* out);

enum bmp_write_status to_bmp(FILE* opened_binary_file, struct image* in);

#endif //IMAGE_TRANSFORMER_IMAGE_BMP_WORKER_H
