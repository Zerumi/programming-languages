#include <libc.h>
#include "image_util.h"

#define PIXELS_COUNT 9

struct image rotate(struct image const src) {
    size_t position = 0;
    uint32_t height = src.height;
    uint32_t width = src.width;

    struct image result = { 0 };

    /* Структура массива: [width0, width1, etc...., width^(height - 1)]
     * Поворот на 90: [(height0[i], height1[i])...]
     */

    result.height = width;
    result.width = height;
    result.pixels = malloc(height * width * sizeof (struct pixel));

    for (uint32_t i = 1; i <= width; i++) {
        for (uint32_t j = 1; j <= height; j++, position++) {
            result.pixels[position] = src.pixels[j * width - i];
        }
    }

    return result;
}

static size_t get_position(uint32_t row, uint32_t col, uint32_t width) {
    return width * row + col;
}

static void blur_padding_solid_color(struct image image, struct pixel solid) {
    for (uint32_t i = 0; i < image.width; ++i) {
        image.pixels[get_position(0, i, image.width)] = solid;
    }
    for (uint32_t i = 0; i < image.width; ++i) {
        image.pixels[get_position(image.height - 1, i, image.width)] = solid;
    }
    for (uint32_t i = 0; i < image.height; ++i) {
        image.pixels[get_position(i, 0, image.width)] = solid;
    }
    for (uint32_t i = 0; i < image.height; ++i) {
        image.pixels[get_position(i, image.width - 1, image.width)] = solid;
    }
}

static void blur_padding_black(struct image image) {
    blur_padding_solid_color(image, (struct pixel) { 0 });
}

static void blur_padding_white(struct image image) {
    blur_padding_solid_color(image, (struct pixel) { .r = 255, .g = 255, .b = 255 });
}

static void blur_padding_reflection(struct image image) {
    for (uint32_t i = 0; i < image.width; ++i) {
        image.pixels[get_position(0, i, image.width)] =
                image.pixels[get_position(1, i, image.width)];
    }
    for (uint32_t i = 0; i < image.width; ++i) {
        image.pixels[get_position(image.height - 1, i, image.width)] =
                image.pixels[get_position(image.height - 2, i, image.width)];
    }
    for (uint32_t i = 0; i < image.height; ++i) {
        image.pixels[get_position(i, 0, image.width)] =
                image.pixels[get_position(i, 1, image.width)];
    }
    for (uint32_t i = 0; i < image.height; ++i) {
        image.pixels[get_position(i, image.width - 1, image.width)] =
                image.pixels[get_position(i, image.width - 2, image.width)];
    }
}

static void blur_padding_img_loop(struct image image) {
    for (uint32_t i = 0; i < image.width; ++i) {
        image.pixels[get_position(0, i, image.width)] =
                image.pixels[get_position(image.height - 2, i, image.width)];
    }
    for (uint32_t i = 0; i < image.width; ++i) {
        image.pixels[get_position(image.height - 1, i, image.width)] =
                image.pixels[get_position(1, i, image.width)];;
    }
    for (uint32_t i = 0; i < image.height; ++i) {
        image.pixels[get_position(i, 0, image.width)] =
                image.pixels[get_position(i, image.width - 2, image.width)];
    }
    for (uint32_t i = 0; i < image.height; ++i) {
        image.pixels[get_position(i, image.width - 1, image.width)] =
                image.pixels[get_position(i, 1, image.width)];
    }
}

static void blur_padding_border_pixel(struct image image) {
    blur_padding_solid_color(image, image.pixels[get_position(0,0,image.width)]);
}

static void(*padding_functions[])(struct image) = {
        [BLACK] = blur_padding_black,
        [WHITE] = blur_padding_white,
        [REFLECTION] = blur_padding_reflection,
        [IMAGE_LOOP] = blur_padding_img_loop,
        [BORDER_PIXEL] = blur_padding_border_pixel
};

static struct image blur_apply_padding(struct image image, void(fill)(struct image)) {
    struct image result = ZERO_IMAGE;
    result.width = image.width + 2;
    result.height = image.height + 2;
    result.pixels = malloc(result.width * result.height * sizeof (struct pixel));

    for (uint32_t row = 1; row < result.height - 1; row++) {
        memcpy(result.pixels + get_position(row, 1, result.width),
               image.pixels + get_position(row - 1, 0, image.width),
               image.width * sizeof (struct pixel));
    }

    fill(result);
    return result;
}

struct image blur(struct image src, enum blur_border_identity_padding mode) {
    uint32_t height = src.height;
    uint32_t width = src.width;

    struct image result = ZERO_IMAGE;

    result.height = height;
    result.width = width;
    result.pixels = malloc(height * width * sizeof (struct pixel));

    /* apply padding */
    if (mode)
        src = blur_apply_padding(src, padding_functions[mode]);

    for (uint32_t image_row = 0; image_row < src.height; image_row++) {
        for (uint32_t image_col = 0; image_col < src.width; image_col++) {

            if (image_row == 0 || image_col == 0 || image_row == src.height - 1 || image_col == src.width - 1) {
                if (!mode) {
                    result.pixels[get_position(image_row, image_col, result.width)] =
                            src.pixels[get_position(image_row, image_col, src.width)];
                }
                continue;
            }

            uint32_t sum_r = 0;
            uint32_t sum_g = 0;
            uint32_t sum_b = 0;

            /* Оперируем в квадрате 3х3 со смещением относительно центра в координатах (2;2) */
            for (uint32_t sub_img_row = 1; sub_img_row <= 3; sub_img_row++) {
                for (uint32_t sub_img_col = 1; sub_img_col <= 3; ++sub_img_col) {
                    uint32_t current_subpixel_coord = src.width * (image_row + sub_img_row - 2) + (image_col + sub_img_col - 2);

                    sum_r += src.pixels[current_subpixel_coord].r;
                    sum_g += src.pixels[current_subpixel_coord].g;
                    sum_b += src.pixels[current_subpixel_coord].b;
                }
            }

            struct pixel result_pixel = {
                    .r = sum_r / PIXELS_COUNT,
                    .g = sum_g / PIXELS_COUNT,
                    .b = sum_b / PIXELS_COUNT
            };

            result.pixels[get_position(image_row - (mode ? 1 : 0), image_col - (mode ? 1 : 0), width)] = result_pixel;
        }
    }

    if (mode) free(src.pixels); /* only if image were replaced, deallocate it's memory */

    return result;
}
