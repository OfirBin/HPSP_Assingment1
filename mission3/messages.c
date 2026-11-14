#include "messages.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int store_init(MessageStore *store, size_t initial_capacity)
{
    if (store == NULL)
        return 0;

    if (initial_capacity == 0) {
        store->messages = NULL;
        store->count = 0;
        store->capacity = 0;
        return 1;
    }

    if (store->messages == NULL && store->count == 0 && store->capacity == 0) {
        store->messages = calloc(initial_capacity, sizeof(Message));
        if (store->messages == NULL)
            return 0;

        for (size_t i = 0; i < initial_capacity; i++) {
            store->messages[i].id = 0;
            store->messages[i].text = NULL;
        }

        store->count = 0;
        store->capacity = initial_capacity;
        return 1;
    }

    return 1;
}

int message_init(Message *msg, int id, const char *text)
{
    if (msg == NULL || text == NULL || id <= 0)
        return 0;

    size_t len = strlen(text) + 1;
    msg->text = malloc(len);
    if (msg->text == NULL)
        return 0;

    for (size_t i = 0; i < len; i++) {
        msg->text[i] = text[i];
    }

    msg->id = id;
    return 1;
}

int store_reserve(Message **messages, size_t *capacity, size_t required)
{
    if (messages == NULL || capacity == NULL)
        return 0;

    if (*capacity >= required)
        return 1;

    size_t new_capacity = (*capacity == 0) ? 4 : *capacity;
    while (new_capacity < required) {
        if (new_capacity > SIZE_MAX / 2)
            return 0;
        new_capacity *= 2;
    }

    Message *new_array = realloc(*messages, new_capacity * sizeof(Message));
    if (new_array == NULL)
        return 0;

    for (size_t i = *capacity; i < new_capacity; i++) {
        new_array[i].id = 0;
        new_array[i].text = NULL;
    }

    *messages = new_array;
    *capacity = new_capacity;
    return 1;
}

int store_append(MessageStore *store, int id, const char *text)
{
    if (store == NULL || text == NULL)
        return 0;

    if (store->count >= store->capacity) {
        size_t required = store->count + 1;
        if (!store_reserve(&store->messages, &store->capacity, required))
            return 0;
    }

    Message *msg = &store->messages[store->count];
    if (!message_init(msg, id, text))
        return 0;

    store->count++;
    return 1;
}

void message_free(Message *msg)
{
    if (msg == NULL)
        return;

    free(msg->text);
    msg->text = NULL;
    msg->id = 0;
}

void store_free(MessageStore *store)
{
    if (store == NULL)
        return;

    for (size_t i = 0; i < store->count; i++) {
        message_free(&store->messages[i]);
    }

    free(store->messages);
    store->messages = NULL;
    store->count = 0;
    store->capacity = 0;
}

int store_clone(MessageStore *dest, const MessageStore *source)
{
    if (dest == NULL || source == NULL)
        return 0;

    if (dest->messages != NULL || dest->count != 0 || dest->capacity != 0)
        store_free(dest);

    if (!store_init(dest, source->capacity))
        return 0;

    for (size_t i = 0; i < source->count; i++) {
        if (!store_append(dest, source->messages[i].id, source->messages[i].text)) {
            store_free(dest);
            return 0;
        }
    }

    return 1;
}
