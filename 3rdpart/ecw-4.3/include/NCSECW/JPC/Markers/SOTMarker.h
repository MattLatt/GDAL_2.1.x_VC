/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     SOTMarker.h 
** CREATED:  11/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSOTMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCSOTMARKER_H
#define NCSJPCSOTMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {
	/**
	 * CSOTMarker class - the JPC SOT marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CSOTMarker: public CMarker {
public:

		/** Tile Index, >= 0 */
	UINT16		m_nIsot;
		/** Length of SOT + tile part bitstream */
	UINT32		m_nPsot;
		/** Tile part index, first = 0 */
	UINT8		m_nTPsot;
		/** Number of tile-parts of a tile in the codestream, 0 == not defined in this tile part */
	UINT8		m_nTNsot;

		/** Default constructor, initialises members */
	CSOTMarker();
		/** Virtual destructor */
	virtual ~CSOTMarker();

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


#endif // !NCSJPCSOTMARKER_H
