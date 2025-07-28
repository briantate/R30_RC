// StateMachine.c
#include "StateMachine.h"
#include <stddef.h>       // For NULL

/**
 * @brief Helper function to execute a state's entry handler if it exists.
 * @param fsm A pointer to the FSM_t instance.
 * @param stateId The ID of the state whose entry handler should be called.
 */
static void callEntryHandler(FSM_t *fsm, uint8_t stateId)
{
    if (fsm->stateTable[stateId].entryHandler != NULL)
    {
        fsm->stateTable[stateId].entryHandler(fsm);
    }
}

/**
 * @brief Helper function to execute a state's exit handler if it exists.
 * @param fsm A pointer to the FSM_t instance.
 * @param stateId The ID of the state whose exit handler should be called.
 */
static void callExitHandler(FSM_t *fsm, uint8_t stateId)
{
    if (fsm->stateTable[stateId].exitHandler != NULL)
    {
        fsm->stateTable[stateId].exitHandler(fsm);
    }
}


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
bool FSM_Init(FSM_t *fsm, uint8_t initialState, const State_t *stateTable, uint8_t numStates)
{
    // Ensure the FSM pointer and state table pointer are not NULL
    if (fsm == NULL || stateTable == NULL)
    {
        // Log an error here if a logging mechanism is available
        return false; // Indicate failure
    }

    // Set the initial state
    fsm->currentState = initialState;
    // Store the pointer to the state table
    fsm->stateTable = stateTable;
    // Store the number of states for bounds checking
    fsm->numStates = numStates;

    // Call the entry handler for the initial state
    callEntryHandler(fsm, fsm->currentState);

    return true; // Indicate success
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
 * @return true if the event was successfully dispatched to a handler, false on critical error.
 */
bool FSM_HandleEvent(FSM_t *fsm, event_t event) // Changed return type to bool
{
    if (fsm == NULL || fsm->stateTable == NULL)
    {
        // Handle error: State machine not initialized or table missing
        return false; // Indicate critical failure
    }

    // Store the current state before processing the event
    uint8_t oldState = fsm->currentState;

    // Check if the current state is within valid bounds defined by numStates
    if (fsm->currentState < fsm->numStates)
    {
        // Get the event handler function for the current state from the state table
        StateEventHandler_t handler = fsm->stateTable[fsm->currentState].eventHandler;

        // If a handler exists for the current state, call it to process the event
        if (handler != NULL)
        {
            handler(fsm, event); // This call might change fsm->currentState

            // Check if a state transition occurred
            if (fsm->currentState != oldState)
            {
                // Call exit handler for the old state
                callExitHandler(fsm, oldState);
                // Call entry handler for the new state
                callEntryHandler(fsm, fsm->currentState);
            }
            return true; // Event was dispatched successfully
        }
        else
        {
            // Handle error: No event handler defined for the current state
            // (e.g., log an error, remain in current state)
            return false; // Indicate critical failure (missing handler)
        }
    }
    else
    {
        // Handle error: Invalid current state (should not happen in a well-defined FSM)
        // (e.g., log an error, potentially reset to a safe state)
        return false; // Indicate critical failure (invalid state)
    }
}
