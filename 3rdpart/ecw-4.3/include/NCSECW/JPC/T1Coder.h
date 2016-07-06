/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:    T1Coder.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CT1Coder class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCT1CODER_H
#define NCSJPCT1CODER_H
 
#include "NCSEcw/JPC/Types/Types.h"
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSMemoryIOStream.h"
#ifndef NCSJPCSEGMENT_H
#include "NCSEcw/JPC/Types/Segment.h"
#endif // NCSJPCSEGMENT_H
#include "NCSEcw/JPC/Nodes/SubBand.h"

#define NCSJPC_ENCODE_BLOCK_SIZE 64

namespace NCS {
namespace JPC {
	/**
	 * CT1Coder class - the JPC T1 Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
	 */	
class NCSECW_IMPEX CT1Coder {
public:	
	const static int PREDICTABLE_TERMINATION;
	const static int PASS_RESET_CTX;
	const static int SEGMENT_SYMBOLS;
	const static int PASS_TERMINATION;
	const static int VERTICAL_CAUSAL_CTX;
	const static int SELECTIVE_CODING_BYPASS;

	CT1Coder();
	virtual ~CT1Coder();

	bool Decode(CSubBand::Type eSBType,
				UINT8 roiShift, 
				UINT8 nBits,
				UINT8 nZeroBits,
				std::vector<CSegment> &m_Segments,
				SDK::CBuffer2D *pDecBuf,
				int Flags,
				CQCDMarker &Quantization,
				UINT8 nComponentBits,
				UINT16 nLevels,
				UINT8 nResolution,
				IEEE4 fReconstructionParameter);
	bool Encode(CSubBand::Type eSBType, 
				UINT8 nBitsTotal,
				UINT8 &nZeroBits,
				std::vector<CSegment> &Segments,
				SDK::CBuffer2D *pDecBuf,
				UINT16 nRatio,
				UINT16 nLayers);
	static NCSTimeStampUs sm_usTotal;
	static NCSTimeStampUs sm_usLast;
	static UINT64 sm_nTotalSamples;
	static UINT64 sm_nLastSamples;
};
}
}

#endif // !NCSJPCT1CODER_H
