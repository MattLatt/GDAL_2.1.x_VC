/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSPooledAllocator.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Memory Pool types and protos.
** EDITS:
 *******************************************************/

#ifndef NCSPOOLEDALLOCATOR_H
#define NCSPOOLEDALLOCATOR_H

#ifndef NCSMEMPOOL_H
#include "NCSMemPool.h"
#endif

namespace tbb {
	template<typename T> class tbb_allocator;
	template<typename Key, typename T, typename HashCompare, typename A = tbb_allocator<std::pair<Key, T> > > class concurrent_hash_map;
}

#ifdef __cplusplus

#pragma warning(push)
#pragma warning(disable: 4251)

namespace NCS {

class NCSUTIL_IMPEX CPooledAllocator {
public:
	CPooledAllocator(UINT32	nMaxChunkSize = 1024*1024);
	virtual ~CPooledAllocator();
	void *Alloc(UINT32 nSize);
	void Free(void *p, UINT32 nSize);
private:
	UINT32	m_nMaxChunkSize;
	class NCSUTIL_IMPEX CItemsHash {
	public:
		bool equal( const UINT32& j, const UINT32& k ) const { return(j == k); };
		size_t hash( const UINT32& k ) { return((size_t)k); };
	};
	tbb::concurrent_hash_map<UINT32, CPool*, CItemsHash> *m_pPools;
};

}

#pragma warning(pop)

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CPool CNCSPool;
#endif

#endif /* __cplusplus */

#endif /* NCSPOOLEDALLOCATOR_H */
