#include "decoder.h"

#include <stdio.h>
#include <string.h>

int decode_message(const char *encoded, int shift, char *decoded, size_t capacity)
{
    size_t len;
    //checks
    if (encoded == NULL || decoded == NULL || shift < 0 || shift > 25) {
        return 0;
    }   
    len = strlen(encoded);
    if (len + 1 > capacity) {
        return 0;
    }

    //validate
    for (size_t i = 0; i < len; i++) {
        char c = encoded[len-1-i];
        if (!((c >= 'A' && c <= 'Z') || c == '_')) {
            return 0;
        }
    }

    //reverse
    for (size_t i = 0; i < len; i++) {
        char c = encoded[len - 1 - i];
        if (c == '_') {
            decoded[i] = c;
        } else {
            decoded[i] = (char)('A' + (c - 'A' - shift + 26) % 26);
        }
    }

    decoded[len] = '\0';
    return 1;
}

int calculate_checksum(const char *message)
{
    unsigned long sum=0;

    if (message == NULL) {
        return -1;
    }

    for (size_t i = 0; message[i] != '\0'; i++) {
        sum += (unsigned char)message[i];
    }

    return (int)(sum % 97);
}

int process_transmission(const char *line)
{
    char buf[256];
    char decoded[161];
    char *sender;
    char *key_str;
    char *checksum_str;
    char *message;
    size_t n;
    int pipes = 0;
    int key = 0;
    int checksum;

    if (line == NULL) {
        goto invalid;
    }
    n=strlen(line);
    if (n>= sizeof(buf)) {
        goto invalid;
    }

    for (size_t i = 0; i < n; i++) {
        if (line[i] == '|') {
            pipes++;
        }
    }
    if (pipes != 3) {
        goto invalid;
    }

    strcpy(buf, line);
    sender = strtok(buf, "|");
    key_str = strtok(NULL, "|");
    checksum_str = strtok(NULL, "|");
    message = strtok(NULL, "|");
    if (sender == NULL || key_str == NULL || checksum_str == NULL || message == NULL) {
        goto invalid;
    }

    n=strlen(sender);
    if (n < 1 || n > 24) {
        goto invalid;
    }

    // sender a to z
    for (size_t i = 0; i < n; i++) {
        if (!((sender[i] >= 'A' && sender[i] <= 'Z') || sender[i] == '_')) {
            goto invalid;
        }
    }

    for(size_t i = 0; i < strlen(key_str); i++) {
        if (key_str[i] < '0' || key_str[i] > '9') {
            goto invalid;
        }
        key = key * 10 + (key_str[i] - '0');
        if (key > 25) {
            goto invalid;
        }
    }

    //checksum two digits
    if (strlen(checksum_str) != 2 ||
        checksum_str[0] < '0' || checksum_str[0] > '9' ||
        checksum_str[1] < '0' || checksum_str[1] > '9') {
        goto invalid;
    }
    checksum = (checksum_str[0] - '0') * 10 + (checksum_str[1] - '0');
    if (checksum > 96) {
        goto invalid;
    }

    //message 1-160 char
    n = strlen(message);
    if (n < 1 || n > 160) {
        goto invalid;
    }
    for (size_t i = 0; i < n; i++) {
        char c = message[i];
        if (!((c >= 'A' && c <= 'Z') || c == '_')) {
            goto invalid;
        }
    }

    //decode and verify checksum
    if (!decode_message(message, key, decoded, sizeof decoded)) {
        goto invalid;
    }
    if (calculate_checksum(decoded) != checksum) {
        goto invalid;
    }

    printf("[%s] %s\n", sender, decoded);
    return 1;

invalid:
    printf("[INVALID]\n");
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

