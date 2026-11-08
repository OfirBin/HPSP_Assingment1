#ifndef OPERATION_B7_MISSION2_AGENTS_H
#define OPERATION_B7_MISSION2_AGENTS_H

#include <stddef.h>

typedef struct {
    int id;
    char name[32];
    char role[32];
    int clearance;
    int active;
} Agent;

typedef struct {
    int agent_id;
    int new_clearance;
    int new_active;
} AgentPatch;

typedef struct {
    int agent_id;
    int resource_id;
    int required_clearance;
    int granted;
    int sequence;
    int encoded_symbol;
} AccessEvent;

enum ResourceId {
    BGU_CAMERAS = 7101,
    CONTROL_SYSTEM = 7102,
    AIR_SUPPORT = 7103,
    EVIL_MORTY_PRIVATE_COMMS = 7104,
    SERVER_BACKUP = 7105,
    PORT_8080 = 8080
};

enum AccessClassification {
    ACCESS_INVALID = -1,
    ACCESS_OK = 0,
    ACCESS_DENIED = 1,
    ACCESS_UNEXPECTED_DENIAL = 2,
    ACCESS_UNAUTHORIZED_GRANTED = 3
};

Agent *find_agent(Agent *agents, size_t count, int id);

int copy_agents(Agent *dest, size_t capacity,
                const Agent *source, size_t count);

int update_agent(Agent *agent, AgentPatch patch);

int apply_patch(Agent *agents, size_t count,
                const AgentPatch *patch);

int classify_access(const Agent *agents, size_t count,
                    const AccessEvent *event);

int recover_code(const Agent *agents, size_t agent_count,
                 const AccessEvent *events, size_t event_count,
                 char *code, size_t capacity);

#endif
