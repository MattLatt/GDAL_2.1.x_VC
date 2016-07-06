/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     InputNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CInputNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWINPUTNODE_H
#define NCSECWINPUTNODE_H

#ifndef NCSJPCNodeTiler2D_H
#include "NCSEcw/SDK/NodeTiler2D.h"
#endif // NCSJPCNodeTiler2D_H

#include "NCSEcw/ECW/ECW.h"
#include "NCSEcw/ECW/File.h"

namespace NCS {
namespace ECW {
	class CFile;
	/**
	 * CInputNode class - Reads ECW input, currently also resamples to view size.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2011/12/12 $ 
	 */	
class NCSECW_IMPEX CInputNode: public SDK::CNode2D {
public:
		/** Default constructor, initialises members */
	CInputNode();
		/** Virtual destructor */
	virtual ~CInputNode();

	virtual bool Init(ContextID nCtx, QmfRegion *pRegion, ECW::CFile *pFile,INT32 old_level);
	virtual bool Fini(ContextID nCtx, QmfRegion *pRegion);

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);


	virtual bool DeleteIncompleteNodes(ContextID nCtx);
#ifdef NOTUSED
		/** 
		 * Get normal Node Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	virtual UINT32 GetNodeWidth(UINT16 iComponent = 0);
		/** 
		 * Get normal Node Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	virtual UINT32 GetNodeHeight(UINT16 iComponent = 0);
		/** 
		 * Get number of nodes wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0);
		/** 
		 * Get number of nodes high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0);
		/** 
		 * Get pointer to specific node.
		 * @param		UINT32		Node nr
		 * @return      CNode2D2D * Ptr to node.
		 */
	virtual CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0);
#endif
protected:
#ifdef COMP_WORKER
	class CReadJob: public CFile::CComponent::CWorkerJob {
	public:
		CReadJob(SDK::CNode2D::ContextID nCtx, CFile::CComponent *pComponent, SDK::CRect2D &r, SDK::CBuffer2D *pBuffer, bool bLock) { 
				m_nCtx = nCtx;
				m_pComponent = pComponent; 
				m_rect = r;
				m_pBuffer = pBuffer;
				m_bLock = bLock;
				m_bDone = false;
			};
		virtual ~CReadJob() {};
		virtual void Work(void) {
			tbb::task_scheduler_init ts(tbb::task_scheduler_init::automatic);
				m_pComponent->Read(m_nCtx, m_pBuffer, 0);
				m_bDone = true;
			};
		virtual bool Done() { return(m_bDone); };
		virtual bool Wait() { 
				bool bRet = true;
				LockStatus();
				while(!Done() || IsRunning()) {
					UnLockStatus();
					bRet = m_Event.Wait();
					LockStatus();
				}
				UnLockStatus();
				return(bRet);
			};
		virtual void SetRunning(bool bRunning) { 
				CFile::CComponent::CWorkerJob::SetRunning(bRunning);
				if(!bRunning) {
					m_Event.Set();		
				}
			};
	protected:
		SDK::CNode2D::ContextID	m_nCtx;
		CFile::CComponent		*m_pComponent;
		SDK::CRect2D					m_rect;
		SDK::CBuffer2D				*m_pBuffer;
		bool					m_bLock;
		bool					m_bDone;
		CEvent					m_Event;
	};

	class CReadJobs: public std::vector<CReadJob*> {
	public:
		CReadJobs() {};
		virtual ~CReadJobs() { Wait(); };

		void Wait(void) {
				for(int i = 0; i < (int)size(); i++) {
					(*this)[i]->Wait();
					delete (*this)[i];
				}
				clear();
			};
	};
#endif
	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
		class CInputInfo {
		public:
			CInputInfo() {};
			virtual ~CInputInfo() {};
			SDK::CBuffer2DVector	m_Buffers;
#ifdef COMP_WORKER
			CReadJobs				m_Jobs;
#endif
		};

		struct QmfRegion		*m_pRegion;		
		std::vector<CInputInfo*> m_Inputs;
		UINT32					m_nCurrentInput;
		CRect2D					m_LastRead;
		INT32					m_X;
		INT32					m_Y;
		BOOLEAN					m_bManageICC;
		INT32					m_old_level;

		Context() : m_pRegion(NULL) { m_nCurrentInput = 0; };
		virtual ~Context() { while(m_Inputs.size()) { delete m_Inputs[0]; m_Inputs.erase(m_Inputs.begin()); } };
	};
	
	
	
	class CFile *m_pFile;
	
	static CNode2DTracker	sm_Tracker;

	virtual bool Resample1Line(Context *pCtx, CBuffer2D *pDst, INT16 *p_in, int nShift);
	virtual CNode2D::Context *NewContext() {
			return((CNode2D::Context*)new Context());
		};
};
}
}

#endif // !NCSECWINPUTNODE_H
