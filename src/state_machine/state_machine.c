// state_machine.c
#include "state_machine.h"

#include <stddef.h>  // For NULL

/**
 * @brief Helper function to execute a state's entry handler if it exists.
 * @param fsm A pointer to the FSM_t instance.
 * @param state_id The ID of the state whose entry handler should be called.
 */
static void call_entry_handler(FSM_t *fsm, uint8_t state_id) {
  if (fsm->state_table[state_id].entry_handler != NULL) {
    fsm->state_table[state_id].entry_handler(fsm);
  }
}

/**
 * @brief Helper function to execute a state's exit handler if it exists.
 * @param fsm A pointer to the FSM_t instance.
 * @param state_id The ID of the state whose exit handler should be called.
 */
static void call_exit_handler(FSM_t *fsm, uint8_t state_id) {
  if (fsm->state_table[state_id].exit_handler != NULL) {
    fsm->state_table[state_id].exit_handler(fsm);
  }
}

/**
 * @brief Initializes the state machine with its default (initial) state, state
 * table, and the total number of states.
 *
 * @param fsm A pointer to the FSM_t instance to initialize.
 * @param initial_state The initial state ID for the state machine.
 * @param state_table A pointer to the array of State_t definitions (the state
 * table).
 * @param num_states The total number of entries in the state_table.
 * @return true if initialization was successful, false otherwise (e.g., NULL
 * pointers).
 */
bool FSM_Init(FSM_t *fsm, uint8_t initial_state, const State_t *state_table,
              uint8_t num_states) {
  // Ensure the FSM pointer and state table pointer are not NULL
  if (fsm == NULL || state_table == NULL) {
    // Log an error here if a logging mechanism is available
    return false;  // Indicate failure
  }

  // Set the initial state
  fsm->current_state = initial_state;
  // Store the pointer to the state table
  fsm->state_table = state_table;
  // Store the number of states for bounds checking
  fsm->num_states = num_states;

  // Call the entry handler for the initial state
  call_entry_handler(fsm, fsm->current_state);

  return true;  // Indicate success
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
 * @return true if the event was successfully dispatched to a handler, false on
 * critical error.
 */
bool FSM_HandleEvent(FSM_t *fsm, event_t event)  // Changed return type to bool
{
  if (fsm == NULL || fsm->state_table == NULL) {
    // Handle error: State machine not initialized or table missing
    return false;  // Indicate critical failure
  }

  // Store the current state before processing the event
  uint8_t oldState = fsm->current_state;

  // Check if the current state is within valid bounds defined by num_states
  if (fsm->current_state < fsm->num_states) {
    // Get the event handler function for the current state from the state table
    state_event_handler_t handler =
        fsm->state_table[fsm->current_state].event_handler;

    // If a handler exists for the current state, call it to process the event
    if (handler != NULL) {
      handler(fsm, event);  // This call might change fsm->current_state

      // Check if a state transition occurred
      if (fsm->current_state != oldState) {
        // Call exit handler for the old state
        call_exit_handler(fsm, oldState);
        // Call entry handler for the new state
        call_entry_handler(fsm, fsm->current_state);
      }
      return true;  // Event was dispatched successfully
    } else {
      // Handle error: No event handler defined for the current state
      // (e.g., log an error, remain in current state)
      return false;  // Indicate critical failure (missing handler)
    }
  } else {
    // Handle error: Invalid current state (should not happen in a well-defined
    // FSM) (e.g., log an error, potentially reset to a safe state)
    return false;  // Indicate critical failure (invalid state)
  }
}
