/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     QCDMarker.h 
** CREATED:  12/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CQCDMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCQCDMARKER_H
#define NCSJPCQCDMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/QuantizationParameter.h"

namespace NCS {
namespace JPC {
	/**
	 * CQCDMarker class - the JPC QCD marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CQCDMarker: public CMarker, public CQuantizationParameter {
public:
		/** Default constructor, initialises members */
	CQCDMarker();
		/** Virtual destructor */
	virtual ~CQCDMarker();

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
		 * Assignment operator
		 * @param		src		QCC Marker.
		 * @return      CQCDMarker	Updated QCDMarker.
		 */
	class CQCDMarker& operator=( const class CQCCMarker& src );
};
}
}

#endif // !NCSJPCQCDMARKER_H
