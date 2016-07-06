/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:    COCMarker.h 
** CREATED:  11/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CCOCMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCOCMARKER_H
#define NCSJPCCOCMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/CodingStyleParameter.h"

namespace NCS {
namespace JPC {

	/**
	 * CCOCMarker class - the JPC COD marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CCOCMarker: public CMarker {
public:

		/** Index of component which the marker relates to */
	UINT16			m_nCcoc;
		/** Coding style for this component */
	bool			m_bScoc;
		/** SP Coding style parameters */
	CCodingStyleParameter	m_SPcoc;

		/** Default constructor, initialises members */
	CCOCMarker();
		/** Copy constructor */
	CCOCMarker(const class CCODMarker &src);
		/** Virtual destructor */
	virtual ~CCOCMarker();

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
		 * @param		src		COD Marker.
		 * @return      CCOCMarker	Updated COCMarker.
		 */
	class CCOCMarker &operator=( const class CCODMarker &src );
};
}
}

#endif // !NCSJPCCODMARKER_H
