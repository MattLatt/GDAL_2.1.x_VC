/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Segment.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPC Segment class
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCSEGMENT_H
#define NCSJPCSEGMENT_H

#ifndef NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/Types.h"
#endif // NCSJPCTYPES_H

namespace NCS {
namespace JPC {

	// Segment class
class NCSECW_IMPEX CSegment {
public:
		/** Segment length */
	UINT32	m_nLength;
		/** Segment Index */
	UINT16	m_nIndex;
		/** Number of passes in this segment */
	UINT8	m_nPasses;
		/** Segment data */
	UINT8	*m_pData;
	
		/** Default constructor */
	CSegment();
		/** Copy constructor */
	CSegment(const CSegment &s);
		/** Virtual destructor */
	virtual ~CSegment();
};
}
}

#endif // NCSJPCSEGMENT_H
