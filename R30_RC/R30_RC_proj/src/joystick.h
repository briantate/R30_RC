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
		
	//Pointer to joystick object - serves as a handle for joystick
	typedef struct JOYSTICK* joystickPtr; //joystick struct
	
	
	/************************************************************************/
	/*                         prototypes                                   */
	/************************************************************************/
	
	/* (*ADC_ReadVal_t)
	 * function pointer type for ADC readings to inject into joystick object
	 * inputs: void
	 * output: value read from ADC
	 *
	 */
	typedef uint8_t (*ADC_ReadVal_t)(void); //ToDo: change to 16b
	
	/*
	 * Joystick_Create
	 * creates an instance of joystick object
	 * inputs: callback function for horizontal ADC reading
	 *         callback function for vertical ADC reading
	 * output: pointer to joystick object instance - serves as a handle
	 */
	joystickPtr Joystick_Create(ADC_ReadVal_t readHorz, ADC_ReadVal_t readVert);
	
	/*
	 * Joystick_Delete
	 * Destroys a specified Joystick instance
	 * inputs: pointer to joystick object instance
	 * output: void //ToDo: return success/failure
	 * Method not yet implemented - joystick destruction typically not needed
	 */
	void Joystick_Delete(joystickPtr joystickInstance);
	
	/*
	 * Joystick_Measure
	 * forces a measurement of joystick parameters
	 * inputs: pointer to joystick object instance
	 * output: void
	 */
	void Joystick_Measure(joystickPtr joystickInstance);
	
	/*
	 * Joystick_GetHorz
	 * Get horizontal measurement of joystick
	 * inputs: pointer to joystick object instance
	 * output: horizontal measurement of distance from center
	 */
	uint8_t Joystick_GetHorz( joystickPtr joystickInstance);
	
	/*
	 * Joystick_GetHorzDirection
	 * get the left/right position of the joystick
	 * inputs: pointer to joystick object instance
	 * output: boolean direction 
	 *         true  = > center value
	 *         false = < center value
	 */
	bool Joystick_GetHorzDirection(joystickPtr joystickInstance);
	
	/*
	 * Joystick_GetVert
	 * Get Vertical measurement of joystick
	 * inputs: pointer to joystick object instance
	 * output: vertical measurement of distance from center
	 */
	uint8_t Joystick_GetVert( joystickPtr joystickInstance);
	
	/*
	 * Joystick_GetVertDirection
	 * get the up/down position of the joystick
	 * inputs: pointer to joystick object instance
	 * output: boolean direction
	 *         true  = > center value
	 *         false = < center value
	 */
	bool Joystick_GetVertDirection(joystickPtr joystickInstance);


#endif /* JOYSTICK_H_ */