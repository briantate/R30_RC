#ifndef NETWORK_FSM_H
#define NETWORK_FSM_H

#include "network_states.h"

typedef struct
{
    state_t currentState;
    void(*StateFunc)(void);
} fsm_t;

void network_fsm_init(fsm_t *fsm);
void network_fsm_handle_event(fsm_t *fsm, event_t event);



#endif // NETWORK_FSM_H
