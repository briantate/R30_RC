/*
 * joystick.h
 *
 * Created: 5/23/2016 5:42:01 PM
 *  Author: brian.tate
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_
	
	#include <stdint.h>
	#include <stdbool.h>
		
	typedef struct JOYSTICK* joystickPtr; //joystick struct
	typedef uint8_t (*ADC_ReadVal_t)(void); //function pointer type //ToDo: change to 16b
	
	//prototypes
	joystickPtr Joystick_Create(ADC_ReadVal_t readHorz, ADC_ReadVal_t readVert);
	void Joystick_Delete(joystickPtr joystickInstance);
	
	void Joystick_Measure(joystickPtr joystickInstance);
	
	uint8_t Joystick_GetHorz( joystickPtr joystickInstance);
	bool Joystick_GetHorzDirection(joystickPtr joystickInstance);
	
	uint8_t Joystick_GetVert( joystickPtr joystickInstance);
	bool Joystick_GetVertDirection(joystickPtr joystickInstance);


#endif /* JOYSTICK_H_ */