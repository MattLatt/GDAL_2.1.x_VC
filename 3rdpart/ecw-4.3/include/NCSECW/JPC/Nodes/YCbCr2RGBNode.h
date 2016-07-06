/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     YCbCr2RGBNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CYCbCr2RGBNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCYCBCR2RGBNODE_H
#define NCSJPCYCBCR2RGBNODE_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H

namespace NCS {
namespace JPC {
	/**
	 * CYCbCr2RGBNode class - YCbCr to RGB colorspace conversion node.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CYCbCr2RGBNode: public SDK::CNode2D {
public:
		/** Default constructor, initialises members */
	CYCbCr2RGBNode();
		/** Virtual destructor */
	virtual ~CYCbCr2RGBNode();

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
};
}
}

#endif // !NCSJPCYCBCR2RGBNODE_H
