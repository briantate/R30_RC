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
#include "state_machine.h"
#include "state.h"

/*************************************************************************/
/*                          definitions and macros                       */
/*************************************************************************/
#define SEND_BUFFER_SIZE 6
#define RECEIVE_BUFFER_SIZE 6
#define ADDR_LENGTH 8

#define L_XDIR_POS 0
#define L_YDIR_POS 1
#define R_XDIR_POS 2
#define R_YDIR_POS 3
#define L_BUTTON_POS 0
#define R_BUTTON_POS 1
#define A_BUTTON_POS 2
#define B_BUTTON_POS 3

#define EVENT_NONE 1

/*************************************************************************/
/*                          callbacks                                    */
/*************************************************************************/
static void net_event_cb(const net_event_t* event, void* user_data);
static void heartbeat_timer_handler(struct SYS_Timer_t *timer);
static void transmit_timer_handler(struct SYS_Timer_t *timer);

static void init_state_handle_event(FSM_t *fsm, event_t event);
static void init_state_exit(FSM_t *fsm);
static void disconnected_state_handle_event(FSM_t *fsm, event_t event);
static void disconnected_state_entry(FSM_t *fsm);
static void disconnected_state_exit(FSM_t *fsm);
static void connected_state_handle_event(FSM_t *fsm, event_t event);
static void connected_state_entry(FSM_t *fsm);
static void connected_state_exit(FSM_t *fsm);
static void error_state_handle_event(FSM_t *fsm, event_t event);
static void error_state_entry(FSM_t *fsm);
static void error_state_exit(FSM_t *fsm);

/*************************************************************************/
/*                          local variables                              */
/*************************************************************************/
static net_api_t* net = NULL; 
static uint8_t sendDataBuffer[SEND_BUFFER_SIZE];
// static uint8_t receiveDataBuffer[RECEIVE_BUFFER_SIZE];
static bool last_sw_state = true;
static bool last_left_button_state = true;
static bool last_right_button_state = true;
volatile joystickPtr leftJoystick;
volatile joystickPtr rightJoystick;

app_data_t app_data = {
  .state = STATE_INIT,
  .isTimeForHeartbeat = false,
  .isTimeForTransmit = false,
  .counter = 0
};

static SYS_Timer_t heartbeat_timer = {
  .interval = 1000, //ms
  .mode = SYS_TIMER_PERIODIC_MODE,
  .handler = heartbeat_timer_handler
};

static SYS_Timer_t transmit_timer = {
  .interval = 100, //ms
  .mode = SYS_TIMER_PERIODIC_MODE,
  .handler = transmit_timer_handler
};

// state table
const State_t app_state_table[NUM_STATES] = {
    [STATE_INIT]         = { .event_handler = init_state_handle_event, .entry_handler = NULL, .exit_handler = init_state_exit },
    [STATE_DISCONNECTED] = { .event_handler = disconnected_state_handle_event, .entry_handler = disconnected_state_entry, .exit_handler = disconnected_state_exit },
    [STATE_CONNECTED]    = { .event_handler = connected_state_handle_event, .entry_handler = connected_state_entry, .exit_handler = connected_state_exit },
    [STATE_ERROR]        = { .event_handler = error_state_handle_event, .entry_handler = error_state_entry, .exit_handler = error_state_exit }
};

FSM_t remote_control_fsm;

/*************************************************************************/
/*                          private prototypes                           */
/*************************************************************************/
static void sample_joysticks(void);
static bool check_double_button_press(void);

/*************************************************************************/
/*                          public API                                   */
/*************************************************************************/

void AppInit(void) {
  CustomBoardInit();
  delay_init();     // used to to initialize radio interface
  SYS_TimerInit();  // used as a symbol timer by the MiWi stack
  SYS_TimerStart(&heartbeat_timer);
  SYS_TimerStart(&transmit_timer);
  configure_console();

  printf("R30 Remote Control Project\r\n");
  uint32_t cpuClock = system_cpu_clock_get_hz();
  DEBUG_OUTPUT(printf("CPU clock %lu Hz\r\n", cpuClock));

  ADC_init(); // Initialize ADC for joystick measurements

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
  FSM_Init(&remote_control_fsm, STATE_INIT, app_state_table, NUM_STATES);
}

volatile uint32_t disconnected_counter = 0;

void AppTask(void) {
  if(app_data.isTimeForHeartbeat)
  {
    app_data.isTimeForHeartbeat = false;
    port_pin_toggle_output_level(LED1);
  }

  FSM_HandleEvent(&remote_control_fsm, EVENT_NONE);
  
}


/*************************************************************************/
/*                          private implementations                      */
/*************************************************************************/

static void net_event_cb(const net_event_t* event, void* user_data){
  if(event->code == NWK_EVENT_CONNECTED || event->code == NWK_EVENT_CUSTOM){
    // DEBUG_OUTPUT(printf("app connected!\r\n"));
    DEBUG_OUTPUT(port_pin_set_output_level(LED0, false));
    remote_control_fsm.current_state = STATE_CONNECTED;
  }
  else if(event->code == NWK_EVENT_DATA_RECEIVED){
    uint8_t start_payload_index = 0;

    for(uint8_t i = 0; i < ADDR_LENGTH; i++){
        DEBUG_OUTPUT(printf("%02x",event->data.source_address[ADDR_LENGTH-1-i]));
    }
    DEBUG_OUTPUT(printf(": "));
    for (uint8_t i = start_payload_index; i < event->data.payload_size; i++) {
      DEBUG_OUTPUT(printf("%02x ", event->data.payload[i]));
    }
    DEBUG_OUTPUT(printf("\r\n"));
  }
  else if(event->code == NWK_EVENT_DISCONNECTED){
    // DEBUG_OUTPUT(printf("app disconnected!\r\n"));
    DEBUG_OUTPUT(port_pin_set_output_level(LED0, true));
    app_data.state = STATE_DISCONNECTED;
  }
  else if (event->code == NWK_EVENT_ERROR){
    DEBUG_OUTPUT(printf("network error!\r\n"));
  }
  else{
    DEBUG_OUTPUT(printf("unknown network event!\r\n"));
  }
}
static void heartbeat_timer_handler(struct SYS_Timer_t *timer){
  app_data.isTimeForHeartbeat = true;
}

static void transmit_timer_handler(struct SYS_Timer_t *timer){
  app_data.isTimeForTransmit = true;
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

static bool check_double_button_press(void){
  bool ret = false;
  bool left_button_state = port_pin_get_input_level(LEFT_BUTTON_PIN);
  bool right_button_state = port_pin_get_input_level(RIGHT_BUTTON_PIN);
  if(!left_button_state && !right_button_state){
    if(last_left_button_state || last_right_button_state){
      ret = true;
    }
  }
  last_left_button_state = left_button_state;
  last_right_button_state = right_button_state;
  return ret;
}

static void init_state_handle_event(FSM_t *fsm, event_t event){
  remote_control_fsm.current_state = STATE_DISCONNECTED;
  net->register_callback(NULL, net_event_cb);
  net->up(NULL);
}

static void init_state_exit(FSM_t *fsm){

}

static void disconnected_state_handle_event(FSM_t *fsm, event_t event){
  // bool sw_state = port_pin_get_input_level(SW0_PIN);
  // if(!sw_state){
  //   if(last_sw_state){
  //     DEBUG_OUTPUT(printf("reconnect...\r\n"));
  //     net->up(NULL);
  //   }
  // }
  // last_sw_state = sw_state;

  if(check_double_button_press()){
    DEBUG_OUTPUT(printf("reconnect...\r\n"));
    net->up(NULL);
  }

  if(disconnected_counter++ >=20000){
    disconnected_counter = 0;
    DEBUG_OUTPUT(printf("."));
  }
}
static void disconnected_state_entry(FSM_t *fsm){
  DEBUG_OUTPUT(printf("app disconnected\r\n"));
}
static void disconnected_state_exit(FSM_t *fsm){

}

static void connected_state_handle_event(FSM_t *fsm, event_t event){
  if(app_data.isTimeForTransmit){
    app_data.isTimeForTransmit = false;
    sample_joysticks();
    net->send(NULL, sendDataBuffer, 6 );
  }
}

static void connected_state_entry(FSM_t *fsm){
  DEBUG_OUTPUT(printf("app connected\r\n"));
}
static void connected_state_exit(FSM_t *fsm){

}

static void error_state_handle_event(FSM_t *fsm, event_t event){
  //ToDo: error handling...
}
static void error_state_entry(FSM_t *fsm){
  DEBUG_OUTPUT(printf("app error\r\n"));
}
static void error_state_exit(FSM_t *fsm){

}

