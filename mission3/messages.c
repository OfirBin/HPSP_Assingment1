#include "messages.h"

int store_init(MessageStore *store, size_t initial_capacity)
{
    (void)store;
    (void)initial_capacity;
    return 0; /* TODO: Stage A */
}

int message_init(Message *msg, int id, const char *text)
{
    (void)msg;
    (void)id;
    (void)text;
    return 0; /* TODO: Stage A */
}

int store_reserve(Message **messages, size_t *capacity, size_t required)
{
    (void)messages;
    (void)capacity;
    (void)required;
    return 0; /* TODO: Stage B */
}

int store_append(MessageStore *store, int id, const char *text)
{
    (void)store;
    (void)id;
    (void)text;
    return 0; /* TODO: Stage B */
}

void message_free(Message *msg)
{
    (void)msg; /* TODO: Stage C */
}

void store_free(MessageStore *store)
{
    (void)store; /* TODO: Stage C */
}

int store_clone(MessageStore *dest, const MessageStore *source)
{
    (void)dest;
    (void)source;
    return 0; /* TODO: Stage D */
}
