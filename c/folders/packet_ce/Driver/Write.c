#include <windows.h>
#include "packet.h"


/*
** Function does nothing. Writing of packet is handled by IO Control
*/
extern DWORD PKT_Write(DWORD dwContext, LPCVOID pSourceBytes, DWORD NumberOfBytes)
{
	return 0;
}


BOOL PKTWrite (POPEN_INSTANCE pOI, PVOID pBufIn, DWORD dwLenIn, PDWORD pdwActualOut)
{
	NDIS_STATUS		Status;
	PNDIS_PACKET	pPacket;
	PNDIS_BUFFER	pNdisBuffer;

	// check if the length of the packet is zero
	if (dwLenIn == 0) {
		return TRUE;
	}

	// mark io pending
	pOI->Status = NDIS_STATUS_PENDING;

	// allocate packet
	NdisAllocatePacket (&Status, &pPacket, pOI->PacketPool);
	if (Status != NDIS_STATUS_SUCCESS) {
		SAFE_SET (pdwActualOut, 0);
		return FALSE;
	}


	// allocate the buffer
	NdisAllocateBuffer (&Status, &pNdisBuffer, pOI->BufferPool, pBufIn, dwLenIn);
	if (Status != NDIS_STATUS_SUCCESS)	{

		NdisReinitializePacket(pPacket);
		NdisFreePacket(pPacket);

		SAFE_SET (pdwActualOut, 0);
		return FALSE;
	}

	// add the buffer to the buffer chain
	NdisChainBufferAtFront (pPacket, pNdisBuffer);

	// send the packet on the network
	NdisSend (&Status, pOI->AdapterHandle, pPacket);

	if (Status == NDIS_STATUS_PENDING) {
		SuspendExecution (pOI);
	} else {
		PacketWriteComplete (pOI, pPacket, Status);
	}

	if (pOI->Status != NDIS_STATUS_SUCCESS) {
		return FALSE;
	}

	return TRUE;
}


VOID PacketWriteComplete(IN NDIS_HANDLE   ProtocolBindingContext,
						 IN PNDIS_PACKET  pPacket,
						 IN NDIS_STATUS   Status)
{
	PNDIS_BUFFER 		pNdisBuffer;
	POPEN_INSTANCE		pOI;

	pOI = (POPEN_INSTANCE) ProtocolBindingContext;

	// un chain the buffer
	NdisUnchainBufferAtFront (pPacket, &pNdisBuffer);
	if (pNdisBuffer) {
		// return to the pool
		NdisFreeBuffer (pNdisBuffer);
	}
		
	// reinit the packet
	NdisReinitializePacket (pPacket);
		
	// release the packet to he pool
	NdisFreePacket (pPacket);

	// set the status
	ResumeExecution (pOI, Status);

	return;
}



BOOL PKTRequest (POPEN_INSTANCE pOI, DWORD dwFunctionCode, PVOID pBufIn, DWORD dwLenIn, 
				 PBYTE pBufOut, DWORD dwLenOut, PDWORD pdwActualOut)
{
	PPACKET_OID_DATA	pOidData;
	NDIS_STATUS			Status;
	
	pOidData = (PPACKET_OID_DATA)(pBufIn);
	
	if (! ((dwLenIn == dwLenOut) && 
		(dwLenIn >= sizeof(PACKET_OID_DATA) - 1 + pOidData->Length)))
	{
		// parameters not correct 
		SAFE_SET(pdwActualOut, 0);
		return FALSE;
	}
	
	switch (dwFunctionCode) {
		
		// code to set the data
	case PIOC_SETOID: 
		pOI->Request.RequestType									= NdisRequestSetInformation;
		pOI->Request.DATA.SET_INFORMATION.Oid						= pOidData->Oid;
		pOI->Request.DATA.SET_INFORMATION.InformationBufferLength	= pOidData->Length;
		pOI->Request.DATA.SET_INFORMATION.InformationBuffer			= pOidData->Data;
		break;
		
	case PIOC_QUERYOID:
		pOI->Request.RequestType									= NdisRequestQueryInformation;
		pOI->Request.DATA.QUERY_INFORMATION.Oid						= pOidData->Oid;
		pOI->Request.DATA.QUERY_INFORMATION.InformationBufferLength	= pOidData->Length;
		pOI->Request.DATA.QUERY_INFORMATION.InformationBuffer		= pOidData->Data;
		break;
		
	default:
		pOI->Request.RequestType									= NdisRequestGeneric1;
		pOI->Request.DATA.QUERY_INFORMATION.Oid						= pOidData->Oid;
		pOI->Request.DATA.QUERY_INFORMATION.InformationBufferLength	= pOidData->Length;
		pOI->Request.DATA.QUERY_INFORMATION.InformationBuffer		= pOidData->Data;
	}
	
	NdisRequest (&Status, pOI->AdapterHandle, &pOI->Request);
	if (Status == NDIS_STATUS_PENDING) {
		SuspendExecution (pOI);
		Status = pOI->Status;
	} else {
		PacketRequestComplete (pOI, &pOI->Request, Status);
	}

	if (pOI->Status == NDIS_STATUS_SUCCESS) {
		SAFE_SET (pdwActualOut, pOI->Request.DATA.SET_INFORMATION.BytesRead);
	} else {
		// if there is an error length contains the type of error
		SAFE_SET (pdwActualOut, 0);
		pOidData->Length = pOI->Status;
		return FALSE;
	}
	
	return TRUE;
}

VOID PacketRequestComplete(IN NDIS_HANDLE   ProtocolBindingContext,
						   IN PNDIS_REQUEST NdisRequest,
						   IN NDIS_STATUS   Status)
{
	POPEN_INSTANCE		pOI;

	pOI		= (POPEN_INSTANCE)ProtocolBindingContext;

	if (Status == NDIS_STATUS_SUCCESS) {
	} else {
	}

	// resume
	ResumeExecution (pOI, Status);

	return;
}