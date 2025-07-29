/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include "asf.h"
#include "custom_board.h"
#include "debug_interface.h"
#include "rf_transceiver.h"
#include "network_management.h"
#include "miwi_api.h"
#include "app.h"

void ReadMacAddress(void);

volatile uint32_t counter = 0;

int main (void)
{
	system_init();
	CustomBoardInit();
	delay_init(); //used to to initialize radio interface
	SYS_TimerInit(); //used as a symbol timer by the MiWi stack
	
	AppInit();

	TransceiverConfig(); //initialize pins to the radio

	ReadMacAddress();
	DEBUG_OUTPUT(printf("address: "));
	for(uint8_t i=0; i<MY_ADDRESS_LENGTH; i++)
	{
		DEBUG_OUTPUT(printf("%u",myLongAddress[MY_ADDRESS_LENGTH - 1 - i]));
	}

	DEBUG_OUTPUT(printf("\r\n"));
	
	while(1)
	{

		// poor man's non-blocking delay to blink a system LED
		if(counter++ >= 100)
		{
			counter = 0;
			port_pin_toggle_output_level(LED1);
		}
		AppTask();
		NetworkTasks();
	}
}


/*********************************************************************
* Function:         void ReadMacAddress()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Reads MAC Address from MAC Address EEPROM
*
* Side Effects:	    none
*
* Overview:		    Uses the MAC Address from the EEPROM for addressing
*
* Note:			    
**********************************************************************/
void ReadMacAddress(void)
{ 
	//placholder function to read MAC address
   for(uint8_t i=0; i<MY_ADDRESS_LENGTH; i++)
   {
	   myLongAddress[i] = i+1;
   }
   if(NETWORK_ROLE)
   {
	   myLongAddress[0] += 1;
   }
}

void HardFault_Handler(void)
{
	while (1) {
	}
}