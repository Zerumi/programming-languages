#ifndef IMAGE_TRANSFORMER_COMMANDS_H
#define IMAGE_TRANSFORMER_COMMANDS_H

#include <stdint.h>

enum opcode {
    FILE_OPEN, FILE_CLOSE,
    IMAGE_LOAD, IMAGE_ROTATE, IMAGE_BLUR, IMAGE_SAVE,
    EXIT
};

struct command_status {
    enum opcode command;
    uint32_t status_code;
};

struct c_noargs {
    enum opcode opcode;
};

struct c_args {
    enum opcode opcode;
    char** args;
};

union ins {
    enum opcode opcode;
    struct c_args as_args;
    struct c_args as_noargs;
};

#endif //IMAGE_TRANSFORMER_COMMANDS_H
