/*
 * app.c
 *
 * Created: 10/23/2018 1:57:40 PM
 *  Author: Brian Tate
 */

#include "app.h"

#include "analog.h"
#include "asf.h"
#include "conf_clocks.h"  //needed for sleep mode operation
#include "custom_board.h"
#include "debug_interface.h" //for debug logging
#include "joystick.h"
#include "miwi_api.h"
#include "network_interface.h"
#include "sysTimer.h"

/*************************************************************************/
/*                          definitions and macros                       */
/*************************************************************************/
#define SEND_BUFFER_SIZE 6
#define RECEIVE_BUFFER_SIZE 6

#define L_XDIR_POS 0
#define L_YDIR_POS 1
#define R_XDIR_POS 2
#define R_YDIR_POS 3
#define L_BUTTON_POS 0
#define R_BUTTON_POS 1
#define A_BUTTON_POS 2
#define B_BUTTON_POS 3

/*************************************************************************/
/*                          callbacks.                                   */
/*************************************************************************/
static void net_event_cb(const net_event_t* event, void* user_data);
static void app_timer_handler(struct SYS_Timer_t *timer);

/*************************************************************************/
/*                          local variables                              */
/*************************************************************************/
static net_api_t* net = NULL; 
static uint8_t sendDataBuffer[SEND_BUFFER_SIZE];
// static uint8_t receiveDataBuffer[RECEIVE_BUFFER_SIZE];
static bool last_sw_state = true;
volatile joystickPtr leftJoystick;
volatile joystickPtr rightJoystick;

app_data_t app_data = {
  .state = STATE_INIT,
  .isTimeForHeartbeat = false,
  .counter = 0
};

static SYS_Timer_t app_timer;

/*************************************************************************/
/*                          private prototypes                           */
/*************************************************************************/
static void sample_joysticks(void);

/*************************************************************************/
/*                          public API                                   */
/*************************************************************************/

void AppInit(void) {
  CustomBoardInit();
  delay_init();     // used to to initialize radio interface
  SYS_TimerInit();  // used as a symbol timer by the MiWi stack
  configure_console();

  app_timer.interval = ONE_SECOND;
  app_timer.mode = SYS_TIMER_PERIODIC_MODE;
  app_timer.handler = app_timer_handler;

  printf("R30 Remote Control Project\r\n");
  uint32_t cpuClock = system_cpu_clock_get_hz();
  DEBUG_OUTPUT(printf("CPU clock %lu Hz\r\n", cpuClock));

  // Initialize ADC for joystick measurements
  ADC_init();

  // Get joystick objects
  leftJoystick = Joystick_Create(ADC_ReadChannel_06, ADC_ReadChannel_07);
  if (leftJoystick == NULL) {
    DEBUG_OUTPUT(printf("[APP] ERROR - unable to create left joystick\r\n"));
    app_data.state = STATE_ERROR;
  }

  rightJoystick = Joystick_Create(ADC_ReadChannel_10, ADC_ReadChannel_11);
  if (rightJoystick == NULL) {
    DEBUG_OUTPUT(printf("[APP] ERROR - unable to create right joystick\r\n"));
    app_data.state = STATE_ERROR;
  }
  net = network_get_interface();
}

volatile uint32_t disconnected_counter = 0;

void AppTask(void) {
    //poor man's non-blocking delay
    if(app_data.counter++ >= 10000){
      app_data.counter = 0;
      port_pin_toggle_output_level(LED1);
    }
  
  // remote control state machine:
  switch (app_data.state) {
    case STATE_INIT: {
      app_data.state = STATE_CONNECTED;
      SYS_TimerStart(&app_timer);
      net->register_callback(NULL, net_event_cb);
      net->up(NULL);
    }
    case STATE_DISCONNECTED: {
      // try to connect
      // timeout?
      if(disconnected_counter++ >=20000){
        disconnected_counter = 0;
        DEBUG_OUTPUT(printf("."));
      }
      
      break;
    }
    case STATE_CONNECTED: {
      //if(timeToSample)
      // sample_joysticks();
      bool sw_state = port_pin_get_input_level(SW0_PIN);
      if(!sw_state){
        if(last_sw_state){
          sendDataBuffer[5] = 0xA5;
          net->send(NULL, sendDataBuffer, 6 );
        }
      }
      last_sw_state = sw_state;
      break;
    }
    case STATE_ERROR: {
      //ToDo: error handling
      break;
    }
  }
}


/*************************************************************************/
/*                          private implementations                      */
/*************************************************************************/
static void net_event_cb(const net_event_t* event, void* user_data){
  if(event->code == NWK_EVENT_CONNECTED || event->code == NWK_EVENT_CUSTOM){
    DEBUG_OUTPUT(printf("app connected!\r\n"));
    DEBUG_OUTPUT(port_pin_set_output_level(LED0, false));
    app_data.state = STATE_CONNECTED;
  }
  else{
    DEBUG_OUTPUT(printf("app disconnected!\r\n"));
    DEBUG_OUTPUT(port_pin_set_output_level(LED0, true));
    app_data.state = STATE_DISCONNECTED;
  }
}

static void app_timer_handler(struct SYS_Timer_t *timer){
  app_data.isTimeForHeartbeat = true;
}

static void sample_joysticks(void){
      // sample joysticks
      Joystick_Measure(leftJoystick);
      Joystick_Measure(rightJoystick);
      // create TX payload
      sendDataBuffer[0] = (uint8_t)Joystick_GetX(leftJoystick);
      sendDataBuffer[1] = (uint8_t)Joystick_GetY(leftJoystick);
      sendDataBuffer[2] = (uint8_t)Joystick_GetX(rightJoystick);
      sendDataBuffer[3] = (uint8_t)Joystick_GetY(rightJoystick);
      // pack direction bits
      sendDataBuffer[4] = (Joystick_GetXpolarity(leftJoystick) << L_XDIR_POS) |
                          (Joystick_GetYpolarity(leftJoystick) << L_YDIR_POS) |
                          (Joystick_GetXpolarity(rightJoystick) << R_XDIR_POS) |
                          (Joystick_GetYpolarity(rightJoystick) << R_YDIR_POS);

      // pack button data
      sendDataBuffer[5] = (uint8_t)port_pin_get_input_level(SW0_PIN);

}

