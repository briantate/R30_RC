#ifndef NETWORK_INTERFACE_MIWI_H
#define NETWORK_INTERFACE_MIWI_H

#define EDGE_NODE false 
#define PAN_COORDINATOR true
#define NETWORK_ROLE EDGE_NODE

net_return_t register_callback(void* context, net_event_callback_t cb);
net_return_t init(void* context);
net_return_t send(void* context, const uint8_t* data, size_t len);
net_return_t up(void* context);
net_return_t down(void* context);
net_state_t  status(void);
void         run_tasks(void);


#ifdef __cplusplus
}
#endif

#endif // NETWORK_INTERFACE_MIWI_H