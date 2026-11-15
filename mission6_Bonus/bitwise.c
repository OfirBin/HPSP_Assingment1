#include "bitwise.h"


int parse_base_number(const char *text, unsigned base, uint32_t *out)
{
    uint32_t value = 0;
    size_t i = 0;
    size_t digits = 0;

    if (text == NULL || out == NULL)
        return 0;
    if (base != 2 && base != 10 && base != 16)
        return 0;

    /* Optional prefix: 0b/0B for base 2, 0x/0X for base 16. */
    if (text[0] == '0') {
        if (base == 2 && (text[1] == 'b' || text[1] == 'B'))
            i = 2;
        else if (base == 16 && (text[1] == 'x' || text[1] == 'X'))
            i = 2;
    }

    for (; text[i] != '\0'; i++) {
        char c = text[i];
        unsigned digit;

        if (c >= '0' && c <= '9')
            digit = (unsigned)(c - '0');
        else if (c >= 'a' && c <= 'f')
            digit = (unsigned)(c - 'a') + 10u;
        else if (c >= 'A' && c <= 'F')
            digit = (unsigned)(c - 'A') + 10u;
        else
            return 0;

        if (digit >= base)
            return 0;

        /* Overflow check before multiplying: value * base + digit <= UINT32_MAX */
        if (value > (UINT32_MAX - digit) / base)
            return 0;

        value = value * base + digit;
        digits++;
    }

    if (digits == 0)
        return 0;

    *out = value;
    return 1;
}

int format_binary(uint32_t value, unsigned width, char *out, size_t capacity)
{
    if (out == NULL || capacity == 0)
        return 0;
    out[0] = '\0';

    if (width < 1 || width > 32)
        return 0;
    if (capacity < (size_t)width + 1)
        return 0;
    if (width < 32 && (value >> width) != 0)
        return 0;

    for (unsigned i = 0; i < width; i++) {
        unsigned shift = width - 1 - i;
        out[i] = ((value >> shift) & 1u) ? '1' : '0';
    }
    out[width] = '\0';
    return 1;
}

int decode_twos_complement(uint32_t raw, unsigned bits, int32_t *out)
{
    uint32_t sign_bit;

    if (out == NULL || bits < 2 || bits > 16)
        return 0;
    if ((raw >> bits) != 0)
        return 0;

    sign_bit = 1u << (bits - 1);
    if (raw & sign_bit)
        *out = (int32_t)raw - (int32_t)(1u << bits);
    else
        *out = (int32_t)raw;
    return 1;
}

int encode_twos_complement(int32_t value, unsigned bits, uint32_t *out)
{
    int32_t min;
    int32_t max;

    if (out == NULL || bits < 2 || bits > 16)
        return 0;

    min = -(int32_t)(1u << (bits - 1));
    max = (int32_t)(1u << (bits - 1)) - 1;
    if (value < min || value > max)
        return 0;

    if (value >= 0)
        *out = (uint32_t)value;
    else
        *out = (uint32_t)(value + (int32_t)(1u << bits));
    return 1;
}


int unpack_packet(uint16_t word, PacketFields *out)
{
    if (out == NULL)
        return 0;

    out->type = (uint8_t)((word >> 12) & 0x0Fu);
    out->index = (uint8_t)((word >> 8) & 0x0Fu);
    out->payload = (uint8_t)(word & 0xFFu);
    return 1;
}

int pack_packet(const PacketFields *fields, uint16_t *out)
{
    if (fields == NULL || out == NULL)
        return 0;
    if (fields->type > 15 || fields->index > 15)
        return 0;

    *out = (uint16_t)(((unsigned)fields->type << 12) |
                      ((unsigned)fields->index << 8) |
                      (unsigned)fields->payload);
    return 1;
}


int decode_channel(const PacketFields *packets, size_t count,
                   uint8_t type, uint8_t key,
                   char *out, size_t capacity)
{
    char buffer[16];
    int seen[16] = {0};
    size_t last = 0;
    int found = 0;

    if (out == NULL || capacity == 0)
        return 0;
    out[0] = '\0';
    if (packets == NULL && count > 0)
        return 0;

    /* Place every packet of this channel at its index. */
    for (size_t i = 0; i < count; i++) {
        size_t index;

        if (packets[i].type != type)
            continue;

        index = packets[i].index;
        if (index > 15 || seen[index])
            return 0;

        seen[index] = 1;
        buffer[index] = (char)(packets[i].payload ^ key);
        if (!found || index > last)
            last = index;
        found = 1;
    }

    if (!found || last == 0)
        return 0;

    /* No holes between 0 and last. */
    for (size_t i = 0; i <= last; i++) {
        if (!seen[i])
            return 0;
    }

    /* Exactly one terminator, at the last index. */
    if (buffer[last] != '\0')
        return 0;
    for (size_t i = 0; i < last; i++) {
        unsigned char c = (unsigned char)buffer[i];
        if (c < 0x20 || c > 0x7E)
            return 0;
    }

    if (capacity < last + 1)
        return 0;

    for (size_t i = 0; i <= last; i++)
        out[i] = buffer[i];
    return 1;
}

int parse_meeting_record(const uint8_t raw[10], MeetingRecord *out)
{
    static const MeetingRecord empty = {0};
    MeetingRecord record;
    uint8_t checksum = 0;
    int32_t adjust;
    int32_t final_minute;

    if (out != NULL)
        *out = empty;
    if (raw == NULL || out == NULL)
        return 0;

    record.meeting_id = (uint16_t)(raw[0] | (raw[1] << 8));
    record.base_minute = (uint16_t)(raw[2] | (raw[3] << 8));
    record.location_code = (uint16_t)(raw[4] | (raw[5] << 8));
    record.flags = raw[6];

    if (record.meeting_id == 0 || record.location_code == 0)
        return 0;
    if (record.base_minute > 1439)
        return 0;
    if ((record.flags & ~0x03u) != 0 || record.flags == 0x03)
        return 0;
    if (raw[7] != 0)
        return 0;

    for (int i = 0; i < 9; i++)
        checksum ^= raw[i];
    if (checksum != raw[9])
        return 0;

    if (!decode_twos_complement(raw[8], 8, &adjust))
        return 0;

    final_minute = (int32_t)record.base_minute + adjust;
    if (final_minute < 0 || final_minute > 1439)
        return 0;

    record.minute_adjust = (int16_t)adjust;
    record.final_minute = (uint16_t)final_minute;
    *out = record;
    return 1;
}

int find_confirmed_meeting(FILE *input, MeetingRecord *out)
{
    static const MeetingRecord empty = {0};
    MeetingRecord match = {0};
    MeetingRecord record;
    uint8_t raw[10];
    size_t matches = 0;
    size_t n;

    if (out != NULL)
        *out = empty;
    if (input == NULL || out == NULL)
        return 0;

    while ((n = fread(raw, 1, sizeof raw, input)) == sizeof raw) {
        if (!parse_meeting_record(raw, &record))
            continue;
        if ((record.flags & 0x01u) && !(record.flags & 0x02u)) {
            matches++;
            match = record;
        }
    }

    /* n != 10 here: 0 at a clean end, 1-9 means a truncated record. */
    if (n != 0 || ferror(input))
        return 0;
    if (matches != 1)
        return 0;

    *out = match;
    return 1;
}


int resolve_location(const PacketFields *packets, size_t count,
                     uint8_t key, uint16_t location_code,
                     char *out, size_t capacity)
{
    char message[16];
    char hex[5];
    uint32_t code;
    const char *place;
    size_t place_length = 0;

    if (out == NULL || capacity == 0)
        return 0;
    out[0] = '\0';

    if (!decode_channel(packets, count, 0x0E, key, message, sizeof message))
        return 0;

    /* Expected format: LOC:HHHH=PLACE */
    if (message[0] != 'L' || message[1] != 'O' || message[2] != 'C' ||
        message[3] != ':')
        return 0;

    for (int i = 0; i < 4; i++) {
        char c = message[4 + i];
        int is_hex = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
                     (c >= 'A' && c <= 'F');
        if (!is_hex)
            return 0;
        hex[i] = c;
    }
    hex[4] = '\0';

    if (message[8] != '=')
        return 0;

    if (!parse_base_number(hex, 16, &code) || code != location_code)
        return 0;

    place = &message[9];
    while (place[place_length] != '\0') {
        char c = place[place_length];
        if (c < 'A' || c > 'Z')
            return 0;
        place_length++;
    }
    if (place_length == 0)
        return 0;
    if (capacity < place_length + 1)
        return 0;

    for (size_t i = 0; i <= place_length; i++)
        out[i] = place[i];
    return 1;
}