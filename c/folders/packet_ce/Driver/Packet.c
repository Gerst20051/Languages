#include <windows.h>
#include "packet.h"
#include "filter.h"



// Global Variables for the Protocol Device Driver
PDEVICE_EXTENSION	g_pDeviceExtension = NULL;
const NDIS_PHYSICAL_ADDRESS NDIS_ADDR_M1 = {(LONG)(-1), (LONG)-1};

/*
 * This function is called bu NDIS when there is some thing to communicate to
 * the upper level. Thid function does not requires any implementation.
 *
 */
VOID PacketStatus(IN NDIS_HANDLE ProtocolBindingContext, 
				  IN NDIS_STATUS Status,
				  IN PVOID StatusBuffer, 
				  IN UINT StatusBufferSize)
{
	return;
}

/*
 * This function is the complete handler for the PacketStatus.
 * this function is not required.
 *
 */
VOID PacketStatusComplete (IN NDIS_HANDLE  ProtocolBindingContext)
{
	return;
}

/* 
 * This function add the adapter names to the list
 * 
 */
NDIS_STATUS PKTBindNames () 
{
	HKEY	hKeyComm;
	HKEY	hKeyAdap;
	HKEY	hKeyLink;
	DWORD	dwCnt = 0;
	DWORD	dwIndex = 0;
	WCHAR	chName[512];
	DWORD	dwName = 512;
	PADAPTER_NAME	pADName;


	// open the comm key
	if (ERROR_SUCCESS != RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"Comm", 0,
		0, &hKeyComm)) {
		return NDIS_STATUS_FAILURE;
	}


	// enumerate all the sub keys in the comm
	while (ERROR_SUCCESS == RegEnumKeyEx (hKeyComm, dwIndex++, chName,
		&dwName, NULL, NULL, NULL, NULL)) {

		// open the adapter key
		if (ERROR_SUCCESS == RegOpenKeyEx (hKeyComm, chName, 0,
			0, &hKeyAdap)) {

			// open the linkage key
			if (ERROR_SUCCESS == RegOpenKeyEx (hKeyAdap, L"Linkage", 0,
				0, &hKeyLink)) 
			{
				dwName = 512;

				// query the route value
				if (ERROR_SUCCESS == RegQueryValueEx (hKeyLink, L"Route", NULL, 
					NULL, (LPBYTE) chName, &dwName)) 
				{
					// store the name in the list
					// we can also get the detailed name here
					// may be in next version
					
					// allocate the memory that describes the adapter
					NdisAllocateMemory ((PVOID*) &pADName, sizeof (ADAPTER_NAME), 0 , NDIS_ADDR_M1);
					if (pADName == NULL) {
						return NDIS_STATUS_FAILURE;
					}
					
					// copy the device name in the adapter name structure
					pADName->ndstrDeviceName.Length			= wcslen (chName) * 2;
					pADName->ndstrDeviceName.MaximumLength	= wcslen (chName) * 2 + 2;
					pADName->ndstrDeviceName.Buffer			= pADName->chDeviceName;
					wcsncpy (pADName->chDeviceName, chName, 32);
					
					// insert the adapter name in the global data structure
					InsertTailList (&g_pDeviceExtension->listAdapterNames, &pADName->ListElement);
					dwCnt++;

				}

				// close linkage
				RegCloseKey (hKeyLink);
			}

			// close the adapter key
			RegCloseKey (hKeyAdap);
		}
		dwName = 512;
	}

	// close the comms key
	RegCloseKey (hKeyComm);

	if (dwCnt == 0)	{
		SetLastError (ERROR_BAD_NET_NAME);
		return NDIS_STATUS_FAILURE;
	}


	return NDIS_STATUS_SUCCESS;
}


/*
 * The DllEntry Routine. This function is called by the system whenever a
 * process attaches/detaches to the dll. The function does nothing.
 *
 * Arguments
 *		hinstDLL	- The instance handle of the dll
 *		dwReason	- The reason for which the function is called
 *		lpReserved	- Reserved parameter not used
 *
 * Return Value
 *		Always return TRUE
 *
 */
BOOL WINAPI DllEntry(HINSTANCE	hinstDll, DWORD	dwReason, LPVOID lpReserved)
{

    switch(dwReason) {
        case DLL_PROCESS_ATTACH:
            break;
    
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

/*
 * Function called to reset the adapter
 *
 */
BOOL PKTReset (POPEN_INSTANCE pOI)
{
	NDIS_STATUS Status;

	// Call NDIS to reset the adapter
	NdisReset (&Status, pOI->AdapterHandle);
	if (Status == NDIS_STATUS_PENDING) {
		SuspendExecution (pOI);
	} else {
		PacketResetComplete (pOI, Status);
	}

	if (Status != NDIS_STATUS_SUCCESS) {
		return FALSE;
	}

	return TRUE;
}

/*
 * Reset complete handler
 *
 */
VOID PacketResetComplete (IN NDIS_HANDLE  ProtocolBindingContext,
						  IN NDIS_STATUS  Status)
{
	POPEN_INSTANCE		pOI;
	pOI = (POPEN_INSTANCE)ProtocolBindingContext;

	// set the status
	ResumeExecution (pOI, Status);

	return;
}

VOID PacketTransferDataComplete (IN NDIS_HANDLE ProtocolBindingContext, 
								 IN PNDIS_PACKET Packet, IN NDIS_STATUS Status,
								 IN UINT BytesTransferred)
{

}


/*
 * This routine initializes the packet driver. The function is called by the 
 * driver manager automatically when RegisterDevice function is called.
 *
 * Arguments
 *		dwContext		-	Specifies a pointer to a string containing the 
 *							registry path to the active key for the stream 
 *							interface driver.
 *
 * Return Value
 *		The function return the device context to be called with other stream
 *		function like _Open, _Close, etc. If the initialization fails then the
 *		function returns NULL. If the function returns 0 then the driver is not 
 *		loaded by the device manager.
 *
 */
DWORD PKT_Init(DWORD dwContext)
{
	NDIS_PROTOCOL_CHARACTERISTICS	ProtocolChar;
	NDIS_STRING						ProtoName = PROTOCOL_NAME;
	NDIS_STATUS						Status;
	
	// Allocate memory for the global device extension
	NdisAllocateMemory ((PVOID*) &g_pDeviceExtension, sizeof (DEVICE_EXTENSION), 
		0, NDIS_ADDR_M1);
	if (g_pDeviceExtension == NULL) {
		return 0;
	}
	
	// reset the global device extension object
	NdisZeroMemory (g_pDeviceExtension, sizeof (DEVICE_EXTENSION));
	
	// reset the protocol characteristics object
	NdisZeroMemory (&ProtocolChar, sizeof (NDIS_PROTOCOL_CHARACTERISTICS));
	
	// Initialize the protocol char structure
	ProtocolChar.MajorNdisVersion            = 0x03;
	ProtocolChar.MinorNdisVersion            = 0x00;
	ProtocolChar.Reserved                    = 0;
	ProtocolChar.Name                        = ProtoName;

	ProtocolChar.BindAdapterHandler			 = PacketBindAdapter;
	ProtocolChar.CloseAdapterCompleteHandler = PacketCloseAdapterComplete;
	ProtocolChar.OpenAdapterCompleteHandler  = PacketOpenAdapterComplete;
	ProtocolChar.ReceiveHandler              = PacketReceive;
	ProtocolChar.ReceiveCompleteHandler      = PacketReceiveComplete;
	ProtocolChar.RequestCompleteHandler      = PacketRequestComplete;
	ProtocolChar.ResetCompleteHandler        = PacketResetComplete;
	ProtocolChar.StatusHandler               = PacketStatus;
	ProtocolChar.StatusCompleteHandler       = PacketStatusComplete;
	ProtocolChar.TransferDataCompleteHandler = PacketTransferDataComplete;
	ProtocolChar.UnbindAdapterHandler        = PacketUnbindAdapter;
	ProtocolChar.SendCompleteHandler         = PacketWriteComplete;
	
	// Registed the protocol handler
	NdisRegisterProtocol (&Status, &g_pDeviceExtension->NdisProtocolHandle,
		&ProtocolChar, sizeof(NDIS_PROTOCOL_CHARACTERISTICS));
	
	// Check the return value
	if (Status != NDIS_STATUS_SUCCESS) {
		NdisFreeMemory (g_pDeviceExtension, sizeof (DEVICE_EXTENSION),  0);
		return 0;
	}
	
	// Initialize the list headers
	InitializeListHead (&g_pDeviceExtension->listAdapterNames);

	// Initialize the open instance pointer
	g_pDeviceExtension->pOpenInstance = NULL;
	
	// Check for the available adapters to bind with
	// not reqd for win ce. the os call packet bind adapter for 
	// every adapters installed in the system
	if (PKTBindNames () != NDIS_STATUS_SUCCESS) {
		NdisFreeMemory (g_pDeviceExtension, sizeof (DEVICE_EXTENSION),  0);
		g_pDeviceExtension = NULL;
		return 0;
	}
	
	// Status is generally succes`s over here
	return PKT_DEVICE_CONTEXT;
}



/*
** Free all the resources allocated in PKT_Init()
*/
BOOL PKT_Deinit(DWORD dwContext)
{
	NDIS_STATUS			Status;
	PLIST_ENTRY			pHead;
	PLIST_ENTRY			pEntry;
    PADAPTER_NAME		pAName;
	POPEN_INSTANCE		pOI;
	
	pOI = g_pDeviceExtension->pOpenInstance;

	if (pOI != NULL) {
		PKTCloseAdapter (pOI);
	}

	//free the names' list
	if (!IsListEmpty (&g_pDeviceExtension->listAdapterNames)) {
	
		pHead = &(g_pDeviceExtension->listAdapterNames);
		if (pHead != NULL )
		{
			pEntry = RemoveTailList (pHead);
			while (!IsListEmpty(pEntry)){

				pAName = CONTAINING_RECORD (pEntry, ADAPTER_NAME, ListElement);

				NdisFreeMemory (pAName, sizeof(ADAPTER_NAME), 0);

				pEntry = RemoveTailList (pHead);
			}
		}
	}

	//unregister the protocol from NDIS
	NdisDeregisterProtocol (&Status, g_pDeviceExtension->NdisProtocolHandle);
	
	//free the global device extension
	NdisFreeMemory (g_pDeviceExtension, sizeof (DEVICE_EXTENSION), 0);
	g_pDeviceExtension = NULL;

	return TRUE;
}


/*
** Handles IOCTLs passed using DeviceIOControl() function
*/

BOOL PKT_IOControl (DWORD dwContext, DWORD dwCode, PBYTE pBufIn,
					DWORD dwLenIn, PBYTE pBufOut, DWORD dwLenOut, PDWORD pdwActualOut)
{
	BOOL			bRet = TRUE;
	PWCHAR			bpf_prog;
	PNDIS_STRING	pstrAName;
	POPEN_INSTANCE	pOpenInst = NULL;
	PUCHAR			pTempBuf;
	ULONG			nValue;

	// if the context is different then return false
	if (dwContext != PKT_FILE_HANDLE) {
		SAFE_SET (pdwActualOut, 0);
		return FALSE;
	}

	// if the code is not to open the adapter then there
	// should be an open instance of the adapter
	if (dwCode != PIOC_OPEN_ADAPTER &&
		dwCode != PIOC_GET_MACNAME) {

		// get the open instance handle
		pOpenInst = g_pDeviceExtension->pOpenInstance;
		if (pOpenInst == NULL) {
			return FALSE;
		}
	}

	switch (dwCode) {

	// open the adapter message
	case PIOC_OPEN_ADAPTER:

		// get the adapter NDIS name
		pstrAName = PKTGetNDISAdapterName ((PWCHAR)pBufIn, dwLenIn);
		if (pstrAName == NULL) {
			return FALSE;
		}

		// open the adapter
		if (! PKTOpenAdapter (pstrAName)) {
			bRet = FALSE;
		}
	break;

	// close the adapter message
	case PIOC_CLOSE_ADAPTER:
		bRet = PKTCloseAdapter (pOpenInst);
	break;

	// function to set new bpf filter
	case PIOC_SETF:

		// free the previous buffer if selected
		if (pOpenInst->bpfprogram != NULL) {
			NdisFreeMemory (pOpenInst->bpfprogram, pOpenInst->bpfprogramlen, 0);
			pOpenInst->bpfprogram = NULL;
			pOpenInst->bpfprogramlen = 0;
		}


		// get the pointer to the new program
		bpf_prog = (PWCHAR) pBufIn;

		// before accepting the program we must check that it's valid
		// otherwise, a bogus program could easily crash the driver
		pOpenInst->bpfprogramlen = dwLenIn;
		if (bpf_validate ((struct bpf_insn*)bpf_prog, 
			pOpenInst->bpfprogramlen/sizeof(struct bpf_insn)) == 0) {
			pOpenInst->bpfprogramlen = 0;
			pOpenInst->bpfprogram = NULL; 
			return FALSE; // filter not accepted
		}

		// allocate the memory to contain the new filter program*/
		if (NdisAllocateMemory(&pOpenInst->bpfprogram, pOpenInst->bpfprogramlen,
			0, NDIS_ADDR_M1) == NDIS_STATUS_FAILURE) {
			pOpenInst->bpfprogramlen = 0;
			pOpenInst->bpfprogram = NULL; 
			return FALSE;
		}

		// copy the program in the new buffer
		NdisMoveMemory (pOpenInst->bpfprogram, bpf_prog, pOpenInst->bpfprogramlen);

		// reset the buffer that could contain packets that don't match the filter
		pOpenInst->Bhead	= 0;
		pOpenInst->Btail	= 0;
		pOpenInst->BLastByte= 0;

		// return the accepted buffer len
		SAFE_SET(pdwActualOut, pOpenInst->bpfprogramlen);
	break;


	// function to set the internal buffer size
	case PIOC_SET_BUFFER_SIZE:

		// get the size to allocate
		nValue = ((PULONG)pBufIn)[0];

		// free the old buffer
		if (pOpenInst->Buffer != NULL) {
			NdisFreeMemory (pOpenInst->Buffer, pOpenInst->BufSize, 0);
		}

		pOpenInst->Buffer = NULL;

		// allocate the new buffer
		if (nValue > 0) {
			NdisAllocateMemory ((PVOID*)&pTempBuf, nValue, 0, NDIS_ADDR_M1);
			if (pTempBuf == NULL) {
				pOpenInst->BufSize = 0;
				return FALSE;
			}
			
			pOpenInst->Buffer = pTempBuf;
		}
			
		pOpenInst->Bhead	= 0;
		pOpenInst->Btail	= 0;
		pOpenInst->BLastByte= 0;
		pOpenInst->BufSize	= (UINT)nValue;

		SAFE_SET(pdwActualOut, nValue);
		
	break;


	// for sharing the event from the user
	case PIOC_EVNAME:
		if (pBufOut == NULL || dwLenOut < 32) {
			SAFE_SET(pdwActualOut, 0);
			bRet = FALSE;
		} else {
			wcscpy ((PWCHAR)pBufOut, SH_EVENT_NAME);
			SAFE_SET (pdwActualOut, wcslen (SH_EVENT_NAME));
		}

	break;


	// set read timeout function
	case PIOC_SRTIMEOUT:
		pOpenInst->TimeOut = ((PULONG)pBufIn)[0];
		SAFE_SET(pdwActualOut, pOpenInst->TimeOut);
	break;

	
	// resets the adapter instance
	case PIOC_RESET:
		bRet = PKTReset (pOpenInst);
	break;


	// requests of different type
	case PIOC_SETOID:
	case PIOC_QUERYOID:
		return PKTRequest (pOpenInst, dwCode, pBufIn, dwLenIn, pBufOut, dwLenOut, pdwActualOut);
	break;


	// read packets
	case PIOC_READ_PACKET:
		return PKTRead (pOpenInst, pBufOut, dwLenOut, pdwActualOut);
	break;


	// write packets
	case PIOC_WRITE_PACKET:
		return PKTWrite (pOpenInst, pBufIn, dwLenIn, pdwActualOut);
	break;


	// read mac name
	case PIOC_GET_MACNAME:
		*pdwActualOut = PKTGetMacNameList ((PWCHAR)pBufOut, dwLenOut);
	break;


	// minimum number of bytes to copy
	case PIOC_SMINTOCOPY:
		pOpenInst->MinToCopy = ((PULONG)pBufIn)[0];
		SAFE_SET(pdwActualOut, 0);
	break;


	// unknown function code. set out length to 0 and return true
	default:
		bRet = FALSE;
		SAFE_SET(pdwActualOut, 0);
	break;
	}

	return bRet;
}