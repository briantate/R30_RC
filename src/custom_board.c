/*
 * custom_board.c
 *
 * Created: 11/1/2018 3:47:44 PM
 *  Author: Brian Tate
 */

#include "custom_board.h"

#include <asf.h>
void CustomBoardInit(void) {
  struct port_config config_port_pin;

  port_get_config_defaults(&config_port_pin);

  // init SW0
  config_port_pin.direction = PORT_PIN_DIR_INPUT;
  config_port_pin.input_pull = PORT_PIN_PULL_UP;
  port_pin_set_config(SW0_PIN, &config_port_pin);
  
  // init LEFT_BUTTON
  config_port_pin.direction = PORT_PIN_DIR_INPUT;
  config_port_pin.input_pull = PORT_PIN_PULL_UP;
  port_pin_set_config(LEFT_BUTTON_PIN, &config_port_pin);

  // init RIGHT_BUTTON
  config_port_pin.direction = PORT_PIN_DIR_INPUT;
  config_port_pin.input_pull = PORT_PIN_PULL_UP;
  port_pin_set_config(RIGHT_BUTTON_PIN, &config_port_pin);

  // init ROLE_PIN
  config_port_pin.direction = PORT_PIN_DIR_INPUT;
  config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
  port_pin_set_config(ROLE_PIN, &config_port_pin);

  // init LED0 and LED1
  config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(LED0, &config_port_pin);
  port_pin_set_config(LED1, &config_port_pin);
  port_pin_set_config(LED2, &config_port_pin);
  port_pin_set_output_level(LED0, LED_OFF);
  port_pin_set_output_level(LED1, LED_OFF);
  port_pin_set_output_level(LED2, LED_OFF);
}

uint8_t extintCount = 0;
static void extint_callback(void) { extintCount++; }

/**
 * \brief Config external interrupt.
 */
void configure_extint_channel(void) {
  struct extint_chan_conf config_extint_chan;
  extint_chan_get_config_defaults(&config_extint_chan);
  config_extint_chan.gpio_pin = SW0_EIC_PIN;
  config_extint_chan.gpio_pin_mux = SW0_EIC_MUX;
  config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
  config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
  extint_chan_set_config(SW0_EIC_LINE, &config_extint_chan);
  extint_register_callback(extint_callback, SW0_EIC_LINE,
                           EXTINT_CALLBACK_TYPE_DETECT);
  extint_chan_enable_callback(SW0_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
  while (extint_chan_is_detected(SW0_EIC_LINE)) {
    extint_chan_clear_detected(SW0_EIC_LINE);
  }
}
