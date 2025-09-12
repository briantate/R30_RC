#include "network_internal.h"
#include "network_management.h" //mi-wi network implementation
// #include "network.h"       // For network_event_callback_t and net_event_t
// #include "network_client.h" // Low-level network driver interface (e.g., net->init(), net->tasks(), net->send(), net->connect())


// --- Internal Network State ---
// static network_event_callback_t app_callback = NULL;
static net_api_t* net = NULL; 

net_return_t network_init(void) {
  net = network_get_interface();

  return net->init(NULL);

    // return NetworkInit();
}

void network_tasks(void) {
    if(net != NULL){
      if(net->status != UNINITIALIZED){
        net->run_tasks();
      }
    }
}