#include "bitwise.h"
#include "investigation.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_PATH "data/mission6_packets.txt"
#define MEETING_PATH "data/mission6_meetings.bin"

static int load_packets(const char *path, uint8_t *key,
                        uint16_t **words_out, PacketFields **packets_out,
                        size_t *count_out)
{
    FILE *file = NULL;
    uint16_t *words = NULL;
    PacketFields *packets = NULL;
    size_t count = 0;
    size_t capacity = 0;
    char line[64];
    uint32_t parsed;
    int success = 0;

    if (path == NULL || key == NULL || words_out == NULL ||
        packets_out == NULL || count_out == NULL) {
        return 0;
    }
    *key = 0;
    *words_out = NULL;
    *packets_out = NULL;
    *count_out = 0;
    file = fopen(path, "r");
    if (file == NULL || fgets(line, sizeof(line), file) == NULL ||
        strncmp(line, "KEY=", 4) != 0) {
        goto cleanup;
    }
    line[strcspn(line, "\r\n")] = '\0';
    if (strlen(line + 4) != 2 ||
        !parse_base_number(line + 4, 16, &parsed) || parsed > UINT8_MAX) {
        goto cleanup;
    }
    *key = (uint8_t)parsed;
    while (fgets(line, sizeof(line), file) != NULL) {
        uint16_t *new_words;
        PacketFields *new_packets;

        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) != 4 || !parse_base_number(line, 16, &parsed) ||
            parsed > UINT16_MAX) {
            goto cleanup;
        }
        if (count == capacity) {
            size_t next = capacity == 0 ? 32 : capacity * 2;

            if (next < capacity || next > SIZE_MAX / sizeof(*words) ||
                next > SIZE_MAX / sizeof(*packets)) {
                goto cleanup;
            }
            new_words = realloc(words, next * sizeof(*words));
            if (new_words == NULL) {
                goto cleanup;
            }
            words = new_words;
            new_packets = realloc(packets, next * sizeof(*packets));
            if (new_packets == NULL) {
                goto cleanup;
            }
            packets = new_packets;
            capacity = next;
        }
        words[count] = (uint16_t)parsed;
        if (!unpack_packet(words[count], &packets[count])) {
            goto cleanup;
        }
        ++count;
    }
    if (ferror(file) || count == 0 || fclose(file) != 0) {
        file = NULL;
        goto cleanup;
    }
    file = NULL;
    *words_out = words;
    *packets_out = packets;
    *count_out = count;
    success = 1;

cleanup:
    if (file != NULL) {
        fclose(file);
    }
    if (!success) {
        free(words);
        free(packets);
        *key = 0;
    }
    return success;
}

int main(void)
{
    uint8_t key = 0;
    uint16_t *words = NULL;
    PacketFields *packets = NULL;
    size_t packet_count = 0;
    FILE *meetings = NULL;
    Mission6Result result = {0};
    int success = 0;

    /* TODO 1: understand and verify how load_packets prepares the evidence. */
    if (!load_packets(PACKET_PATH, &key, &words, &packets, &packet_count)) {
        fputs("Could not load Mission 6 packet evidence.\n", stderr);
        goto cleanup;
    }

    result.numeric_value = words[0];
    result.packet_count = packet_count;
    (void)key;
    (void)result;
    /* TODO 2: format numeric_value as 16 bits. */
    /* TODO 3: decode channels 1, 2, and 3 into result.fragments. */

    meetings = fopen(MEETING_PATH, "rb");
    if (meetings == NULL) {
        fputs("Could not open Mission 6 meeting evidence.\n", stderr);
        goto cleanup;
    }
    /* TODO 4: find the one confirmed meeting, then close the file. */
    result.raw_signed = (uint8_t)result.meeting.minute_adjust;
    /* TODO 2: decode the meeting's raw_signed correction as 8 bits. */
    if (fclose(meetings) != 0) {
        meetings = NULL;
        goto cleanup;
    }
    meetings = NULL;

    /* TODO 5: resolve the meeting's location from channel 0xE. */
    /* TODO 6: submit the fully computed result with mission6_report_success. */
    if (!success) {
        fputs("Mission 6 evidence could not be fully verified.\n", stderr);
        goto cleanup;
    }

cleanup:
    if (meetings != NULL) {
        fclose(meetings);
    }
    free(words);
    free(packets);
    return success ? 0 : 1;
}
