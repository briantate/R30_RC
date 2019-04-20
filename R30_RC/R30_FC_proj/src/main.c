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
#include <asf.h>
#include "custom_board.h"
#include "debug_interface.h"
#include "rf_transceiver.h"
#include "network_management.h"
#include "miwi_api.h"
#include "app.h"

void ReadMacAddress(void);

static volatile bool netRole;

int main (void)
{
	system_init();
	CustomBoardInit();
	delay_init(); //used to to initialize radio interface
	SYS_TimerInit(); //used as a symbol timer by the MiWi stack
	
	AppInit();

	TransceiverConfig(); //initialize pins to the radio
	
	//check switch state at startup to determine network role
	netRole = 1; //port_pin_get_input_level(SW0_PIN);   
	port_pin_set_output_level(LED0, !netRole); //LED on if PAN coordinator
	ReadMacAddress();
	DEBUG_PRINT(printf("address: "));
	for(uint8_t i=0; i<MY_ADDRESS_LENGTH; i++)
	{
		DEBUG_PRINT(printf("%u",myLongAddress[MY_ADDRESS_LENGTH - 1 - i]));
	}

	DEBUG_PRINT(printf("\r\n"));
	
	NetworkInit(NETWORK_FREEZER_OFF, netRole);
	
	while(1)
	{
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
   if(netRole)
   {
	   myLongAddress[0] += 1;
   }
}

void HardFault_Handler(void)
{
	while (1) {
	}
}