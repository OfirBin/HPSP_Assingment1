#ifndef OPERATION_B7_MISSION3_MESSAGES_H
#define OPERATION_B7_MISSION3_MESSAGES_H

#include <stddef.h>

typedef struct {
    int id;
    char *text;
} Message;

typedef struct {
    Message *messages;
    size_t count;
    size_t capacity;
} MessageStore;

int store_init(MessageStore *store, size_t initial_capacity);
int message_init(Message *msg, int id, const char *text);

int store_reserve(Message **messages, size_t *capacity,
                  size_t required);
int store_append(MessageStore *store, int id,
                 const char *text);

void message_free(Message *msg);
void store_free(MessageStore *store);

int store_clone(MessageStore *dest,
                const MessageStore *source);

#endif
