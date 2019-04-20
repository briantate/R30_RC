/*
 * app.c
 *
 * Created: 10/23/2018 1:57:40 PM
 *  Author: C41175
 */ 

#include <asf.h>
#include "app.h"
#include "debug_interface.h"
#include "miwi_api.h"
#include "conf_clocks.h" //needed for sleep mode operation
#include "custom_board.h"

#define SEND_BUFFER_SIZE  1

static bool switchLastState = false;
static bool switchState = false;
static uint8_t cntVal = 0;
static uint8_t sendDataBuffer[SEND_BUFFER_SIZE];
uint8_t msghandledemo = 0;
uint8_t extintCount = 0;



static void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer);

void AppInit(void)
{
	configure_console();
	
	printf("R30 Flight Controller Project\r\n");
	uint32_t cpuClock = system_cpu_clock_get_hz();
	DEBUG_PRINT(printf("CPU clock %lu Hz\r\n", cpuClock));
	
}

void AppTask(void)
{

	switchState = port_pin_get_input_level(SW0_PIN);
	if(!switchState & switchLastState)
	{
//		port_pin_toggle_output_level(LED1);
		sendDataBuffer[0] = 0x41 + cntVal%10; //start at ascii A
		cntVal++;
		DEBUG_PRINT(printf("sending char: %u\r\n", sendDataBuffer[0]));
			
		//send broadcast data
		uint16_t broadcastAddress = 0xFFFF;
		MiApp_SendData(SHORT_ADDR_LEN, (uint8_t *)&broadcastAddress,
		SEND_BUFFER_SIZE, sendDataBuffer, msghandledemo++, true, dataConfcb);
	}
	switchLastState = switchState;

}

static void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer)
{
	port_pin_toggle_output_level(LED1);
//	DEBUG_PRINT(printf("\nData Confirm: Handle: %d status:%d\r\n", handle, status));
//	MiMem_Free(msgPointer);
}

void ReceivedDataIndication (RECEIVED_MESSAGE *ind)
{
	uint8_t startPayloadIndex = 0;
	/*******************************************************************/
	// If a packet has been received, handle the information available
	// in rxMessage.
	/*******************************************************************/
	DEBUG_PRINT(printf("data received: "));
	
	// Toggle LED to indicate receiving a packet.
	port_pin_toggle_output_level(LED0);
	
	for(uint8_t i=startPayloadIndex; i<rxMessage.PayloadSize;i++)
	{
		DEBUG_PRINT(printf("%03i ",ind->Payload[i]);)
	}
	DEBUG_PRINT(printf("\r\n"));
}

