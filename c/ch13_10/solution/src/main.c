#include "command.h"
#include "interpreter.h"

#define ERR_NOT_ENOUGH_ARGUMENTS 42

#define ARGV_PROGRAM_NAME 0
#define ARGV_INPUT_FILENAME 1
#define ARGV_OUTPUT_FILENAME 2
#define ARGV_ANGLE 3

int main( int argc, char** argv ) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source-image> <transformed-image> <angle>\n", argv[ARGV_PROGRAM_NAME]);
        return ERR_NOT_ENOUGH_ARGUMENTS;
    }

    const char* arguments_f_open[] = { argv[ARGV_INPUT_FILENAME], "rb" };
    const char* arguments_i_rotate[] = { argv[ARGV_ANGLE] };
    const char* arguments_o_open[] = { argv[ARGV_OUTPUT_FILENAME], "wb" };

    const union ins program[] = {
            { .as_args = {FILE_OPEN, 2, (char **)arguments_f_open} },
            { IMAGE_LOAD_BMP },
            { FILE_CLOSE },
            { .as_args = {IMAGE_ROTATE, 1, (char **)arguments_i_rotate} },
            { .as_args = {FILE_OPEN, 2, (char **)arguments_o_open} },
            { IMAGE_SAVE_BMP },
            { FILE_CLOSE },
            { EXIT }
    };

    struct vm_state state;

    initialize_vm(program, &state);

    struct interpreter_state result = interpret(&state);

    // provide status to error handler

    return result.last_executed_command.status_code;
}

