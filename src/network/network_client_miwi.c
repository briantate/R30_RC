/***
 * a generic network interface wrapper for Mi-Wi
 */
#include "network_client_miwi.h"
#include "network_client_miwi_internal.h"
#include "network_client.h"
#include "miwi_api.h"
#include "network_fsm.h"
#include <stdint.h>



static void PacketIndCallback(RECEIVED_MESSAGE *ind){


}

net_return_t miwi_init(void* context) {
  net_return_t ret = NWK_FAILURE;
  miwi_context_t* ctx = (miwi_context_t*)context;  

//   if (MiApp_SubscribeDataIndicationCallback(ReceivedDataIndication)) {
//     DEBUG_OUTPUT(printf("MiWi receive callback registered\r\n"));
//   } else {
//     DEBUG_OUTPUT(printf("error: MiWi receive callback not registered\r\n"));
//     return ret;
//   }

  if(MiApp_ProtocolInit(NULL, NULL) == SUCCESS)
  {
    ctx->initialized = true;
    ret = NWK_SUCCESS;
  }
  else
  {
    ctx->initialized = false;
  }
  return ret;
}

// net_return_t miwi_send(void* context, const uint8_t* data, size_t len);
// net_return_t miwi_up(void* context);
// net_return_t miwi_down(void* context);
// net_status_t miwi_status(void);

// net_return_t network_connect(void) {

//   return NWK_FAILURE;
// }

// net_status_t network_status(void) {
//   return 1;  // Always connected for now
// }

