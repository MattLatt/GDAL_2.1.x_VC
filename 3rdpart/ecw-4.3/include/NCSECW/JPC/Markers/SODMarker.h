/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   SODMarker.h 
** CREATED:  11/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSODMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCSODMARKER_H
#define NCSJPCSODMARKER_H

#ifndef NCSJPCMARKER_H
#include "NCSEcw/JPC/Markers/Marker.h"
#endif // NCSJPCMARKER_H

namespace NCS {
namespace JPC {
	/**
	 * CSODMarker class - the JPC SOD marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: jshao $ $Date: 2012/07/12 $ 
	 */	
class NCSECW_IMPEX CSODMarker: public CMarker {
public:
		/** Byte offset of this tile-part's data in the codestream */
	INT64	m_nDataOffset;

		/** Default constructor, initialises members */
	CSODMarker();
		/** Virtual destructor */
	virtual ~CSODMarker();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream, void *pioMainTP, void *pinCTP);	// pass in caller's address 12/07/2012
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

private:
	INT64 FindSOP(CIOStream &Stream, UINT64 &nBytesLeft);

};
}
}

#endif // !NCSJPCSODMARKER_H
