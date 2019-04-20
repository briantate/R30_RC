/*
 * app.h
 *
 * Created: 10/23/2018 1:57:25 PM
 *  Author: C41175
 */ 


#ifndef APP_H_
#define APP_H_

#include "miwi_api.h"

void AppInit(void);
void AppTask(void);
void ReceivedDataIndication (RECEIVED_MESSAGE *ind);


#endif /* APP_H_ */