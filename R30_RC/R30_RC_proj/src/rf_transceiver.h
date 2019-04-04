/*
 * rf_transceiver.h
 *
 * Created: 10/23/2018 9:06:49 AM
 *  Author: C41175
 */ 


#ifndef RF_TRANSCEIVER_H_
#define RF_TRANSCEIVER_H_

#include "asf.h"
#include "conf_trx_access.h"


//RF switch pins
#define PIN_RFCTRL1                  PIN_PA09
#define PIN_RFCTRL2                  PIN_PA12

//Antenna diversity control
#define RFCTRL_CFG_ANT_DIV           4  // value 4 => F2CFG = 00 and F1CFG = 01

/************************************************************************
 *   functions                                                                   
 ************************************************************************/
void TransceiverConfig(void);

#endif /* RF_TRANSCEIVER_H_ */