#include "interpreter.h"

#define FILE_OK 0
#define FILE_ERROR 1

static struct command_status interpret_file_open(struct vm_state* state) {
    struct command_status status = { .command = FILE_OPEN };
    state->current_opened_file = fopen(state->ip->as_args.args[1], state->ip->as_args.args[2]);
    if (state->current_opened_file) status.status_code = FILE_ERROR;
    return status;
}

static struct command_status interpret_file_close(struct vm_state* state) {
    struct command_status status = { .command = FILE_CLOSE };
    return status;
}

static struct command_status interpret_image_load(struct vm_state* state) {
    struct command_status status = { .command = IMAGE_LOAD };
    return status;
}

static struct command_status interpret_image_rotate(struct vm_state* state) {
    struct command_status status = { .command = IMAGE_ROTATE };
    return status;
}

static struct command_status interpret_image_blur(struct vm_state* state) {
    struct command_status status = { .command = IMAGE_BLUR };
    return status;
}

static struct command_status interpret_image_save(struct vm_state* state) {
    struct command_status status = { .command = IMAGE_SAVE };
    return status;
}

static struct command_status interpret_exit(struct vm_state* state) {
    struct command_status status = { .command = EXIT };
    state->ip = NULL;
    return status;
}

struct command_status (*interpreters[]) (struct vm_state*) = {
        [FILE_OPEN] = interpret_file_open,
        [FILE_CLOSE] = interpret_file_close,
        [IMAGE_LOAD] =interpret_image_load,
        [IMAGE_SAVE] = interpret_image_save,
        [IMAGE_ROTATE] = interpret_image_rotate,
        [IMAGE_BLUR] = interpret_image_blur,
        [EXIT] = interpret_exit
};

struct interpreter_state interpret(struct vm_state* state) {

    struct interpreter_state i_state = {0};

    for (;state->ip;) {
        struct command_status status = interpreters[state->ip->opcode](state);
        i_state.last_executed_command = status;
        if (status.status_code) {

        }
        if (state->ip) state->ip++;
    }
}
