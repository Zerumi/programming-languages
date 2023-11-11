#ifndef IMAGE_TRANSFORMER_ERROR_HANDLER_H
#define IMAGE_TRANSFORMER_ERROR_HANDLER_H

#include "command.h"
#include "interpreter.h"

#include <errno.h>
#include <stdbool.h>
#include <string.h>

void console_handle_vm_state(struct interpreter_state state, FILE* out_file);

#endif //IMAGE_TRANSFORMER_ERROR_HANDLER_H
