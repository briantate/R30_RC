#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UNINITIALIZED,
    DISCONNECTED,
    CONNECTED
}net_state_t;

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

#define ACCESS_POINT (true)
#define CLIENT_NODE (false)

typedef struct{
    bool role;  //true = access point, false = edge node
}net_config_t;

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
    net_return_t (*register_callback)(void* context, net_event_callback_t cb);
    net_return_t (*init)(void* context);
    net_return_t (*send)(void* context, const uint8_t* data, size_t len);
    net_return_t (*up)(void* context);
    net_return_t (*down)(void* context);
    net_state_t  (*status)(void);
    void         (*run_tasks)(void);
}net_api_t;

net_api_t* network_get_interface(void);


#ifdef __cplusplus
}
#endif

#endif // NETWORK_INTERFACE_H
