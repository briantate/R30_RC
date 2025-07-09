/**
 *
 * Head Unit board definition
 *
 */

#ifndef HEAD_UNIT_BOARD_H
#define HEAD_UNIT_BOARD_H

#include <conf_board.h>
#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

void system_board_init(void);


#define BOARD_NAME                "Head Unit board"

// Resonator definitions
#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625

//DM Head Unit board SW2
#define SW2_PIN                   PIN_PA06
#define SW2_ACTIVE                false
#define SW2_INACTIVE              !SW2_ACTIVE
#define SW2_EIC_PIN               PIN_PA06A_EIC_EXTINT6
#define SW2_EIC_MUX               MUX_PA06A_EIC_EXTINT6
#define SW2_EIC_PINMUX            PINMUX_PA06A_EIC_EXTINT6
#define SW2_EIC_LINE              6


//DM Head Unit board SW4
#define SW4_PIN                   PIN_PA07
#define SW4_ACTIVE                false
#define SW4_INACTIVE              !SW4_ACTIVE
#define SW4_EIC_PIN               PIN_PA07A_EIC_EXTINT7
#define SW4_EIC_MUX               MUX_PA07A_EIC_EXTINT7
#define SW4_EIC_PINMUX            PINMUX_PA07A_EIC_EXTINT7
#define SW4_EIC_LINE              7


//DM Transducer board button macros
#define BUTTON_DOWN_NAME             "SW2"
#define BUTTON_DOWN_PIN              SW2_PIN
#define BUTTON_DOWN_ACTIVE           SW2_ACTIVE
#define BUTTON_DOWN_INACTIVE         SW2_INACTIVE
#define BUTTON_DOWN_EIC_PIN          SW2_EIC_PIN
#define BUTTON_DOWN_EIC_MUX          SW2_EIC_MUX
#define BUTTON_DOWN_EIC_PINMUX       SW2_EIC_PINMUX
#define BUTTON_DOWN_EIC_LINE         SW2_EIC_LINE

#define BUTTON_UP_NAME             "SW4"
#define BUTTON_UP_PIN              SW4_PIN
#define BUTTON_UP_ACTIVE           SW4_ACTIVE
#define BUTTON_UP_INACTIVE         SW4_INACTIVE
#define BUTTON_UP_EIC_PIN          SW4_EIC_PIN
#define BUTTON_UP_EIC_MUX          SW4_EIC_MUX
#define BUTTON_UP_EIC_PINMUX       SW4_EIC_PINMUX
#define BUTTON_UP_EIC_LINE         SW4_EIC_LINE

/** Number of on-board buttons */
#define BUTTON_COUNT 2

/* LCD SPI definitions */
#define LCD_SPI_MODULE              SERCOM1
#define LCD_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define LCD_SPI_SERCOM_PINMUX_PAD0  PINMUX_PA16C_SERCOM1_PAD0
#define LCD_SPI_SERCOM_PINMUX_PAD1  PINMUX_PA17C_SERCOM1_PAD1
#define LCD_SPI_SERCOM_PINMUX_PAD2  PINMUX_PA18C_SERCOM1_PAD2
#define LCD_SPI_SERCOM_PINMUX_PAD3  PINMUX_PA19C_SERCOM1_PAD3

/* LCD PIN Definitions */
#define LCD_SPI_CS               PIN_PA17
#define LCD_SPI_MOSI             PIN_PA18
#define LCD_SPI_MISO             PIN_PA16
#define LCD_SPI_SCK              PIN_PA19
#define LCD_A0_PIN               PIN_PA27


// UART module for debug print
#define CDC_MODULE              SERCOM3
#define CDC_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define CDC_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define CDC_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define CDC_SERCOM_PINMUX_PAD2  PINMUX_PA24C_SERCOM3_PAD2
#define CDC_SERCOM_PINMUX_PAD3  PINMUX_PA25C_SERCOM3_PAD3
#define CDC_SERCOM_DMAC_ID_TX   SERCOM3_DMAC_ID_TX
#define CDC_SERCOM_DMAC_ID_RX   SERCOM3_DMAC_ID_RX

#define RF_SPI_MODULE              SERCOM4
#define RF_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define RF_SPI_SERCOM_PINMUX_PAD0  PINMUX_PC19F_SERCOM4_PAD0
#define RF_SPI_SERCOM_PINMUX_PAD1  PINMUX_PB31F_SERCOM4_PAD1
#define RF_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB30F_SERCOM4_PAD2
#define RF_SPI_SERCOM_PINMUX_PAD3  PINMUX_PC18F_SERCOM4_PAD3


#define RF_IRQ_MODULE           EIC
#define RF_IRQ_INPUT            0
#define RF_IRQ_PIN              PIN_PB00A_EIC_EXTINT0
#define RF_IRQ_MUX              MUX_PB00A_EIC_EXTINT0
#define RF_IRQ_PINMUX           PINMUX_PB00A_EIC_EXTINT0


// 802.15.4 TRX Interface definitions
#define AT86RFX_SPI                  SERCOM4
#define AT86RFX_RST_PIN              PIN_PB15
#define AT86RFX_IRQ_PIN              PIN_PB00
#define AT86RFX_SLP_PIN              PIN_PA20
#define AT86RFX_SPI_CS               PIN_PB31
#define AT86RFX_SPI_MOSI             PIN_PB30
#define AT86RFX_SPI_MISO             PIN_PC19
#define AT86RFX_SPI_SCK              PIN_PC18
#define PIN_RFCTRL1                  PIN_PA09
#define PIN_RFCTRL2                  PIN_PA12
#define RFCTRL_CFG_ANT_DIV           4

#define AT86RFX_SPI_SERCOM_MUX_SETTING   RF_SPI_SERCOM_MUX_SETTING
#define AT86RFX_SPI_SERCOM_PINMUX_PAD0   RF_SPI_SERCOM_PINMUX_PAD0
#define AT86RFX_SPI_SERCOM_PINMUX_PAD1   PINMUX_UNUSED
#define AT86RFX_SPI_SERCOM_PINMUX_PAD2   RF_SPI_SERCOM_PINMUX_PAD2
#define AT86RFX_SPI_SERCOM_PINMUX_PAD3   RF_SPI_SERCOM_PINMUX_PAD3

#define AT86RFX_IRQ_CHAN       RF_IRQ_INPUT
#define AT86RFX_IRQ_PINMUX     RF_IRQ_PINMUX


/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()     \
		extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()    \
		extint_chan_disable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()      \
		extint_chan_clear_detected(AT86RFX_IRQ_CHAN);

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()   \
		{ extint_chan_disable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()   \
		extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT); }


#ifdef __cplusplus
}
#endif

#endif  /* HEAD_UNIT_BOARD_H */
