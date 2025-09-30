// TestStateMachine.cpp
// Tests the generic StateMachine.c module.
// It uses a minimal, internal state machine definition for testing purposes.

#include "CppUTest/TestHarness.h"
#include "state_machine.h"   // Include the generic state machine interface
#include "state.h"          // Include generic State_t definition
#include <stdbool.h>        // For bool type

// --- Internal Generic State Machine Definitions for Testing ---

// Define a minimal set of generic states for testing the core StateMachine logic
typedef enum
{
    GENERIC_STATE_A,
    GENERIC_STATE_B,
    GENERIC_STATE_INVALID, // Added for testing invalid state
    GENERIC_NUM_STATES // Total number of generic states
} generic_state_t;

// Define generic event constants for testing purposes.
// These are arbitrary integers that simulate events.
#define GENERIC_EVENT_TRIGGER_TRANSITION 100
#define GENERIC_EVENT_UNHANDLED          200

// --- Mocking for Entry/Exit Handlers ---
// We'll use simple counters to verify if entry/exit functions are called.
static int entryA_calls = 0;
static int exitA_calls = 0;
static int entryB_calls = 0;
static int exitB_calls = 0;

// Reset mocks before each test
static void ResetMockCounters()
{
    entryA_calls = 0;
    exitA_calls = 0;
    entryB_calls = 0;
    exitB_calls = 0;
}

// Forward declarations of generic state handler functions
static void GenericStateA_HandleEvent(FSM_t *fsm, event_t event);
static void GenericStateB_HandleEvent(FSM_t *fsm, event_t event);

// Generic state handler for State A
static void GenericStateA_HandleEvent(FSM_t *fsm, event_t event)
{
    // In this test state, if a specific event occurs, transition to State B
    if (event == GENERIC_EVENT_TRIGGER_TRANSITION)
    {
        fsm->current_state = GENERIC_STATE_B;
    }
    // Other events in State A do not cause a transition for this test
}

// Generic state handler for State B
static void GenericStateB_HandleEvent(FSM_t *fsm, event_t event)
{
    (void)fsm;   // Suppress unused parameter warning
    (void)event; // Suppress unused parameter warning
    // State B has no transitions defined for this minimal test
}

// Entry/Exit Handlers for Generic States
static void GenericStateA_Entry(FSM_t *fsm)
{
    (void)fsm;
    entryA_calls++;
}

static void GenericStateA_Exit(FSM_t *fsm)
{
    (void)fsm;
    exitA_calls++;
}

static void GenericStateB_Entry(FSM_t *fsm)
{
    (void)fsm;
    entryB_calls++;
}

static void GenericStateB_Exit(FSM_t *fsm)
{
    (void)fsm;
    exitB_calls++;
}


// The generic state table used for testing StateMachine.c
const State_t GenericTestStateTable[GENERIC_NUM_STATES] = {
    [GENERIC_STATE_A]       = { .event_handler = GenericStateA_HandleEvent, .entry_handler = GenericStateA_Entry, .exit_handler = GenericStateA_Exit },
    [GENERIC_STATE_B]       = { .event_handler = GenericStateB_HandleEvent, .entry_handler = GenericStateB_Entry, .exit_handler = GenericStateB_Exit },
    [GENERIC_STATE_INVALID] = { .event_handler = NULL, .entry_handler = NULL, .exit_handler = NULL } // Explicitly set to NULL for testing
};

// --- CppUTest Test Group for "Happy Path" StateMachine.c tests ---

TEST_GROUP(GenericStateMachine)
{
    FSM_t fsm; // Declare a state machine instance for each test

    // Setup function: called before each test in this group
    TEST_SETUP()
    {
        ResetMockCounters(); // Reset counters for each test
        // Initialize the generic state machine with its initial state and the generic test state table.
        // This setup assumes successful initialization for most tests.
        CHECK_TRUE(FSM_Init(&fsm, GENERIC_STATE_A, GenericTestStateTable, GENERIC_NUM_STATES));
    }

    // Teardown function: called after each test in this group
    TEST_TEARDOWN()
    {
        // No specific teardown needed for this simple state machine yet
    }
};

// Test: Verify that the generic state machine initializes to GENERIC_STATE_A
TEST(GenericStateMachine, InitialStateIsGenericStateA)
{
    LONGS_EQUAL(GENERIC_STATE_A, fsm.current_state);
    // Verify entry handler for initial state was called once by TEST_SETUP
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
    LONGS_EQUAL(0, entryB_calls);
    LONGS_EQUAL(0, exitB_calls);
}

// // Test: Verify that the generic state machine transitions from A to B on a specific event
TEST(GenericStateMachine, TransitionAToBOnEvent)
{
    LONGS_EQUAL(GENERIC_STATE_A, fsm.current_state); // Pre-condition check

    // Send the event that triggers the transition from A to B
    CHECK_TRUE(FSM_HandleEvent(&fsm, GENERIC_EVENT_TRIGGER_TRANSITION));

    LONGS_EQUAL(GENERIC_STATE_B, fsm.current_state); // Assertion
    // Verify exit A and entry B were called exactly once during the transition
    LONGS_EQUAL(1, exitA_calls);
    LONGS_EQUAL(1, entryB_calls);
    LONGS_EQUAL(1, entryA_calls); // Should not have changed
    LONGS_EQUAL(0, exitB_calls);  // Should not have changed
}

// // Test: Verify that an unhandled event does not change state
TEST(GenericStateMachine, UnhandledEventDoesNotChangeState)
{
    LONGS_EQUAL(GENERIC_STATE_A, fsm.current_state); // Pre-condition check

    // Send an event that is not handled in GENERIC_STATE_A
    CHECK_TRUE(FSM_HandleEvent(&fsm, GENERIC_EVENT_UNHANDLED));

    // Assert that the state remains GENERIC_STATE_A
    LONGS_EQUAL(GENERIC_STATE_A, fsm.current_state);
    // No state change, so entry/exit handlers should not be called again
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
    LONGS_EQUAL(0, entryB_calls);
    LONGS_EQUAL(0, exitB_calls);
}

// // Test: Verify that an event in a state with no transitions defined does not change state
TEST(GenericStateMachine, NoTransitionFromGenericStateB)
{
    // First, transition to GENERIC_STATE_B
    CHECK_TRUE(FSM_HandleEvent(&fsm, GENERIC_EVENT_TRIGGER_TRANSITION));
    LONGS_EQUAL(GENERIC_STATE_B, fsm.current_state); // Verify pre-condition
    LONGS_EQUAL(1, entryB_calls);
    
    // Send any event to State B
    CHECK_TRUE(FSM_HandleEvent(&fsm, GENERIC_EVENT_UNHANDLED));

    // Assert that the state remains GENERIC_STATE_B
    LONGS_EQUAL(GENERIC_STATE_B, fsm.current_state);
    // No state change, so entry/exit handlers should not be called again
    LONGS_EQUAL(1, entryB_calls);
    LONGS_EQUAL(0, exitB_calls);
}

// // --- Tests for FSM_HandleEvent error handling ---

TEST(GenericStateMachine, FSM_HandleEvent_ReturnsFalseOnNullFSM)
{
    // The FSM is already initialized by TEST_SETUP
    // Test with a NULL FSM pointer, should fail before touching the initialized FSM
    bool result = FSM_HandleEvent(NULL, GENERIC_EVENT_TRIGGER_TRANSITION);
    CHECK_FALSE(result);
    // No change to entry/exit counts beyond initial setup
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
}

TEST(GenericStateMachine, FSM_HandleEvent_ReturnsFalseOnNullStateTable)
{
    // FSM is initialized by TEST_SETUP. Now, manually corrupt its state_table pointer
    fsm.state_table = NULL;

    // Attempt to handle an event with a NULL state_table
    bool result = FSM_HandleEvent(&fsm, GENERIC_EVENT_TRIGGER_TRANSITION);
    CHECK_FALSE(result);
    // No change to entry/exit counts beyond initial setup
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
}

TEST(GenericStateMachine, FSM_HandleEvent_ReturnsFalseOnInvalidCurrentState)
{
    // FSM is initialized by TEST_SETUP. Now, manually set an invalid current state
    fsm.current_state = GENERIC_NUM_STATES + 1; // An out-of-bounds state

    // Attempt to handle an event from an invalid state
    bool result = FSM_HandleEvent(&fsm, GENERIC_EVENT_TRIGGER_TRANSITION);
    CHECK_FALSE(result);
    // No change to entry/exit counts beyond initial setup
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);


    // FSM is initialized by TEST_SETUP. Now, manually set an invalid current state
    fsm.current_state = -1; // An out-of-bounds state

    // Attempt to handle an event from an invalid state
    result = FSM_HandleEvent(&fsm, GENERIC_EVENT_TRIGGER_TRANSITION);
    CHECK_FALSE(result);
    // No change to entry/exit counts beyond initial setup
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);


}

TEST(GenericStateMachine, FSM_HandleEvent_ReturnsFalseOnNullStateHandler)
{
    // FSM is initialized by TEST_SETUP. Now, manually set a state with a NULL handler
    fsm.current_state = GENERIC_STATE_INVALID;

    // Attempt to handle an event in a state with a NULL handler
    bool result = FSM_HandleEvent(&fsm, GENERIC_EVENT_UNHANDLED);
    CHECK_FALSE(result);
    // No change to entry/exit counts beyond initial setup
    LONGS_EQUAL(1, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
}

// --- New Test Group for "Sad Path" FSM_Init tests (no TEST_SETUP) ---

TEST_GROUP(GenericStateMachine_InitFailures)
{
    // No TEST_SETUP or TEST_TEARDOWN, as these tests are designed to
    // check initialization failure and should not be affected by prior state.
};

TEST(GenericStateMachine_InitFailures, FSM_Init_ReturnsFalseOnNullFSM)
{
    ResetMockCounters(); // Ensure mocks are clean for this test
    // Attempt to initialize with a NULL FSM pointer
    bool result = FSM_Init(NULL, GENERIC_STATE_A, GenericTestStateTable, GENERIC_NUM_STATES);
    CHECK_FALSE(result); // Assert that initialization failed
    // No entry/exit handlers should have been called
    LONGS_EQUAL(0, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
}

TEST(GenericStateMachine_InitFailures, FSM_Init_ReturnsFalseOnNullStateTable)
{
    ResetMockCounters();
    FSM_t local_fsm; // A local FSM instance for this specific test
    // Attempt to initialize with a NULL state_table pointer
    bool result = FSM_Init(&local_fsm, GENERIC_STATE_A, NULL, GENERIC_NUM_STATES);
    CHECK_FALSE(result); // Assert that initialization failed
    // No entry/exit handlers should have been called
    LONGS_EQUAL(0, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
}

TEST(GenericStateMachine_InitFailures, FSM_Init_ReturnsFalseOnNullFSMAndNullStateTable)
{
    ResetMockCounters();
    // Attempt to initialize with both FSM and state_table pointers as NULL
    bool result = FSM_Init(NULL, GENERIC_STATE_A, NULL, GENERIC_NUM_STATES);
    CHECK_FALSE(result); // Assert that initialization failed
    // No entry/exit handlers should have been called
    LONGS_EQUAL(0, entryA_calls);
    LONGS_EQUAL(0, exitA_calls);
}
