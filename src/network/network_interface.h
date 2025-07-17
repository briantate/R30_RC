#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

typedef enum
{
    DISCONNECTED,
    CONNECTED
}net_state_t;

typedef enum
{
    SUCCESS,
    FAILURE
}net_return_t;

net_return_t network_init(void); // Starts network
net_return_t network_connect(void); // Returns success/fail
net_status_t network_status(void);  // 0 = disconnected, 1 = connected

#endif // NETWORK_INTERFACE_H
