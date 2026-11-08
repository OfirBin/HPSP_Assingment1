#ifndef OPERATION_B7_MISSION5_INVESTIGATION_H
#define OPERATION_B7_MISSION5_INVESTIGATION_H

#include "emergency.h"

typedef struct {
    int request_id;
    int unit_id;
    AuthorizationRecord *records;
    size_t record_count;
    int *steps;
    size_t step_count;
    void *internal;
} Mission5Evidence;

int mission5_evidence_load(const char *path, Mission5Evidence *evidence);
void mission5_evidence_free(Mission5Evidence *evidence);
int mission5_authorize_via_port(const Mission5Evidence *evidence,
                                EmergencyRequest *request);
int mission5_reveal_stage(char stage, const char *evidence_path);
int mission5_submit_request(const Mission5Evidence *evidence,
                            const EmergencyRequest *request);

#endif
