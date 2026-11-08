#ifndef OPERATION_B7_MISSION5_EMERGENCY_H
#define OPERATION_B7_MISSION5_EMERGENCY_H

#include <stddef.h>

#define M5_DEVICE_CAPACITY 32
#define M5_MODE_CAPACITY 16
#define M5_ACTION_CAPACITY 32

typedef struct {
    char device[M5_DEVICE_CAPACITY];
    char mode[M5_MODE_CAPACITY];
    char action[M5_ACTION_CAPACITY];
} LaunchOptions;

typedef struct {
    int request_id;
    int unit_id;
    int approver_id;
    int clearance;
    int approved;
} AuthorizationRecord;

typedef struct {
    int request_id;
    int unit_id;
    AuthorizationRecord authorization;
    char *device;
    char mode[M5_MODE_CAPACITY];
    char action[M5_ACTION_CAPACITY];
    int *steps;
    size_t step_count;
} EmergencyRequest;

enum {
    STEP_PREPARE_EMERGENCY = 1,
    STEP_REQUEST_AUTHORIZATION = 2,
    STEP_EXECUTE_COMMAND = 3
};

#define M5_ADMIN_CLEARANCE 4

int parse_launch_options(int argc, char *const argv[], LaunchOptions *out);

const AuthorizationRecord *find_authorization(
    const AuthorizationRecord *records, size_t count,
    int request_id, int unit_id, int required_clearance);

int emergency_request_init(EmergencyRequest *dest,
                           const LaunchOptions *options,
                           const AuthorizationRecord *authorization,
                           const int *steps, size_t step_count);

void emergency_request_free(EmergencyRequest *request);

int validate_emergency_request(const EmergencyRequest *request);

#endif
