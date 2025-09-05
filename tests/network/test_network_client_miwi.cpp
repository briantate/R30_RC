// TestNetwork.cpp

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <stdbool.h>

#include "network_client_miwi.h"
#include "network_client_miwi_internal.h"
#include "miwi_api.h"

static miwi_context_t miwi_ctx;

TEST_GROUP(NetworkClientMiwi)
{
    // Setup function: called before each test in this group
    TEST_SETUP()
    {

    }

    // Teardown function: called after each test in this group
    TEST_TEARDOWN()
    {
        mock().clear();
    }
};

TEST(NetworkClientMiwi, InitReturnsSuccessIfAllSubmodulesInitialized)
{

    mock().expectOneCall("MiApp_SubscribeDataIndicationCallback")
          .andReturnValue(false);

    mock().expectOneCall("MiApp_ProtocolInit")
          .withParameter("romParams", (void*)NULL)
          .withParameter("ramParams", (void*)NULL)
          .andReturnValue(SUCCESS);

    LONGS_EQUAL(NWK_SUCCESS, miwi_init(&miwi_ctx));
}

