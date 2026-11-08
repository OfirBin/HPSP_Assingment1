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

    /* TODO 1: call parse_launch_options with argc, argv, and options. */
    if (!options_ready) {
        fputs("Invalid emergency launch options.\n", stderr);
        goto cleanup;
    }
    if (!mission5_evidence_load("data/mission5_emergency.b7", &evidence)) {
        fputs("Could not load Mission 5 evidence.\n", stderr);
        goto cleanup;
    }

    /* TODO 2: find the first matching pending admin authorization. */
    if (authorization == NULL) {
        fputs("No usable emergency authorization was found.\n", stderr);
        goto cleanup;
    }

    /* TODO 3: initialize request from options, authorization, and steps. */
    if (!request_ready) {
        fputs("Could not stage the emergency request.\n", stderr);
        goto cleanup;
    }

    if (!mission5_authorize_via_port(&evidence, &request)) {
        fputs("PORT_8080 could not authorize the emergency request.\n", stderr);
        goto cleanup;
    }

    /* TODO 4: validate the complete request. */
    if (!request_valid) {
        fputs("Emergency request validation failed.\n", stderr);
        goto cleanup;
    }

    /* TODO 5: submit the validated request to the local simulator. */
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
