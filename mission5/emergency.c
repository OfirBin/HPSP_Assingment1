#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "emergency.h"


static int my_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        i++;
    }
    return (unsigned char)s1[i] - (unsigned char)s2[i];
}

static char *my_strcpy(char *dest, const char *src)
{
    size_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}


static void clear_launch_options(LaunchOptions *out)
{
    if (out != NULL) {
        out->device[0] = '\0';
        out->mode[0] = '\0';
        out->action[0] = '\0';
    }
}

int parse_launch_options(int argc, char *const argv[], LaunchOptions *out)
{
    if (out == NULL) {
        return 0;
    }
    clear_launch_options(out);
    if (argv == NULL || argc != 7) {
        return 0;
    }

    int seen_device = 0;
    int seen_mode = 0;
    int seen_action = 0;

    for (int i = 1; i < argc; i += 2) {
        if (argv[i] == NULL || argv[i + 1] == NULL) {
            clear_launch_options(out);
            return 0;
        }

        if (my_strcmp(argv[i], "--device") == 0) {
            if (seen_device || my_strcmp(argv[i + 1], "BEERSHEVANATOR") != 0) {
                clear_launch_options(out);
                return 0;
            }
            my_strcpy(out->device, argv[i + 1]);
            seen_device = 1;
        } else if (my_strcmp(argv[i], "--mode") == 0) {
            if (seen_mode || my_strcmp(argv[i + 1], "emergency") != 0) {
                clear_launch_options(out);
                return 0;
            }
            my_strcpy(out->mode, argv[i + 1]);
            seen_mode = 1;
        } else if (my_strcmp(argv[i], "--action") == 0) {
            if (seen_action || my_strcmp(argv[i + 1], "self-destruct") != 0) {
                clear_launch_options(out);
                return 0;
            }
            my_strcpy(out->action, argv[i + 1]);
            seen_action = 1;
        } else {
            clear_launch_options(out);
            return 0;
        }
    }

    if (!seen_device || !seen_mode || !seen_action) {
        clear_launch_options(out);
        return 0;
    }

    return 1;
}
static int check_record_is_valid(const AuthorizationRecord *record)
{
    if (record->unit_id < 1 || record->request_id < 1 || record->approver_id < 1 ||
        record->clearance < 0 || record->clearance > 5 ||
        record->approved < 0 || record->approved > 1) {
        return 0;
    }
    return 1;
}

static int that_is_the_record(const AuthorizationRecord *record, int request_id, int unit_id, int required_clearance)
{
    if (request_id == record->request_id &&
        unit_id == record->unit_id &&
        record->approved == 0 &&
        required_clearance <= record->clearance) {
        return 1;
    }
    return 0;
}

const AuthorizationRecord *find_authorization(const AuthorizationRecord *records, size_t count,
                                              int request_id, int unit_id, int required_clearance)
{
    if (records == NULL || count == 0 || unit_id < 1 || request_id < 1 ||
        required_clearance < 0 || required_clearance > 5) {
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        if (check_record_is_valid(&records[i]) == 1 &&
            that_is_the_record(&records[i], request_id, unit_id, required_clearance) == 1) {
            return &records[i];
        }
    }
    return NULL;
}



static void reset_emergency_request(EmergencyRequest *request)
{
    if (request == NULL) {
        return;
    }

    request->device = NULL;
    request->steps = NULL;

    request->request_id = 0;
    request->unit_id = 0;
    request->step_count = 0;

    request->mode[0] = '\0';
    request->action[0] = '\0';

    request->authorization.request_id = 0;
    request->authorization.unit_id = 0;
    request->authorization.approver_id = 0;
    request->authorization.clearance = 0;
    request->authorization.approved = 0;
}

static int string_is_valid(const char *str, size_t capacity)
{
    for (size_t i = 0; i < capacity; i++) {
        if (str[i] == '\0') {
            return 1;
        }
    }
    return 0;
}

int emergency_request_init(EmergencyRequest *dest,
                           const LaunchOptions *options,
                           const AuthorizationRecord *authorization,
                           const int *steps, size_t step_count)
{
    if (dest == NULL || dest->steps != NULL || dest->device != NULL) {
        return 0;
    }

    reset_emergency_request(dest);

    if (options == NULL || authorization == NULL || steps == NULL ||
        step_count < 1 || step_count > 8 || options->device[0] == '\0') {
        return 0;
    }

    if (string_is_valid(options->device, M5_DEVICE_CAPACITY) == 0 ||
        string_is_valid(options->mode, M5_MODE_CAPACITY) == 0 ||
        string_is_valid(options->action, M5_ACTION_CAPACITY) == 0) {
        return 0;
    }

    if (check_record_is_valid(authorization) == 0) {
        return 0;
    }

    size_t device_len = strlen(options->device);
    char *new_device = malloc(device_len + 1);
    if (new_device == NULL) {
        return 0;
    }
    my_strcpy(new_device, options->device);

    if (step_count > SIZE_MAX / sizeof(int)) {
        free(new_device);
        return 0;
    }

    int *new_steps = malloc(step_count * sizeof(int));
    if (new_steps == NULL) {
        free(new_device);
        return 0;
    }

    for (size_t i = 0; i < step_count; i++) {
        new_steps[i] = steps[i];
    }

    dest->request_id = authorization->request_id;
    dest->unit_id = authorization->unit_id;
    dest->authorization = *authorization;
    dest->device = new_device;
    my_strcpy(dest->mode, options->mode);
    my_strcpy(dest->action, options->action);
    dest->steps = new_steps;
    dest->step_count = step_count;

    return 1;
}

void emergency_request_free(EmergencyRequest *request)
{
    if(request == NULL)
        return;

    free(request->device);
    free(request->steps);
    reset_emergency_request(request);
}

int validate_emergency_request(const EmergencyRequest *request)
{
    if (request == NULL || request->device == NULL || request->steps == NULL) {
        return 0;
    }

    if (string_is_valid(request->mode, M5_MODE_CAPACITY) == 0 ||
        string_is_valid(request->action, M5_ACTION_CAPACITY) == 0) {
        return 0;
    }

    if (my_strcmp(request->device, "BEERSHEVANATOR") != 0 ||
        my_strcmp(request->mode, "emergency") != 0 ||
        my_strcmp(request->action, "self-destruct") != 0) {
        return 0;
    }

    if (request->request_id <= 0 || request->unit_id <= 0 ||
        request->authorization.approver_id <= 0) {
        return 0;
    }

    if (request->authorization.request_id != request->request_id ||
        request->authorization.unit_id != request->unit_id) {
        return 0;
    }

    if (request->authorization.approved != 1 ||
        request->authorization.clearance < M5_ADMIN_CLEARANCE ||
        request->authorization.clearance > 5) {
        return 0;
    }

    if (request->step_count != 3) {
        return 0;
    }

    if (request->steps[0] != STEP_PREPARE_EMERGENCY ||
        request->steps[1] != STEP_REQUEST_AUTHORIZATION ||
        request->steps[2] != STEP_EXECUTE_COMMAND) {
        return 0;
    }

    return 1;
}
