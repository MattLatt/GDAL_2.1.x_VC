/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     QCCMarker.h 
** CREATED:  12/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CQCCMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCQCCMARKER_H
#define NCSJPCQCCMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/QuantizationParameter.h"

namespace NCS {
namespace JPC {
	/**
	 * CQCCMarker class - the JPC QCC marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CQCCMarker: public CMarker, public CQuantizationParameter {
public:
		/** Component index for this QCC */
	UINT16	m_nCqcc;

		/** Default constructor, initialises members */
	CQCCMarker();
		/** Copy constructor */
	CQCCMarker(const class CQCDMarker& src);
		/** Virtual destructor */
	virtual ~CQCCMarker();

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
		 * @param		src		QCD Marker.
		 * @return      CQCCMarker	Updated QCCMarker.
		 */
	class CQCCMarker& operator=( const class CQCDMarker& src );
};
}
}

#endif // !NCSJPCQCCMARKER_H
