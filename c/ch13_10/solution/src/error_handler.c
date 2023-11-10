#include <string.h>
#include <errno.h>
#include "error_handler.h"

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


void print_error(const char* error) {
    fputs(error, stderr);
}

bool console_handle_read_error(const enum bmp_read_status read_status) {
    if (read_status) {
        print_error(read_statuses[read_status]);
    }
    return read_status;
}

bool console_handle_write_error(const enum bmp_write_status write_status) {
    if (write_status) {
        print_error(write_statuses[write_status]);
    }
    return write_status;
}

bool console_handle_file_error(const enum file_status file_status, const char* filename) {
    if (file_status) {
        print_error(strcat("Something went wrong during open ", filename));
        print_error(strerror(errno));
    }
    return file_status;
}
