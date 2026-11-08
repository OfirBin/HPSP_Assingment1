#include "agents.h"

Agent *find_agent(Agent *agents, size_t count, int id)
{
    (void)agents;
    (void)count;
    (void)id;
    return NULL; /* TODO: Stage A */
}

int copy_agents(Agent *dest, size_t capacity, const Agent *source, size_t count)
{
    (void)dest;
    (void)capacity;
    (void)source;
    (void)count;
    return 0; /* TODO: Stage B */
}

int update_agent(Agent *agent, AgentPatch patch)
{
    (void)agent;
    (void)patch;
    return 0; /* TODO: Stage B */
}

int apply_patch(Agent *agents, size_t count, const AgentPatch *patch)
{
    (void)agents;
    (void)count;
    (void)patch;
    return 0; /* TODO: Stage B */
}

int classify_access(const Agent *agents, size_t count, const AccessEvent *event)
{
    (void)agents;
    (void)count;
    (void)event;
    return ACCESS_INVALID; /* TODO: Stage C */
}

int recover_code(const Agent *agents, size_t agent_count, const AccessEvent *events, size_t event_count, char *code, size_t capacity)
{
    (void)agents;
    (void)agent_count;
    (void)events;
    (void)event_count;
    if (code != NULL && capacity > 0)
        code[0] = '\0';
        
    return 0; /* TODO: Stage D */
}
