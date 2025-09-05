#ifndef NETWORK_CLIENT_MIWI_H
#define NETWORK_CLIENT_MIWI_H

#include <stdint.h>
#include <stdbool.h>

#include <network_client.h>

#ifdef __cplusplus
extern "C" {
#endif

// User must allocate memory for MiWi context
typedef struct miwi_context_t miwi_context_t;

net_return_t miwi_init(void* context);
net_return_t miwi_send(void* context, const uint8_t* data, size_t len);
net_return_t miwi_up(void* context);
net_return_t miwi_down(void* context);
net_status_t miwi_status(void);

#ifdef __cplusplus
}
#endif

#endif // NETWORK_CLIENT_MIWI_H