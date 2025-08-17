/***
 * a generic network interface wrapper for Mi-Wi
 */
#include "network_interface.h"
#include "miwi_api.h"
#include "network_fsm.h"
#include <stdint.h>

net_return_t network_init(void) {

  return 0;
}

net_return_t network_connect(void) {

  return 1;
}

net_status_t network_status(void) {
  return 1;  // Always connected for now
}
