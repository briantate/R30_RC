#include "miwi_api.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {

miwi_status_t MiApp_ProtocolInit(defaultParametersRomOrRam_t *defaultRomOrRamParams,
    defaultParametersRamOnly_t *defaultRamOnlyParams)
    {
        mock().actualCall("MiApp_ProtocolInit")
              // Use withParameter to handle void* consistently
              .withParameter("romParams", (void*)defaultRomOrRamParams)
              .withParameter("ramParams", (void*)defaultRamOnlyParams);

        return (miwi_status_t)mock().unsignedIntReturnValue();
    }


// bool    MiApp_Set(miwi_params_t id, uint8_t *value);

// typedef void (*connectionConf_callback_t)(miwi_status_t status);

// bool    MiApp_StartConnection( uint8_t Mode, uint8_t ScanDuration, uint32_t ChannelMap,
//             connectionConf_callback_t ConfCallback);

// typedef void (*SearchConnectionConf_callback_t)(uint8_t foundScanResults, void* ScanResults);

// uint8_t    MiApp_SearchConnection(uint8_t ScanDuration, uint32_t ChannelMap,
//                 SearchConnectionConf_callback_t ConfCallback);


// uint8_t    MiApp_EstablishConnection(uint8_t Channel, uint8_t addr_len, uint8_t *addr, uint8_t Capability_info,
//             connectionConf_callback_t ConfCallback);


// void    MiApp_RemoveConnection(uint8_t ConnectionIndex);
// void    MiApp_ConnectionMode(uint8_t Mode);

// typedef void (*DataConf_callback_t)(uint8_t msgConfHandle, miwi_status_t status, uint8_t* msgPointer);


// bool MiApp_SendData(uint8_t addr_len, uint8_t *addr, uint8_t msglen, uint8_t *msgpointer, uint8_t msghandle,
//         bool ackReq, bool secEnabled, DataConf_callback_t ConfCallback);

// #if defined(PROTOCOL_MESH)
// typedef void (*PacketIndCallback_t)(RECEIVED_MESH_MESSAGE *ind);
// #endif

bool  MiApp_SubscribeDataIndicationCallback(PacketIndCallback_t callback)
{
    mock().actualCall("MiApp_SubscribeDataIndicationCallback")
              .withParameter("callback", callback);

        return (bool)mock().boolReturnValue();
}

// uint8_t    MiApp_NoiseDetection(uint32_t ChannelMap, uint8_t ScanDuration, uint8_t DetectionMode, OUTPUT uint8_t *NoiseLevel);

// uint8_t    MiApp_TransceiverPowerState(uint8_t Mode);


// bool MiApp_InitChannelHopping( uint32_t ChannelMap);

// typedef void (*resyncConnection_callback_t)(uint8_t channel, miwi_status_t status);

// bool MiApp_ResyncConnection(uint8_t ConnectionIndex, uint32_t ChannelMap, resyncConnection_callback_t callback);


// uint8_t Total_Connections(void);


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

}