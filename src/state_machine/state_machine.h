/***
 * defines the generic concept of a "State Machine" itself 
 * how it operates, holds its current state, 
 * and references a table of State_t objects
 */

// src/StateMachine.h
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdbool.h>

#include "Event.h" // Include generic event definitions
#include "State.h" // Include state structure and handler types

// Forward declaration of FSM_t structure
// This is needed because State_t's eventHandler points back to FSM_t.
struct FSM_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents the state machine instance.
 *
 * This structure holds the current state of the state machine,
 * a pointer to the state table that defines its behavior, and
 * the total number of states in that table for bounds checking.
 */
typedef struct FSM_t
{
    uint8_t currentState;
    const State_t *stateTable; // Pointer to the array of State_t definitions (the state table)
    uint8_t numStates;
    // Add any other state machine specific context data here, e.g., timers, flags, etc.
} FSM_t; // Renamed from StateMachine_t to FSM_t

/**
 * @brief Initializes the state machine with its default (initial) state, state table,
 * and the total number of states.
 *
 * @param fsm A pointer to the FSM_t instance to initialize.
 * @param initialState The initial state ID for the state machine.
 * @param stateTable A pointer to the array of State_t definitions (the state table).
 * @param numStates The total number of entries in the stateTable.
 * @return true if initialization was successful, false otherwise (e.g., NULL pointers).
 */
bool FSM_Init(FSM_t *fsm, uint8_t initialState, const State_t *stateTable, uint8_t numStates);

/**
 * @brief Handles an event for the state machine.
 *
 * This function processes an incoming event by dispatching it to the
 * event handler function of the current state, as defined in the state table.
 *
 * @param fsm A pointer to the FSM_t instance.
 * @param event The event to be handled.
 * @return true if the event was successfully dispatched to a handler, false on critical error.
 */
bool FSM_HandleEvent(FSM_t *fsm, event_t event);

#ifdef __cplusplus
}
#endif

#endif // STATE_MACHINE_H
