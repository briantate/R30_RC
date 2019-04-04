/*
* joystick.c
*
* Created: 5/23/2016 5:41:28 PM
*  Author: brian.tate
*/ 

#include "joystick.h"
#include <string.h>

//macro's for 8b ADC data processing
#define GUARD_BAND 20
#define CENTER_VAL 128
#define FORWARD_VAL CENTER_VAL + GUARD_BAND
#define BACKWARD_VAL CENTER_VAL - GUARD_BAND
#define MAX_VAL 127
#define POSITIVE true
#define NEGATIVE false


  
//private prototypes
void joystick_filter(joystickPtr joystickInstance);
  
struct JOYSTICK
{
	uint8_t horzRaw;           //raw horizontal value
	uint8_t horzFiltered;      //filtered horizontal value
	ADC_ReadVal_t readHorz;    //pointer to func to read horizontal adc value
	bool horzDir;              //right = positive, left = negative
	uint8_t vertRaw;           //raw vertical value
	uint8_t vertFiltered;      //filtered vertical value
	ADC_ReadVal_t readVert;    //pointer to func to read vertical adc value
	bool vertDir;              //up = positive, down = negative
};
  
#define MAX_JOYSTICKS 2 //arbitrary limit -- my game controller has joysticks
static struct JOYSTICK JoystickPool[MAX_JOYSTICKS];
static uint8_t numberOfJoysticks = 0;
  
joystickPtr Joystick_Create(ADC_ReadVal_t readHorz, ADC_ReadVal_t readVert){
	joystickPtr joystick = NULL;
	if(numberOfJoysticks < MAX_JOYSTICKS){
		joystick = &JoystickPool[numberOfJoysticks++];
		//initialize the object
		joystick->horzRaw = CENTER_VAL;
		joystick->vertRaw = CENTER_VAL;
		joystick->readHorz = readHorz;
		joystick->readVert = readVert;
	}
	  
	return joystick;
}
  
void Joystick_Delete(joystickPtr joystickInstance){
	//to do -- shouldn't really ever need to delete a joystick...
}
  
uint8_t Joystick_GetHorz( joystickPtr joystickInstance){
	return joystickInstance->horzFiltered;
}

bool Joystick_GetHorzDirection(joystickPtr joystickInstance){
	return joystickInstance->horzDir;
}

uint8_t Joystick_GetVert( joystickPtr joystickInstance){
	return joystickInstance->vertFiltered;
}

bool Joystick_GetVertDirection(joystickPtr joystickInstance){
	return joystickInstance->vertDir;
}
   
	
void Joystick_Measure(joystickPtr joystickInstance){
	
	//call injected ADC functions	 
	joystickInstance->horzRaw = joystickInstance->readHorz();
	joystickInstance->vertRaw = joystickInstance->readVert();
		 
	joystick_filter(joystickInstance);
		 
}
   




/************************************************************************/
/*			Private Functions                                           */
/************************************************************************/

void joystick_filter(joystickPtr joystickInstance){
	
	//filter vertical
	if (joystickInstance->vertRaw > FORWARD_VAL){
		joystickInstance->vertFiltered = joystickInstance->vertRaw - FORWARD_VAL; 
		joystickInstance->vertDir = POSITIVE;
	}
	else if (joystickInstance->vertRaw < BACKWARD_VAL){
		joystickInstance->vertFiltered = BACKWARD_VAL - joystickInstance->vertRaw;
		joystickInstance->vertDir = NEGATIVE;
	}
	else{
		joystickInstance->vertFiltered = 0;
		joystickInstance->vertDir = POSITIVE;
	}
	
	//make sure it doesn't overflow...
	if(joystickInstance->vertFiltered > MAX_VAL){
		joystickInstance->vertFiltered = MAX_VAL;
	}
	
	joystickInstance->vertFiltered = 2*joystickInstance->vertFiltered; //multiply by 2 since we only get half the resolution
	
	
	//filter horizontal
	if (joystickInstance->horzRaw > FORWARD_VAL){
		joystickInstance->horzFiltered = joystickInstance->horzRaw - FORWARD_VAL;
		joystickInstance->horzDir = POSITIVE;
	}
	else if (joystickInstance->horzRaw < BACKWARD_VAL){
		joystickInstance->horzFiltered = BACKWARD_VAL - joystickInstance->horzRaw;
		joystickInstance->horzDir = NEGATIVE;
	}
	else{
		joystickInstance->horzFiltered = 0;
		joystickInstance->horzDir = POSITIVE;
	}
	  
	//make sure it doesn't overflow...
	if(joystickInstance->horzFiltered > MAX_VAL){
		joystickInstance->horzFiltered = MAX_VAL;
	}
	  
	joystickInstance->horzFiltered = 2*joystickInstance->horzFiltered; //multiply by 2 since we only get half the resolution
}
 