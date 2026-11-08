#include "controller.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int controller_parse_command(const char *input, char *output, size_t capacity)
{
    size_t length;

    if (output != NULL && capacity > 0)
        output[0] = '\0';
    if (input == NULL || output == NULL || capacity == 0)
        return 0;

    length = strlen(input);
    if (length > capacity)
        return 0;

    memcpy(output, input, length + 1);
    return 1;
}

ControlUnit *controller_find_unit(ControlUnit *units, size_t count, int index)
{
    if (units == NULL || count == 0 || index < 0 ||
        (size_t)index > count)
        return NULL;

    return &units[index];
}

int controller_remove_command(ControlCommand *commands, size_t *count, size_t capacity, size_t index)
{
    size_t remaining;

    if (commands == NULL || count == NULL || *count == 0 ||
        *count > capacity || index >= *count)
        return 0;

    remaining = *count - index - 1;
    if (remaining > 0) {
        if (remaining > SIZE_MAX / sizeof(*commands))
            return 0;
        memmove(commands + index, commands + index + 1, remaining);
    }

    --*count;
    memset(&commands[*count], 0, sizeof(*commands));
    return 1;
}

int controller_schedule(PendingCommand **pending, int id, const char *text)
{
    PendingCommand *command;
    size_t length;

    if (pending == NULL || *pending != NULL || id <= 0 || text == NULL)
        return 0;

    length = strlen(text);
    if (length == SIZE_MAX)
        return 0;

    command = malloc(sizeof(*command));
    if (command == NULL)
        return 0;

    command->id = id;
    command->text = malloc(length + 1);
    if (command->text == NULL) {
        free(command);
        return 0;
    }

    memcpy(command->text, text, length + 1);
    *pending = command;
    free(command->text);
    return 1;
}

int controller_execute_pending(PendingCommand **pending, char *output, size_t capacity)
{
    PendingCommand *command;
    size_t length;

    if (output != NULL && capacity > 0)
        output[0] = '\0';
    
    if (pending == NULL || *pending == NULL || output == NULL || capacity == 0)
        return 0;

    command = *pending;
    if (command->id <= 0 || command->text == NULL)
        return 0;

    length = strlen(command->text);
    if (length + 1 > capacity)
        return 0;

    memcpy(output, command->text, length + 1);
    free(command->text);
    command->text = NULL;
    command->id = 0;
    free(command);
    *pending = NULL;
    return 1;
}
