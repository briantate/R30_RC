// File: network_client_miwi_internal.h
// Exposes the internals of the miwi_client for static allocation of data structures

#ifndef NETWORK_CLIENT_MIWI_INTERNAL_H
#define NETWORK_CLIENT_MIWI_INTERNAL_H

#include "network_client_miwi.h"
#include "network_client.h"

#ifdef __cplusplus
extern "C" {
#endif

// Full definition of the context for use in unit tests
struct miwi_context_t {

    net_status_t status;
    uint8_t   channel;
    uint16_t  pan_id;
    uint8_t   peer_addr[8];
    bool      use_broadcast;

    /* Runtime state */
    volatile bool initialized;
    volatile bool link_up;
    int last_rssi;
    int last_lqi;

    /* Back pointer */
    net_device_t* dev;
};

#ifdef __cplusplus
}
#endif

#endif // NETWORK_CLIENT_MIWI_INTERNAL_H
