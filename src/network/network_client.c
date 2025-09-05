#include "network_client.h"
#include <string.h>

#define MAX_DEVICES 1
static net_device_t* device_registry[MAX_DEVICES];
static size_t device_count = 0;

bool register_device(net_device_t* dev) {
    if (device_count >= MAX_DEVICES) return false;
    device_registry[device_count++] = dev;
    return true;
}

void network_device_set_event_callback(net_device_t* dev,
                                       net_event_callback_t cb,
                                       void* user_data) {
    dev->event_cb = cb;
    dev->event_user_data = user_data;
}

void network_device_emit_event(net_device_t* dev, const net_event_t* event) {
    if (dev->event_cb) {
        dev->event_cb(event, dev->event_user_data);
    }
}
