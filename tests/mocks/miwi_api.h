// /**
// * \file  mock miwi_api.h
// *
// */

// #ifndef __MIWI_API_H_
// #define __MIWI_API_H_

// #include "miwi_config.h"          //MiWi Application layer configuration file
// #if defined(PROTOCOL_P2P) || defined (PROTOCOL_STAR)
// #include "miwi_config_p2p.h"      //MiWi Protocol layer configuration file
// #endif

// #define MIWI_MAJOR_VERSION    '6'
// #define MIWI_MINOR_VERSION    '7'

// #define INPUT
// #define OUTPUT
// #define IOPUT


// #define SHORT_ADDR_LEN  2
// #define LONG_ADDR_LEN   8

// #define COORDINATOR_ADDRESS_MASK    0xFF00
// #define ENDEVICE_ADDRESS_MASK       0x00FF
// #define RXON_ENDEVICE_ADDRESS_MASK  0x0080

// #define MESH_BROADCAST_TO_ALL            (0xFFFF)
// #define MESH_BROADCAST_TO_FFDS           (0xFFFE)
// #define MESH_BROADCAST_TO_COORDINATORS   (0xFFFD)

// typedef union
// {
// 	uint8_t  v[4];
// 	uint16_t w[2];
// 	uint32_t Val;
// }API_UINT32_UNION;

// typedef union
// {
// 	uint8_t  v[8];
// 	uint16_t w[4];
// 	uint32_t d[2];
// 	uint64_t Val;
// }API_UINT64_UNION;

// typedef union
// {
// 	uint8_t  v[2];
// 	uint16_t Val;
// }API_UINT16_UNION;

// typedef enum miwi_status {
// 	SUCCESS = 0,
// 	FAILURE,
// 	CHANNEL_ACCESS_FAILURE,
// 	NO_ACK,
// 	TIMER_EXPIRED,
// 	TRANSACTION_EXPIRED = 5,
// 	ALREADY_EXISTS,
// 	NO_ROUTE,
// 	SCAN_NO_BEACON,
// 	SCAN_MAX_REACHED,
// 	MEMORY_UNAVAILABLE = 10,
// 	ERR_TX_FAIL,
// 	ERR_TRX_FAIL,
// 	ERR_INVALID_INPUT,
// 	RECONNECTION_IN_PROGRESS,
// 	RECONNECTED,
// 	ADDR_NOT_FOUND_IN_SCANNED_LIST,
//     ENTRY_NOT_EXIST = 0xF0,
//     NOT_ENOUGH_SPACE = 0xF1,
//     NOT_SAME_PAN = 0xF2,
//     NOT_PERMITTED = 0xF3,
//     ACTIVE_SCAN = 0xF4,
// }miwi_status_t;

// enum miwi_params {
// 	CHANNEL              =  0x00,
// 	PANID                =  0x01,
// 	SHORT_ADDRESS        =  0x02,
// 	PARENT_SHORT_ADDRESS =  0x03,
// 	DEVICE_TIMEOUT       =  0x04,
// 	CHANNELMAP           =  0x05,
// 	CAPABILITYINFO       =  0x06,
// 	BLOOM_AUTO_JOIN      =  0x80,
// 	ROUTE_TEST_MODE      =  0x81
// };

// typedef enum miwi_params miwi_params_t;


// #if defined(PROTOCOL_MESH)
// #include "miwi_mesh_app.h"      //MiWi Protocol layer configuration file
// #endif

// #if defined(PROTOCOL_P2P) || defined (PROTOCOL_STAR)

// #if defined (PROTOCOL_STAR)
// /* Device Role Enumerations */
// typedef enum _deviceRole
// {
//     PAN_COORD = 0x01,
//     END_DEVICE,
// }DeviceRole_t;
// #endif

// /***************************************************************************/
// // Status information of the connected peer information
// //
// //      This structure contains the information regarding the status of
// //      the connected peer device.
// /***************************************************************************/
// typedef union __CONNECTION_STATUS
// {
// 	uint8_t Val;
// 	struct _CONNECTION_STAUTS_bits
// 	{
// 		//1 = transceiver always on, 0 = transceiver sleeps when idle
// 		uint8_t RXOnWhenIdle       :1;
// 		//1 = can talk to this device directly, 0 = must route to this device
// 		uint8_t directConnection   :1;
// 		//1 = long address valid, 0 = long address unknown
// 		uint8_t longAddressValid   :1;
// 		//1 = short address valid, 0 = short address unknown
// 		uint8_t shortAddressValid  :1;
// 		//1 = already finish joining procedure, 0 = in the process of join
// 		uint8_t FinishJoin         :1;
// 		//1 = family member (parent/child), 0 = not family
// 		uint8_t isFamily           :1;
// 		uint8_t filler             :1;
// 		//1 = this entry is valid, 0 = this entry is not valid
// 		uint8_t isValid            :1;
// 	} bits;
// } CONNECTION_STATUS;


// /***************************************************************************
// * Peer Device Information in Connection Table
// *
// *      This structure contains device information about the peer device
// *      of current node. It is the element structure for connection table.
// *      Due to the bank limitation in PIC18 MCU architecture, the size of
// *      CONNECTION_ENTRY must be dividable by 256 in case the array is across
// *      the bank. In this case, the user need to make sure that there is no
// *      problem
// **************************************************************************/
// typedef struct __CONNECTION_ENTRY
// {
// #if !defined(PROTOCOL_P2P)
// 	API_UINT16_UNION    PANID;                      // PAN Identifier of the peer device. May not necessary in P2P protocol
// 	API_UINT16_UNION    AltAddress;                 // Alternative address of the peer device. Not necessary in P2P protocol
// #endif
// 	uint8_t        Address[MY_ADDRESS_LENGTH];     // Permanent address of peer device

// 	CONNECTION_STATUS status;

// #if ADDITIONAL_NODE_ID_SIZE > 0
// 	uint8_t        PeerInfo[ADDITIONAL_NODE_ID_SIZE];  // Additional Node ID information, if defined in application layer
// #endif
// #if defined(PROTOCOL_STAR)
// #if defined(ENABLE_LINK_STATUS)
//     uint8_t link_status;
//     uint8_t permanent_connections;
// #endif
// #endif
// } CONNECTION_ENTRY;

// #if defined(PROTOCOL_P2P) || defined (PROTOCOL_STAR)
// /***************************************************************************
// * Active Scan result
// *
// *      This structure contains information from active scan. Application
// *      layer will depend on this information to decide the way to establish
// *      connections.
// **************************************************************************/
// typedef struct
// {
// 	uint8_t		Channel;                        // Operating Channel of the PAN
// 	uint8_t        Address[MY_ADDRESS_LENGTH];     // Responding device address
// 	API_UINT16_UNION    PANID;                          // PAN Identifier
// 	uint8_t       RSSIValue;                      // RSSI value for the response
// 	uint8_t        LQIValue;                       // LQI value for the response
// 	union
// 	{
// 		uint8_t        Val;
// 		struct
// 		{
// 			uint8_t    Role:		2;              // Role of the responding device in the PAN
// 			uint8_t    Sleep:		1;              // Whether the responding device goes to sleep when idle
// 			uint8_t    SecurityEn:	1;              // Whether the responding device is capable of securing the data
// 			uint8_t    RepeatEn:	1;              // Whether the responding device allow repeat
// 			uint8_t    AllowJoin:	1;              // Whether the responding device allows other device to join
// 			uint8_t    Direct:		1;              // Whether the responding device in radio range or through a repeater
// 			uint8_t    altSrcAddr: 1;              // Whether the Address is alternative network address or permanent address
// 		} bits;
// 	} Capability;
// 	#if ADDITIONAL_NODE_ID_SIZE > 0
// 	uint8_t        PeerInfo[ADDITIONAL_NODE_ID_SIZE];  // Additional Node ID information, if defined in application layer
// 	#endif
// } ACTIVE_SCAN_RESULT;
// #endif
// #endif

// #if defined(PROTOCOL_MESH)
// #define BLOOM_FILTER_SIZE    8

// typedef struct
// {
// 	/**  coordinator hop */
// 	uint8_t coordinatorHop;

// 	/**  connection permit */
// 	uint8_t connectionPermit;

// 	/**  End device Capacity percentage */
// 	uint8_t enddeviceCapacity;

// 	/**  Sleeping End device Capacity percentage */
// 	uint8_t sleepEnddeviceCapacity;

// 	/** The current logical channel used by the network */
// 	uint8_t  logicalChannel;

// 	/** LQI at which the beacon was received */
// 	uint8_t LinkQuality;

// 	/**  PANID  */
// 	uint16_t panId;

// 	/**  Short Address  */
// 	uint16_t shortAddress;

// 	/**  bloom filter value */
// 	uint8_t bloomFilterValue[BLOOM_FILTER_SIZE];

// } beaconDescriptor_t;

// typedef struct
// {
// 	/*  status of the search request */
// 	uint8_t status;

// 	/*  the number of results found    */
// 	uint8_t resultSize;

// 	/* unscanned channels during search */
// 	uint32_t unscannedChannels;

// 	beaconDescriptor_t beaconList[MAX_BEACON_RESULTS];
// } searchConf_t;
// #endif

// extern uint8_t            currentChannel;

// #ifdef MESH_SECURITY
// extern API_UINT32_UNION meshOutgoingFrameCounter;
// #endif

// // Source address when sending a packet // can be MAC_Address or EUI based on Users choice
// extern uint8_t myLongAddress[MY_ADDRESS_LENGTH];

// // the no of connections on a device.
// // In case of Star network conn_size will be shared by pan co to all the end devices.
// // In case of p2p network every device will have all its on conn_size.
// extern uint8_t conn_size ;

// #define SECURITY_KEY_SIZE         16

// #if defined(PROTOCOL_P2P) || defined (PROTOCOL_STAR)
// #ifdef ENABLE_SECURITY
//    /* As security fields would add additional bytes in multiples of 16bytes,
//     memory for the same has to be allocated. */
// #define CALC_SEC_PAYLOAD_SIZE(x)	(SECURITY_KEY_SIZE > x)? \
// 	SECURITY_KEY_SIZE : (x % SECURITY_KEY_SIZE) ? \
// 	((x / SECURITY_KEY_SIZE) * SECURITY_KEY_SIZE) + SECURITY_KEY_SIZE : \
// 	(x / SECURITY_KEY_SIZE) * SECURITY_KEY_SIZE
// #else
// #define CALC_SEC_PAYLOAD_SIZE(x)	(x)
// #endif
// #endif

// #define HOP_TABLE_COUNT   (((NUM_OF_COORDINATORS % 2) == 0)? \
// 							(NUM_OF_COORDINATORS / 2) : \
// 							((NUM_OF_COORDINATORS / 2) + 1))

// /* Table maintained by the PAN Coordinator to store the list of joiner which it
//  * wants to commission*/
// typedef struct
// {
//     uint64_t deviceAddress;
// } CommDeviceTable_t;

// typedef struct _CoordHopCount_t
// {
// 	uint8_t lsb : 4;
// 	uint8_t msb : 4;
// } CoordHopCount_t;

// typedef struct _CoordRouteTable_t
// {
// 	uint8_t nextHopAddr;
// 	uint8_t lqi : 4;
// 	uint8_t score : 4;
// } CoordRouteTable_t;

// typedef struct _RouteEntry_t
// {
// 	uint8_t coordNextHop;
// 	uint8_t coordHopCount;
// 	uint8_t coordNextHopLQI;
// 	uint8_t coordScore;
// } RouteEntry_t;

// /* Coordinator Table - which holds the joined router details */
// typedef struct coordinatorTable_
// {
// 	uint8_t ieeeaddr[LONG_ADDR_LEN];
// 	uint32_t currentTimeOut;
// }CoordinatorTable_t;

// /* Capability Information of a device */
// typedef union CapabilityInfo
// {
// 	uint8_t        Val;
// 	struct
// 	{
// 		uint8_t    rxonwhenIdle: 1;
// 		uint8_t    deviceType:	 2;
// 		uint8_t    reserved:	 5;
// 	} bits;
// } CapabilityInfo_t;

// /* Wish to Join As */
// typedef union JoinWish
// {
// 	uint8_t        Val;
// 	struct
// 	{
// 		uint8_t    ed:           1;
// 		uint8_t    coordinator:	 2;
// 		uint8_t    reserved:	 5;
// 	} bits;
// } JoinWish_t;

// /* Device Table - which holds the joined end device RxONdetails */
// typedef struct deviceTable_
// {
//     uint8_t ieeeaddr[LONG_ADDR_LEN];
//     uint32_t currentTimeOut;
//     CapabilityInfo_t capabilityInfo;
// }DeviceTable_t;

// /* Device Table - which holds the joined end device details */
// typedef struct SleepDeviceTable_
// {
//     uint8_t ieeeaddr[LONG_ADDR_LEN];
//     uint32_t actualTimeOut;
//     uint32_t currentTimeOut;
//     CapabilityInfo_t capabilityInfo;
// }SleepDeviceTable_t;

// typedef struct RebroadcastTable_
// {
// 	uint16_t nwkSrcAddr;
// 	uint8_t nwkSeqNo;
// 	uint8_t timeout;
// }RebroadcastTable_t;

// typedef struct DuplicateRejectionTable_
// {
// 	uint16_t srcAddr;
// 	uint8_t seqNo;
// 	uint8_t timeout;
// }DuplicateRejectionTable_t;

// typedef struct __defaultParametersRomOrRam
// {
// #if defined(PROTOCOL_MESH)
// #ifndef ENDDEVICE
// #ifdef PAN_COORDINATOR
// 	CoordinatorTable_t *coordTable;
// 	CommDeviceTable_t *commDevTable;
// #endif
// 	DeviceTable_t *devTable;
// 	SleepDeviceTable_t *sleepingDevTable;
// 	CoordRouteTable_t *coordinatorRouteTable;
// 	CoordHopCount_t *coordinatorHopCount;
// 	RebroadcastTable_t *rebroadcastTable;

// 	uint32_t indirectDataWaitInterval;

// 	uint16_t keepAliveCoordSendInterval;
// 	uint16_t keepAliveCoordTimeoutSec;

// 	uint8_t maxNumOfDevicesInNetwork;

// 	uint8_t roleUpgradeIntervalInSec;
// 	uint8_t numOfRxOnEnddevices;
// 	uint8_t numOfRxOffEnddevices;

// 	uint8_t routeUpdateInterval;
// 	uint8_t routeReqWaitInterval;

// 	uint8_t rebroadcastTableSize;
// 	uint8_t rebroadcastTimeout;
// #endif
// 	DuplicateRejectionTable_t *duplicateRejectionTable;
// 	uint32_t deviceTimeout;
// 	uint16_t keepAliveRxOnEdSendInterval;
// 	uint16_t keepAliveRxOnEdTimeoutSec;
// 	uint16_t myPANID;
// 	uint8_t duplicateRejectionTableSize;
// 	uint8_t numOfCoordinators;
// 	uint32_t dataRequestInterval;
// 	uint32_t maxDataRequestInterval;
	
// 	uint8_t edLinkFailureAttempts;
// 	uint8_t connRespWaitInSec;
// 	uint8_t frameRetry;
// 	uint8_t joinWish;
// #ifndef PAN_COORDINATOR
// 	searchConf_t* searchConfMem;
// 	uint8_t maxNoOfBeacons;
// #endif
// #else
//    CONNECTION_ENTRY  *ConnectionTable;
//    ACTIVE_SCAN_RESULT  *ActiveScanResults;
//    uint8_t *AdditionalNodeID;
//    uint8_t networkFreezerRestore;
// #endif
// } defaultParametersRomOrRam_t;

// typedef struct __defaultParametersRamOnly
// {
// #ifdef MESH_SECURITY
// 	uint8_t miwiPublicKey[SECURITY_KEY_SIZE];
// 	uint8_t miwiNetworkKey[SECURITY_KEY_SIZE];
// 	uint8_t securityLevel;
// #endif
// 	uint8_t dummy;
// } defaultParametersRamOnly_t;


// extern defaultParametersRomOrRam_t defaultParamsRomOrRam;

// extern defaultParametersRamOnly_t defaultParamsRamOnly;


// miwi_status_t MiApp_ProtocolInit(defaultParametersRomOrRam_t *defaultRomOrRamParams,
// 						defaultParametersRamOnly_t *defaultRamOnlyParams);


// bool    MiApp_Set(miwi_params_t id, uint8_t *value);


// #define START_CONN_DIRECT       0x00
// #define START_CONN_ENERGY_SCN   0x01
// #define START_CONN_CS_SCN       0x02

// typedef void (*connectionConf_callback_t)(miwi_status_t status);

// bool    MiApp_StartConnection( uint8_t Mode, uint8_t ScanDuration, uint32_t ChannelMap,
// connectionConf_callback_t ConfCallback);

// typedef void (*SearchConnectionConf_callback_t)(uint8_t foundScanResults, void* ScanResults);

// uint8_t    MiApp_SearchConnection(uint8_t ScanDuration, uint32_t ChannelMap,
// SearchConnectionConf_callback_t ConfCallback);

// #define CONN_MODE_DIRECT        0x00
// #define CONN_MODE_INDIRECT      0x01

// uint8_t    MiApp_EstablishConnection(uint8_t Channel, uint8_t addr_len, uint8_t *addr, uint8_t Capability_info,
// connectionConf_callback_t ConfCallback);


// void    MiApp_RemoveConnection(uint8_t ConnectionIndex);

// #define ENABLE_ALL_CONN         0x00
// #define ENABLE_PREV_CONN        0x01
// #define ENABLE_ACTIVE_SCAN_RSP  0x02
// #define DISABLE_ALL_CONN        0x03

// void    MiApp_ConnectionMode(uint8_t Mode);

// typedef void (*DataConf_callback_t)(uint8_t msgConfHandle, miwi_status_t status, uint8_t* msgPointer);


// bool MiApp_SendData(uint8_t addr_len, uint8_t *addr, uint8_t msglen, uint8_t *msgpointer, uint8_t msghandle,
// bool ackReq, bool secEnabled, DataConf_callback_t ConfCallback);


// #define BROADCAST_TO_ALL            0x01
// #define MULTICAST_TO_COORDINATORS   0x02
// #define MULTICAST_TO_FFDS           0x03

// typedef struct
// {
// union
// {
// uint8_t Val;
// struct
// {
// uint8_t		broadcast:  2;      // 1: broadcast message
// uint8_t		ackReq:     1;      // 1: sender request acknowledgement in MAC.
// uint8_t		secEn:      1;      // 1: application payload has been secured
// uint8_t		repeat:     1;      // 1: message received through a repeater
// uint8_t		command:    1;      // 1: message is a command frame
// uint8_t        srcPrsnt:   1;      // 1: source address present in the packet
// uint8_t        altSrcAddr: 1;      // 1: source address is alternative network address
// } bits;
// }  flags;

// API_UINT16_UNION    SourcePANID;                // PAN Identifier of the sender
// uint8_t		*SourceAddress;             // pointer to the source address
// uint8_t		*Payload;                   // pointer to the application payload
// uint8_t		PayloadSize;                // application payload length
// uint8_t		PacketRSSI;                 // RSSI value of the receive message
// uint8_t 	PacketLQI;                  // LQI value of the received message

// } RECEIVED_MESSAGE;

// #if defined(PROTOCOL_P2P) || defined (PROTOCOL_STAR)
// typedef void (*PacketIndCallback_t)(RECEIVED_MESSAGE *ind);
// #endif

// #if defined(PROTOCOL_MESH)
// typedef struct
// {
// uint16_t	sourceAddress;           // source address
// uint8_t		*payload;                   // pointer to the application payload
// uint8_t		payloadSize;                // application payload length
// uint8_t		packetRSSI;                 // RSSI value of the receive message
// uint8_t 	packetLQI;                  // LQI value of the received message
// } RECEIVED_MESH_MESSAGE;

// #endif 

// #if defined(PROTOCOL_MESH)
// typedef void (*PacketIndCallback_t)(RECEIVED_MESH_MESSAGE *ind);
// #endif

// bool  MiApp_SubscribeDataIndicationCallback(PacketIndCallback_t callback);


// #define NOISE_DETECT_ENERGY 0x00
// #define NOISE_DETECT_CS     0x01

// uint8_t    MiApp_NoiseDetection(uint32_t ChannelMap, uint8_t ScanDuration, uint8_t DetectionMode, OUTPUT uint8_t *NoiseLevel);

// #define POWER_STATE_SLEEP       0x00
// #define POWER_STATE_WAKEUP      0x01
// #define POWER_STATE_WAKEUP_DR   0x02

// uint8_t    MiApp_TransceiverPowerState(uint8_t Mode);


// bool MiApp_InitChannelHopping( uint32_t ChannelMap);

// typedef void (*resyncConnection_callback_t)(uint8_t channel, miwi_status_t status);

// bool MiApp_ResyncConnection(uint8_t ConnectionIndex, uint32_t ChannelMap, resyncConnection_callback_t callback);


// uint8_t Total_Connections(void);

// #if defined (PROTOCOL_STAR)
// // End devices in Star Network get the total no of devices in Ntwrk
// // from PAN Co and they save that in variable end_nodes
// extern uint8_t end_nodes ;
// // Role == PAN CO or END Device
// extern DeviceRole_t role;
// // In case of star network an FFD||RFD End device also saves its index value.
// // Pan Co has connection table where in it store all the FFD || RFD device Information.
// // Each FFD and RFD device is provided a Index by PAN CO on Connection Request
// // this Index is saved by the FFD || RFD end device
// extern  uint8_t  MyindexinPC;

// /***************************************************************************
// * All Peer FFD's && RFD device Information
// * In Star Network PAN CO , broadcasts its Connection Table Information to all its child
// * FFD's && RFD's. This information is gives FFD's && RFD's idea of Peer FFD's || RFD's
// * Presence
// * . It is the element structure.
// * Note : The below structure is used by only end devices(FFD's || RFD's) in a star network
// **************************************************************************/
// typedef struct __END_DEVICES_Unique_Short_Address
// {
// // Short address of peer END device
// uint8_t        Address[3];
// //Each end device belongs to a particular connection slot in PAN CO
// uint8_t        connection_slot;

// } END_DEVICES_Unique_Short_Address;
// extern  END_DEVICES_Unique_Short_Address  END_DEVICES_Short_Address[CONNECTION_SIZE];
// // lost connection is true if a END device is considered In active by PAN CO
// // though the device is active
// extern bool lost_connection;
// //Used by END_DEVICE to store the index value shared by PAN Co on join
// extern uint8_t myConnectionIndex_in_PanCo;

// #endif


// bool  MiApp_Get(miwi_params_t id, uint8_t *value);

// #if defined(ENABLE_NETWORK_FREEZER)

// bool MiApp_ResetToFactoryNew(void);
// #endif

// #if defined(PROTOCOL_P2P) || defined (PROTOCOL_STAR)
// void P2PTasks(void);
// void DumpConnection(INPUT uint8_t index);
// #endif
// #if defined(PROTOCOL_MESH)
// void MeshTasks(void);
// #endif

// typedef void (*ReconnectionCallback_t)(miwi_status_t status);

// bool MiApp_SubscribeReConnectionCallback(ReconnectionCallback_t callback);

// typedef void (*roleUpgrade_callback_t)(uint16_t newShortAddress);
// #ifdef COORDINATOR

// bool MiApp_RoleUpgradeNotification_Subscribe(roleUpgrade_callback_t callback);
// #endif


// bool MiApp_Commissioning_AddNewDevice(uint64_t joinerAddress, bool triggerBloomUpdate);

// #if defined(PROTOCOL_MESH)

// uint16_t MiApp_MeshGetNextHopAddr(uint16_t destAddress);


// bool MiApp_ManuSpecSendData(uint8_t addr_len, uint8_t *addr, uint8_t msglen, uint8_t *msgpointer,
// uint8_t msghandle, bool ackReq, DataConf_callback_t ConfCallback);


// bool  MiApp_SubscribeManuSpecDataIndicationCallback(PacketIndCallback_t callback);

// bool MiApp_IsConnected(void);

// #ifdef MIWI_MESH_TOPOLOGY_SIMULATION_MODE
// void MiApp_MeshSetRouteEntry(uint8_t coordIndex, RouteEntry_t *routeEntry);
// void MiApp_MeshGetRouteEntry(uint8_t coordIndex, RouteEntry_t *routeEntry);
// #endif
// #endif


// bool MiApp_ReadyToSleep(uint32_t* sleepTime);

// typedef void (*LinkFailureCallback_t)(void);

// bool MiApp_SubscribeLinkFailureCallback(LinkFailureCallback_t callback);

// // Callback functions
// #define MiApp_CB_AllowConnection(handleInConnectionTable) true
// //BOOL MiApp_CB_AllowConnection(uint8_t handleInConnectionTable);
// #define MiApp_CB_RFDAcknowledgement(SourceShortAddress, AcknowledgementSeqNum)
// //void MiApp_CB_RFDAcknowledgement(uint16_t sourceAddr, uint8_t Seq);
// extern RECEIVED_MESSAGE rxMessage;
// #if defined(ENABLE_TIME_SYNC)
// extern API_UINT16_UNION CounterValue;
// extern API_UINT16_UNION WakeupTimes;
// #endif
// #endif
