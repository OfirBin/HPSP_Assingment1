#include "decoder.h"

#include <stdio.h>

int decode_message(const char *encoded, int shift, char *decoded, size_t capacity)
{
    /* TODO: validate, reverse the full string, and undo the shift. */
    (void)encoded;
    (void)shift;
    (void)decoded;
    (void)capacity;
    return 0;
}

int calculate_checksum(const char *message)
{
    /* TODO: sum decoded characters modulo 97. */
    (void)message;
    return -1;
}

int process_transmission(const char *line)
{
    /* TODO: parse, validate, decode, verify, and print one record. */
    (void)line;
    puts("[INVALID]");
    return 0;
}

int decode_advanced(const char *encoded, const int *keys, size_t key_count, char *decoded, size_t capacity)
{
    /* Optional TODO. */
    (void)encoded;
    (void)keys;
    (void)key_count;
    (void)decoded;
    (void)capacity;
    return 0;
}

