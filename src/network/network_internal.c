#include "network_internal.h"
// #include "network.h"       // For network_event_callback_t and net_event_t
// #include "network_client.h" // Low-level network driver interface (e.g., net->init(), net->tasks(), net->send(), net->connect())

static net_api_t* net = NULL; 

net_return_t network_init(void) {
  net = network_get_interface();

  return net->init(NULL);
}

void network_tasks(void) {
    if(net != NULL){
      if(net->status != UNINITIALIZED){
        net->run_tasks();
      }
    }
}