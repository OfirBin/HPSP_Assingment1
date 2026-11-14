#include "emergency.h"
#include "investigation.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    LaunchOptions options = {{0}, {0}, {0}};
    Mission5Evidence evidence = {0, 0, NULL, 0, NULL, 0, NULL};
    EmergencyRequest request = {0};
    const AuthorizationRecord *authorization = NULL;
    int options_ready = 0;
    int request_ready = 0;
    int request_valid = 0;
    int submitted = 0;
    int result = 1;

    (void)argc;
    (void)argv;
    (void)options;

    options_ready = parse_launch_options(argc, argv, &options);
    if (!options_ready) {
        fputs("Invalid emergency launch options.\n", stderr);
        goto cleanup;
    }
    if (!mission5_evidence_load("data/mission5_emergency.b7", &evidence)) {
        fputs("Could not load Mission 5 evidence.\n", stderr);
        goto cleanup;
    }

    authorization = find_authorization(
        evidence.records,
        evidence.record_count,
        evidence.request_id,
        evidence.unit_id,
        M5_ADMIN_CLEARANCE
    );
    if (authorization == NULL) {
        fputs("No usable emergency authorization was found.\n", stderr);
        goto cleanup;
    }

    request_ready = emergency_request_init(
        &request,
        &options,
        authorization,
        evidence.steps,
        evidence.step_count
    );
    if (!request_ready) {
        fputs("Could not stage the emergency request.\n", stderr);
        goto cleanup;
    }

    if (!mission5_authorize_via_port(&evidence, &request)) {
        fputs("PORT_8080 could not authorize the emergency request.\n", stderr);
        goto cleanup;
    }

    request_valid = validate_emergency_request(&request);
    if (!request_valid) {
        fputs("Emergency request validation failed.\n", stderr);
        goto cleanup;
    }

    submitted = mission5_submit_request(&evidence, &request);
    if (!submitted) {
        fputs("Emergency control rejected the request.\n", stderr);
        goto cleanup;
    }
    result = 0;

cleanup:
    emergency_request_free(&request);
    mission5_evidence_free(&evidence);
    return result;
}
