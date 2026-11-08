#ifndef OPERATION_B7_MISSION6_BITWISE_H
#define OPERATION_B7_MISSION6_BITWISE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t type;
    uint8_t index;
    uint8_t payload;
} PacketFields;

typedef struct {
    uint16_t meeting_id;
    uint16_t base_minute;
    uint16_t location_code;
    uint8_t flags;
    int16_t minute_adjust;
    uint16_t final_minute;
} MeetingRecord;

int parse_base_number(const char *text, unsigned base, uint32_t *out);
int format_binary(uint32_t value, unsigned width, char *out, size_t capacity);

int decode_twos_complement(uint32_t raw, unsigned bits, int32_t *out);
int encode_twos_complement(int32_t value, unsigned bits, uint32_t *out);

int unpack_packet(uint16_t word, PacketFields *out);
int pack_packet(const PacketFields *fields, uint16_t *out);

int decode_channel(const PacketFields *packets, size_t count,
                   uint8_t type, uint8_t key,
                   char *out, size_t capacity);

int parse_meeting_record(const uint8_t raw[10], MeetingRecord *out);
int find_confirmed_meeting(FILE *input, MeetingRecord *out);

int resolve_location(const PacketFields *packets, size_t count,
                     uint8_t key, uint16_t location_code,
                     char *out, size_t capacity);

#endif
