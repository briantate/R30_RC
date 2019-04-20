/*
 * debug_interface.c
 *
 * Created: 10/23/2018 8:35:00 AM
 *  Author: C41175
 */ 
#include "debug_interface.h"

/** UART module for debug. */
static struct usart_module cdc_uart_module;

/**
 * \brief Configure UART console.
 */
void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = DEBUG_UART_MUX_SETTING;
	usart_conf.pinmux_pad0 = DEBUG_UART_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = DEBUG_UART_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = DEBUG_UART_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = DEBUG_UART_PINMUX_PAD3;
	usart_conf.baudrate    = 115200;

	stdio_serial_init(&cdc_uart_module, DEBUG_UART_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}
