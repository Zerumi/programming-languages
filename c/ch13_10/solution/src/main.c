#include <stdio.h>
#include "image_bmp_worker.h"
#include "image_util.h"

#define ANGLE_SHOULD_BE_DIVIDABLE_BY_90 90
#define STRING_TO_LONG_VALUE_BASE 10
#define ARGV_PROGRAM_NAME 0
#define ARGV_INPUT_FILENAME 1
#define ARGV_OUTPUT_FILENAME 2
#define ARGV_ANGLE 3
#define ERR_NOT_ENOUGH_ARGUMENTS 1
#define ERR_UNSUPPORTED_ANGLE 2
#define FULL_ROTATE_ANGLE 360

/*
    READ_OK = 0,
    READ_CORRUPTED,
    READ_INVALID_SIGNATURE,
    READ_UNSUPPORTED_BITS,
    READ_INVALID_HEADER
 */
const char* read_statuses[] = {"OK",
                         "There's an unexpected error occurred when reading file",
                         "Unexpected end-of-file reached",
                         ".bmp file is not rgb-format (24-bits). Only this format is supported.",
                         ".bmp file have an invalid header and may be broken"
};

/*
    WRITE_OK = 0,
    WRITE_CORRUPTION
 */
const char* write_statuses[] = { "OK",
                           "There's an unexpected error occurred when writing in the file"
};

int main( int argc, char** argv ) {
    (void) argc; (void) argv; // suppress 'unused parameters' warning

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source-image> <transformed-image> <angle>\n", argv[ARGV_PROGRAM_NAME]);
        return ERR_NOT_ENOUGH_ARGUMENTS;
    }

    const char* input_filename = argv[ARGV_INPUT_FILENAME];
    const char* output_filename = argv[ARGV_OUTPUT_FILENAME];
    const char* angle_c = argv[ARGV_ANGLE];

    char* str_to_l_p;
    const long angle = strtol(angle_c, &str_to_l_p, STRING_TO_LONG_VALUE_BASE);

    if (angle % ANGLE_SHOULD_BE_DIVIDABLE_BY_90) {
        fputs("Angle should be one of this values: 0, 90, -90, 180, -180, 270, -270", stderr);
        return ERR_UNSUPPORTED_ANGLE;
    }

    /* allocate memory for source image */
    struct image img = { 0 };

    /* open a file and load source image */
    FILE* f = fopen(input_filename, "rb");
    if (!f) {
        fputs("Something went wrong during opening input file. Maybe we don't have access to it.", stderr);
    }

    const enum bmp_read_status rs = from_bmp(f, &img);
    if (rs) {
        fputs(read_statuses[rs], stderr);
        goto undo_open_f;
    }

    /* rotate them */
    const long angle_to_rotate = angle < 0 ? angle + FULL_ROTATE_ANGLE : angle;
    struct image rotated_img = img;
    for (long i = 0; i < angle_to_rotate / 90; i++) {
        rotated_img = rotate(rotated_img);
    }

    /* and save to output file */
    FILE* o = fopen(output_filename, "wb");
    if (!o) {
        fputs("Something went wrong during opening output file. Maybe we don't have access to it.", stderr);
        goto undo_load_image_from_f;
    }

    const enum bmp_write_status ws = to_bmp(o, &rotated_img);
    if (ws) {
        fputs(write_statuses[ws], stderr);
        goto undo_open_output_file;
    }

    printf("Image rotated by %ld degrees %s\n", labs(angle), angle > 0 ? "clockwise" : "counterclockwise");
    if (rotated_img.pixels != img.pixels)
        free(rotated_img.pixels);
    undo_open_output_file:
        fclose(o);
    undo_load_image_from_f:
        free(img.pixels);
    undo_open_f:
        fclose(f);

    return 0;
}
