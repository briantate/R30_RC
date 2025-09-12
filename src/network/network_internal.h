#ifndef NETWORK_INTERNAL_H
#define NETWORK_INTERNAL_H

/**
 * @brief Runs the network's background tasks.
 * This function should be called periodically from the main loop.
 * It handles radio communication, processing incoming data, and sending queued data.
 */
 
#include <stdbool.h>
#include "network_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

net_return_t network_init(void);
void network_tasks(void);

#ifdef __cplusplus
}
#endif

#endif // NETWORK_INTERNAL_H