
/* The header file required for every c file */
#ifndef __PACKET_H__
#define __PACKET_H__

// include the header files required by this file here
#include <ntcompat.h>
#include <ndis.h>
#include "support.h"

#ifdef DRIVER_EXPORTS
#define DRIVER_API __declspec(dllexport)
#else
#define DRIVER_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SAFE_SET(_x, _y) if ((_x)) *(_x) = (_y)
#define NDIS_API
//#define MAX_REQUESTS			4
#define MAX_PACKET_LENGTH		1514
#define TRANSMIT_PACKETS		32

#define PKT_FILE_HANDLE			0xA5A5
#define PKT_DEVICE_CONTEXT		0x4545						// the protocol device context
#define PROTOCOL_NAME			NDIS_STRING_CONST("PKT")	// the protocol name
#define SH_EVENT_NAME			L"PKT0EV00"					// shared event name case sensitive

/////////////////////////////////////////////////////////////////////////////////////
// Macros.


// NDIS Address macro constant. needs to be changed
extern const NDIS_PHYSICAL_ADDRESS NDIS_ADDR_M1;



#define IMMEDIATE 1					// immediate timeout

/////////////////////////////////////////////////////////////////////////////////////
// Function prototypes

// Lower egde functions
VOID NDIS_API PacketReceiveComplete (IN NDIS_HANDLE ProtocolBindingContext);
VOID NDIS_API PacketStatusComplete (IN NDIS_HANDLE  ProtocolBindingContext);
VOID NDIS_API PacketResetComplete (IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS  Status);
VOID NDIS_API PacketUnbindAdapter (OUT PNDIS_STATUS Status, IN NDIS_HANDLE BindContext, IN NDIS_HANDLE UnbindContext); 
VOID NDIS_API PacketBindAdapter (OUT PNDIS_STATUS Status, IN NDIS_HANDLE BindContext, IN PNDIS_STRING DeviceName, IN PVOID SystemSpecific1, IN PVOID SystemSpecific2);
VOID NDIS_API PacketCloseAdapterComplete (IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS  Status);
VOID NDIS_API PacketWriteComplete (IN NDIS_HANDLE ProtocolBindingContext, IN PNDIS_PACKET  pPacket, IN NDIS_STATUS   Status);
VOID NDIS_API PacketRequestComplete (IN NDIS_HANDLE ProtocolBindingContext, IN PNDIS_REQUEST pRequest, IN NDIS_STATUS Status);
VOID NDIS_API PacketOpenAdapterComplete (IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS  Status, IN NDIS_STATUS  OpenErrorStatus);
VOID NDIS_API PacketStatus (IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_STATUS Status, IN PVOID StatusBuffer, IN UINT StatusBufferSize);
VOID NDIS_API PacketTransferDataComplete (IN NDIS_HANDLE	ProtocolBindingContext, IN PNDIS_PACKET	Packet, IN NDIS_STATUS Status, IN UINT BytesTransferred);
NDIS_STATUS NDIS_API PacketReceive (IN NDIS_HANDLE ProtocolBindingContext, IN NDIS_HANDLE	MacReceiveContext, IN PVOID	HeaderBuffer, IN UINT HeaderBufferSize, IN PVOID LookAheadBuffer, IN UINT LookaheadBufferSize, IN UINT PacketSize);

// Helper functions for lower edge protocol functions

// Upper edge exported functions
DWORD DRIVER_API PKT_Init(DWORD);
BOOL DRIVER_API PKT_Deinit(DWORD);
DWORD DRIVER_API PKT_Seek(DWORD, LONG, WORD);
DWORD DRIVER_API PKT_Open(DWORD, DWORD, DWORD);
BOOL DRIVER_API PKT_Close(DWORD);
DWORD DRIVER_API PKT_Read(DWORD, LPVOID, DWORD);
DWORD DRIVER_API PKT_Write(DWORD, LPCVOID, DWORD);
BOOL DRIVER_API PKT_IOControl(DWORD, DWORD, PBYTE, DWORD, PBYTE, DWORD, PDWORD);
VOID DRIVER_API PKT_PowerUp(VOID);
VOID DRIVER_API PKT_PowerDown(VOID);


/////////////////////////////////////////////////////////////////////////////////////
// Structure definitions


// Structure for reserved packet
typedef struct _PACKET_RESERVED 
{
	LIST_ENTRY	ListElement;
	char*		lpBuffer;
	DWORD		cbBuffer;
	DWORD*		lpcbBytesReturned;
}PACKET_RESERVED, *PPACKET_RESERVED;


// Structure to represent the open instance of the adapter
typedef struct _OPEN_INSTANCE
{
	NDIS_STATUS			Status;						// 
	NDIS_HANDLE			AdapterHandle;				// 
	NDIS_HANDLE			BindAdapterContext;

	PUCHAR				Buffer;						// 
	NDIS_HANDLE			BufferPool;					// 
    NDIS_SPIN_LOCK      BufferLock;

	NDIS_REQUEST		Request;
	NDIS_HANDLE			PacketPool;					// 
	
	PUCHAR				bpfprogram;					// 
	UINT				bpfprogramlen;				// 
	
	LARGE_INTEGER		StartTime;					// 
	
	UINT				Bhead;						// 
	UINT				Btail;						// 
	UINT				BufSize;					// 
	UINT				BLastByte;					// 

	UINT				MinToCopy;					// 

	UINT				TimeOut;					// 
	HANDLE				ReadEvent;					// 
}OPEN_INSTANCE, *POPEN_INSTANCE;



// This structure is specific to the device driver
typedef struct _DEVICE_EXTENSION 
{
	NDIS_HANDLE			NdisProtocolHandle;
	POPEN_INSTANCE		pOpenInstance;
	LIST_ENTRY			listAdapterNames;
}DEVICE_EXTENSION, *PDEVICE_EXTENSION; 

extern PDEVICE_EXTENSION g_pDeviceExtension;


// Structure to store the names of adapters available
typedef struct _ADAPTER_NAME
{
LIST_ENTRY			ListElement;
//char				chRealName[32];
WCHAR				chDeviceName[32];
NDIS_STRING			ndstrDeviceName;
}ADAPTER_NAME, *PADAPTER_NAME;


#define RESERVED(_p)	((PPACKET_RESERVED)((_p)->ProtocolReserved))

// Helper functions for upper edge exported functions
VOID SuspendExecution (POPEN_INSTANCE);
VOID ResumeExecution (POPEN_INSTANCE, NDIS_STATUS);

DWORD PKTGetMacNameList (PWCHAR, DWORD);
BOOL  PKTOpenAdapter (PNDIS_STRING);
BOOL  PKTCloseAdapter ();
BOOL  PKTReset (POPEN_INSTANCE);
VOID PacketCancelRoutine (POPEN_INSTANCE);

PNDIS_STRING PKTGetNDISAdapterName (PWCHAR, DWORD);
VOID PKTFreeResources (POPEN_INSTANCE);
NDIS_STATUS PKTBindNames ();
BOOL PKTRequest (POPEN_INSTANCE, DWORD, PVOID, DWORD, PBYTE, DWORD, PDWORD);


BOOL PKTRead (POPEN_INSTANCE, PBYTE, DWORD, PDWORD);
BOOL PKTWrite (POPEN_INSTANCE, PVOID, DWORD, PDWORD);

// WinCE undeclared kernel mode functions.
HANDLE GetCallerProcess (VOID);
LPVOID MapPtrToProcess (LPVOID lpv, HANDLE hProc);

#ifdef __cplusplus
}
#endif

#endif //__PACKET_H__