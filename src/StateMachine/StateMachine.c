#include "StateMachine.h"

// src/StateMachine.c
#include "StateMachine.h" // Include the generic state machine interface
#include <stddef.h>       // For NULL

/**
 * @brief Initializes the state machine with its default (initial) state, state table,
 * and the total number of states.
 *
 * @param fsm A pointer to the FSM_t instance to initialize.
 * @param initialState The initial state ID for the state machine.
 * @param stateTable A pointer to the array of State_t definitions (the state table).
 * @param numStates The total number of entries in the stateTable.
 */
void FSM_Init(FSM_t *fsm, int initialState, const State_t *stateTable, int numStates)
{
    // Ensure the FSM pointer and state table pointer are not NULL
    if (fsm != NULL && stateTable != NULL)
    {
        // Set the initial state
        fsm->currentState = initialState;
        // Store the pointer to the state table
        fsm->stateTable = stateTable;
        // Store the number of states for bounds checking
        fsm->numStates = numStates;
    }
}

/**
 * @brief Handles an event for the state machine.
 *
 * This function processes an incoming event by dispatching it to the
 * event handler function of the current state, as defined in the state table.
 * This is the generic part of the state machine, adhering to OCP.
 *
 * @param fsm A pointer to the FSM_t instance.
 * @param event The event to be handled.
 */
void FSM_HandleEvent(FSM_t *fsm, event_t event)
{
    // Ensure the FSM pointer and its state table are valid
    if (fsm == NULL || fsm->stateTable == NULL)
    {
        // Handle error: State machine not initialized or table missing
        return;
    }

    // Check if the current state is within valid bounds defined by numStates
    if (fsm->currentState >= 0 && fsm->currentState < fsm->numStates)
    {
        // Get the event handler function for the current state from the state table
        StateEventHandler_t handler = fsm->stateTable[fsm->currentState].eventHandler;

        // If a handler exists for the current state, call it to process the event
        if (handler != NULL)
        {
            handler(fsm, event);
        }
        else
        {
            // Handle error: No event handler defined for the current state
            // (e.g., log an error, remain in current state)
        }
    }
    else
    {
        // Handle error: Invalid current state (should not happen in a well-defined FSM)
    }
}
