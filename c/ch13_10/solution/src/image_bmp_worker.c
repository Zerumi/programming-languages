#include "image_bmp_worker.h"

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

static uint32_t get_padding (uint32_t width, uint16_t bit_count) {
    uint32_t total_bytes = width * (bit_count / BITS_IN_BYTE);
    return (BMP_ALIGN - total_bytes % BMP_ALIGN) % BMP_ALIGN;
}

static struct bmp_header generate_header( uint32_t height, uint32_t width, uint16_t bit_count ) {
    struct bmp_header result = {
            .bfType = BMP_TYPE,
            .bfileSize =
            sizeof (struct bmp_header) + height * width * bit_count
            + get_padding(width, bit_count) * height,
            .bfReserved = BMP_RESERVED,
            .bOffBits = sizeof (struct bmp_header),

            .biSize = BMP_SUBHEADER_SIZE,
            .biWidth = width,
            .biHeight = height,
            .biPlanes = BMP_PLANES,
            .biBitCount = bit_count,
            .biCompression = BMP_NO_COMPRESSION,
            .biSizeImage = 0, /* allowed because .biCompression = 0 */
            .biXPelsPerMeter = BMP_DEFAULT_SCALE,
            .biYPelsPerMeter = BMP_DEFAULT_SCALE,
            .biClrUsed = BMP_CLR_UNUSED,
            .biClrImportant = BMP_CLR_NOT_IMPORTANT
    };

    return result;
}

enum bmp_read_status from_bmp(FILE* opened_binary_file, struct image* out) {

    struct bmp_header header = { 0 };

    if (fread(&header, sizeof(struct bmp_header), 1, opened_binary_file) < 1) {
        if (feof(opened_binary_file))
            return READ_INVALID_HEADER;
        if (ferror(opened_binary_file))
            return READ_CORRUPTED;
    }

    if (header.biBitCount != BITS_SUPPORTED)
        return READ_UNSUPPORTED_BITS;

    uint32_t height = header.biHeight;
    uint32_t width = header.biWidth;

    uint32_t padding_value = get_padding(width, header.biBitCount);

    struct pixel* pixels = malloc(height * width * sizeof (struct pixel));

    for (uint32_t i = 0; i < height; i++) {
        if (fread(pixels + i * width,
                  width * sizeof(struct pixel),
                  ITEMS_TO_IO, opened_binary_file) < ITEMS_TO_IO) {
            free(pixels);
            if (feof(opened_binary_file))
                return READ_INVALID_SIGNATURE;
            return READ_CORRUPTED;
        }

        if (fseek(opened_binary_file, padding_value, SEEK_CUR) == -1)
            return READ_CORRUPTED;
    }

    out->height = height;
    out->width = width;
    out->pixels = pixels;
    return READ_OK;
}

enum bmp_write_status to_bmp(FILE* opened_binary_file, struct image* in) {

    struct bmp_header generated_header = generate_header(in->height, in->width, 24);

    if (fwrite(&generated_header, sizeof(struct bmp_header), 1, opened_binary_file) < 1) {
        return WRITE_CORRUPTION;
    }

    uint8_t padding[] = {42, 42, 42, 42}; /* 42! 42! 42! 42! */

    for (uint32_t i = 0; i < in->height; i++) {
        if (fwrite(in->pixels + i * in->width,
               in->width * sizeof(struct pixel),
                   ITEMS_TO_IO, opened_binary_file) < ITEMS_TO_IO) {

            return WRITE_CORRUPTION;
        }

        if (get_padding(in->width, sizeof (struct pixel) * BITS_IN_BYTE) &&
                fwrite(&padding, get_padding(in->width, sizeof (struct pixel) * BITS_IN_BYTE),
                   ITEMS_TO_IO, opened_binary_file) < ITEMS_TO_IO) {

            return WRITE_CORRUPTION;
        }
    }

    return WRITE_OK;
}

