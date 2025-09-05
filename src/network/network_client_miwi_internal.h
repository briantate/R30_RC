// File: network_client_miwi_internal.h
// Exposes the internals of the miwi_client for static allocation of data structures

#ifndef NETWORK_CLIENT_MIWI_INTERNAL_H
#define NETWORK_CLIENT_MIWI_INTERNAL_H

#include "network_client_miwi.h"

#ifdef __cplusplus
extern "C" {
#endif

// Full definition of the context for use in unit tests
struct miwi_context_t {
    bool initialized;
    int some_internal_state;
    uint8_t tx_buffer[128];
    uint8_t rx_buffer[128];
};

#ifdef __cplusplus
}
#endif

#endif // NETWORK_CLIENT_MIWI_INTERNAL_H
