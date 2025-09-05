#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    DISCONNECTED,
    CONNECTED
}net_status_t;

typedef enum
{
    NWK_SUCCESS,
    NWK_FAILURE
}net_return_t;

typedef enum {
    NWK_EVENT_DATA_RECEIVED,
    NWK_EVENT_DISCONNECTED,
    NWK_EVENT_CONNECTED,
    NWK_EVENT_ERROR,
    NWK_EVENT_CUSTOM
} net_event_code_t;

typedef struct {
    net_event_code_t code;
    void* source;  // back-pointer to NetworkDevice or context

    union {
        struct {
            const uint8_t* data;
            size_t len;
        } data;

        struct {
            int error_code;
        } error;

        struct {
            int rssi;
            int lqi;
        } metrics;

        struct {
            const void* custom_data;
        } custom;
    };
} net_event_t;

typedef void (*net_event_callback_t)(const net_event_t* event, void* user_data);

typedef struct{
    net_return_t (*init)(void* context);
    net_return_t (*send)(void* context, const uint8_t* data, size_t len);
    net_return_t (*up)(void* context);
    net_return_t (*down)(void* context);
    net_status_t (*status)(void);
}net_api_t;

typedef struct {
    const char* name;
    const net_api_t* api;
    void* context;  // Implementation-specific state

    net_event_callback_t event_cb;
    void* event_user_data;
} net_device_t;

bool register_device(net_device_t* dev);

// Register event handler
void network_device_set_event_callback(net_device_t* dev,
                                       net_event_callback_t cb,
                                       void* user_data);

// Called by driver
void network_device_emit_event(net_device_t* dev, const net_event_t* event);


#ifdef __cplusplus
}
#endif

#endif // NETWORK_CLIENT_H
