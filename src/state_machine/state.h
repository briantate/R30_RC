/***
 * defines the generic concept of a "State" within this state machine framework
 * It specifies what a state is: a structure containing
 * function pointers for event handling
 */

// src/State.h
#ifndef STATE_H
#define STATE_H

#include "Event.h" // Include event definitions

// Forward declaration of FSM_t to avoid circular dependency
// State functions will need access to the FSM instance.
struct FSM_t;

/**
 * @brief Function pointer type for handling events within a specific state.
 *
 * Each state will have a function of this type responsible for processing
 * incoming events and potentially triggering state transitions.
 *
 * @param fsm A pointer to the FSM_t instance.
 * @param event The event to be handled.
 */
typedef void (*state_event_handler_t)(struct FSM_t *fsm, event_t event);

/**
 * @brief Function pointer type for functions called upon entering a state.
 *
 * This function is executed when the state machine transitions into a state.
 *
 * @param fsm A pointer to the FSM_t instance.
 */
typedef void (*state_entry_handler_t)(struct FSM_t *fsm);

/**
 * @brief Function pointer type for functions called upon exiting a state.
 *
 * This function is executed when the state machine transitions out of a state.
 *
 * @param fsm A pointer to the FSM_t instance.
 */
typedef void (*state_exit_handler_t)(struct FSM_t *fsm);

/**
 * @brief Defines the structure for a single state in the state machine.
 *
 * This structure encapsulates the behavior associated with a particular state,
 * primarily through its event handler function, and now includes entry and exit handlers.
 */
typedef struct
{
    state_event_handler_t event_handler; ///< Pointer to the function that handles events for this state.
    state_entry_handler_t entry_handler; ///< Optional: Function called when entering this state.
    state_exit_handler_t exit_handler;   ///< Optional: Function called when exiting this state.
} State_t;

#endif // STATE_H
