#include "error_handler.h"

/*
    READ_OK = 0,
    READ_CORRUPTED,
    READ_INVALID_SIGNATURE,
    READ_UNSUPPORTED_BITS,
    READ_INVALID_HEADER
 */
static const char* read_statuses[] = {"OK",
                               "There's an unexpected error occurred when reading file",
                               "Unexpected end-of-file reached",
                               ".bmp file is not rgb-format (24-bits). Only this format is supported.",
                               ".bmp file have an invalid header and may be broken"
};

/*
    WRITE_OK = 0,
    WRITE_CORRUPTION
 */
static const char* write_statuses[] = { "OK",
                                 "There's an unexpected error occurred when writing in the file"
};

/*
    FILE_OPEN, FILE_CLOSE,
    IMAGE_LOAD_BMP, IMAGE_SAVE_BMP,
    IMAGE_ROTATE, IMAGE_BLUR,
    EXIT
 */
static const char* command_fetch_list[] = {
        "Open file",
        "Close file",
        "Loading image from bmp",
        "Saving image to bmp",
        "Rotating image",
        "Blurring image",
        "Exit from interpreter"
};

static const char* command_arguments_fetch_list[] = {
        "Usage: FILE_OPEN <filename> <mode>",
        "N/A",
        "N/A",
        "N/A",
        "Usage: IMAGE_ROTATE <angle>. Angle must be dividable by 90!",
        "Usage: IMAGE_BLUR <mode>. Supported modes: from 0 to 4.",
        "N/A"
};

static void print_error(FILE* out, const char* error) {
    fputs(error, out);
    fputs("\n", out);
}

void console_handle_vm_state(struct interpreter_state state, FILE* out_file) {
    if (state.status_code == OK) return; /* nothing to handle */

    /* next something definitely went wrong... */
    print_error(out_file,  "Something went wrong during command execution!");
    fprintf(out_file, "Command: %s\n", command_fetch_list[state.last_executed_command_status.command]);
    print_error(out_file, "-------------------------------------");
    fprintf(out_file, "Status code of broken command: %" PRId32 "\n", state.last_executed_command_status.status_code);

    switch (state.last_executed_command_status.status_code) {
        case ERROR_ARGUMENT_ERROR:
            print_error(out_file, "Function rejected provided arguments!");
            print_error(out_file, command_arguments_fetch_list[state.last_executed_command_status.command]);
            break;
        case ERROR_ARGUMENT_COUNT_ERROR:
            print_error(out_file, "Wrong amount of arguments!");
            print_error(out_file, command_arguments_fetch_list[state.last_executed_command_status.command]);
            break;
        case ERROR_NO_FILE_ERROR:
            print_error(out_file, "No loaded file for interpreter. Use FILE_OPEN first.");
            break;
        case ERROR_NO_IMAGE_ERROR:
            print_error(out_file, "No loaded file for interpreter. Use IMAGE_LOAD_* first");
        default:
            goto command_errors;
    }
    return;

command_errors:
    switch (state.last_executed_command_status.command) {
        case FILE_OPEN:
        case FILE_CLOSE:
            print_error(out_file, strerror(errno));
            break;
        case IMAGE_LOAD_BMP:
            print_error(out_file, read_statuses[state.last_executed_command_status.status_code]);
            break;
        case IMAGE_SAVE_BMP:
            print_error(out_file, write_statuses[state.last_executed_command_status.status_code]);
            break;
        case IMAGE_ROTATE:
        case IMAGE_BLUR:
            print_error(out_file, "No additional information.");
            break;
        case EXIT:
            break;
    }
}
