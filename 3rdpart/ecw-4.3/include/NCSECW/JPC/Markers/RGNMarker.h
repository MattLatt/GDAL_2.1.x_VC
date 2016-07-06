/********************************************************
*** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     rGNMarker.h 
** CREATED:  12/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CRGNMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCRGNMARKER_H
#define NCSJPCRGNMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {
	/**
	 * CRGNMarker class - the JPC RGN marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CRGNMarker: public CMarker {
public:
	typedef enum {
		IMPLICIT	= 0
	} Style;
		/** Index of component this region is for */
	UINT16		m_nCrgn;
		/** Style of the region */
	Style		m_eSrgn;
		/** Resion style parameter */
	UINT8		m_nSPrgn;

		/** Default constructor, initialises members */
	CRGNMarker();
		/** Virtual destructor */
	virtual ~CRGNMarker();

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


#endif // !NCSJPCRGNMARKER_H
