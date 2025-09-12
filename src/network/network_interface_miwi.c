#include "network_interface.h"
#include "network_interface_miwi.h"
#include "debug_interface.h"
#include "miwi_api.h"
#include "miwi_nvm.h"
#include "rf_transceiver.h"


//file scope variables
static uint8_t myChannel = 8;

//private prototypes
static void ReadMacAddress(void);
static void Connection_Confirm(miwi_status_t status);


//public API
net_return_t register_callback(void* context, net_event_callback_t cb){
//   if (MiApp_SubscribeDataIndicationCallback(ReceivedDataIndication)) {
//     DEBUG_OUTPUT(printf("MiWi receive callback registered\r\n"));
//   } else {
//     DEBUG_OUTPUT(printf("error: MiWi receive callback not registered\r\n"));
//     return ret;
//   }
    return NWK_SUCCESS;
}

net_return_t init(void* context){
    net_return_t ret = NWK_FAILURE;

    TransceiverConfig();  // initialize pins to the radio

    DEBUG_OUTPUT(printf("network init\r\n"));

    miwi_status_t status = MiApp_ProtocolInit(NULL, NULL);
    if (status != SUCCESS)  // initializes MiApp, MAC, PHY, & radio i/f
    {
        DEBUG_OUTPUT(printf("Error initializing Miwi: %d\r\n", status));
        return ret;
    }

    DEBUG_OUTPUT(printf("set default channel: %d\r\n", myChannel));
    if (MiApp_Set(CHANNEL, &myChannel) == false) {
        DEBUG_OUTPUT(printf("channel %d not supported\r\n", myChannel));
        return ret;
    }

    DEBUG_OUTPUT(printf("set connection mode\r\n"));
    MiApp_ConnectionMode(ENABLE_ALL_CONN);

    ReadMacAddress();
    DEBUG_OUTPUT(printf("address: "));
    for (uint8_t i = 0; i < MY_ADDRESS_LENGTH; i++) {
        DEBUG_OUTPUT(printf("%u", myLongAddress[MY_ADDRESS_LENGTH - 1 - i]));
    }

    DEBUG_OUTPUT(printf("\r\n"));

    ret = NWK_SUCCESS;

    return ret;
}

net_return_t send(void* context, const uint8_t* data, size_t len){
    return NWK_SUCCESS;
}

net_return_t up(void* context){

    bool ret = false;
    if (NETWORK_ROLE == PAN_COORDINATOR) {
        DEBUG_OUTPUT(printf("Role = PAN Coordinator\r\n"));
        DEBUG_OUTPUT(printf("start PAN\r\n"));
        ret = MiApp_StartConnection(START_CONN_DIRECT, 10, (1L << myChannel),
                                    Connection_Confirm);
    } else {
        DEBUG_OUTPUT(printf("Role = Edge Node\r\n"));
        DEBUG_OUTPUT(printf("Connect to PAN\r\n"));
        uint8_t PeerIndex =
            MiApp_EstablishConnection(myChannel, 0, NULL, 0, Connection_Confirm);
        if (PeerIndex == 0xFF) {
        DEBUG_OUTPUT(printf("Connection not established\r\n"));
        ret = false;
        } else {
        DEBUG_OUTPUT(printf("Connected to peer %u\r\n", PeerIndex));
        ret = true;
        }
    }
    if(ret == true){
        return NWK_SUCCESS;
    } else {
        return NWK_FAILURE;
    }
}

net_return_t down(void* context){
    return NWK_SUCCESS;
}

net_state_t  status(void){
    return DISCONNECTED;
}

void run_tasks(void){
    P2PTasks();
}



// private functions
static void ReadMacAddress(void) {
  // placholder function to read MAC address
  for (uint8_t i = 0; i < MY_ADDRESS_LENGTH; i++) {
    myLongAddress[i] = i + 1;
  }
  if (NETWORK_ROLE) {
    myLongAddress[0] += 1;
  }
}

static void Connection_Confirm(miwi_status_t status) {
  printf("\r\nConnect Operation Status: ");
  switch (status) {
    case SUCCESS: {
      DEBUG_OUTPUT(printf("SUCCESS\r\n"));
      break;
    }
    case FAILURE: {
      DEBUG_OUTPUT(printf("FAILURE\r\n"));
      DEBUG_OUTPUT(printf("retrying connection\r\n"));
    //   NetworkConnect();
        up(NULL);
      break;
    }
    case CHANNEL_ACCESS_FAILURE: {
      DEBUG_OUTPUT(printf("CHANNEL_ACCESS_FAILURE\r\n"));
      break;
    }
    case NO_ACK: {
      DEBUG_OUTPUT(printf("NO_ACK\r\n"));
      break;
    }
    case TIMER_EXPIRED: {
      DEBUG_OUTPUT(printf("TIMER_EXPIRED\r\n"));
      break;
    }
    case TRANSACTION_EXPIRED: {
      DEBUG_OUTPUT(printf("TRANSACTION_EXPIRED\r\n"));
      break;
    }
    case ALREADY_EXISTS: {
      DEBUG_OUTPUT(printf("ALREADY_EXISTS\r\n"));
      break;
    }
    case NO_ROUTE: {
      DEBUG_OUTPUT(printf("NO_ROUTE\r\n"));
      break;
    }
    case SCAN_NO_BEACON: {
      DEBUG_OUTPUT(printf("SCAN_NO_BEACON\r\n"));
      break;
    }
    case SCAN_MAX_REACHED: {
      DEBUG_OUTPUT(printf("SCAN_MAX_REACHED\r\n"));
      break;
    }
    case MEMORY_UNAVAILABLE: {
      DEBUG_OUTPUT(printf("MEMORY_UNAVAILABLE\r\n"));
      break;
    }
    case ERR_TX_FAIL: {
      DEBUG_OUTPUT(printf("ERR_TX_FAIL\r\n"));
      break;
    }
    case ERR_TRX_FAIL: {
      DEBUG_OUTPUT(printf("ERR_TRX_FAIL\r\n"));
      break;
    }
    case ERR_INVALID_INPUT: {
      DEBUG_OUTPUT(printf("ERR_INVALID_INPUT\r\n"));
      break;
    }
    default: {
      DEBUG_OUTPUT(printf("connection fail? default case\r\n"));
      break;
    }
  }
}
