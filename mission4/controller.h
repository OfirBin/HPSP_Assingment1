#ifndef OPERATION_B7_MISSION4_CONTROLLER_H
#define OPERATION_B7_MISSION4_CONTROLLER_H

#include <stddef.h>

#define CONTROL_UNIT_NAME_CAPACITY 32
#define CONTROL_UNIT_ROLE_CAPACITY 48
#define CONTROL_COMMAND_CAPACITY 32

typedef struct {
    int id;
    char name[CONTROL_UNIT_NAME_CAPACITY];
    char role[CONTROL_UNIT_ROLE_CAPACITY];
} ControlUnit;

typedef struct {
    int id;
    char payload[CONTROL_COMMAND_CAPACITY];
} ControlCommand;

typedef struct {
    int id;
    char *text;
} PendingCommand;

int controller_parse_command(const char *input, char *output,
                             size_t capacity);

ControlUnit *controller_find_unit(ControlUnit *units, size_t count,
                                  int index);

int controller_remove_command(ControlCommand *commands, size_t *count,
                              size_t capacity, size_t index);

int controller_schedule(PendingCommand **pending, int id,
                        const char *text);

int controller_execute_pending(PendingCommand **pending, char *output,
                               size_t capacity);

#endif
