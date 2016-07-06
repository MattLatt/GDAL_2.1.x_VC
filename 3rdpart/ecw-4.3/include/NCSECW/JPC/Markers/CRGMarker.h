/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     CRGMarker.h 
** CREATED:  19/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CCRGMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCRGMARKER_H
#define NCSJPCCRGMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {

	/**
	 * CCRGMarker class - the JPC CRG marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CCRGMarker: public CMarker {
public:
	typedef struct {
			/** Component horizontal offset on grid in 1/65535 units */
		UINT16		m_nXcrg;
			/** Component vertical offset on grid in 1/65535 units */
		UINT16		m_nYcrg;		
	} Registration;

		/** Component registration offsets */
	std::vector<Registration> m_Offsets;

		/** Default constructor, initialises members */
	CCRGMarker();
		/** Virtual destructor */
	virtual ~CCRGMarker();

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

#endif // !NCSJPCCRGMARKER_H
