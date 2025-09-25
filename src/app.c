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
#include "network_management.h"
#include "network_interface.h"
#include "sysTimer.h"
// #include "rf_transceiver.h"

static net_api_t* net = NULL; 

#define SEND_BUFFER_SIZE 6
#define RECEIVE_BUFFER_SIZE 6

// static bool switchLastState = false;
// static bool switchState = false;
static uint8_t sendDataBuffer[SEND_BUFFER_SIZE];
// static uint8_t receiveDataBuffer[RECEIVE_BUFFER_SIZE];
bool last_sw_state = true;
// uint32_t timeoutCount = 0;  // timeout variable

// static bool txComplete = true;
// static bool connected = false;



#define L_XDIR_POS 0
#define L_YDIR_POS 1
#define R_XDIR_POS 2
#define R_YDIR_POS 3
#define L_BUTTON_POS 0
#define R_BUTTON_POS 1
#define A_BUTTON_POS 2
#define B_BUTTON_POS 3

// handles for joysticks
volatile joystickPtr leftJoystick;
volatile joystickPtr rightJoystick;

static void net_event_cb(const net_event_t* event, void* user_data);
// static void dataConfcb(uint8_t handle, miwi_status_t status,
//                        uint8_t* msgPointer);
// static void extint_callback(void);
// static void main_clock_select_osc16m(void);
// static void main_clock_select_dfll(void);
// static void main_clock_select(const enum system_clock_source clock_source);

app_data_t app_data = {
  .state = STATE_INIT,
  .isTimeForHeartbeat = false,
  .counter = 0
};
// APP_STATE_T appState = STATE_INIT;

static void app_timer_handler(struct SYS_Timer_t *timer);

static SYS_Timer_t app_timer;

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
    while (1) {
      // ToDo: add error handling?
    }
  }

  rightJoystick = Joystick_Create(ADC_ReadChannel_10, ADC_ReadChannel_11);
  if (rightJoystick == NULL) {
    DEBUG_OUTPUT(printf("[APP] ERROR - unable to create right joystick\r\n"));
    while (1) {
      // ToDo: add error handling?
    }
  }
  net = network_get_interface();
}

volatile uint32_t disconnected_counter = 0;

void AppTask(void) {

  // poor man's non-blocking delay to blink a system LED
    // if (counter++ >= 100) {
    //   counter = 0;
    //   port_pin_toggle_output_level(LED1);
    // }

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
      // blink red LED every 500ms
      // try to connect
      // timeout?
      // if (connected) {
      //   app_data.state = STATE_WAIT;
      // }
      if(disconnected_counter++ >=20000){
        disconnected_counter = 0;
        DEBUG_OUTPUT(printf("."));
      }
      bool sw_state = port_pin_get_input_level(SW0_PIN);
      if(!sw_state){
        if(!last_sw_state){
          sendDataBuffer[5] = 0xA5;
          net->send(NULL, sendDataBuffer, 6 );
        }
      }
      last_sw_state = sw_state;
      
      break;
    }
    case STATE_CONNECTED: {
      //if(timeToSample)
      // sample_joysticks();
      bool sw_state = port_pin_get_input_level(SW0_PIN);
      if(!sw_state){
        if(!last_sw_state){
          sendDataBuffer[5] = 0xA5;
          net->send(NULL, sendDataBuffer, 6 );
        }
      }
      last_sw_state = sw_state;
      break;
    }
  }
}

void sample_joysticks(void){
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
      sendDataBuffer[5] = (uint8_t)port_pin_get_input_level(SW0_PIN);  // no button data yet

}

void ReceivedDataIndication(RECEIVED_MESSAGE* ind) {
  uint8_t startPayloadIndex = 0;
  /*******************************************************************/
  // If a packet has been received, handle the information available
  // in rxMessage.
  /*******************************************************************/
  DEBUG_OUTPUT(printf("data received: "));

  for (uint8_t i = startPayloadIndex; i < rxMessage.PayloadSize; i++) {
    DEBUG_OUTPUT(printf("%03i ", ind->Payload[i]));
  }
  DEBUG_OUTPUT(printf("\r\n"));
}

uint8_t retry_count = 0;
static void net_event_cb(const net_event_t* event, void* user_data){
  if(event->code == NWK_EVENT_CONNECTED || event->code == NWK_EVENT_CUSTOM){
    DEBUG_OUTPUT(printf("app connected!\r\n"));
    DEBUG_OUTPUT(port_pin_set_output_level(LED0, false));
    app_data.state = STATE_CONNECTED;
  }
  else{
    DEBUG_OUTPUT(printf("app disconnected! retry attempt %d\r\n", ++retry_count));
    DEBUG_OUTPUT(port_pin_set_output_level(LED0, true));
    app_data.state = STATE_DISCONNECTED;
  }
}

static void app_timer_handler(struct SYS_Timer_t *timer){
  app_data.isTimeForHeartbeat = true;
}

