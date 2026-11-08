#include "emergency.h"

int parse_launch_options(int argc, char *const argv[], LaunchOptions *out)
{
    (void)argc;
    (void)argv;
    (void)out;
    /* TODO Stage A: validate and copy all three option/value pairs. */
    return 0;
}

const AuthorizationRecord *find_authorization(const AuthorizationRecord *records, size_t count,
                                              int request_id, int unit_id, int required_clearance)
{
    (void)records;
    (void)count;
    (void)request_id;
    (void)unit_id;
    (void)required_clearance;
    /* TODO Stage B: return the first usable record in the original array. */
    return NULL;
}

int emergency_request_init(EmergencyRequest *dest,
                           const LaunchOptions *options,
                           const AuthorizationRecord *authorization,
                           const int *steps, size_t step_count)
{
    (void)dest;
    (void)options;
    (void)authorization;
    (void)steps;
    (void)step_count;
    /* TODO Stage C: create independent owned copies with rollback. */
    return 0;
}

void emergency_request_free(EmergencyRequest *request)
{
    (void)request;
    /* TODO Stage C: release owned allocations and reset every field. */
}

int validate_emergency_request(const EmergencyRequest *request)
{
    (void)request;
    /* TODO Stage D: validate the complete request without changing it. */
    return 0;
}
