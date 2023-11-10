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

struct image blur(struct image src) {
    size_t position = 0;
    uint32_t height = src.height;
    uint32_t width = src.width;

    struct image result = { 0 };

    result.height = height;
    result.width = width;
    result.pixels = malloc(height * width * sizeof (struct pixel));

    for (uint32_t image_row = 0; image_row < height; image_row++) {
        for (uint32_t image_col = 0; image_col < width; image_col++, position++) {

            uint32_t current_pixel_coord = width * image_row + image_col;

            if (image_row == 0 || image_col == 0 || image_row == width - 1 || image_col == height - 1) {
                result.pixels[current_pixel_coord] = src.pixels[current_pixel_coord];
                continue;
            }

            uint32_t sum_r = 0;
            uint32_t sum_g = 0;
            uint32_t sum_b = 0;

            /* Оперируем в квадрате 3х3 со смещением относительно центра в координатах (2;2) */
            for (uint32_t sub_img_row = 1; sub_img_row <= 3; sub_img_row++) {
                for (uint32_t sub_img_col = 1; sub_img_col <= 3; ++sub_img_col) {
                    uint32_t current_subpixel_coord = width * (image_row + sub_img_row - 2) + (image_col + sub_img_col - 2);

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

            result.pixels[current_pixel_coord] = result_pixel;
        }
    }

    return result;
}
