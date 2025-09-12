#include "network_interface.h"
#include "network_interface_miwi.h"

static net_api_t net_interface = {
    .register_callback = register_callback,
    .init = init,
    .send = send,
    .up = up,
    .down = down,
    .status = status,
    .run_tasks = run_tasks
};

net_api_t* network_get_interface(void){
    return &net_interface;
}