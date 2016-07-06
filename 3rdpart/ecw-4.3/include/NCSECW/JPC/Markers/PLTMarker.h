/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:    PLTMarker.h 
** CREATED:  19/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPLTMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPLTMARKER_H
#define NCSJPCPLTMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/PacketLengthType.h"
#include "NCSEcw/SDK/Node2D.h"

//#define USE_CHUNKED_VECTOR 1
#ifdef USE_CHUNKED_VECTOR
#include "NCSChunkedVector.h"
#else
#include <vector>
#endif //USE_CHUNKED_VECTOR

namespace NCS {
namespace JPC {

	/**
	 * CPLTMarker class - the JPC PLT marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #4 $ $Author: jshao $ $Date: 2012/07/12 $ 
	 */	
class NCSECW_IMPEX CPLTMarker: public CMarker {
public:
		/** Index of this marker WRT other PLT markers */
	UINT8		m_nZplt;

		/** Number of packet lengths in this marker
		  *
		  * m_nLengths was changed to a UINT32 instead of UINT16 as
		  * it may wrap around if the file contains a large number of
		  * packets.
		  */
	UINT32		m_nLengths;

		/** Lengths in this PLT are cumulative */
	bool	m_bCumulativeLengths;

		/** PLT's were dynamically generated */
	bool		m_bDynamic;

		/** Length offsets when hashing index */
	UINT32 m_nFirstPacket;
	UINT64 m_nHeaderLengthOffset;
	UINT64 m_nDataLengthOffset;

		/** Default constructor, initialises members */
	CPLTMarker();
	CPLTMarker(const CPLTMarker &Marker);
		/** Virtual destructor */
	virtual ~CPLTMarker();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	//virtual bool ParseLength(class CJPC &JPC, CIOStream &Stream, UINT16 iLength);
	virtual bool ParseLength(class CJPC &JPC, CIOStream &Stream, UINT16 iLength, void *pMainTP); // changed on 12/07/2012

		/** 
		 * Assignment operator.
		 * @param		src			Copy source.
		 * @return      void
		 */
	void operator=( const class CPLTMarker& src );
		/** 
		 * Allocate the lengths array vector.
		 */
	void AllocLengths(void);
		/** 
		 * Free the lengths array vector.
		 */
	//void FreeLengths(void);
	bool FreeLengths(void); // changed on 02/01/2012

#ifdef USE_CHUNKED_VECTOR
		/** 
		 * Get a pointer to the lengths array vector.
		 */
	TNCSChunkedVector<CPacketLengthType> *GetLengths();
#else
	std::vector<CPacketLengthType> *GetLengths();
#endif //USE_CHUNKED_VECTOR
		/** 
		 * Add a length to the lengths array.
		 */
	void AddLength(CPacketLengthType &Len);

protected:

		/** Packet length(s) for tile-part */
#ifdef USE_CHUNKED_VECTOR
	TNCSChunkedVector<CPacketLengthType>	*m_pLengths;
#else
	std::vector<CPacketLengthType>	*m_pLengths;

	NCS::CMutex m_Mutex_PLT;// Against bug EMS-407, added 22/12/2011

#endif //USE_CHUNKED_VECTOR
};
}
}

#endif // !NCSJPCPLTMARKER_H
