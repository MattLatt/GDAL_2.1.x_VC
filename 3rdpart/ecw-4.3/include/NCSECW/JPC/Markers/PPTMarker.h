/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     PPTMarker.h 
** CREATED:  19/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPPTMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPPTMARKER_H
#define NCSJPCPPTMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Packet.h"

namespace NCS {
namespace JPC {
	/**
	 * CPPTMarker class - the JPC PPT marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CPPTMarker: public CMarker {
public:
		/** Index of this marker WRT other PPT markers */
	UINT8		m_nZppt;
		/** Packet header(s) for tile-part */
//	CPacketList		m_Headers;

		/** Default constructor, initialises members */
	CPPTMarker();
		/** Virtual destructor */
	virtual ~CPPTMarker();

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

#endif // !NCSJPCPPTMARKER_H
