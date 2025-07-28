/***
 * a generic network interface wrapper
 */
#include "network_interface.h"
#include "network_fsm.h"
#include "miwi_api.h"
#include "debug_interface.h"

net_return_t network_init(void) {
    uint32_t ret = 1;

	return ret; 
}

net_return_t network_connect(void) {
    int ret = 1;

	return ret;
}

net_status_t network_status(void) {
    return 1; // Always connected for now
}
