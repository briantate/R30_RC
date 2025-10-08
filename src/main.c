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
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip
 * Support</a>
 */
#include "remote_control_app.h"
#include "asf.h"
#include "network_internal.h"

int main(void) {
  system_init();

  RemoteControlInit();
  network_init();

  while (1) {
    RemoteControlTask();
    network_tasks();
  }
}

void HardFault_Handler(void) {
  while (1) {
  }
}