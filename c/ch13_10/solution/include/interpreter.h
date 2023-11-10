#ifndef IMAGE_TRANSFORMER_INTERPRETER_H
#define IMAGE_TRANSFORMER_INTERPRETER_H

#include "command.h"
#include "image.h"
#include "image_bmp_worker.h"
#include "image_util.h"

#include <errno.h>
#include <stdio.h>

enum interpreter_states {
    OK = 0,
    EXECUTION_ERROR,
    INTERPRETATION_ERROR
};

struct vm_state {
    const union ins* ip;
    FILE* current_opened_file;
    struct image current_image;
};

struct interpreter_state {
    enum interpreter_states status_code;
    struct command_status last_executed_command;
};

struct interpreter_state interpret(struct vm_state* state);

void initialize_vm(const union ins program[], struct vm_state* out);

#endif //IMAGE_TRANSFORMER_INTERPRETER_H
