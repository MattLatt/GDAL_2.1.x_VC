/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSMemPool.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Memory Pool types and protos.
** EDITS:
 *******************************************************/

#ifndef NCSMEMPOOL_H
#define NCSMEMPOOL_H

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif
#ifndef NCSARRAY_H
#include "NCSArray.h"
#endif
#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif
#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif
#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif

#ifdef __cplusplus

#pragma warning(push)
#pragma warning(disable: 4251)

extern "C" void NCSPoolInitAllocator(void);
extern "C" void NCSPoolFiniAllocator(void);

#include <vector>

namespace tbb {
	template<typename T> class cache_aligned_allocator;
	class queuing_mutex;
}

namespace NCS {

class NCSUTIL_IMPEX CPool {
public:
	typedef struct {
		UINT32 iElementSize;
		UINT32 nElementsPerNode;
		UINT32 nNodes;
		UINT32 nElementsInUse;

		UINT32 nAddNodes;
		NCSTimeStampMs tsAddNodeTime;
		
		UINT32 nRemoveNodes;
		NCSTimeStampMs tsRemoveNodeTime;
		
		UINT32 nAllocElements;
		NCSTimeStampMs tsAllocElementTime;
		
		UINT32 nFreeElements;
		NCSTimeStampMs tsFreeElementTime;
	} Stats;

	CPool(UINT32 iElementSize, UINT32 nElementsPerNode = 1024);
	virtual ~CPool();

	void *Alloc(bool bClear = false);
	void Free(void *p);
	void SetMaxSize(UINT32 nMaxElements);

	Stats GetStats();
	void EnableStats();
	void DisableStats();

	static void Init();
	static void Fini();

protected:
	class NCSUTIL_IMPEX CNode {
	public:
		CPool *m_pPool;
		INT32 m_nElementsInUse;
#ifdef NCS_64BIT
		INT64 m_iLastFreeElement;
#else
		INT32 m_iLastFreeElement;
#endif
		void *m_pElements;
		bool *m_pbElementInUse;

		CNode(CPool *pPool);
		virtual ~CNode();

		bool Init();
		void Free();
protected:
	friend class CPool;
		// md_fixme
		static tbb::cache_aligned_allocator<UINT8> *sm_pallocator;
	};

	// md_fixme
	tbb::queuing_mutex	*m_pMutex;
	Stats			m_Stats;
	UINT32			m_nMaxElements;
	bool			m_bCollectStats;
	std::vector<CNode*> m_Nodes;
	
	CNode *AddNode();
	void RemoveNode(CNode *pNode);
	void *GetElement();
	void FreeElement(void *pElement);
	bool InitNodeContents(CNode *pNode);
};

}

#pragma warning(pop)

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CPool CNCSPool;
#endif

#else
void NCSPoolInitAllocator(void);
void NCSPoolFiniAllocator(void);
#endif /* __cplusplus */

#endif /* NCSMEMPOOL_H */
