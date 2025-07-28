#ifndef NETWORK_FSM_H
#define NETWORK_FSM_H

typedef enum {
    STATE_INIT_NETWORK,
    STATE_DISCONNECTED,
    STATE_CONNECTED,
    STATE_ERROR,
    STATE_COUNT,
    /* */
    NUM_STATES
} network_state_t;

typedef enum {
    EVENT_NONE,
    EVENT_NETWORK_INIT_DONE,
    EVENT_NETWORK_CONNECTED,
    EVENT_NETWORK_DISCONNECTED,
    EVENT_ERROR,
    /* */
    NUM_EVENTS
} network_event_t;

#endif /* NETWORK_FSM_H */