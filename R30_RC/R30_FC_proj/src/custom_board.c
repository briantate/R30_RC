/*
 * custom_board.c
 *
 * Created: 11/1/2018 3:47:44 PM
 *  Author: C41175
 */ 

#include "asf.h"
#include "custom_board.h"
void CustomBoardInit(void)
{
	struct port_config config_port_pin;
	
	port_get_config_defaults(&config_port_pin);
	
	//init SW0
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(SW0_PIN, &config_port_pin);

	//init LED0 and LED1
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED0, &config_port_pin);
	port_pin_set_config(LED1, &config_port_pin);
	port_pin_set_output_level(LED0, LED_OFF);
	port_pin_set_output_level(LED1, LED_OFF);
}
