/*
 * KNOWN ISSUES! Please, help me fix it:
 * 1) Duplicated lines in case of undoing some operations or freeing resources
 */

#include "image_bmp_worker.h"
#include "image_util.h"
#include "error_handler.h"

#include <stdio.h>
#include <assert.h>
#include <memory.h>

#define ANGLE_SHOULD_BE_DIVIDABLE_BY_90 90
#define STRING_TO_LONG_VALUE_BASE 10
#define ARGV_PROGRAM_NAME 0
#define ARGV_INPUT_FILENAME 1
#define ARGV_OUTPUT_FILENAME 2
#define ARGV_ANGLE 3
#define FULL_ROTATE_ANGLE 360

#define ERR_NOT_ENOUGH_ARGUMENTS 1
#define ERR_UNSUPPORTED_ANGLE 2
#define ERR_READ 3
#define ERR_WRITE 4
#define ERR_FILE 5

static struct image rotate_by_angle(struct image img, long angle) {
    assert(!(angle % ANGLE_SHOULD_BE_DIVIDABLE_BY_90));
    const long angle_to_rotate = angle < 0 ? angle + FULL_ROTATE_ANGLE : angle;
    struct image rotated_img = img;
    for (long i = 0; i < angle_to_rotate / ANGLE_SHOULD_BE_DIVIDABLE_BY_90; i++) {
        struct image new_rotated_img = rotate(rotated_img);
        free(rotated_img.pixels);
        rotated_img = new_rotated_img;
    }
    return rotated_img;
}

static void print_rotation_info(long angle) {
    printf("Image rotated by %ld degrees %s\n", labs(angle), angle > 0 ? "clockwise" : "counterclockwise");
}

int main( int argc, char** argv ) {
    (void) argc; (void) argv; // suppress 'unused parameters' warning

    if (argc != 4) {
        print_error(strcat("Usage: ", strcat(argv[ARGV_PROGRAM_NAME], " <source-image> <transformed-image> <angle>\n")));
        return ERR_NOT_ENOUGH_ARGUMENTS;
    }

    const char* input_filename = argv[ARGV_INPUT_FILENAME];
    const char* output_filename = argv[ARGV_OUTPUT_FILENAME];
    const char* angle_c = argv[ARGV_ANGLE];

    char* str_to_l_p;
    const long angle = strtol(angle_c, &str_to_l_p, STRING_TO_LONG_VALUE_BASE);

    if (angle % ANGLE_SHOULD_BE_DIVIDABLE_BY_90) {
        print_error("Angle should be one of this values: 0, 90, -90, 180, -180, 270, -270");
        return ERR_UNSUPPORTED_ANGLE;
    }

    /* allocate memory for source image */
    struct image img = { 0 };

    /* open a file and load source image */
    FILE* f;
    if (console_handle_file_error(
            try_open_file(input_filename, "rb", &f),
            input_filename))
        return ERR_FILE;
    if (console_handle_read_error(from_bmp(f, &img))) {
        fclose(f);
        return ERR_READ;
    }

    /* rotate them */
    struct image rotated_img = rotate_by_angle(img, angle);

    /* and save to output file */
    FILE* o;
    if (console_handle_file_error(
            try_open_file(output_filename, "wb", &o),
            output_filename)) {
        if(angle != 0)
            free(rotated_img.pixels);
        fclose(f);
        return ERR_FILE;
    }
    if (console_handle_write_error(to_bmp(o, &rotated_img))) {
        if(angle != 0)
            free(rotated_img.pixels);
        fclose(o);
        fclose(f);
        return ERR_WRITE;
    }

    if(angle != 0)
        free(rotated_img.pixels);
    fclose(o);
    fclose(f);

    print_rotation_info(angle);
    return 0;
}
