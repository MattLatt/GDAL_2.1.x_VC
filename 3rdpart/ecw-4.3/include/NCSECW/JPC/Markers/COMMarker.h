/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     COMMarker.h 
** CREATED:  19/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CCOMMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCOMMARKER_H
#define NCSJPCCOMMARKER_H

#ifndef NCSJPCMARKER_H
#include "NCSEcw/JPC/Markers/Marker.h"
#endif // NCSJPCMARKER_H

namespace NCS {
namespace JPC {
	/**
	 * CCOMMarker class - the JPC COM marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CCOMMarker: public CMarker {
public:
	typedef enum {
		BINARY			= 0,
		IS_8859_LATIN	= 1
	} Registration;

	Registration	m_eRegistration;
	void			*m_pComment;

		/** Default constructor, initialises members */
	CCOMMarker();
		/** copy constructor */
	CCOMMarker(const CCOMMarker &s);
		/** Virtual destructor */
	virtual ~CCOMMarker();

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

#endif // !NCSJPCCOMMARKER_H
