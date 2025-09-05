// test_network_client.cpp

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "network_client.h"

static net_return_t fake_init(void* context){
    return NWK_SUCCESS;
}

static net_return_t fake_send(void* context, const uint8_t* data, size_t len){
    return NWK_SUCCESS;
}

static net_return_t fake_up(void* context){
    return NWK_SUCCESS;
}

static net_return_t fake_down(void* context){
    return NWK_SUCCESS;
}

static net_status_t fake_status(void){
    return CONNECTED;
}


static const net_api_t dummy_api = {
    .init   = fake_init,
    .send   = fake_send,
    .up     = fake_up,
    .down   = fake_down,
    .status = fake_status
};

TEST_GROUP(network_client)
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

static net_event_t last_event;
static void* last_event_user_data;

static void test_event_handler(const net_event_t* event, void* user_data) {
    memcpy(&last_event, event, sizeof(net_event_t));
    last_event_user_data = user_data;
}

TEST(network_client, EventCallbackIsCalled)
{
    static net_device_t dev = {
        .name = "eventdev",
        .api = &dummy_api,
        .context = NULL
    };

    network_register_client(&dev);

    network_client_set_event_callback(&dev, test_event_handler, (void*)0x1234);

    static uint8_t fake_data[] = { 0x55 };
    net_event_t e = {
        .code = NWK_EVENT_DATA_RECEIVED,
        .source = &dev,
        .data = {
            .data = fake_data,
            .len = sizeof(fake_data)
        }
    };

    network_client_emit_event(&dev, &e);

    CHECK_EQUAL(NWK_EVENT_DATA_RECEIVED, last_event.code);
    CHECK_EQUAL(&dev, last_event.source);
    BYTES_EQUAL(0x55, last_event.data.data[0]);
    CHECK_EQUAL(sizeof(fake_data), last_event.data.len);
    POINTERS_EQUAL((void*)0x1234, last_event_user_data);
}

