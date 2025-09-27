// Mi-Wi Wrapper for network interface

#include "network_interface.h"
#include "network_interface_miwi.h"
#include "debug_interface.h"
#include "miwi_api.h"
#include "miwi_nvm.h"
#include "mimem.h"
#include "phy.h"
#include "rf_transceiver.h"
#include "asf.h"
#include "custom_board.h"


/*************************************************************************/
/*                          definitions and macros                       */
/*************************************************************************/
#define CHANNEL_MAP (0x03FF) //enable channels 1-10 for 900MHz band
#define NVM_UID_ADDRESS   ((volatile uint16_t *)(0x0080400AU))

/*************************************************************************/
/*                          local variables                              */
/*************************************************************************/
static const uint16_t broadcastAddress = 0xFFFF;
static net_config_t miwi_net_config = {0};
static uint8_t myChannel = 3;
static net_event_callback_t callback = NULL;

extern API_UINT16_UNION  myPANID;

/*************************************************************************/

// AdditionalNodeID variable array defines the additional
// information to identify a device on a PAN. This array
// will be transmitted when initiate the connection between
// the two devices. This  variable array will be stored in
// the Connection Entry structure of the partner device. 
// Typical use cases might be a node type (sensor, actuator, etc...), 
// device specific role, etc... 
// In the future, I might use this to define a specific type of remote 
// control or add capabilites, etc...

/*************************************************************************/
uint8_t        AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {0x00};


CONNECTION_ENTRY connectionTable[CONNECTION_SIZE]; // Connection Table Memory

defaultParametersRomOrRam_t defaultParamsRomOrRam = { //MiWi configuration data
    .ConnectionTable = &connectionTable[0],
    #if ADDITIONAL_NODE_ID_SIZE > 0
    .AdditionalNodeID = &AdditionalNodeID[0],
    #endif
    .networkFreezerRestore = 0,
};

defaultParametersRamOnly_t defaultParamsRamOnly = {
    .dummy = 0,
};

// a structure to hold meta data about a packet
typedef struct {
  uint8_t *address;
  bool txComplete;
  uint8_t send_sequence_number;
  uint32_t timeoutCount;
}packet_meta_t;

static packet_meta_t packet_meta = {
  .address = (uint8_t*)&broadcastAddress, //ToDo: change to unicast
  .txComplete = true,
  .send_sequence_number = 0,
  .timeoutCount = 0
};

/*************************************************************************/
/*                          private prototypes                           */
/*************************************************************************/
static void ReadMacAddress(void);
static void Connection_Confirm(miwi_status_t status);
static void dataConfcb(uint8_t handle, miwi_status_t status,
                       uint8_t* msgPointer);
static void ReceivedDataIndication(RECEIVED_MESSAGE* ind);
static void set_random_ieee_address(void);


/*************************************************************************/
/*                          public API                                   */
/*************************************************************************/
net_return_t register_callback(void* context, net_event_callback_t cb){
    callback = cb;
    return NWK_SUCCESS;
}

net_return_t init(void* context){
    net_return_t ret = NWK_FAILURE;

    TransceiverConfig();  // initialize pins to the radio
    ReadMacAddress();

    miwi_status_t status = MiApp_ProtocolInit(&defaultParamsRomOrRam, &defaultParamsRamOnly);
    if (status != SUCCESS)  // initializes MiApp, MAC, PHY, & radio i/f
    {
        DEBUG_OUTPUT(printf("Error initializing Miwi: %d\r\n", status));
        return ret;
    }

    // for testing purposes:
    // added a pin to select the mi-wi role type (coordinator or edge)
    // to allow the same board to be either based on pin state
    // add a jumper to ROLE_PIN to VCC for coordinator
    miwi_net_config.role = port_pin_get_input_level(ROLE_PIN);

    set_random_ieee_address();

    MiApp_SubscribeDataIndicationCallback(ReceivedDataIndication);

    DEBUG_OUTPUT(printf("set default channel: %d\r\n", myChannel));
    if (MiApp_Set(CHANNEL, &myChannel) == false) {
        DEBUG_OUTPUT(printf("channel %d not supported\r\n", myChannel));
        return ret;
    }

    DEBUG_OUTPUT(printf("set connection mode\r\n"));
    MiApp_ConnectionMode(ENABLE_ALL_CONN);

    
    DEBUG_OUTPUT(printf("address: "));
    for (uint8_t i = 0; i < MY_ADDRESS_LENGTH; i++) {
        DEBUG_OUTPUT(printf("%02x", myLongAddress[MY_ADDRESS_LENGTH - 1 - i]));
    }

    DEBUG_OUTPUT(printf("\r\n"));

    ret = NWK_SUCCESS;

    return ret;
}

net_return_t send(void* context, const uint8_t* data, size_t len){
    net_return_t ret = NWK_FAILURE;
    packet_meta.txComplete = false;
    //***ToDo: change to unicast
    bool res = MiApp_SendData(SHORT_ADDR_LEN, packet_meta.address,
                            len, (uint8_t *)data,
                            packet_meta.send_sequence_number++, true, false, dataConfcb);
    if (res == true) {
        ret = NWK_SUCCESS;
    } else {
        ret = NWK_FAILURE;
    }
    return ret;
}

net_return_t up(void* context){
  net_return_t ret = NWK_FAILURE;

  if (miwi_net_config.role == ACCESS_POINT) {
      DEBUG_OUTPUT(printf("Role = PAN Coordinator\r\n"));
      DEBUG_OUTPUT(printf("start PAN\r\n"));
      MiApp_StartConnection(START_CONN_DIRECT, 10, (1L << myChannel),
                                  Connection_Confirm);
  } else {
      DEBUG_OUTPUT(printf("Role = Edge Node\r\n"));
      DEBUG_OUTPUT(printf("Connect to PAN\r\n"));
      uint8_t i = 0xFF;
      while(i == 0xFF)
      {
          i = MiApp_EstablishConnection(myChannel, 0, NULL, 0, Connection_Confirm);
      }
  }
  

#ifdef ENABLE_DUMP
    DumpConnection(0xFF);
#endif
    
  return ret;
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

/*************************************************************************/
/*                          private implementations                      */
/*************************************************************************/

static void ReadMacAddress(void){
    uint8_t i = 0, j = 0;
    for (i = 0; i < MY_ADDRESS_LENGTH; i += 2, j++)
    {
        myLongAddress[i] = (NVM_UID_ADDRESS[j] & 0xFF);
        myLongAddress[i + 1] = (NVM_UID_ADDRESS[j] >> 8);
    }
}

static void Connection_Confirm(miwi_status_t status) {

  net_event_t connect_event; 
  DEBUG_OUTPUT(printf("Connection Confirmation Callback status: %d\r\n", status));
  if((SUCCESS == status)||(ALREADY_EXISTS == status)){
    if(miwi_net_config.role == CLIENT_NODE){
      DEBUG_OUTPUT(printf("connected!!!\r\n"));
      connect_event.code = NWK_EVENT_CONNECTED;
    } else {
      DEBUG_OUTPUT(printf("Network Successfully Created!!!\r\n"));
      connect_event.code = NWK_EVENT_CUSTOM;
    }
  } else if (FAILURE == status) {
    DEBUG_OUTPUT(printf("connection failure\r\n"));
      if(miwi_net_config.role == CLIENT_NODE){
        connect_event.code = NWK_EVENT_DISCONNECTED;
      } else {
        connect_event.code = NWK_EVENT_ERROR;
      }
    }
  if(callback){
    callback(&connect_event, NULL);
  }
}

static void dataConfcb(uint8_t handle, miwi_status_t status,
                       uint8_t* msgPointer) {
  DEBUG_OUTPUT(printf("Data Confirmation Callback\r\n"));
  MiMem_Free(msgPointer);
}

static void ReceivedDataIndication(RECEIVED_MESSAGE* ind) {
  uint8_t startPayloadIndex = 0;
  DEBUG_OUTPUT(printf("data received: "));

  for (uint8_t i = startPayloadIndex; i < rxMessage.PayloadSize; i++) {
    DEBUG_OUTPUT(printf("%03i ", ind->Payload[i]));
  }
  DEBUG_OUTPUT(printf("\r\n"));
}

static void set_random_ieee_address(void)
{
    bool invalidIEEEAddrFlag = false;
    uint64_t invalidIEEEAddr;

    srand(PHY_RandomReq());

    /* Check if a valid IEEE address is available.
    0x0000000000000000 and 0xFFFFFFFFFFFFFFFF is persumed to be invalid */
    /* Check if IEEE address is 0x0000000000000000 */
    memset((uint8_t *)&invalidIEEEAddr, 0x00, LONG_ADDR_LEN);
    if (0 == memcmp((uint8_t *)&invalidIEEEAddr, (uint8_t *)&myLongAddress, LONG_ADDR_LEN))
    {
        invalidIEEEAddrFlag = true;
    }

    /* Check if IEEE address is 0xFFFFFFFFFFFFFFFF */
    memset((uint8_t *)&invalidIEEEAddr, 0xFF, LONG_ADDR_LEN);
    if (0 == memcmp((uint8_t *)&invalidIEEEAddr, (uint8_t *)&myLongAddress, LONG_ADDR_LEN))
    {
        invalidIEEEAddrFlag = true;
    }

    if (invalidIEEEAddrFlag)
    {
         /* In case no valid IEEE address is available, a random
          * IEEE address will be generated to be able to run the
          * applications for demonstration purposes.
          * In production code this can be omitted.
         */
        uint8_t* peui64 = (uint8_t *)&myLongAddress;
        for(uint8_t i = 0; i < MY_ADDRESS_LENGTH; i++)
        {
            *peui64++ = (uint8_t)rand();
        }
    }
    
    PHY_SetIEEEAddr((uint8_t *)&myLongAddress);
}

#ifdef ENABLE_DUMP
    /*********************************************************************
     * void DumpConnection(uint8_t index)
     *
     * Overview:        This function prints out the content of the connection
     *                  with the input index of the P2P Connection Entry
     *
     * PreCondition:
     *
     * Input:
     *          index   - The index of the P2P Connection Entry to be printed out
     *
     * Output:  None
     *
     * Side Effects:    The content of the connection pointed by the index
     *                  of the P2P Connection Entry will be printed out
     *
     ********************************************************************/
    void DumpConnection(INPUT uint8_t index)
    {
        uint8_t i, j;

        if( index > CONNECTION_SIZE )
        {
            printf("\r\n\r\nMy Address: 0x");
            for(i = 0; i < MY_ADDRESS_LENGTH; i++)
            {
                printf("%02x",myLongAddress[MY_ADDRESS_LENGTH-1-i]);
            }
            #if defined(IEEE_802_15_4)
                printf("  PANID: 0x");
                printf("%x",myPANID.v[1]);
                printf("%x",myPANID.v[0]);
            #endif
            printf("  Channel: ");
            printf("%d",currentChannel);
        }

        if( index < CONNECTION_SIZE )
        {
            printf("\r\nConnection \tPeerLongAddress \tPeerInfo\r\n");
            if( connectionTable[index].status.bits.isValid )
            {
                printf("%02x",index);
                printf("\t\t\t");
                for(i = 0; i < 8; i++)
                {
                    if(i < MY_ADDRESS_LENGTH)
                    {
                        printf("%02x", connectionTable[index].Address[MY_ADDRESS_LENGTH-1-i] );
                    }
                    else
                    {
                        printf("\t");
                    }
                }
                printf("/t");
                #if ADDITIONAL_NODE_ID_SIZE > 0
                    for(i = 0; i < ADDITIONAL_NODE_ID_SIZE; i++)
                    {
                        printf("%02x", connectionTable[index].PeerInfo[i] );
                    }
                #endif
                printf("\r\n");
            }
        }
        else
        {
            printf("\r\n\r\nConnection     PeerLongAddress     PeerInfo\r\n");
            for(i = 0; i < CONNECTION_SIZE; i++)
            {

                if( connectionTable[i].status.bits.isValid )
                {
                    printf("%02x",i);
                    printf("             ");
                    for(j = 0; j < 8; j++)
                    {
                        if( j < MY_ADDRESS_LENGTH )
                        {
                            printf("%02x", connectionTable[i].Address[MY_ADDRESS_LENGTH-1-j] );
                        }
                        else
                        {
                            printf("  ");
                        }
                    }
                    printf("    ");
                    #if ADDITIONAL_NODE_ID_SIZE > 0
                        for(j = 0; j < ADDITIONAL_NODE_ID_SIZE; j++)
                        {
                            printf("%02x", connectionTable[i].PeerInfo[j] );
                        }
                    #endif
                    printf("\r\n");
                }
            }
        }
    }
#endif
