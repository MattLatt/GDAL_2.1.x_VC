/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     CodeBlock.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CCodeBlock class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCODEBLOCK_H
#define NCSJPCCODEBLOCK_H

#ifndef NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/Types.h"
#endif // NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/CodingStyleParameter.h"
#include "NCSEcw/JPC/Types/QuantizationParameter.h"
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#ifndef NCSJPCT1CODER_H
#include "NCSEcw/JPC/T1Coder.h"
#endif // NCSJPCT1CODER_H

namespace NCS {
namespace JPC {

	/**
	 * CCodeBlock class - the JPC codestream CodeBlock.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: jshao $ $Date: 2011/12/05 $ 
	 */	
class NCSECW_IMPEX CCodeBlock: public SDK::CNode2D
{
public:
		/** SubBand for this code block */
	class CSubBand *m_pSubBand;

		/** Lblock value - UINT8 represents <=256bits LENGTH field, = 32 bytes */
	UINT8	m_nLblock;
		/** Codeblock has zero width or height */
	bool	m_bZeroSize;
		/** Segments for this codeblock */	
	std::vector<CSegment>	m_Segments;
		/** Next Segments for this codeblock */	
	std::vector<CSegment>	m_NextSegments;

		/** Codeblock number within subband */
	UINT32	m_nCB;
		/** Layer # first included on */
	UINT32	m_nLayerIncluded;
		/** Number of passes included */
	UINT8	m_nPasses;
		/** Number of zero bits */
	UINT8	m_nZeroBits;

	UINT32	m_nDecodedToLayer;

		/** Default Constructor */
	CCodeBlock();
		/** Copy Constructor */
	CCodeBlock(const CCodeBlock &s);
		/** 
		 * Constructor 
		 * @param pSubBand	Parent subband this block belongs to
		 * @param nCB		Codeblock number within this subband
		 */
	CCodeBlock(CSubBand *pSubBand, UINT32 nCB);
		/** Virtual destructor */
	virtual ~CCodeBlock();
		/** 
		 * Init
		 * @param pSubBand	Parent subband this block belongs to
		 * @param nCB		Codeblock number within this subband
		 */
	void Init(CSubBand *pSubBand, UINT32 nCB);

		/** 
		 * Read a BufferType line from the input.
 		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
		/** 
		 * Write a BufferType line to the output.
 		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent);

	virtual INT32 ReadNewSegs(CIOStream &Stream);
	virtual INT32 WriteNewSegs(CIOStream &Stream);

		/** Decoded codeblock */
	CBuffer2D	m_DecBuf;

	static CT1Coder	sm_Coder;

	CError Decode(Type eType);
protected:

		/** 
		 * Decode the codeblock.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	

public:
	//friend class CView;
	static NCSTimeStampUs sm_usTotal;
	static NCSTimeStampUs sm_usLast;
	static UINT64 sm_nTotalSamples;
	static UINT64 sm_nLastSamples;
};
}
}

#endif // !NCSJPCCODEBLOCK_H
