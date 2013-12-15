#ifndef __PCAENET_H
#define __PCAENET_H

/****************************************************************************
 *                                                                          *
 *                  Ethernet Protocol Definitions - PCAEnet.H               *
 *                                                                          *
 *     Copyright (c) 1989-2000, Printing Communications Associates, Inc.    *
 *                          http://www.pcausa.com
 *                                                                          *
 *                            Thomas F. Divine                              *
 *                          4201 Brunswick Court                            *
 *                       Smyrna, Georgia 30080 USA                          *
 *                       mailto:tdivine@pcausa.com
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 *                        Ethernet Packet Definitions                       *
 ****************************************************************************/

/*
 * NOTE: Ethernet packets are defined by offsets into the packet instead of
 * as a data structure to reduce alignment problems.
 */

#define	ETHER_ADDR_LENGTH		6
#define	ETHER_TYPE_LENGTH		2
#define	MAX_802_3_LENGTH		1500	// Maximum Value For 802.3 Length Field
#define	MAX_ETHER_SIZE			1514	// Maximum Ethernet Packet Length
#define	MIN_ETHER_SIZE	  		60		// Minimum Ethernet Packet Length


/* Offsets Into Ethernet 802.3 Medium Access Control (MAC) Packet Header
------------------------------------------------------------------------ */
#define	MDstAddr	0											// Offset To Destination Address
#define	MSrcAddr	ETHER_ADDR_LENGTH						// Offset To Source Address
#define	MLength	(MSrcAddr + ETHER_ADDR_LENGTH)	// Of Bytes Following MAC Header
#define	MHdrSize	(MLength + ETHER_TYPE_LENGTH )	// MAC 802.3 Header Size

/* Offsets Into Ethernet 802.2 LLC (Type 1) Packet Header (From MAC Data)
------------------------------------------------------------------------- */
#define	LDSAP			0					// Destination Service Access Point
#define	LSSAP			(LDSAP + 1)		// Source Service Access Point
#define	LCntrl		(LSSAP + 1)		// LLC Control Field
#define	LHdrSize		(LCntrl + 1)	// LLC Header Size


/* Offsets Into Sub-Network Access Protocol (SNAP) Header (From MAC Data)
------------------------------------------------------------------------- */
#define	SType			LHdrSize			// SNAP Type
#define	SHdrSize		(SType + 5)		// SNAP Type Size



/****************************************************************************
 *                         TokenRing Packet Definitions                     *
 ****************************************************************************/

/*
 * NOTE: TokenRing packets are defined by offsets into the packet instead of
 * as a data structure to reduce alignment problems.
 */

#define	TOKENRING_ADDR_LENGTH	6
#define	MAX_TOKENRING_4MBPS_SIZE	4550	// Maximum 4MBPS TokenRing Packet Length
#define	MAX_TOKENRING_16MBPS_SIZE	18200	// Maximum 16MBPS TokenRing Packet Length

#endif

