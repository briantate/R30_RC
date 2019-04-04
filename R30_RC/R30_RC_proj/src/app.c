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
#include "analog.h"
#include "joystick.h"

#define SEND_BUFFER_SIZE  1

static bool switchLastState = false;
static bool switchState = false;
static uint8_t cntVal = 0;
static uint8_t sendDataBuffer[SEND_BUFFER_SIZE];
uint8_t msghandledemo = 0;
uint8_t extintCount = 0;

static bool txComplete = true;

//measurement variables
volatile uint8_t jstkLeftHorzVal = 0;
volatile bool jstkLeftHorzDir = 0;
volatile uint8_t jstkLeftVertVal = 0;
volatile bool jstkLeftVertDir = 0;
volatile uint8_t jstkRightHorzVal = 0;
volatile bool jstkRightHorzDir = 0;
volatile uint8_t jstkRightVertVal = 0;
volatile bool jstkRightVertDir = 0;

//data structures for joysticks
volatile joystickPtr leftJoystick;
volatile joystickPtr rightJoystick;

static void configure_extint_channel(void);
static void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer);
static void extint_callback(void);
static void main_clock_select_osc16m(void);
static void main_clock_select_dfll(void);
static void main_clock_select(const enum system_clock_source clock_source);



void AppInit(void)
{
	configure_console();
	
	printf("R30 Remote Control Project\r\n");
	uint32_t cpuClock = system_cpu_clock_get_hz();
	DEBUG_OUTPUT(printf("CPU clock %lu Hz\r\n", cpuClock));
	
	//enable wakeup via SW0
	configure_extint_channel();
	
	//Initialize ADC for joystick measurements
	ADC_init();
	
	//initialize data structures for joysticks
	leftJoystick  = Joystick_Create(ADC_ReadChannel_06, ADC_ReadChannel_07);
	rightJoystick = Joystick_Create(ADC_ReadChannel_10, ADC_ReadChannel_11);
	
}

void AppTask(void)
{
	//Sample joysticks and print:
	Joystick_Measure(leftJoystick);
	jstkLeftHorzVal = Joystick_GetHorz(leftJoystick);
	jstkLeftHorzDir = Joystick_GetHorzDirection(leftJoystick);
	jstkLeftVertVal = Joystick_GetVert(leftJoystick);
	jstkLeftVertDir = Joystick_GetVertDirection(leftJoystick);
	Joystick_Measure(rightJoystick);
	jstkRightHorzVal = Joystick_GetHorz(rightJoystick);
	jstkRightHorzDir = Joystick_GetHorzDirection(rightJoystick);
	jstkRightVertVal = Joystick_GetVert(rightJoystick);
	jstkRightVertDir = Joystick_GetVertDirection(rightJoystick);
	printf("L H:%03i, D: %01i, V:%03i, D: %01i, R H:%03i, D: %01i, V:%03i, D: %01i\r\n",
		jstkLeftHorzVal, jstkLeftHorzDir,
		jstkLeftVertVal, jstkLeftVertDir,
		jstkRightHorzVal, jstkRightHorzDir,
		jstkRightVertVal, jstkRightVertDir);

// 	//ToDo: build data buffer
// 	sendDataBuffer[0] = 0x41 + cntVal%10; //start at ascii A
// 	cntVal++;
// 		
// 	//send broadcast data ***ToDo: change to unicast
// 	uint16_t broadcastAddress = 0xFFFF;
// 	DEBUG_OUTPUT(printf("sending char: %u\r\n", sendDataBuffer[0]));
// 		
// 		
// 	//wait until transceiver is ready to send?
// 	txComplete = false;
// 	bool res = MiApp_SendData(SHORT_ADDR_LEN, (uint8_t *)&broadcastAddress,
// 		SEND_BUFFER_SIZE, sendDataBuffer, msghandledemo++, true, dataConfcb);
// 	if(!res)
// 	{
// 		printf("send fail\r\n");
// 	}
// 
// 	//Wait until the transmission is complete before sleeping
// 	while(txComplete != true)
// 	{
// 		P2PTasks();
// 	}
// 		

}

static void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer)
{
	txComplete = true;
	if(status != SUCCESS)
	{
		printf("callback: fail\r\n");
	}
// 	
// 		SUCCESS = 0,
// 		FAILURE,
// 		CHANNEL_ACCESS_FAILURE,
// 		NO_ACK,
// 		TIMER_EXPIRED,
// 		TRANSACTION_EXPIRED = 5,
// 		ALREADY_EXISTS,
// 		NO_ROUTE,
// 		SCAN_NO_BEACON,
// 		SCAN_MAX_REACHED,
// 		MEMORY_UNAVAILABLE = 10,
// 		ERR_TX_FAIL,
// 		ERR_TRX_FAIL,
// 		ERR_INVALID_INPUT,
//	port_pin_toggle_output_level(LED0);
	
	//DEBUG_PRINT(printf("\nData Confirm: Handle: %d status:%d\r\n", handle, status));
	//MiMem_Free(msgPointer);
}

static void extint_callback(void)
{
	extintCount++;
}

void ReceivedDataIndication (RECEIVED_MESSAGE *ind)
{
	uint8_t startPayloadIndex = 0;
	/*******************************************************************/
	// If a packet has been received, handle the information available
	// in rxMessage.
	/*******************************************************************/
	DEBUG_OUTPUT(printf("data received: "));
	
	// Toggle LED to indicate receiving a packet.
	DEBUG_OUTPUT(port_pin_toggle_output_level(LED0));
	
	for(uint8_t i=startPayloadIndex; i<rxMessage.PayloadSize;i++)
	{
		DEBUG_OUTPUT(putchar(rxMessage.Payload[i]));
	}
	DEBUG_OUTPUT(printf("\r\n"));
	
}

/**
 * \brief Config external interrupt.
 */
static void configure_extint_channel(void)
{

	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = SW0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = SW0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
	extint_chan_set_config(SW0_EIC_LINE, &config_extint_chan);
	extint_register_callback(extint_callback,SW0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
 	extint_chan_enable_callback(SW0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	while (extint_chan_is_detected(SW0_EIC_LINE)) {
		extint_chan_clear_detected(SW0_EIC_LINE);
	}
}

/**
 * \brief Select OSC16M as main clock source.
 */
static void main_clock_select_osc16m(void)
{
	struct system_gclk_gen_config gclk_conf;
	struct system_clock_source_osc16m_config osc16m_conf;

	/* Switch to new frequency selection and enable OSC16M */
	system_clock_source_osc16m_get_config_defaults(&osc16m_conf);
	osc16m_conf.fsel = CONF_CLOCK_OSC16M_FREQ_SEL;
	osc16m_conf.on_demand = 0;
	osc16m_conf.run_in_standby = CONF_CLOCK_OSC16M_RUN_IN_STANDBY;
	system_clock_source_osc16m_set_config(&osc16m_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC16M);
	while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_OSC16M));

	/* Select OSC16M as mainclock */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_OSC16M;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);
	if (CONF_CLOCK_OSC16M_ON_DEMAND) {
		OSCCTRL->OSC16MCTRL.reg |= OSCCTRL_OSC16MCTRL_ONDEMAND;
	}

}

/**
 * \brief Setect DFLL as main clock source.
 */
static void main_clock_select_dfll(void)
{
	struct system_gclk_gen_config gclk_conf;

	/* Select OSCULP32K as new clock source for mainclock temporarily */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_XOSC32K;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);

	/* Select XOSC32K for GCLK1. */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_XOSC32K;
	gclk_conf.division_factor = CONF_CLOCK_GCLK_1_PRESCALER;
	system_gclk_gen_set_config(GCLK_GENERATOR_1, &gclk_conf);
	system_gclk_gen_enable(GCLK_GENERATOR_1);

	struct system_gclk_chan_config dfll_gclk_chan_conf;

	system_gclk_chan_get_config_defaults(&dfll_gclk_chan_conf);
	dfll_gclk_chan_conf.source_generator = GCLK_GENERATOR_1;
	system_gclk_chan_set_config(OSCCTRL_GCLK_ID_DFLL48, &dfll_gclk_chan_conf);
	system_gclk_chan_enable(OSCCTRL_GCLK_ID_DFLL48);
	
	struct system_clock_source_dfll_config dfll_conf;
	system_clock_source_dfll_get_config_defaults(&dfll_conf);

	dfll_conf.loop_mode      = SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED;
	dfll_conf.on_demand      = false;
	dfll_conf.run_in_stanby  = CONF_CLOCK_DFLL_RUN_IN_STANDBY;
	dfll_conf.multiply_factor = CONF_CLOCK_DFLL_MULTIPLY_FACTOR;
	system_clock_source_dfll_set_config(&dfll_conf);
	system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DFLL);
	while(!system_clock_source_is_ready(SYSTEM_CLOCK_SOURCE_DFLL));
	if (CONF_CLOCK_DFLL_ON_DEMAND) {
		OSCCTRL->DFLLCTRL.bit.ONDEMAND = 1;
	}

	/* Select DFLL for mainclock. */
	system_gclk_gen_get_config_defaults(&gclk_conf);
	gclk_conf.source_clock = SYSTEM_CLOCK_SOURCE_DFLL;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &gclk_conf);

}

/**
 * \brief Main clock source selection between DFLL and OSC16M.
 */
static void main_clock_select(const enum system_clock_source clock_source)
{
	if (clock_source == SYSTEM_CLOCK_SOURCE_DFLL) {
		main_clock_select_dfll();
		system_clock_source_disable(SYSTEM_CLOCK_SOURCE_OSC16M);
	} else if (clock_source == SYSTEM_CLOCK_SOURCE_OSC16M) {
		main_clock_select_osc16m();
		system_clock_source_disable(SYSTEM_CLOCK_SOURCE_DFLL);
		system_gclk_chan_disable(OSCCTRL_GCLK_ID_DFLL48);
		system_gclk_gen_disable(GCLK_GENERATOR_1);
	} else {
		return ;
	}
}
