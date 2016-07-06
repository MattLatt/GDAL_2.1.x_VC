/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.,  
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/

// INCSPerformanceManager.h: interface for the INCSPerformanceManager class.
//

#pragma once

namespace NCS {
namespace SDK {

class CPerfMonCache :
	public CPerfObject
{
public:
	DECLARE_PERF_CATEGORY(CPerfMonCache, 1, L"ECW/JP2 Cache", _T("ECW/JP2 Cache Statistics"), -1);
public:
	BEGIN_COUNTER_MAP(CPerfMonCache)
		DEFINE_COUNTER(m_nCacheBytes, _T("TotalCacheBytes"), _T("Total Cache size in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nSetViewHits, _T("TotalSetViewHits"), _T("Total number of times a SetView call hit a cached block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nSetViewMisses, _T("TotalSetViewMisses"), _T("Total number of times a SetView call misses the cache."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadHits, _T("TotalReadHits"), _T("Total number of times a read hit a cached block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadUnpackedHits, _T("TotalReadUnpackedHits"), _T("Total number of times a read hit an unpacked cached block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadMisses, _T("TotalReadMisses"), _T("Total number of times a read missed a cached block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadBypass, _T("TotalReadBypass"), _T("Total number of times a read bypassed the cached."), PERF_COUNTER_LARGE_RAWCOUNT, -1)

		DEFINE_COUNTER(m_nECWCacheBytes, _T("TotalECWCacheBytes"), _T("Total ECW Cache size in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nECWPackedBytes, _T("TotalECWPackedBytes"), _T("Total size of packed ECW blocks in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nECWUnPackedBytes, _T("TotalECWUnPackedBytes"), _T("Total size of unpacked ECW blocks in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nSetViewECWHits, _T("TotalSetViewECWHits"), _T("Total number of times a SetView call hit a cached ECW block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nSetViewECWMisses, _T("TotalSetViewECWMisses"), _T("Total number of times a SetView call misses the ECW cache."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadECWHits, _T("TotalReadECWHits"), _T("Total number of times a read hit a cached ECW block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadECWUnpackedHits, _T("TotalReadECWUnpackedHits"), _T("Total number of times a read hit an unpacked cached ECW block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadECWMisses, _T("TotalReadECWMisses"), _T("Total number of times a read missed a cached ECW block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadECWBypass, _T("TotalReadECWBypass"), _T("Total number of times a read bypassed the ECW cached."), PERF_COUNTER_LARGE_RAWCOUNT, -1)

		DEFINE_COUNTER(m_nJP2CacheBytes, _T("TotalJP2CacheBytes"), _T("Total JP2 Cache size in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nJP2PackedBytes, _T("TotalJP2PackedBytes"), _T("Total size of packed JP2 blocks in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nJP2UnPackedBytes, _T("TotalJP2UnPackedBytes"), _T("Total size of unpacked JP2 blocks in bytes"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nSetViewJP2Hits, _T("TotalSetViewJP2Hits"), _T("Total number of times a SetView call hit a cached JP2 block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nSetViewJP2Misses, _T("TotalSetViewJP2Misses"), _T("Total number of times a SetView call misses the JP2 cache."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadJP2Hits, _T("TotalReadJP2Hits"), _T("Total number of times a read hit a cached JP2 block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadJP2UnpackedHits, _T("TotalReadJP2UnpackedHits"), _T("Total number of times a read hit an unpacked cached JP2 block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadJP2Misses, _T("TotalReadJP2Misses"), _T("Total number of times a read missed a cached JP2 block."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nReadJP2Bypass, _T("TotalReadJP2Bypass"), _T("Total number of times a read bypassed the JP2 cached."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
	END_COUNTER_MAP()
public:
	ULONGLONG m_nCacheBytes;
	ULONGLONG m_nSetViewHits;
	ULONGLONG m_nSetViewMisses;
	ULONGLONG m_nReadHits;
	ULONGLONG m_nReadUnpackedHits;
	ULONGLONG m_nReadMisses;
	ULONGLONG m_nReadBypass;

	ULONGLONG m_nECWCacheBytes;
	ULONGLONG m_nECWPackedBytes;
	ULONGLONG m_nECWUnPackedBytes;
	ULONGLONG m_nSetViewECWHits;
	ULONGLONG m_nSetViewECWMisses;
	ULONGLONG m_nReadECWHits;
	ULONGLONG m_nReadECWUnpackedHits;
	ULONGLONG m_nReadECWMisses;
	ULONGLONG m_nReadECWBypass;

	ULONGLONG m_nJP2CacheBytes;
	ULONGLONG m_nJP2PackedBytes;
	ULONGLONG m_nJP2UnPackedBytes;
	ULONGLONG m_nSetViewJP2Hits;
	ULONGLONG m_nSetViewJP2Misses;
	ULONGLONG m_nReadJP2Hits;
	ULONGLONG m_nReadJP2UnpackedHits;
	ULONGLONG m_nReadJP2Misses;
	ULONGLONG m_nReadJP2Bypass;
};

class CPerfMonFiles :
	public CPerfObject
{
public:
	DECLARE_PERF_CATEGORY(CPerfMonFiles, 2, _T("ECW/JP2 Files"), _T("ECW/JP2 File Statistics"), -1);
public:
	BEGIN_COUNTER_MAP(CPerfMonFiles)
		DEFINE_COUNTER(m_nOpen, _T("TotalFilesOpen"), _T("Total number of ECW/JP2 files open"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nCached, _T("TotalFilesCached"), _T("Total number of ECW/JP2 files cached"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nCacheHits, _T("TotalFilesCacheHits"), _T("Total number of ECW/JP2 files cache hits"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nCacheMisses, _T("TotalFilesCacheMisses"), _T("Total number of ECW/JP2 files cache misses"), PERF_COUNTER_RAWCOUNT, 0)

		DEFINE_COUNTER(m_nECWOpen, _T("TotalECWFilesOpen"), _T("Total number of ECW files open"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nECWCached, _T("TotalECWFilesCached"), _T("Total number of ECW files cached"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nECWCacheHits, _T("TotalECWFilesCacheHits"), _T("Total number of ECW files cache hits"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nECWCacheMisses, _T("TotalECWFilesCacheMisses"), _T("Total number of ECW files cache misses"), PERF_COUNTER_RAWCOUNT, 0)

		DEFINE_COUNTER(m_nJP2Open, _T("TotalJP2FilesOpen"), _T("Total number of JP2 files open"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nJP2Cached, _T("TotalJP2FilesCached"), _T("Total number of JP2 files cached"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nJP2CacheHits, _T("TotalJP2FilesCacheHits"), _T("Total number of JP2 files cache hits"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nJP2CacheMisses, _T("TotalJP2FilesCacheMisses"), _T("Total number of JP2 files cache misses"), PERF_COUNTER_RAWCOUNT, 0)
	END_COUNTER_MAP()
public:
	ULONG m_nOpen;
	ULONG m_nCached;			// number of files currently cached but not open
	ULONG m_nCacheHits;		// number of times an open found an existing cached file
	ULONG m_nCacheMisses;		// number of times an open could not find the file in cache

	ULONG m_nECWOpen;
	ULONG m_nECWCached;			// number of files currently cached but not open
	ULONG m_nECWCacheHits;		// number of times an open found an existing cached file
	ULONG m_nECWCacheMisses;		// number of times an open could not find the file in cache

	ULONG m_nJP2Open;
	ULONG m_nJP2Cached;			// number of files currently cached but not open
	ULONG m_nJP2CacheHits;		// number of times an open found an existing cached file
	ULONG m_nJP2CacheMisses;		// number of times an open could not find the file in cache
};

class CPerfMonFileViews :
	public CPerfObject
{
public:
	DECLARE_PERF_CATEGORY(CPerfMonFileViews, 3, _T("ECW/JP2 FileViews"), _T("ECW/JP2 FileView Statistics"), -1);
public:
	BEGIN_COUNTER_MAP(CPerfMonFileViews)
		DEFINE_COUNTER(m_nOpen, _T("TotalFileViewsOpen"), _T("Total number of ECW/JP2 FileViews open"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nCached, _T("TotalFileViewsCached"), _T("Total number of ECW/JP2 FileViews cached"), PERF_COUNTER_RAWCOUNT, 0)

		DEFINE_COUNTER(m_nECWOpen, _T("TotalECWFileViewsOpen"), _T("Total number of ECW FileViews open"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nECWCached, _T("TotalECWFileViewsCached"), _T("Total number of ECW FileViews cached"), PERF_COUNTER_RAWCOUNT, 0)

		DEFINE_COUNTER(m_nJP2Open, _T("TotalJP2FileViewsOpen"), _T("Total number of JP2 FileViews open"), PERF_COUNTER_RAWCOUNT, 0)
		DEFINE_COUNTER(m_nJP2Cached, _T("TotalJP2FileViewsCached"), _T("Total number of JP2 FileViews cached"), PERF_COUNTER_RAWCOUNT, 0)
	END_COUNTER_MAP()
public:
	ULONG m_nOpen;			// number of file views currently open
	ULONG m_nCached;

	ULONG m_nECWOpen;			// number of file views currently open
	ULONG m_nECWCached;

	ULONG m_nJP2Open;			// number of file views currently open
	ULONG m_nJP2Cached;
};

class CPerfMonECWP :
	public CPerfObject
{
public:
	DECLARE_PERF_CATEGORY(CPerfMonECWP, 4, _T("ECW/JP2 ECWP"), _T("ECW/JP2 ECWP Statistics"), -1);
public:
	BEGIN_COUNTER_MAP(CPerfMonECWP)
		DEFINE_COUNTER(m_nRequestsSent, _T("TotalRequestsSent"), _T("Total number of block read requests sent to the server"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nCancelsSent, _T("TotalCancelsSent"), _T("Total number of block read request cancels sent to the server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nBlocksReceived, _T("TotalBlocksReceived"), _T("Total number of blocks received."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nCancelledBlocksReceived, _T("TotalCancelledBlocksReceived"), _T("Total number of blocks cancelled that were still received."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nRequestsXmitPending, _T("TotalRequestsXmitPending"), _T("Total number of block read requests waiting to be sent to server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nCancelsXmitPending, _T("TotalCancelsXmitPending"), _T("Total number of block cancel requests waiting to be sent to server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)

		DEFINE_COUNTER(m_nECWRequestsSent, _T("TotalECWRequestsSent"), _T("Total number of ECW block read requests sent to the server"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nECWCancelsSent, _T("TotalECWCancelsSent"), _T("Total number of ECW block read request cancels sent to the server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nECWBlocksReceived, _T("TotalECWBlocksReceived"), _T("Total number of ECW blocks received."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nECWCancelledBlocksReceived, _T("TotalECWCancelledBlocksReceived"), _T("Total number of ECW blocks cancelled that were still received."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nECWRequestsXmitPending, _T("TotalECWRequestsXmitPending"), _T("Total number of ECW block read requests waiting to be sent to server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nECWCancelsXmitPending, _T("TotalECWCancelsXmitPending"), _T("Total number of ECW block cancel requests waiting to be sent to server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)

		DEFINE_COUNTER(m_nJP2RequestsSent, _T("TotalJP2RequestsSent"), _T("Total number of JP2 block read requests sent to the server"), PERF_COUNTER_LARGE_RAWCOUNT, -8)
		DEFINE_COUNTER(m_nJP2CancelsSent, _T("TotalJP2CancelsSent"), _T("Total number of JP2 block read request cancels sent to the server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nJP2BlocksReceived, _T("TotalJP2BlocksReceived"), _T("Total number of JP2 blocks received."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nJP2CancelledBlocksReceived, _T("TotalJP2CancelledBlocksReceived"), _T("Total number of JP2 blocks cancelled that were still received."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nJP2RequestsXmitPending, _T("TotalJP2RequestsXmitPending"), _T("Total number of JP2 block read requests waiting to be sent to server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
		DEFINE_COUNTER(m_nJP2CancelsXmitPending, _T("TotalJP2CancelsXmitPending"), _T("Total number of JP2 block cancel requests waiting to be sent to server."), PERF_COUNTER_LARGE_RAWCOUNT, -1)
	END_COUNTER_MAP()
public:
	ULONGLONG	m_nRequestsSent;					// number of block read requests sent to the server
	ULONGLONG	m_nCancelsSent;					// number of block read request cancels sent to the server
	ULONGLONG	m_nBlocksReceived;				// number of blocks received
	ULONGLONG	m_nCancelledBlocksReceived;		// number of blocks cancelled that were still received
	ULONGLONG	m_nRequestsXmitPending;			// number of block read requests waiting to be sent to server
	ULONGLONG	m_nCancelsXmitPending;			// number of block cancel requests waiting to be sent to server

	ULONGLONG	m_nECWRequestsSent;					// number of block read requests sent to the server
	ULONGLONG	m_nECWCancelsSent;					// number of block read request cancels sent to the server
	ULONGLONG	m_nECWBlocksReceived;				// number of blocks received
	ULONGLONG	m_nECWCancelledBlocksReceived;		// number of blocks cancelled that were still received
	ULONGLONG	m_nECWRequestsXmitPending;			// number of block read requests waiting to be sent to server
	ULONGLONG	m_nECWCancelsXmitPending;			// number of block cancel requests waiting to be sent to server

	ULONGLONG	m_nJP2RequestsSent;					// number of block read requests sent to the server
	ULONGLONG	m_nJP2CancelsSent;					// number of block read request cancels sent to the server
	ULONGLONG	m_nJP2BlocksReceived;				// number of blocks received
	ULONGLONG	m_nJP2CancelledBlocksReceived;		// number of blocks cancelled that were still received
	ULONGLONG	m_nJP2RequestsXmitPending;			// number of block read requests waiting to be sent to server
	ULONGLONG	m_nJP2CancelsXmitPending;			// number of block cancel requests waiting to be sent to server
};

class CPerfMonApps :
	public CPerfObject
{
public:
	DECLARE_PERF_CATEGORY(CPerfMonApps, 5, _T("ECW/JP2 Applications"), _T("ECW/JP2 Application Statistics"), -1);
public:
	BEGIN_COUNTER_MAP(CPerfMonApps)
		DEFINE_COUNTER(m_nOpen, _T("TotalApplicationsOpen"), _T("Total number of ECW/JP2 Applications open"), PERF_COUNTER_RAWCOUNT, 0)
	END_COUNTER_MAP()
public:
	ULONG m_nOpen;			// number of file views currently open
};

class CPerfMonObjManager :
	public CPerfMon
{
public:
#define Perf_CPerfMonObjManager _T("Perf_CPerfMonObjManager")
	BEGIN_PERF_MAP(Perf_CPerfMonObjManager)
		CHAIN_PERF_CATEGORY(CPerfMonCache)
		CHAIN_PERF_CATEGORY(CPerfMonFiles)
		CHAIN_PERF_CATEGORY(CPerfMonFileViews)
		CHAIN_PERF_CATEGORY(CPerfMonECWP)
		CHAIN_PERF_CATEGORY(CPerfMonApps)
	END_PERF_MAP()
public:

	CPerfMonCache *m_pCache;
	CPerfMonFiles *m_pFiles;
	CPerfMonFileViews *m_pFileViews;
	CPerfMonECWP	*m_pECWP;
	CPerfMonApps *m_pApps;
public:


};
}
}

using namespace NCS;
using namespace SDK;
PERFREG_ENTRY(CPerfMonObjManager);
