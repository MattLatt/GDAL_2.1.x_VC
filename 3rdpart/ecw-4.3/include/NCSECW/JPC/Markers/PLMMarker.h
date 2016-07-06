/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   PLMMarker.h 
** CREATED:  18/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPLMMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPLMMARKER_H
#define NCSJPCPLMMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/PacketLengthType.h"

namespace NCS {
namespace JPC {

	/**
	 * CPLMMarker class - the JPC PLM marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CPLMMarker: public CMarker {
public:
		/** Packet length info within a tile part */
	typedef struct {
			/** Packet length(s) for tile-part */
		std::vector<CPacketLengthType>	m_Lengths;
			/** nBytes of packet-length remaining to parse */
		UINT8			m_nNplm;
	} TilePartPacketLength;

		/** Index of this marker WRT other PLM markers */
	UINT8		m_nZplm;
		/** Packet lengths, each tile-part in this PLM */
	std::vector<TilePartPacketLength> m_TileParts;

		/** Default constructor, initialises members */
	CPLMMarker();
		/** Virtual destructor */
	virtual ~CPLMMarker();

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
};
}
}

#endif // !NCSJPCPLMMARKER_H
