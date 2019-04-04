/*
 * rf_transceiver.c
 *
 * Created: 10/23/2018 9:06:10 AM
 *  Author: C41175
 */ 

#include "rf_transceiver.h"

void TransceiverConfig(void)
{
	struct port_config pin_conf;
	
	port_get_config_defaults(&pin_conf);
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	
	port_pin_set_config(AT86RFX_SPI_SCK, &pin_conf);
	port_pin_set_config(AT86RFX_SPI_MOSI, &pin_conf);
	port_pin_set_config(AT86RFX_SPI_CS, &pin_conf);
	port_pin_set_config(AT86RFX_RST_PIN, &pin_conf);
	port_pin_set_config(AT86RFX_SLP_PIN, &pin_conf);
	port_pin_set_output_level(AT86RFX_SPI_SCK, true);
	port_pin_set_output_level(AT86RFX_SPI_MOSI, true);
	port_pin_set_output_level(AT86RFX_SPI_CS, true);
	port_pin_set_output_level(AT86RFX_RST_PIN, true);
	port_pin_set_output_level(AT86RFX_SLP_PIN, true);
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(AT86RFX_SPI_MISO, &pin_conf);
	
	/* SAMR30 Antenna Diversity Configuration */

	MCLK->APBCMASK.reg |= (1<<MCLK_APBCMASK_RFCTRL_Pos);
		
	/*Pins  PA12/RFCTRL2 and PA09/RFCTRL1 are used as DIG1 and DIG2 pins respectively in SAMR30 Xplained Pro*/	
	
	/* FECTRL register is Written with value 4 => F2CFG = 00 and F1CFG = 01 */	
	REG_RFCTRL_FECFG = RFCTRL_CFG_ANT_DIV;
	
	struct system_pinmux_config config_pinmux;
	system_pinmux_get_config_defaults(&config_pinmux);
	
	/*MUX Position is 'F' i.e 5 for FECTRL Function and is same for all  FECTRL supported pins
	 * as provided in the data sheet */
	config_pinmux.mux_position = MUX_PA09F_RFCTRL_FECTRL1 ;
	
	config_pinmux.direction    = SYSTEM_PINMUX_PIN_DIR_OUTPUT;	
	system_pinmux_pin_set_config(PIN_RFCTRL1, &config_pinmux);
	system_pinmux_pin_set_config(PIN_RFCTRL2, &config_pinmux);
}
