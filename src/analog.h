/*
 * analog.h
 *
 * Created: 4/2/2019 5:58:39 AM
 *  Author: Brian Tate
 * 
 * Wrapper for ADC to match Joystick interface
 *
 */ 


#ifndef ANALOG_H_
#define ANALOG_H_

#include <asf.h>

void ADC_init(void);
uint8_t ADC_ReadChannel_06(void);
uint8_t ADC_ReadChannel_07(void);
uint8_t ADC_ReadChannel_08(void);
uint8_t ADC_ReadChannel_09(void);
uint8_t ADC_ReadChannel_10(void);
uint8_t ADC_ReadChannel_11(void);

#endif /* ANALOG_H_ */