#ifndef IMAGE_TRANSFORMER_IMAGE_BMP_WORKER_H
#define IMAGE_TRANSFORMER_IMAGE_BMP_WORKER_H

#include "image.h"

#include <stdint.h>
#include <stdio.h>

#define BITS_SUPPORTED 24
#define BITS_IN_BYTE 8
#define BMP_ALIGN 4
#define ITEMS_TO_IO 1

#define BMP_TYPE 19778
#define BMP_RESERVED 0
#define BMP_SUBHEADER_SIZE 40
#define BMP_PLANES 1
#define BMP_NO_COMPRESSION 0
#define BMP_DEFAULT_SCALE 2834
#define BMP_CLR_UNUSED 0
#define BMP_CLR_NOT_IMPORTANT 0

enum bmp_read_status {
    READ_OK = 0,
    READ_CORRUPTED,
    READ_INVALID_SIGNATURE,
    READ_UNSUPPORTED_BITS,
    READ_INVALID_HEADER,
    READ_NO_FILE
};

enum bmp_write_status {
    WRITE_OK = 0,
    WRITE_CORRUPTION,
    WRITE_NO_FILE,
    WRITE_NO_IMAGE
};

enum bmp_read_status from_bmp(FILE* opened_binary_file, struct image* out);

enum bmp_write_status to_bmp(FILE* opened_binary_file, struct image* in);

#endif //IMAGE_TRANSFORMER_IMAGE_BMP_WORKER_H
