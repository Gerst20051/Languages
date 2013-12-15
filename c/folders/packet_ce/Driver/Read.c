#include "packet.h"
#include "filter.h"

/*
 * This function does nothing. There is no concept of seek in
 * packet io. Function simply returns -1
 *
 */
DWORD PKT_Seek(DWORD Handle, LONG lDistance, WORD dwMoveMethod)
{
	return 0xFFFFFFFF;
}

/*
 * This function does nothing
 * all the read calls are handled by IO Control function
 *
 */
DWORD PKT_Read(DWORD dwContext, LPVOID pBuffer, DWORD Count)
{
	return 0;
}



UINT GetBuffOccupancy(POPEN_INSTANCE pOpen)
{
	if (pOpen->Btail - pOpen->Bhead >= 0) {
		return pOpen->Btail - pOpen->Bhead;
	}
	
	return pOpen->BLastByte - pOpen->Bhead + pOpen->Btail;
}


void PacketMoveMem(PVOID Destination, PVOID Source, ULONG Length, UINT *Bhead)
{
	ULONG WordLength;
	UINT n,i,NBlocks;

	WordLength = Length >> 2;
	NBlocks = WordLength >> 8;
	
	for (n = 0; n < NBlocks; n++) {
		for (i = 0; i < 256; i++) {
			*((PULONG)Destination)++ = *((PULONG)Source)++;
		}
		*Bhead += 1024;
	}

	n = WordLength - (NBlocks << 8);
	for (i = 0; i < n; i++) {
		*((PULONG)Destination)++ = *((PULONG)Source)++;
	}

	*Bhead += n << 2;
	
	n = Length - (WordLength << 2);
	for (i = 0; i < n; i++) {
		*((PUCHAR)Destination)++ = *((PUCHAR)Source)++;
	}
	*Bhead += n;
}

/*
 * Reads into user buffer
 *
 */
BOOL PKTRead (POPEN_INSTANCE pOI, PBYTE pBufOut, DWORD dwLenOut, PDWORD pdwActualOut)
{
	UINT				Thead;
	UINT				Ttail;
	UINT				TLastByte;
	ULONG				Input_Buffer_Length;
	UINT				nCount;
	PBYTE				pCurrBuff;
	UINT				cplen;
	UINT				CpStart;
    PUCHAR				packp;		//buffer that maps the application memory

	if (dwLenOut < ETHERNET_HEADER_LENGTH) {
		// the application's buffer if too small to contain the packet
		SAFE_SET(pdwActualOut, 0);
		return FALSE;
	}


	//See if the buffer is full enough to be copied
	if (GetBuffOccupancy (pOI) <= pOI->MinToCopy) {

		//there are not enough buffered packets: the application must wait
		//wait until some packets arrive or the timeout expires		
		ResetEvent (pOI->ReadEvent);
		if (pOI->TimeOut != -1) {
			WaitForSingleObject (pOI->ReadEvent, pOI->TimeOut);
		}

		// reset the event
		ResetEvent (pOI->ReadEvent);

		if (pOI->Bhead == pOI->Btail)
		{
			//the timeout has expired, but the buffer is still empty.
			//we must awake the application returning an empty buffer.
			SetEvent (pOI->ReadEvent);
			SAFE_SET (pdwActualOut, 0);
			return TRUE;
		}
	}
		

	// see if there are packets in the buffer
	Thead = pOI->Bhead;
	Ttail = pOI->Btail;
	TLastByte = pOI->BLastByte;

	// there is at least a buffered packet: the read call can be completed
	Input_Buffer_Length = dwLenOut;
	packp = (PUCHAR)pBufOut;
	nCount = 0;


	// get the address of the buffer
	pCurrBuff = pOI->Buffer;
	
	// fill the application buffer

	// first of all see if it we can copy all the buffer in one go
	if (Ttail > Thead) {

		if ((Ttail - Thead) < Input_Buffer_Length) {

			PacketMoveMem (packp, pCurrBuff + Thead, Ttail - Thead, &(pOI->Bhead));
			SAFE_SET (pdwActualOut, Ttail - Thead);

			// The buffer is empty: reset the read event
			ResetEvent (pOI->ReadEvent);

			return TRUE;
		}
	}
	else if ((TLastByte - Thead) < Input_Buffer_Length) {

			PacketMoveMem (packp, pCurrBuff + Thead, TLastByte - Thead, &(pOI->Bhead));
			pOI->BLastByte = Ttail;
			pOI->Bhead = 0;
			SAFE_SET(pdwActualOut, TLastByte - Thead);

			return TRUE;
	}

	// scan the buffer
	CpStart = Thead;
	while (TRUE) {

		if (Thead == Ttail) {
			break;
		}

		if (Thead == TLastByte) {

			PacketMoveMem (packp, pCurrBuff + CpStart, Thead - CpStart, &(pOI->Bhead));
			packp += (Thead - CpStart);
			pOI->Bhead = 0;
			Thead = 0;
			CpStart = 0;
		}

		cplen = ((struct bpf_hdr*) (pCurrBuff + Thead))->bh_caplen + 
			sizeof(struct bpf_hdr);

		if ((nCount + cplen > Input_Buffer_Length)) {
			PacketMoveMem (packp, pCurrBuff + CpStart, Thead - CpStart, &(pOI->Bhead));
			SAFE_SET (pdwActualOut, nCount);
			ResetEvent (pOI->ReadEvent);
			return TRUE;
		}

		cplen = PACKET_WORDALIGN(cplen);
		nCount += cplen;
		Thead += cplen;
	}

	ResetEvent (pOI->ReadEvent);
	PacketMoveMem (packp, pCurrBuff + CpStart, Thead - CpStart, &(pOI->Bhead));
	pOI->Bhead = Thead;

	SAFE_SET (pdwActualOut, nCount);

    return TRUE;
}


NDIS_STATUS PacketReceive(IN NDIS_HANDLE ProtocolBindingContext,
						  IN NDIS_HANDLE MacReceiveContext,
						  IN PVOID HeaderBuffer,
						  IN UINT HeaderBufferSize,
						  IN PVOID LookAheadBuffer,
						  IN UINT LookaheadBufferSize,
						  IN UINT PacketSize)
{
	POPEN_INSTANCE		pOI;
	UINT				fres; //filter result
	UINT				Thead;
	UINT				Ttail;
	UINT				TLastByte;
	UINT				maxbufspace;
	PUCHAR				CurrBuff;
    NDIS_STATUS         Status;
	PNDIS_BUFFER		pNdisBuffer;
    PNDIS_PACKET        pPacket;
    ULONG               BufferLength;
    ULONG               SizeToTransfer;
    UINT                BytesTransfered;
	LARGE_INTEGER		CapTime;
	LARGE_INTEGER		TimeFreq;
	struct bpf_hdr		*header;

	pOI = (POPEN_INSTANCE) ProtocolBindingContext;

	/*
	Check if the lookahead buffer follows the mac header.
	If the data follow the header (i.e. there is only a buffer) a normal bpf_filter() is
	executed on the packet.
	Otherwise if there are 2 separate buffers (this could be the case of LAN emulation or
	stuff like this) bpf_filter_with_2_buffers() is executed.
	*/

	if ((UINT)LookAheadBuffer - (UINT)HeaderBuffer != HeaderBufferSize) {

		fres = bpf_filter_with_2_buffers ((struct bpf_insn*)(pOI->bpfprogram),
			HeaderBuffer, LookAheadBuffer, HeaderBufferSize, 
			PacketSize + HeaderBufferSize, LookaheadBufferSize + HeaderBufferSize);
	} else {

		fres = bpf_filter ((struct bpf_insn*)(pOI->bpfprogram),
			HeaderBuffer, PacketSize + HeaderBufferSize, 
			LookaheadBufferSize + HeaderBufferSize);
	}

	// if filter returns 0 the packet is rejected
	if (fres == 0) {
		return NDIS_STATUS_NOT_ACCEPTED;
	}

	//if the filter returns -1 the whole packet must be accepted
	if (fres == -1) {
		fres = PacketSize + HeaderBufferSize;
	}

	//
	if (pOI->BufSize == 0) {
		return NDIS_STATUS_NOT_ACCEPTED;
	}

	Thead = pOI->Bhead;
	Ttail = pOI->Btail;
	TLastByte = pOI->BLastByte;

	maxbufspace =fres + sizeof(struct bpf_hdr);

	if (Ttail + maxbufspace >= pOI->BufSize) {

		if(Thead <= maxbufspace) {
			//the buffer is full: the packet is lost
			return NDIS_STATUS_NOT_ACCEPTED;
		} else {
			Ttail = 0;
		}
	}
	
	if((Ttail < Thead) && (Ttail + maxbufspace >= Thead)) {

		//the buffer is full: the incoming packet is lost
		return NDIS_STATUS_NOT_ACCEPTED;
	}

	CurrBuff = pOI->Buffer+Ttail;

	// allocate the ndis buffer
	NdisAllocateBuffer (&Status, &pNdisBuffer, pOI->BufferPool, 
		CurrBuff + sizeof(struct bpf_hdr) + HeaderBufferSize, fres);
	if (Status != NDIS_STATUS_SUCCESS) {
		return NDIS_STATUS_NOT_ACCEPTED;
	}

	// allocate the packet from NDIS
	NdisAllocatePacket (&Status, &pPacket, pOI->PacketPool);
	if (Status != NDIS_STATUS_SUCCESS) {
		NdisFreeBuffer( pNdisBuffer );
		return NDIS_STATUS_NOT_ACCEPTED;
	}

	// link the buffer to the packet
	NdisChainBufferAtFront(pPacket, pNdisBuffer);
		
	BufferLength = fres - HeaderBufferSize;

	//Find out how much to transfer
	SizeToTransfer = (PacketSize < BufferLength) ? PacketSize : BufferLength;
	
	//copy the ethernet header into buffer
	NdisMoveMemory ((CurrBuff) + sizeof(struct bpf_hdr),
		HeaderBuffer, HeaderBufferSize);
	
	// call the Mac to transfer the packet
	NdisTransferData (&Status, pOI->AdapterHandle, 
		MacReceiveContext, 0, SizeToTransfer, pPacket,
		&BytesTransfered);
	
	if (Status != NDIS_STATUS_FAILURE) {
		//store the capture time
		QueryPerformanceCounter (&CapTime);
		QueryPerformanceFrequency (&TimeFreq);

		if (fres > BytesTransfered + HeaderBufferSize) {
			fres = BytesTransfered + HeaderBufferSize;
		}

		//fill the bpf header for this packet
		CapTime.QuadPart += pOI->StartTime.QuadPart;
		header = (struct bpf_hdr*)CurrBuff;
		header->bh_tstamp.tv_usec	= (long)((CapTime.QuadPart % TimeFreq.QuadPart * 1000000) / TimeFreq.QuadPart);
		header->bh_tstamp.tv_sec	= (long)(CapTime.QuadPart / TimeFreq.QuadPart);
		header->bh_caplen			= fres;
		header->bh_datalen			= PacketSize + HeaderBufferSize;
		header->bh_hdrlen			= sizeof(struct bpf_hdr);
		
		//update the buffer	
		Ttail += PACKET_WORDALIGN(fres + sizeof (struct bpf_hdr));
		if (Ttail > Thead) {
			TLastByte = Ttail;
		}

		pOI->Btail		= Ttail;
		pOI->BLastByte	= TLastByte;
	}

	// free the allocated buffer
	NdisFreeBuffer (pNdisBuffer);

	// recylcle the packet
	NdisReinitializePacket(pPacket);

	// put the packet on the free queue
	NdisFreePacket (pPacket);

	// unfreeze PacketRead
	if (GetBuffOccupancy (pOI) > pOI->MinToCopy) {
		SetEvent(pOI->ReadEvent);
	}

	return NDIS_STATUS_SUCCESS;
}

/*
 * The Receive Complete handler. This function is not required
 *
 */
VOID PacketReceiveComplete(IN NDIS_HANDLE  ProtocolBindingContext)
{
	return;
}