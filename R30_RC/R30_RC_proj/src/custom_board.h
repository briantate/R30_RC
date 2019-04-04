/*
 * custom_board.h
 *
 * Created: 11/1/2018 3:48:58 PM
 *  Author: C41175
 */ 


#ifndef CUSTOM_BOARD_H_
#define CUSTOM_BOARD_H_

#define LED0 PIN_PA19
#define LED1 PIN_PA18
#define LED_ON  false
#define LED_OFF true

#define SW0_PIN                   PIN_PA28
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA28A_EIC_EXTINT8
#define SW0_EIC_MUX               MUX_PA28A_EIC_EXTINT8
#define SW0_EIC_PINMUX            PINMUX_PA28A_EIC_EXTINT8
#define SW0_EIC_LINE              8

void CustomBoardInit(void);


#endif /* CUSTOM_BOARD_H_ */