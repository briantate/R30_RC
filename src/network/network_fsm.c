#include "network_fsm.h"

void network_fsm_init(fsm_t *fsm)
{
    if (fsm != NULL)
    {
        fsm->currentState = STATE_INIT_NETWORK;
    }
}

void network_fsm_handle_event(fsm_t *fsm, event_t event)
{

}
