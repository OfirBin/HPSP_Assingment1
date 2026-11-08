#ifndef OPERATION_B7_MISSION6_INVESTIGATION_H
#define OPERATION_B7_MISSION6_INVESTIGATION_H

#include "bitwise.h"

#define M6_BINARY_CAPACITY 33
#define M6_FRAGMENT_CAPACITY 16
#define M6_LOCATION_CAPACITY 16

typedef struct {
    uint32_t numeric_value;
    char binary[M6_BINARY_CAPACITY];
    uint8_t raw_signed;
    int32_t signed_value;
    size_t packet_count;
    char fragments[3][M6_FRAGMENT_CAPACITY];
    MeetingRecord meeting;
    char location[M6_LOCATION_CAPACITY];
} Mission6Result;

int mission6_report_success(const char *packet_path,
                            const char *meeting_path,
                            const Mission6Result *result);

#endif
