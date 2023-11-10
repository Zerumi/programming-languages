#ifndef IMAGE_TRANSFORMER_COMMANDS_H
#define IMAGE_TRANSFORMER_COMMANDS_H

#include <stdint.h>
#include <stddef.h>

enum opcode {
    FILE_OPEN, FILE_CLOSE,
    IMAGE_LOAD_BMP, IMAGE_SAVE_BMP,
    IMAGE_ROTATE, IMAGE_BLUR,
    EXIT
};

struct command_status {
    enum opcode command;
    int32_t status_code;
};

struct c_noargs {
    enum opcode opcode;
};

struct c_args {
    enum opcode opcode;
    size_t argc;
    char** args;
};

union ins {
    enum opcode opcode;
    struct c_args as_args;
    struct c_args as_noargs;
};

#endif //IMAGE_TRANSFORMER_COMMANDS_H
