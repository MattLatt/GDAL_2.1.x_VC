/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Precinct.h 
** CREATED:  11/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPrecinct class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPRECINCT_H
#define NCSJPCPRECINCT_H

#ifndef NCSJPCSUBBAND_H
#include "NCSEcw/JPC/Nodes/SubBand.h"
#endif // NCSJPCSUBBAND_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#include "NCSEcw/JPC/Progression.h"
//#include "NCSEcw/JPC/Types/ProgressionOrderType.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSPacketIOStream.h"
#include "NCSThreadPool.h"

#include "NCSEcw/JPC/Packet.h" // Against bug EMS-407, added 25/10/2011

namespace tbb {
	class spin_mutex;
}


namespace NCS {
namespace JPC {
	/**
	 * CPrecinct class - the JPC codestream Precinct.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #8 $ $Author: jshao $ $Date: 2012/01/09 $ 
	 */	
class NCSECW_IMPEX CPrecinct: public SDK::CNode2D {
public:
		/** Resolution level for this precinct */
	class CResolution *m_pResolution;
		/** Precinct number within this resolution */
	UINT32	m_nPrecinct;

		/** SubBands in this precinct */
	CSubBand *m_SubBands[4];

		/** Packet number for each layer in this precinct */
	std::vector<UINT32> m_Packets;

		/** Packets for each layer in this precinct */
	std::vector<CPacket*> m_pPackets; // Against bug EMS-407, added 25/10/2011

	UINT16	m_nProgressionLayer;
		/** Precinct has zero width or height */
	bool	m_bZeroSize;		
		/** 
	     * Constructor 
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 */
	CPrecinct(class CResolution *pResolution, UINT32 nPrecinct, bool bCreateSubBands = false);
		/** Virtual destructor */
	virtual ~CPrecinct();

		/** 
		 * Get X0 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	//virtual NCS_INLINE INT32 GetX0() { return(CRect2D::GetX0()); };
		/** 
		 * Get Y0 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	//virtual NCS_INLINE INT32 GetY0() { return(CRect2D::GetY0()); };
		/** 
		 * Get X1 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	//virtual NCS_INLINE INT32 GetX1() { return(CRect2D::GetX1()); };
		/** 
		 * Get Y1 of this rect.
		 * @return      INT32		Coordinate value.
		 */
	//virtual NCS_INLINE INT32 GetY1() { return(CRect2D::GetY1()); };

		/** 
		 * Get X0 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetX0(CResolution *pResolution, INT32 nPrecinct);
		/** 
		 * Get X0 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinctX	Precinct X within resolution level
		 * @param nPrecinctY	Precinct Y within resolution level
		 * @return      INT32	Coordinate value.
		 */
	static INT32 sGetX0(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

		/** 
		 * Get Y0 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetY0(CResolution *pResolution, INT32 nPrecinct);
		/** 
		 * Get Y0 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinctX	Precinct X within resolution level
		 * @param nPrecinctY	Precinct Y within resolution level
		 * @return      INT32	Coordinate value.
		 */
	static INT32 sGetY0(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

		/** 
		 * Get X1 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetX1(CResolution *pResolution, INT32 nPrecinct);
		/** 
		 * Get X1 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinctX	Precinct X within resolution level
		 * @param nPrecinctY	Precinct Y within resolution level
		 * @return      INT32	Coordinate value.
		 */
	static INT32 sGetX1(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

		/** 
		 * Get Y1 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 * @return      INT32		Coordinate value.
		 */
	static INT32 sGetY1(CResolution *pResolution, INT32 nPrecinct);
		/** 
		 * Get Y1 of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinctX	Precinct X within resolution level
		 * @param nPrecinctY	Precinct Y within resolution level
		 * @return      INT32	Coordinate value.
		 */
	static INT32 sGetY1(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

		/** 
		 * Get Width of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 * @return      INT32		Width.
		 */
//	static INT32 GetWidth(CResolution *pResolution, INT32 nPrecinct);
		/** 
		 * Get Height of specified precinct.
		 * @param pResolution	Parent resolution level
		 * @param nPrecinct		Precinct number within resolution level
		 * @return      INT32		Coordinate value.
		 */
//	static INT32 GetHeight(CResolution *pResolution, INT32 nPrecinct);

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);

	virtual CNode2D::Context *NewContext() {
		return((CNode2D::Context*)new CPrecinct::Context());
	};

	void AddRef();
	void UnRef();
	UINT32 NrRefs();
	static bool HaveZeroRefs();
	static UINT32	ZeroRefs(){ return sm_nZeroRefs;}

		/**
		 * Create the subbands
		 */
	virtual bool CreateSubBands(ContextID nCtx, bool bRead, NCS::SDK::CBuffer2D::Type eType = NCS::SDK::CBuffer2D::BT_INT16);
		/**
		 * Destroy the subbands
		 */
	virtual bool DestroySubBands();

	virtual void Request(CIOStream *pStream);
	virtual void Cancel(CIOStream *pStream);

	void SetContextQualityLevel(ContextID nCtx, UINT32 nLevel);
	virtual bool UnLink(ContextID nCtx, UINT16 nInputs);

	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets();	// Against bug EMS-407, added 02/11/2011

	bool m_bSubBandsRead;
	UINT32 m_nDecodedToLayer;
	friend class CSubBand;

	CPrecinct *m_pNext;
	CPrecinct *m_pPrev;
protected:

	class NCSECW_IMPEX Context : public NCS::SDK::CNode2D::Context
	{
	public:
		UINT32 m_nRequestedQualityLevel;

		Context();
	};

	static UINT32				sm_nZeroRefs;
	
		/** Packet Read error is stored here */
	CError	m_Error;
		/** Reference count */
	UINT32		m_nRefs;
		/**
		 * Read the packets for this precinct
		 */	
	virtual bool ReadPackets(ContextID nCtx);	

	tbb::spin_mutex *m_pMutex_Refs;
	tbb::spin_mutex *m_pMutex;
	bool m_bSubBandsQueued;
};

class NCSECW_IMPEX CSPPrecinct: public NCS::JPC::CPrecinct {
public:
	CSPPrecinct(class CResolution *pResolution, UINT32 nPrecinct, bool bCreateSubBands = false) : CPrecinct(pResolution, nPrecinct, bCreateSubBands) {};
	virtual bool CreateSubBands(ContextID nCtx, bool bRead, NCS::SDK::CBuffer2D::Type eType = NCS::SDK::CBuffer2D::BT_INT16);
};

class NCSECW_IMPEX CMTPrecinct: public NCS::JPC::CPrecinct {
public:
	CMTPrecinct(class CResolution *pResolution, UINT32 nPrecinct, bool bCreateSubBands = false);
	virtual ~CMTPrecinct();
	virtual bool CreateSubBands(ContextID nCtx, bool bRead, NCS::SDK::CBuffer2D::Type eType = NCS::SDK::CBuffer2D::BT_INT16);

	class CReadJob: public NCS::CThreadPool::CJob {
	public:
		CReadJob(NCS::JPC::CSubBand *pSubBand, NCS::SDK::CBuffer2D::Type eType) { 
				m_pSubBand = pSubBand;
				m_eType = eType;
				m_bDone = false;
			};
		virtual ~CReadJob() {};
		virtual void Work(void) {
				//this seems to slow it down
				//tbb::task_scheduler_init ts(tbb::task_scheduler_init::automatic);

				m_pSubBand->DecodeCodeBlocks(m_eType);
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
				NCS::CThreadPool::CJob::SetRunning(bRunning);
				if(!bRunning) {
					m_Event.Set();		
				}
			};
	protected:
		NCS::JPC::CSubBand			*m_pSubBand;
		NCS::SDK::CBuffer2D::Type	m_eType;
		bool						m_bDone;
		NCS::CEvent					m_Event;
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
	CReadJobs m_Jobs;
};

class NCSECW_IMPEX CPrecinctMap : public NCS::SDK::CNode2D {
public:
/*	const CPrecinct *&operator[](const UINT& _Key) {
		std::map<UINT32, CPrecinct*>::iterator it = find(_Key);
		if(it == end()) {
			static CPrecinct *NP = NULL;
			return(NP);
		} else {
			return(it->second);
		}
	}*/
	CPrecinctMap();
	virtual ~CPrecinctMap();
	void Init(class CResolution *pResolution);
	void ResetProgressionLayer(void);
	CPrecinct *find_or_create(UINT32 x, UINT32 y, bool *bCreated = NULL);
	CPrecinct *find(UINT32 nPrecinct, bool bRef = false);
	CPrecinct *find(UINT32 x, UINT32 y, bool bRef = false);
	CPrecinct *findPacketPrecinct(UINT32 nPacket, bool bRef = false);
	bool empty(UINT32 nY);
	void Clear(bool bLock = true); // Shao changed on 08/11/2011 to speed up when lock is not needed
	bool ClearIfZero();
	UINT32 GetMaxSize() {
		return(m_nWidth * m_nHeight);
	}
	__inline UINT32 size(void) {
		return(m_nItems);
	}	
	bool remove(UINT32 nPrecinct);
	bool remove(UINT32 x, UINT32 y);
	bool remove(CPrecinct *p);

	UINT32 remove_front();
#ifdef NOT_DEF
	void insert(ContextID nCtx, UINT32 nPrecinct, CPrecinct *p);
	void insert(ContextID nCtx, UINT32 x, UINT32 y, CPrecinct *p);
	void insert(UINT32 nPrecinct, CPrecinct *p);
	void insert(UINT32 x, UINT32 y, CPrecinct *p);
#endif
		/** 
		 * UnLink all precincts.
		 * @param		nCtx		Read context
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool UnLink(SDK::CNode2D::ContextID nCtx, UINT16 nInputs = 0);

	CNode2D::Context *NewContext() {
		return((CNode2D::Context*)new Context());
	};

	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets();	// Against bug EMS-407, added 02/11/2011
	bool HasRefs();	// Against bug EMS-407, added 01/12/2011
	void AddRefs();	// Against bug EMS-407, added 29/11/2011

	tbb::spin_mutex *m_pMutex_Map; // Against bug EMS-407, added 29/11/2011

private:
	class NCSECW_IMPEX CPrecinctMapRow {
	public:
		typedef std::map<UINT32, CPrecinct*>::iterator CPrecinctMapColumnIterator;
		tbb::spin_mutex *m_pMutex;
		std::map<UINT32, CPrecinct*> m_Columns;

		CPrecinctMapRow();
		virtual ~CPrecinctMapRow();

		void Clear(bool bLock = true); // Shao changed on 08/11/2011 to speed up when lock is not needed
		
		void MoveBack(CPrecinct *pPrecinct);

		CPrecinct *m_pFront;
		CPrecinct *m_pBack;
	};
	typedef std::vector<CPrecinctMapRow>::iterator CPrecinctMapRowIterator;

	class NCSECW_IMPEX ICProgressionOrderPacketCalculator
	{
	public:
		ICProgressionOrderPacketCalculator() {
			m_pPrecinctMap = NULL;
			m_pResolution = NULL;
			m_nComponents = 0;
			m_nWidth = 0;
			m_nHeight = 0;
			m_nLayers = 0;
		};
		virtual ~ICProgressionOrderPacketCalculator() {
			m_pPrecinctMap = NULL; // just in case re-visit it, added on 01/12/2011
			m_pResolution = NULL; // just in case re-visit it, added on 01/12/2011
		};

		virtual void Init(CPrecinctMap *pPrecinctMap);
		virtual void Set(UINT32 x, UINT32 y, CPrecinct *p) = 0;
	protected:
		class CPrecinctMap *m_pPrecinctMap;
		class CResolution *m_pResolution;
		UINT32 m_nComponents;
		UINT32 m_nWidth;
		UINT32 m_nHeight;
		INT32 m_nLayers;
	};

	class NCSECW_IMPEX CPacketCalculatorLRCP : public ICProgressionOrderPacketCalculator
	{
	public:
		CPacketCalculatorLRCP() : ICProgressionOrderPacketCalculator() {
			m_nLayerFactor = 0;
			m_nResolutionOffset = 0;
		};
		virtual ~CPacketCalculatorLRCP() {};

		virtual void Init(CPrecinctMap *pPrecinctMap);
		virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
	private:
		UINT32 m_nLayerFactor;
		UINT32 m_nResolutionOffset;
	};

	class NCSECW_IMPEX CPacketCalculatorRLCP : public ICProgressionOrderPacketCalculator
	{
	public:
		CPacketCalculatorRLCP() : ICProgressionOrderPacketCalculator() {
			m_nStartPrecinctNumber = 0;
		};
		virtual ~CPacketCalculatorRLCP() {};

		virtual void Init(CPrecinctMap *pPrecinctMap);
		virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
	private:
		UINT32 m_nStartPrecinctNumber;
	};

	class NCSECW_IMPEX CPacketCalculatorRPCL : public ICProgressionOrderPacketCalculator
	{
	public:
		CPacketCalculatorRPCL() : ICProgressionOrderPacketCalculator() {
			m_nStartPrecinctNumber = 0;
		};
		virtual ~CPacketCalculatorRPCL() {};

		virtual void Init(CPrecinctMap *pPrecinctMap);
		virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
	private:
		UINT32 m_nStartPrecinctNumber;
	};

	class NCSECW_IMPEX CPacketCalculatorPCRL : public ICProgressionOrderPacketCalculator
	{
	public:
		CPacketCalculatorPCRL() : ICProgressionOrderPacketCalculator(){
			m_nResolutions = 0;
			m_XOffset.resize(0);
			m_Widths.resize(0);
			m_Heights.resize(0);
		};
		virtual ~CPacketCalculatorPCRL() {
			m_XOffset.clear();
			m_Widths.clear();
			m_Heights.clear();
		};

		virtual void Init(CPrecinctMap *pPrecinctMap);
		virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
	private:
		UINT32 m_nResolutions;
		std::vector<UINT32> m_XOffset;
		std::vector<UINT32> m_Widths;
		std::vector<UINT32> m_Heights;
	};

	class NCSECW_IMPEX CPacketCalculatorCPRL : public ICProgressionOrderPacketCalculator
	{
	public:
		CPacketCalculatorCPRL() {
			m_nComponentOffset = 0;
			m_nResolutionOffset = 0;
		};
		virtual ~CPacketCalculatorCPRL() {};

		virtual void Init(CPrecinctMap *pPrecinctMap);
		virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
	private:
		UINT32 m_nComponentOffset;
		UINT32 m_nResolutionOffset;
	};

	UINT32 m_nWidth;
	UINT32 m_nHeight;
	class CResolution *m_pResolution;
	
	UINT32 m_nItems;

	std::vector<CPrecinctMapRow *> m_Rows;
	ICProgressionOrderPacketCalculator *m_pProgressionOrderCalculator;

};
}
}

#endif // !NCSJPCPRECINCT_H
