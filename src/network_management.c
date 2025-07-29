/*
 * network_management.c
 *
 * Created: 10/23/2018 11:24:26 AM
 *  Author: C41175
 */

#include "network_management.h"

#include <asf.h>

#include "app.h"  //needed for receive callback function
#include "debug_interface.h"
#include "miwi_api.h"
#include "miwi_nvm.h"

uint8_t AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {
    0x1};  // ToDo: what is this?

uint8_t myChannel = 8;

// forward declarations
static void Connection_Confirm(miwi_status_t status);

// helper functions
static bool NetworkConnect(void) {
  bool ret = false;
  //	 nvmPutMyRole(&role);  // Saving the Role of the device
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

  return ret;
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
      NetworkConnect();
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

bool NetworkInit(bool freezer_enable, bool networkRole) {
  bool ret = false;
  if (MiApp_SubscribeDataIndicationCallback(ReceivedDataIndication)) {
    DEBUG_OUTPUT(printf("MiWi receive callback registered\r\n"));
  } else {
    DEBUG_OUTPUT(printf("error: MiWi receive callback not registered\r\n"));
    return ret;
  }

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

  ret = NetworkConnect();

  return ret;
}

void NetworkTasks(void) {
  P2PTasks();  // maintain the operation of the stack - call often
}
