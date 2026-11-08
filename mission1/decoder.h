#ifndef DECODER_H
#define DECODER_H

#include <stddef.h>

int decode_message(const char *encoded, int shift,
                   char *decoded, size_t capacity);

int calculate_checksum(const char *message);

int process_transmission(const char *line);

/* Optional challenge; not part of the mandatory grade. */
int decode_advanced(const char *encoded, const int *keys,
                    size_t key_count, char *decoded,
                    size_t capacity);

#endif

