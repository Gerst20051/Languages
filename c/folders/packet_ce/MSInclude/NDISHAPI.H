#ifndef __NDISHAPI_H__
#define __NDISHAPI_H__

/* Header For IOCTL's
---------------------
 * This header references the CTL_CODE macro and the METHOD_XYZ definitions.
 * These are defined in the VC/SDK WinIoctl.h header for applications and
 * in the DDK DevIoctl.h for drivers. Unfortunately, including WinIoctl.h
 * when compiling a NT device drivers causes problems.
 *
 * The following preprocessor directive includes the <WinIoctl.h> header
 * when compiling a Win32 application or DLL. The driver includes DevIoctl.h
 * through a different mechanism.
 */

#ifdef _WINDOWS
#include <WinIoctl.h>   // Compiling Win32 Applications Or DLL's
#endif // _WINDOWS

#include "PCAEnet.h"
#include "W32Ndis.h"

/////////////////////////////////////////////////////////////////////////////
//// INCLUDE FILES

// Copyright And Configuration Management ----------------------------------
//
//               PCANDIS Protocol Driver IOCTL Codes - NdisHApi.H
//
//                      Win32 NDIS Framework (WinDis 32)
//                                    For
//                         Windows NT And Windows 9X
//
//     Copyright (c) 1995-2000, Printing Communications Associates, Inc.
//                          http://www.pcausa.com
//
//                             Thomas F. Divine
//                           4201 Brunswick Court
//                        Smyrna, Georgia 30080 USA
//                              (770) 432-4580
//                         mailto:tdivine@pcausa.com
//
//
// 
// End ---------------------------------------------------------------------

//
// PCANDIS Transport Name Strings
//
#define PCANDIS3_BASE_NAME_A        "PCANDIS3"
#define PCANDIS3_BASE_NAME_W        L"PCANDIS3"
#define PCANDIS3_PROTOCOL_NAME_A    PCANDIS3_BASE_NAME_A
#define PCANDIS3_PROTOCOL_NAME_W    PCANDIS3_BASE_NAME_W
#define PCANDIS3_DISPLAY_NAME_A     "PCANDIS3 Protocol Driver"
#define PCANDIS3_DISPLAY_NAME_W     L"PCANDIS3 Protocol Driver"

#define PCANDIS4_BASE_NAME_A        "PCANDIS4"
#define PCANDIS4_BASE_NAME_W        L"PCANDIS4"
#define PCANDIS4_PROTOCOL_NAME_A    PCANDIS4_BASE_NAME_A
#define PCANDIS4_PROTOCOL_NAME_W    PCANDIS4_BASE_NAME_W
#define PCANDIS4_DISPLAY_NAME_A     "PCANDIS4 Protocol Driver"
#define PCANDIS4_DISPLAY_NAME_W     L"PCANDIS4 Protocol Driver"
#define PCANDIS4_DEVICE_NAME_A      "\\Device\\PCANDIS4"
#define PCANDIS4_DEVICE_NAME_W      L"\\Device\\PCANDIS4"

#define PCANDIS5_BASE_NAME_A        "PCANDIS5"
#define PCANDIS5_BASE_NAME_W        L"PCANDIS5"
#define PCANDIS5_PROTOCOL_NAME_A    PCANDIS5_BASE_NAME_A
#define PCANDIS5_PROTOCOL_NAME_W    PCANDIS5_BASE_NAME_W
#define PCANDIS5_DISPLAY_NAME_A     "PCANDIS5 Protocol Driver"
#define PCANDIS5_DISPLAY_NAME_W     L"PCANDIS5 Protocol Driver"
#define PCANDIS5_DEVICE_NAME_A      "\\Device\\PCANDIS5"
#define PCANDIS5_DEVICE_NAME_W      L"\\Device\\PCANDIS5"

#ifdef UNICODE

#define PCANDIS3_BASE_NAME       PCANDIS3_BASE_NAME_W
#define PCANDIS3_PROTOCOL_NAME   PCANDIS3_BASE_NAME_W
#define PCANDIS3_DISPLAY_NAME    PCANDIS3_DISPLAY_NAME_W

#define PCANDIS4_BASE_NAME       PCANDIS4_BASE_NAME_W
#define PCANDIS4_PROTOCOL_NAME   PCANDIS4_BASE_NAME_W
#define PCANDIS4_DISPLAY_NAME    PCANDIS4_DISPLAY_NAME_W
#define PCANDIS4_DEVICE_NAME     PCANDIS4_DEVICE_NAME_W

#define PCANDIS5_BASE_NAME       PCANDIS5_BASE_NAME_W
#define PCANDIS5_PROTOCOL_NAME   PCANDIS5_BASE_NAME_W
#define PCANDIS5_DISPLAY_NAME    PCANDIS5_DISPLAY_NAME_W
#define PCANDIS5_DEVICE_NAME     PCANDIS5_DEVICE_NAME_W

#else

#define PCANDIS3_BASE_NAME       PCANDIS3_BASE_NAME_A
#define PCANDIS3_PROTOCOL_NAME   PCANDIS3_BASE_NAME_A
#define PCANDIS3_DISPLAY_NAME    PCANDIS3_DISPLAY_NAME_A

#define PCANDIS4_BASE_NAME       PCANDIS4_BASE_NAME_A
#define PCANDIS4_PROTOCOL_NAME   PCANDIS4_BASE_NAME_A
#define PCANDIS4_DISPLAY_NAME    PCANDIS4_DISPLAY_NAME_A
#define PCANDIS4_DEVICE_NAME     PCANDIS4_DEVICE_NAME_A

#define PCANDIS5_BASE_NAME       PCANDIS5_BASE_NAME_A
#define PCANDIS5_PROTOCOL_NAME   PCANDIS5_BASE_NAME_A
#define PCANDIS5_DISPLAY_NAME    PCANDIS5_DISPLAY_NAME_A
#define PCANDIS5_DEVICE_NAME     PCANDIS4_DEVICE_NAME_A

#endif


//
// WinDis 32 API Version Information
// ---------------------------------
// Make sure that this is coordinated with information in the VERSION
// resource.
//
#define PCANDIS_API_VERSION     0x05000D31

/////////////////////////////////////////////////////////////////////////////
//// Linked List Definitions And Macros
//
// Modeled after Kernel-Mode/NDIS LIST_ENTRY functionality

typedef
struct _W32N_LIST_ENTRY
{
   struct _W32N_LIST_ENTRY *Flink;
   struct _W32N_LIST_ENTRY *Blink;
}
   W32N_LIST_ENTRY, *PW32N_LIST_ENTRY;

//
//  VOID
//  W32N_InitListHead(
//      PW32N_LIST_ENTRY ListHead
//      );
//

#define W32N_InitListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead) )

//
//  BOOLEAN
//  W32N_IsListEmpty(
//      PW32N_LIST_ENTRY ListHead
//      );
//

#define W32N_IsListEmpty(ListHead) (\
    ( ((ListHead)->Flink == (ListHead)) ? TRUE : FALSE ) )

//
//  PW32N_LIST_ENTRY
//  W32N_RemoveHeadList(
//      PW32N_LIST_ENTRY ListHead
//      );
//

#define W32N_RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {\
    PW32N_LIST_ENTRY FirstEntry;\
    FirstEntry = (ListHead)->Flink;\
    FirstEntry->Flink->Blink = (ListHead);\
    (ListHead)->Flink = FirstEntry->Flink;\
    }

//
//  PW32N_LIST_ENTRY
//  W32N_RemoveTailList(
//      PW32N_LIST_ENTRY ListHead
//      );
//

#define W32N_RemoveTailList(ListHead) \
    (ListHead)->Blink;\
    {W32N_RemoveEntryList((ListHead)->Blink)}

//
//  VOID
//  W32N_RemoveEntryList(
//      PW32N_LIST_ENTRY Entry
//      );
//

#define W32N_RemoveEntryList(Entry) {\
    PW32N_LIST_ENTRY _EX_Entry;\
    _EX_Entry = (Entry);\
    _EX_Entry->Blink->Flink = _EX_Entry->Flink;\
    _EX_Entry->Flink->Blink = _EX_Entry->Blink;\
    }

//
//  VOID
//  W32N_InsertTailList(
//      PW32N_LIST_ENTRY ListHead,
//      PW32N_LIST_ENTRY Entry
//      );
//

#define W32N_InsertTailList(ListHead,Entry) \
    (Entry)->Flink = (ListHead);\
    (Entry)->Blink = (ListHead)->Blink;\
    (ListHead)->Blink->Flink = (Entry);\
    (ListHead)->Blink = (Entry)

//
//  VOID
//  W32N_InsertHeadList(
//      PW32N_LIST_ENTRY ListHead,
//      PW32N_LIST_ENTRY Entry
//      );
//

#define W32N_InsertHeadList(ListHead,Entry) \
    (Entry)->Flink = (ListHead)->Flink;\
    (Entry)->Blink = (ListHead);\
    (ListHead)->Flink->Blink = (Entry);\
    (ListHead)->Flink = (Entry)


/////////////////////////////////////////////////////////////////////////////
//// Function Codes For PCANDIS Protocol (Transport) Driver
//
// Passed in AL register on DeviceioControl call.
// Macros and public IOCTL device codes are defined in WINIOCTL.H.
//

#define FILE_DEVICE_PROTOCOL        0x8000
#define IOCTL_PCANDIS_BASE         FILE_DEVICE_PROTOCOL

#define IOCTL_PCANDIS_BIND_ADAPTER\
   CTL_CODE(IOCTL_PCANDIS_BASE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_UNBIND_ADAPTER\
   CTL_CODE(IOCTL_PCANDIS_BASE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_OPEN_ADAPTER\
   CTL_CODE(IOCTL_PCANDIS_BASE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_CLOSE_ADAPTER\
   CTL_CODE(IOCTL_PCANDIS_BASE, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_REQUEST\
   CTL_CODE(IOCTL_PCANDIS_BASE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_BUFFER_SEND\
   CTL_CODE(IOCTL_PCANDIS_BASE, 5, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_BUFFER_SEND_EX\
   CTL_CODE(IOCTL_PCANDIS_BASE, 6, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_PACKET_READ\
   CTL_CODE(IOCTL_PCANDIS_BASE, 7, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_PACKET_READ_EX\
   CTL_CODE(IOCTL_PCANDIS_BASE, 8, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_CANCEL_PACKET_READ\
   CTL_CODE(IOCTL_PCANDIS_BASE, 9, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_PRIVATE_REQUEST\
   CTL_CODE(IOCTL_PCANDIS_BASE, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PCANDIS_SET_BPF_PROGRAM\
   CTL_CODE(IOCTL_PCANDIS_BASE, 11, METHOD_BUFFERED, FILE_ANY_ACCESS)


/////////////////////////////////////////////////////////////////////////////
//// W32N_REQUEST Structure Definition

#pragma pack(1)      // x86, MS compiler; MIPS, MIPS compiler

// Special Build For OEM 549
// -------------------------
// Prvate OEM build, never released through normal channels. Not compatible
// with standard WinDis 32 releases.
//
//#define OEM_549

#define W32N_REQUEST_BUFFSIZE_OLD 512

typedef
struct _W32N_REQUEST
{
   NDIS_STATUS       nRequestStatus;   // Request Completion Status

   W32N_NDIS_REQUEST NdisRequest;

   //
   // ATTENTION!!! RequestBuffer NOT USED starting with V5.00.13.47
   // -------------------------------------------------------------
   // It is retained in an attempt to maintain compatibility with
   // V5.00.13.46.
   //
   UCHAR Reserved[ W32N_REQUEST_BUFFSIZE_OLD ];  // Was RequestBuffer...
}   
   W32N_REQUEST, *PW32N_REQUEST;


/////////////////////////////////////////////////////////////////////////////
//// Private OID's To Access PCANDIS Driver Objects
//
// Call the driver using IOCTL_PCANDIS_PRIVATE_REQUEST to make private
// requests.
//
// The following "standard" OID's are also supported via the private
// request mechanism:
//   OID_GEN_SUPPORTED_LIST
//   OID_GEN_DRIVER_VERSION
//   OID_GEN_VENDOR_DESCRIPTION
//   OID_GEN_XMIT_OK
//   OID_GEN_RCV_OK
//

#define   OID_PCANDIS_RCV_ERROR_KERNEL_RESOURCE      0xF0020101
   // The number of times the protocol driver cannot receive due to a
   // lack of resources at the protocol driver level.

#define   OID_PCANDIS_RCV_ERROR_USER_RESOURCE      0xF0020102
   // The number of times the protocol driver cannot receive because
   // a Win32 packet read was not pending when it was required.

#define   OID_PCANDIS_API_VERSION                  0xF0020103
   // Returns a long integer containing the PCANDIS_API_VERSION.

#define   OID_PCANDIS_BOUND_ADAPTER_LIST            0xF0020104
   // Returns information describing the adapters that the PCANDIS protocol
   // driver is currently bound to.
   //
   // If successful, InformationBuffer will be filled with one or more
   // PCANDIS_ADAPTER_INFO structures.
   //
   // A valid PCANDIS_ADAPTER_INFO structure has a non-zero value in the
   // nAdapterInfoFlags field.
   //
   // The adapter's "AdapterName", as passed to W32N_OpenAdapter(), is
   // returned in the InformationBuffer:
   //
   //   The nAdapterNameLength field specifies the length of the "AdapterName",
   //   including a zero terminating byte. The zero-terminated "AdapterName"
   //   string can be found in the InformationBuffer. The PCANDIS driver
   //   will place the zero-terminated "AdapterName" string in the caller's
   //   buffer at a location of it's choosing. The nAdapterNameOffset field
   //   will contain the offset of the zero-terminated "AdapterName" string
   //   relative to the begining of the InformationBuffer.
   //
   // The first PCANDIS_ADAPTER_INFO structure with a zero value for 
   // nAdapterInfoFlags terminates the adapter information list. Thus,
   // if ther are no bound adapters, the first ULONG in the returned
   // InformationBuffer will be zero.
   //

#define   OID_PCANDIS_XMIT_ERROR_KERNEL_RESOURCE      0xF0020105
   // The number of times the protocol driver cannot send due to a
   // lack of resources at the protocol driver level.

typedef
struct _PCANDIS_ADAPTER_INFO
{
   ULONG    nAdapterInfoFlags;
   ULONG    nAdapterNameLength;   // Length Of Adapter Name, Including Zero Terminator
   LONG     nAdapterNameOffset;
}
   PCANDIS_ADAPTER_INFO, *PPCANDIS_ADAPTER_INFO;

/* Definitions For nAdapterInfoFlags Field (Bit Flags...)
--------------------------------------------------------- */
#define  PCANDIS_END_OF_LIST     0x00000000
#define  PCANDIS_SYSTEM_BINDING  0x00000001
#define  PCANDIS_USER_BINDING    0x00000002

#define  OID_PCANDIS_FLAGS       0xF0020106
   // Returns flags which are interpreted by W32NdisApi

#define  PCANDIS_FLAGS_SYSTEM_BINDING     0x00000001
#define  PCANDIS_FLAGS_BIND_LIST_EMPTY    0x00000002


#define  OID_PCANDIS_DISABLE_LOOPBACK     0xF0020107
   // Command the PCANDIS driver to reject loopback packet.

typedef
struct _PCANDIS_LOOPBACK_INFO
{
   ULONG    nLinkAddrOffset;      // Offset Of Link Source Address Into Packet
   ULONG    nLinkAddrLength;      // Length Of Source Address
   UCHAR    LinkAddrBytes[ 1 ];   // Actually, nLinkAddrLength Bytes
}
   PCANDIS_LOOPBACK_INFO, *PPCANDIS_LOOPBACK_INFO;



#define  OID_PCANDIS_SET_TIMESTAMP_RESOLUTION   0xF0020108
   // Set the PCANDIS receive packet timestamp resolution
   // Pass ponter to a BOOLEAN as InformationBuffer data...

#define  OID_PCANDIS_GET_TIMESTAMP_INFO         0xF0020109
   // Get the PCANDIS receive packet timestamp resolution, scaling factors
   // and time synchronization information. This information would be used
   // by a user mode application to properly display packet timestamps.

typedef
struct _PCANDIS_TIMESTAMP_INFO
{
   BOOLEAN        bHiResTimestamp;
   BOOLEAN        bIsWindowsNT;     // TRUE for Windows NT or Windows 2000
   LARGE_INTEGER  Timestamp;
   LARGE_INTEGER  Frequency;

}
   PCANDIS_TIMESTAMP_INFO, *PPCANDIS_TIMESTAMP_INFO;

/////////////////////////////////////////////////////////////////////////////
//// W32N_PACKET Structure Definition
//

typedef
struct _W32N_PACKET
{
   ULONG          nUserReserved1;
   ULONG          nUserReserved2;

   ULONG          nDriverReserved1;
   ULONG          nDriverReserved2;
   ULONG          nDriverReserved3;
   ULONG          nDriverReserved4;

   NDIS_STATUS    nNdisStatus;      // Completion NDIS Status

   LARGE_INTEGER  nPacketNumber;

   LARGE_INTEGER  TimeReceived;      // See notes below.

   UINT           nPacketDataLength;// Actual Size Of Network Packet (Header + Data)
   UINT           nBufferDataLength;// Bytes Written To Buffer At PacketBuffer
                                    // Note: May be less then nPacketDataLength

   UINT           nBufferSize;      // Size Of Buffer At PacketBuffer
   UCHAR          PacketBuffer[ 1 ];// Placeholder. See Notes.
}
   W32N_PACKET, *PW32N_PACKET;

/* Notes:
---------
 * 1.) PacketBuffer Field
 * ----------------------
 * This is a one (1) byte placeholder which locates the first byte of the
 * packet buffer. The caller must allocate at least (nBufferSize - 1) bytes
 * of additional memory space immediately following PacketBuffer.
 *
 * 2.) Length Fields
 * -----------------
 * nBufferSize - This field specifies the size of the buffer at PacketBuffer.
 * This value is specified by the caller before calling W32N_PacketRead().
 *
 * nPacketDataLength - When a packet is received the PCANDIS driver fills
 * this field with the actual size of the network packet.
 *
 * nBufferDataLength - When a packet is received the PCANDIS driver will
 * save received packet data at the buffer at PacketBuffer. This field
 * indicates the ACTUAL number of bytes which were saved by the PCANDIS
 * driver in PacketBuffer. This value will always be less then of equal
 * nBufferSize, and may be less then nPacketDataLength.
 * 
 * 3.) Time Fields
 * ---------------
 * High-Resolution Timestamp Mode
 *   On the Windows NT platform, the TimeReceived timestamp is obtained from
 *   KeQueryPerformanceCounter(). (Which see.)
 *
 *   On the Windows 9X platform the TimeReceived timestamp is obtained from
 *   VTD_Get_Real_Time().
 *
 * Low-Resolution Timestamp Mode (Default)
 *   On the Windows NT platform, the TimeReceived timestamp is obtained from
 *   KeQuerySystemTime(). (Which see.)
 *
 *   On the Windows 9X platform the TimeReceived timestamp is obtained from
 *   Get_System_Time(), which has a 1 ms accuracy.
 *
 * The frequency of the timestamp clock can be fetched using the
 * W32N_GetTimestampInformation function introduced in V5.00.13.47.
 */

#pragma pack()                  // x86, MS compiler; MIPS, MIPS compiler

#endif // __NDISHAPI_H__

