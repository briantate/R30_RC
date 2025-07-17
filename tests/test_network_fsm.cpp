#include "CppUTest/TestHarness.h"

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
    #include "network_fsm.h"    
}

TEST_GROUP(network_fsm)
{

    fsm_t fsm;

    void setup()
    {
        network_fsm_init(&fsm);
    }

    void teardown()
    {
    }
};

TEST(network_fsm, InitStateIs_InitNetwork)
{
    // Init func already ran in the setup
    CHECK_EQUAL(STATE_INIT_NETWORK, fsm.currentState);
}

