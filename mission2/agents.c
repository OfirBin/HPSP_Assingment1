#include "agents.h"

Agent *find_agent(Agent *agents, size_t count, int id)
{
    if(agents == NULL || count == 0)
        return NULL;
    for (size_t i = 0; i < count; i++)
    {
        if (agents[i].id == id)
            return &agents[i];
    }
    return NULL;
    
    /*(void)agents;
    (void)count;
    (void)id;
    return NULL; TODO: Stage A */
}

int copy_agents(Agent *dest, size_t capacity, const Agent *source, size_t count)
{
    if (count==0)
        return 1;
    if(dest == NULL || source == NULL || capacity < count)
        return 0;
    for (size_t i = 0; i < count; i++)
    {
        const Agent *src = &source[i];
        Agent *dst = &dest[i];
        *dst = *src;
    }
    return 1;
    
    /*(void)dest;
    (void)capacity;
    (void)source;
    (void)count;
    return 0;  TODO: Stage B */
}

int update_agent(Agent *agent, AgentPatch patch)
{
    if(agent == NULL || patch.agent_id != agent->id)
        return 0;
    if(patch.new_clearance < 0 || patch.new_clearance > 5 || (patch.new_active != 0 && patch.new_active != 1))
        return 0;
    Agent copy = *agent;
    copy.clearance = patch.new_clearance;
    copy.active = patch.new_active;
    *agent = copy;
    return 1;

    /*(void)agent;
    (void)patch;
    return 0;  TODO: Stage B */
}

int apply_patch(Agent *agents, size_t count, const AgentPatch *patch)
{
    if(agents == NULL || patch == NULL || count == 0)
        return 0;

    return update_agent(find_agent(agents, count, patch->agent_id), *patch);
    /*(void)agents;
    (void)count;
    (void)patch;
    return 0;  TODO: Stage B */
}

int classify_access(const Agent *agents, size_t count, const AccessEvent *event)
{
     if (event == NULL)                         
        return ACCESS_INVALID;
    if (agents == NULL && count > 0)           
        return ACCESS_INVALID;
    if (event->resource_id != BGU_CAMERAS &&   
        event->resource_id != CONTROL_SYSTEM &&
        event->resource_id != AIR_SUPPORT &&
        event->resource_id != EVIL_MORTY_PRIVATE_COMMS &&
        event->resource_id != SERVER_BACKUP &&
        event->resource_id != PORT_8080)
        return ACCESS_INVALID;
    if (event->required_clearance < 0 || event->required_clearance > 5)
        return ACCESS_INVALID;
    if (event->granted != 0 && event->granted != 1)
        return ACCESS_INVALID;
    if (event->sequence < -1 || event->sequence > 7)             
        return ACCESS_INVALID;
    if (event->encoded_symbol < -1 || event->encoded_symbol > 25) 
        return ACCESS_INVALID;
    if (event->agent_id < 1)
        return ACCESS_INVALID;

    const Agent *agent = NULL;
    for (size_t i = 0; i < count; i++) {
        if (agents[i].id == event->agent_id) {
            agent = &agents[i];
            break;
        }
    }

    if (event->granted == 1){
        if (agent == NULL || agent->active != 1 ||             
            agent->clearance < event->required_clearance)
            return ACCESS_UNAUTHORIZED_GRANTED;
        return ACCESS_OK;
    }
    if (event->granted == 0){
        if (agent == NULL || agent->active != 1 ||             
            agent->clearance < event->required_clearance)
            return ACCESS_DENIED;
        return ACCESS_UNEXPECTED_DENIAL;
    }

    return ACCESS_INVALID;
    /*
    (void)agents;
    (void)count;
    (void)event;
    return ACCESS_INVALID;  TODO: Stage C */
}

int recover_code(const Agent *agents, size_t agent_count, const AccessEvent *events, size_t event_count, char *code, size_t capacity)
{
    if (code == NULL || capacity == 0)
        return 0;
    code[0] = '\0';
    if (capacity < 9 || events == NULL || (agents == NULL && agent_count > 0))
        return 0;

    char result[8];
    int seen[8] = {0};
    int count = 0;

    for (size_t i = 0; i < event_count; i++) {
        if (events[i].agent_id == 67 && classify_access(agents, agent_count, &events[i]) == ACCESS_UNAUTHORIZED_GRANTED) {
            if (events[i].sequence >= 0 && events[i].sequence < 8 && events[i].encoded_symbol >= 0 && events[i].encoded_symbol < 26) {
                int pos = events[i].sequence;
                if (seen[pos])
                    return 0;
                seen[pos] = 1;
                result[pos] = (char)('A' + ((events[i].encoded_symbol - pos + 26) % 26));
                count++;
            }
        }
    }

    if (count != 8)
        return 0;

    for (int pos = 0; pos < 8; pos++)
        code[pos] = result[pos];
    code[8] = '\0';
    return 1;
    
    /*(void)agents;
    (void)agent_count;
    (void)events;
    (void)event_count;
    if (code != NULL && capacity > 0)
        code[0] = '\0';
        
    return 0;  TODO: Stage D */
}
