// TestNetwork.cpp

#include "CppUTest/TestHarness.h"

#include <stdbool.h>        // For bool type

#include "network_interface.h"



// --- CppUTest Test Group for "Happy Path" StateMachine.c tests ---

TEST_GROUP(NetworkInterface)
{
    // Setup function: called before each test in this group
    TEST_SETUP()
    {
        // This setup assumes successful initialization for most tests.
        // CHECK_TRUE(FSM_Init(&fsm, GENERIC_STATE_A, GenericTestStateTable, GENERIC_NUM_STATES));
    }

    // Teardown function: called after each test in this group
    TEST_TEARDOWN()
    {
        // No specific teardown needed for this simple state machine yet
    }
};

TEST(NetworkInterface, InitReturnsSuccessIfAllSubmodulesInitialized)
{

    LONGS_EQUAL(SUCCESS, network_init());
}


