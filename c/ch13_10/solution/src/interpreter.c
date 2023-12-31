#include "interpreter.h"

static int32_t interpret_file_open(struct vm_state* state) {
    if (state->ip->as_args.argc != ARG_COUNT_FILE_OPEN) {
        return ERROR_ARGUMENT_COUNT_ERROR;
    }

    state->current_opened_file = fopen(state->ip->as_args.args[ARGUMENT_FILENAME],
                                       state->ip->as_args.args[ARGUMENT_FILEMODE]);

    if (!state->current_opened_file)
        return ERROR_FILE_ERROR;

    return EXECUTED_OK;
}

static int32_t interpret_file_close(struct vm_state* state) {
    if (!state->current_opened_file) return ERROR_NO_FILE_ERROR;
    if (fclose(state->current_opened_file)) return ERROR_FILE_ERROR;
    return EXECUTED_OK;
}

static int32_t interpret_image_load_bmp(struct vm_state* state) {
    if (!state->current_opened_file) {
        return READ_NO_FILE;
    }
    enum bmp_read_status read_status = from_bmp(state->current_opened_file,
            &state->current_image);
    return read_status;
}

static int32_t interpret_image_save_bmp(struct vm_state* state) {
    if (!state->current_opened_file) {
        return WRITE_NO_FILE;
    }
    if (image_compare(&state->current_image, &ZERO_IMAGE)) {
        return WRITE_NO_IMAGE;
    }
    enum bmp_write_status write_status = to_bmp(state->current_opened_file,
            &state->current_image);
    return write_status;
}

static int32_t exec_image_rotate(struct vm_state* state) {
    if (state->ip->as_args.argc != ARG_COUNT_IMAGE_ROTATE) {
        return ERROR_ARGUMENT_COUNT_ERROR;
    }

    const char* angle_c = state->ip->as_args.args[ARGUMENT_ANGLE];

    char* str_to_l_p;
    const long angle = strtol(angle_c, &str_to_l_p, STRING_TO_LONG_VALUE_BASE);

    if (angle % ANGLE_SHOULD_BE_DIVIDABLE_BY_90) {
        return ERROR_ARGUMENT_ERROR;
    }

    const long angle_to_rotate = angle < 0 ? angle + FULL_ROTATE_ANGLE : angle;
    struct image rotated_img = state->current_image;
    for (long i = 0; i < angle_to_rotate / ANGLE_SHOULD_BE_DIVIDABLE_BY_90; i++) {
        struct image new_rotated_img = rotate(rotated_img);
        free(rotated_img.pixels);
        rotated_img = new_rotated_img;
    }

    state->current_image = rotated_img;
    return EXECUTED_OK;
}

static int32_t exec_image_blur(struct vm_state* state) {
    if (state->ip->as_args.argc != ARG_COUNT_IMAGE_BLUR) {
        return ERROR_ARGUMENT_COUNT_ERROR;
    }

    const char* mode_c = state->ip->as_args.args[ARGUMENT_MODE];

    char* str_to_l_p;
    const long mode = strtol(mode_c, &str_to_l_p, STRING_TO_LONG_VALUE_BASE);
    
    if (mode > ARGUMENT_BLUR_MODE_LIMIT_ABOVE || mode < ARGUMENT_BLUR_MODE_LIMIT_BEYOND) {
        return ERROR_ARGUMENT_ERROR;
    }
    
    struct image blurred_image  = blur(state->current_image, mode);
    free(state->current_image.pixels);

    state->current_image = blurred_image;
    return EXECUTED_OK;
}

static int32_t interpret_image_command(struct vm_state* state, int32_t(image_operation)(struct vm_state*)) {
    if (image_compare(&state->current_image, &ZERO_IMAGE)) {
        return ERROR_NO_IMAGE_ERROR;
    }
    return image_operation(state);
}

static int32_t interpret_image_rotate(struct vm_state* state) {
    return interpret_image_command(state, &exec_image_rotate);
}

static int32_t interpret_image_blur(struct vm_state* state) {
    return interpret_image_command(state, &exec_image_blur);
}


static int32_t interpret_exit(struct vm_state* state) {
    if (state->current_image.pixels) {
        free(state->current_image.pixels);
    }
    state->ip = NULL;
    return EXECUTED_OK;
}

int32_t (*interpreters[]) (struct vm_state*) = {
        [FILE_OPEN] = interpret_file_open,
        [FILE_CLOSE] = interpret_file_close,
        [IMAGE_LOAD_BMP] = interpret_image_load_bmp,
        [IMAGE_SAVE_BMP] = interpret_image_save_bmp,
        [IMAGE_ROTATE] = interpret_image_rotate,
        [IMAGE_BLUR] = interpret_image_blur,
        [EXIT] = interpret_exit
};

struct interpreter_state interpret(struct vm_state* state) {

    struct interpreter_state i_state = {0};

    for (;state->ip;) {
        struct command_status status = { .command = state->ip->opcode };
        status.status_code = interpreters[state->ip->opcode](state);
        i_state.last_executed_command_status = status;
        if (status.status_code) {
            i_state.status_code = EXECUTION_ERROR;
            break;
        }
        if (state->ip) state->ip++;
    }

    return i_state;
}

void initialize_vm(const union ins program[], struct vm_state* out) {
    *out = (struct vm_state) {
            .ip = program,
            .current_image = ZERO_IMAGE,
            .current_opened_file = NULL
    };
}
