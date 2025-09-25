/*
 * app.h
 *
 * Created: 10/23/2018 1:57:25 PM
 *  Author: Brian Tate
 */ 


#ifndef APP_H_
#define APP_H_

#include "miwi_api.h"
#include <stdbool.h>

typedef enum {
  STATE_INIT,
  STATE_DISCONNECTED,
  STATE_CONNECTED
} app_state_t;

typedef struct{
    bool isTimeForHeartbeat;
    uint32_t counter;
    app_state_t state;
} app_data_t;

void AppInit(void);
void AppTask(void);
void ReceivedDataIndication (RECEIVED_MESSAGE *ind);


#endif /* APP_H_ */