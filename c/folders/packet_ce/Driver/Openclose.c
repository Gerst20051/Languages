#include <windows.h>
#include "packet.h"

/*
 * Relesae the time slice of execution
 */

// Donot Optimize this code
#pragma optimize ("", off)

VOID SuspendExecution (POPEN_INSTANCE pOI)
{
	volatile NDIS_STATUS Status;

	Status = pOI->Status;
	while (Status == NDIS_STATUS_PENDING) {
		// release the time slice
		Sleep (0);
		Status = pOI->Status;
	}
}

VOID ResumeExecution (POPEN_INSTANCE pOI, NDIS_STATUS Status)
{
	// Set the status
	pOI->Status = Status;
}
#pragma optimize ("", on)

/*
 * This function invariably return the constant PKT_OPEN_CONTEXT. 
 * this is done beause at this point we donot know the adapter to which
 * our protocol should bind.
 *
 */
DWORD PKT_Open(DWORD dwContext, DWORD accessCode, DWORD ShareMode)
{
	// if the adapter is already opened then cannot open
	// one more instance.
	if (g_pDeviceExtension->pOpenInstance == NULL &&
		dwContext == PKT_DEVICE_CONTEXT) {
		return PKT_FILE_HANDLE;
	} else {
		return -1;
	}
}


/*
** This function does the cleanup after adapter is closed
*/
BOOL PKT_Close(DWORD dwContext)
{
	if (dwContext != PKT_FILE_HANDLE) {
		return FALSE;
	}

	return TRUE;
}

/*
** This function does nothing
*/
VOID PKT_PowerUp(VOID)
{
	return;
}


/*
** This function does nothing
*/
VOID PKT_PowerDown(VOID)
{
	return;
}

VOID PacketBindAdapter (OUT PNDIS_STATUS Status, IN NDIS_HANDLE BindContext, 
						IN PNDIS_STRING DeviceName, IN PVOID SS1, IN PVOID SS2)
{
	return;
}

/*
 * Copies all the adapter names in the out buffer
 * This function is not called directly. It is called by
 * IO control routine
 *
 */
DWORD PKTGetMacNameList (PWCHAR pBufOut, DWORD dwLenOut)
{

	PADAPTER_NAME	pAName;
	DWORD			dwRet = 0;
	PLIST_ENTRY		pEntry;
	PLIST_ENTRY		pHead;

	// Iterate through all the available adapters
	// and add the adapter names to the out buffer

	if (pBufOut == NULL || dwLenOut == 0) {
		return 0;
	}

	// check if the list is empty
	if (IsListEmpty (&g_pDeviceExtension->listAdapterNames)) {
		return 0;
	}

	// get the head element
	pHead = &g_pDeviceExtension->listAdapterNames;

	// get first entry
	pEntry = pHead->Flink;

	do {
		// get the record
		pAName = CONTAINING_RECORD (pEntry, ADAPTER_NAME, ListElement);

		// check if output buffer can hold the string
		if ((INT)(dwLenOut - (dwRet + wcslen (pAName->chDeviceName) + 1)) <= 0) {
			break;
		}

		// add the string to the outpur buffer
		wcscpy (pBufOut + dwRet, pAName->chDeviceName);
		dwRet = dwRet + wcslen (pAName->chDeviceName) + 1;

		// get next entry
		pEntry = pEntry->Flink;
	} while (pEntry != pHead);
	
	// append one more zero
	pBufOut[dwRet] = L'\0';

	return dwRet;
}

/*
 * This function checks whether the adapter name passed to the 
 * open adapter function exists and is valid.
 *
 * Parameters
 *		pName	- The name of the adapter to which protocol should bind
 *		dwLenIn	- The length of the in buffer
 *
 * Return Value
 *		Returns pointer to ndis string object if the adapter is available
 *		otherwise returns NULL
 *
 */
PNDIS_STRING PKTGetNDISAdapterName (PWCHAR pName, DWORD dwLenIn)
{
    PADAPTER_NAME	pAName;
	PLIST_ENTRY		pHead = &g_pDeviceExtension->listAdapterNames;
	PLIST_ENTRY		pEntry;

	// check the parameters
	if (pName == NULL || dwLenIn == 0) {
		return NULL;
	}

	// check if the adapter name list is empty
	if (IsListEmpty (pHead)){	
			return NULL;
	}

	// get the first pointer
	pEntry = pHead->Flink; 

	do {
		// retrieve the element
		pAName = CONTAINING_RECORD (pEntry, ADAPTER_NAME, ListElement);

		// check if the names are matching
		if(wcscmp(pAName->chDeviceName, pName) == 0) {
			return &pAName->ndstrDeviceName;
		}

		// get next pointer
		pEntry=pEntry->Flink;
	}while (pEntry != pHead); 

	return NULL;
}


/*
 * The mediums for which this protocol can bind
 *
 */
static NDIS_MEDIUM MediumArray[] = {
    NdisMedium802_3,
    NdisMedium802_5,
	NdisMediumWan,
    NdisMediumFddi,
    NdisMediumIrda
};
#define NUM_NDIS_MEDIA  (sizeof MediumArray / sizeof MediumArray[0])

/*
 * Opens the specified adapter and binds the protocol to it
 *
 * Arguments
 *		AdapterName		- The name of the adapter with which binding should happen
 *
 * Return Value
 *		Return TRUE if successfully bound otherwise returns FALSE
 *
 */
BOOL PKTOpenAdapter (PNDIS_STRING pAdapterName)
{
	POPEN_INSTANCE	pOI;
	NDIS_STATUS		nsOpen;
	NDIS_STATUS		nsError;
	SYSTEMTIME		SystemTime;
	FILETIME		FileTime;
	LARGE_INTEGER	liSysTime;
	LARGE_INTEGER	TimeFreq;
	LARGE_INTEGER	PTime;
	UINT			uiMedium;


	// Check if an instance is already opened
	if (g_pDeviceExtension->pOpenInstance) {
		SetLastError (ERROR_ALREADY_INITIALIZED);
		return FALSE;
	}

	// Time initialization
	GetSystemTime (&SystemTime);
	if (! (SystemTimeToFileTime (&SystemTime, &FileTime) &&
		QueryPerformanceCounter (&PTime) && QueryPerformanceFrequency (&TimeFreq))) {
		return FALSE;
	}
	NdisMoveMemory (&liSysTime, &FileTime, sizeof (LARGE_INTEGER));
	
	
	
	// allocate an instance that describes the adapter
	NdisAllocateMemory (&pOI, sizeof (OPEN_INSTANCE), 0, NDIS_ADDR_M1);
	if (pOI == NULL) {
		return FALSE;
	}
	NdisZeroMemory (pOI, sizeof (OPEN_INSTANCE));

	// init struct variables
	pOI->bpfprogram			= NULL;		//set an accept-all filter
	pOI->bpfprogramlen		= 0;
	pOI->BufSize			= 0;		//set an empty buffer
	pOI->Buffer				= NULL;		//reset the buffer
	pOI->Bhead				= 0;
	pOI->Btail				= 0;
	pOI->BLastByte			= 0;
	pOI->TimeOut			= 0;		//reset the timeouts


	// create the shared name read event
	pOI->ReadEvent = CreateEvent (NULL, FALSE, FALSE, SH_EVENT_NAME);
	if (pOI->ReadEvent == NULL) {
		NdisFreeMemory (pOI, sizeof (OPEN_INSTANCE), 0);
		return FALSE;
	}

	// set time values
	pOI->StartTime.QuadPart = (((liSysTime.QuadPart) % 10000000) * TimeFreq.QuadPart)/10000000;
	liSysTime.QuadPart = liSysTime.QuadPart / 10000000 - 11644473600;
	pOI->StartTime.QuadPart += (liSysTime.QuadPart) * TimeFreq.QuadPart - PTime.QuadPart;


	// allocate pool for the packet headers
	NdisAllocatePacketPool (&nsError, &(pOI->PacketPool), 
		TRANSMIT_PACKETS, sizeof (PACKET_RESERVED));
	if (nsError != NDIS_STATUS_SUCCESS) {
		CloseHandle (pOI->ReadEvent);
		NdisFreeMemory (pOI, sizeof (OPEN_INSTANCE), 0);
		return FALSE;
	}


	// allocate buffer pool for the packet data
	NdisAllocateBufferPool (&nsError, &(pOI->BufferPool), TRANSMIT_PACKETS);
	if (nsError != NDIS_STATUS_SUCCESS) {
		CloseHandle (pOI->ReadEvent);
		NdisFreePacketPool (pOI->PacketPool);
		NdisFreeMemory (pOI, sizeof (OPEN_INSTANCE), 0);
		return FALSE;
	}

	// set status pending
	pOI->Status = NDIS_STATUS_PENDING;

	// open the MAC driver
	NdisOpenAdapter (&nsOpen, &nsError, &pOI->AdapterHandle, &uiMedium, MediumArray,
		NUM_NDIS_MEDIA, g_pDeviceExtension->NdisProtocolHandle, pOI, pAdapterName, 0, NULL);


	if (nsOpen == NDIS_STATUS_PENDING) {
		SuspendExecution (pOI);
	} else {
		PacketOpenAdapterComplete (pOI, nsOpen, nsError);
	}

	// free the packet instance if not successful
	if (pOI->Status != NDIS_STATUS_SUCCESS) {
		CloseHandle (pOI->ReadEvent);
		NdisFreeMemory (pOI, sizeof (OPEN_INSTANCE), 0);
		return FALSE;
	}

	return TRUE;
}


/*
 * Called by NDIS to indicate the completion of the bind operation
 *
 */
VOID PacketOpenAdapterComplete (IN NDIS_HANDLE  ProtocolBindingContext,
								IN NDIS_STATUS  Status,
								IN NDIS_STATUS  OpenErrorStatus)
{
	POPEN_INSTANCE	pOI;

	pOI = (POPEN_INSTANCE) ProtocolBindingContext;

	if (Status == NDIS_STATUS_SUCCESS) {
		// int the global structure
		g_pDeviceExtension->pOpenInstance = pOI;
	} else {
		// free resources
		PKTFreeResources (pOI);
	}

	// set status
	ResumeExecution (pOI, Status);

	return;
}


/*
 * Frees the resources allocated
 *
 */
VOID PKTFreeResources (POPEN_INSTANCE pOI)
{
	// release the packet buffer
	if (pOI->Buffer != NULL) {
		NdisFreeMemory (pOI->Buffer, pOI->BufSize, 0);
		pOI->Buffer = NULL;
	}

	// release the bpf memory
	if (pOI->bpfprogram != NULL) {
		NdisFreeMemory (pOI->bpfprogram, pOI->bpfprogramlen, 0);
		pOI->bpfprogram = NULL;
	}

	// free the packet & buffer pools
	NdisFreeBufferPool (pOI->BufferPool);
	NdisFreePacketPool (pOI->PacketPool);

} 

/*
 * Closes the open adapter
 *
 */
BOOL PKTCloseAdapter (POPEN_INSTANCE pOI)
{
	BOOL			bRet = TRUE;
	NDIS_STATUS		nsError;


	// close the adapter
	NdisCloseAdapter (&nsError, pOI->AdapterHandle);

	if (nsError == NDIS_STATUS_PENDING) {
		SuspendExecution (pOI);
	} else {
		PacketCloseAdapterComplete (pOI, nsError);
	}

	if (pOI->Status != NDIS_STATUS_SUCCESS) {
		bRet = FALSE;
	}

	// set all the events to release the waits
	SetEvent (pOI->ReadEvent);
	CloseHandle (pOI->ReadEvent);

	// Free the open instance memory
	NdisFreeMemory (pOI, sizeof (OPEN_INSTANCE), 0);

	// set no instances open
	g_pDeviceExtension->pOpenInstance = NULL;

	return bRet;
}



/*
 * Called by NDIS to indicate that the adapter close is complete
 *
 */
VOID PacketCloseAdapterComplete(IN NDIS_HANDLE  ProtocolBindingContext,
								IN NDIS_STATUS  Status)
{
	POPEN_INSTANCE pOI;

	pOI = (POPEN_INSTANCE) ProtocolBindingContext;

	// irrespective of the status free the resources
	PKTFreeResources (pOI);

	// Set the status and resume execution
	ResumeExecution (pOI, Status);
	
	return;
}



/*
 * The plug and play support function. This function removes the adapter from
 * the global list of the adapter.
 *
 */
VOID PacketUnbindAdapter (OUT PNDIS_STATUS pStatus,
						  IN NDIS_HANDLE ProtocolBindingContext,
						  IN NDIS_HANDLE UnbindContext)
{
	POPEN_INSTANCE	pOI;
	NDIS_STATUS		nsCloseStatus;


	pOI = (POPEN_INSTANCE)ProtocolBindingContext;
	pOI->BindAdapterContext = UnbindContext;

	// Calls NDIS to close the selected adapter
	NdisCloseAdapter (&nsCloseStatus, pOI->AdapterHandle);
	if (nsCloseStatus == NDIS_STATUS_PENDING) {
		SuspendExecution (pOI);
	} else {
		PacketCloseAdapterComplete (pOI, nsCloseStatus);
	}

	// return the status back to the ndis framework
	*pStatus = pOI->Status;

	// set all the events to release the waits
	SetEvent (pOI->ReadEvent);
	CloseHandle (pOI->ReadEvent);

	// Free the open instance memory
	//NdisFreeMemory (pOI, sizeof (OPEN_INSTANCE), 0);

	// set no instances open
	g_pDeviceExtension->pOpenInstance = NULL;

	return;	
}