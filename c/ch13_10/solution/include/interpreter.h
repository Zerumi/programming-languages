#ifndef IMAGE_TRANSFORMER_INTERPRETER_H
#define IMAGE_TRANSFORMER_INTERPRETER_H

#include "command.h"
#include "image.h"
#include "image_bmp_worker.h"
#include "image_util.h"

#include <errno.h>
#include <stdio.h>

#define ARG_COUNT_IMAGE_BLUR 1
#define ARG_COUNT_FILE_OPEN 2
#define ARG_COUNT_IMAGE_ROTATE 1

#define ARGUMENT_FILENAME 0
#define ARGUMENT_FILEMODE 1
#define ARGUMENT_ANGLE 0
#define ARGUMENT_MODE 0

#define FULL_ROTATE_ANGLE 360
#define ANGLE_SHOULD_BE_DIVIDABLE_BY_90 90
#define STRING_TO_LONG_VALUE_BASE 10

#define ARGUMENT_MODE 0
#define ARGUMENT_BLUR_MODE_LIMIT_ABOVE 5
#define ARGUMENT_BLUR_MODE_LIMIT_BEYOND 0
enum interpreter_states {
    OK = 0,
    EXECUTION_ERROR,
    INTERPRETATION_ERROR /* reserved for parser */
};

enum command_error_codes {
    EXECUTED_OK = 0,
    ERROR_ARGUMENT_COUNT_ERROR = 43,
    ERROR_ARGUMENT_ERROR,
    ERROR_FILE_ERROR,
    ERROR_NO_FILE_ERROR,
    ERROR_NO_IMAGE_ERROR
};

struct vm_state {
    const union ins* ip;
    FILE* current_opened_file;
    struct image current_image;
};

struct interpreter_state {
    enum interpreter_states status_code;
    struct command_status last_executed_command_status;
};

struct interpreter_state interpret(struct vm_state* state);

void initialize_vm(const union ins program[], struct vm_state* out);

#endif //IMAGE_TRANSFORMER_INTERPRETER_H
