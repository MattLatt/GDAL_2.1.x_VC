/********************************************************
*** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Component.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CComponent class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCOMPONENT_H
#define NCSJPCCOMPONENT_H

#ifndef NCSJPCCODMARKER_H
#include "NCSEcw/JPC/Markers/CODMarker.h"
#endif // NCSJPCCODMARKER_H
#ifndef NCSJPCQCDMARKER_H
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#endif // NCSJPCQCDMARKER_H
#ifndef NCSJPCRESOLUTION_H
#include "NCSEcw/JPC/Nodes/Resolution.h"
#endif // NCSJPCRESOLUTION_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H

#include "NCSThreadPool.h"

namespace NCS {
namespace JPC {

	/**
	 * CComponent class - the JPC Tile Component.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #5 $ $Author: jshao $ $Date: 2012/01/09 $ 
	 */	
class NCSECW_IMPEX CComponent: public SDK::CNode2D {
public:

	static const UINT32 CREATE;
	static const UINT32 ADDREF;
	static const UINT32 UNREF;
	static const UINT32 STATS;
	static const UINT32 REQUEST;
	static const UINT32 CANCEL;

		/** Tile-Part for this component */
	class CTilePartHeader *m_pTilePart;

		/** Component index */
	UINT16	m_iComponent;
		/** Component is paletted */
	bool	m_bPaletted;

		/** CodingStyle */
	CCODMarker	m_CodingStyle;
		/** QuantizationStyle */
	CQCDMarker	m_QuantizationStyle;
		/** Decomposition Resolution levels */
	std::vector<CResolution*> m_Resolutions;

		/** Deepest Allocated Resolution (< m_Resolutions.size()) being decoded for the view */
	INT32	m_nDeepestAllocatedResolution; // Against bug EMS-407, added 19/10/2011

		/** Deepest Working Resolution (< m_Resolutions.size()) being decoded for the view */
	//INT32	m_nDeepestWorkingResolution; // Against bug EMS-407, added 17/11/2011
	// As different thread may have different Deepes tWorking Resolution, this should be used

		/** 
		 * Constructor
		 * @param pTilePart		Parent tile part
		 * @param iComponent	Component index
		 */
	//CComponent(CTilePartHeader *pTilePart, UINT16 iComponent);
	CComponent(CTilePartHeader *pTilePart, UINT16 iComponent, INT32 nDeepestAllocatedResolution = -1); // changed on 08/12/2011
		/** Virtual destructor */
	virtual ~CComponent();

		/**
		 * Get the context for the given ContextID
		 * @param		nCtx		ContextID to retrive context
		 * @param		bAutoConstruct Autoconstruct a new context for this ContextID if one doesn't exist
		 * @return		Context*	Context for given ID.
		 */
//	virtual CNode2D::Context *GetContext(ContextID nCtx, bool bAutoConstruct = true);
	virtual CNode2D::Context *NewContext() {
				return((CNode2D::Context*)new Context());
			};
		/** 
		 * Link this node to the view.
		 * @param		nCtx		Read context
		 * @param		nResolution Input resolution level.
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool Link(CNode2D::ContextID nCtx, UINT32 nResolution, INT32 nXCMul, INT32 nYCMul);
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
	UINT8 TraversePyramid(ContextID nCtx, UINT32 nResolution, UINT32 Flags, INT32 nTopX, INT32 nLeftY, INT32 nBottomX, INT32 nRightY, UINT32 nViewSizeX, UINT32 nViewSizeY, UINT32 &nBlocksTotal, UINT32 &nBlocksAvailable, UINT32 nMaxQualityLayer = 0xffffffff, bool bCreatePackets = false); // changed on 14/12/2011
	void AddResolutions(INT32 &nDeepestResolution_curr); // Against bug EMS-407, added 21/10/2011
	bool Check_n_AddResolutions(INT32 nDeepestResolution_curr);// Against bug EMS-407, added 30/11/2011
	bool PurgeResolutions(INT32 nPurgeResolution = 0);	// Against bug EMS-407, added 01/12/2011
	bool PurgeResolution(INT32 nDeepestResolution_curr);	// Against bug EMS-407, added 01/12/2011
	void RES_SAFE_DELETE(INT32 r, bool bReducingDeepestAllocatedResolution = true);// Against bug EMS-407, added 30/11/2011
	void RES_SAFE_CLEAR(INT32 r, bool bReducingDeepestAllocatedResolution = true);// Against bug EMS-407, added 03/12/2011
	void RES_SAFE_CREATE(INT32 r);// Against bug EMS-407, added 09/12/2011
	NCS_INLINE UINT8 NrResolutions() { return((UINT8)m_Resolutions.size()); } // Against bug EMS-407, added 21/10/2011
	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets();	// Against bug EMS-407, added 02/11/2011
	void CalcPrecinctSize(UINT32 nResolution, INT32 &nioNumPrecinctsWide, INT32 &nioNumPrecinctsHigh); // Against bug EMS-407, added 16/12/2011

	virtual void QueueJob(NCS::CThreadPool::CJob *p) { 
#ifdef LOTS_OF_THREADS
		m_Worker.QueueJob(p); 
#else
		CComponent::sm_Worker.QueueJob(p); 
#endif
	};
		
protected:

	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
		INT32 m_nXCMul;
		INT32 m_nYCMul;
		Context();
		virtual ~Context();
	};
#ifdef LOTS_OF_THREADS
	NCS::CThreadPool m_Worker;
#else
	static NCS::CThreadPool sm_Worker;
#endif

	//tbb::spin_mutex *m_pMutex_Comp; // Against bug EMS-407, added 30/11/2011
	NCS::CMutex m_Mutex_Comp;// Against bug EMS-407, changed 21/12/2011, as spin lock for short term

};
}
}
#endif // !NCSJPCCOMPONENT_H
