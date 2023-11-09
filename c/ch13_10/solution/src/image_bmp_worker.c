#include "image_bmp_worker.h"
#define BITS_IN_BYTE 8
#define BMP_ALIGN 4
#define ITEMS_TO_IO 1

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
    return total_bytes % BMP_ALIGN;
}

enum bmp_read_status from_bmp(FILE* opened_binary_file, struct image* out) {

    struct bmp_header header = { 0 };

    if (fread(&header, sizeof(struct bmp_header), 1, opened_binary_file) < 1) {
        if (feof(opened_binary_file))
            return READ_INVALID_HEADER;
        if (ferror(opened_binary_file))
            return READ_CORRUPTED;
    }

    if (header.biBitCount != 24)
        return READ_UNSUPPORTED_BITS;

    uint32_t height = header.biHeight;
    uint32_t width = header.biWidth;

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

        if (fseek(opened_binary_file, get_padding(width, header.biBitCount), SEEK_CUR) == -1)
            return READ_CORRUPTED;
    }

    out->height = height;
    out->width = width;
    out->pixels = pixels;
    return READ_OK;
}

static struct bmp_header generate_header( uint32_t height, uint32_t width, uint16_t bit_count ) {
    struct bmp_header result = {
            .bfType = 19778,
            .bfileSize =
                    sizeof (struct bmp_header) + height * width * bit_count
                            + get_padding(width, bit_count) * height,
            .bfReserved = 0,
            .bOffBits = 54,

            .biSize = 40,
            .biWidth = width,
            .biHeight = height,
            .biPlanes = 1,
            .biBitCount = bit_count,
            .biCompression = 0,
            .biSizeImage = 0, /* allowed because .biCompression = 0 */
            .biXPelsPerMeter = 2834,
            .biYPelsPerMeter = 2834,
            .biClrUsed = 0,
            .biClrImportant = 0
    };

    return result;
}

enum bmp_write_status to_bmp(FILE* opened_binary_file, struct image* in) {

    struct bmp_header generated_header = generate_header(in->height, in->width, 24);

    if (fwrite(&generated_header, sizeof(struct bmp_header), 1, opened_binary_file) < 1) {
        return WRITE_CORRUPTION;
    }

    uint8_t padding[] = {0, 0, 0, 0};

    for (uint32_t i = 0; i < in->height; i++) {
        if (fwrite(in->pixels + i * in->width,
               in->width * sizeof(struct pixel),
                   ITEMS_TO_IO, opened_binary_file) < ITEMS_TO_IO) {

            return WRITE_CORRUPTION;
        }

        if (get_padding(in->width, sizeof (struct pixel) * 3) &&
                fwrite(&padding, get_padding(in->width, sizeof (struct pixel) * 3),
                   ITEMS_TO_IO, opened_binary_file) < ITEMS_TO_IO) {

            return WRITE_CORRUPTION;
        }
    }

    return WRITE_OK;
}

