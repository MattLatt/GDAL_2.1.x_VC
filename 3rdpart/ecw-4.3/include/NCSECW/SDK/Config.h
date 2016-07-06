/********************************************************** 
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
** 
** FILE:   	Config.h
** CREATED:	15 Dec 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	Ecw stats class header for SDK instrumentation
** EDITS:
** [01] sjc	15Dec06 Created file
** 
** NOTES
**
********************************************************/

#ifndef NCSSDKCONFIG_H
#define NCSSDKCONFIG_H

#include "NCSDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace NCS {
namespace SDK {

class NCS_EXPORT CConfig {
public:
	bool	m_bForceFileReopen;				// normally FALSE - only set to TRUE for server load testing
	bool	m_bNoTextureDither;				// normally FALSE - TRUE for compression, so input compressed files have no texture dither added
	bool	m_bForceLowMemCompress;			// Force a low memory compressioin

	UINT32	m_nMaximumOpen;					// [15] Max # files open in cache
	
	NCSTimeStampMs	m_nPurgeDelay;			// [16] Time delay after last purge before new purge allowed
	NCSTimeStampMs  m_nFilePurgeDelay;		// [16] Time delay between last view closing and file being purged from cache
	NCSTimeStampMs  m_nMinFilePurgeDelay;		// [16] Min Time delay between last view closing and file being purged from cache

	UINT32			m_nMaxOffsetCache;		// [16] Max size of offset cache for ECW files with RAW block tables.

	bool			m_bEcwpReConnect;					// [19] normally FALSE - if TRUE the ecw library will try and reconnect if connection has been lost to IWS
	bool			m_bICCManage;					// [20] normally TRUE - if FALSE the ecw library does not do ICC management on READ
	UINT32			m_nMaxFileIOCache;				// [20] JP2 file IO cache size
	UINT32			m_nMaxProgressiveViewSize;		// [21] Maximum height and width of progressive mode views
	UINT32			m_nMaxFileReads;
	UINT32			m_nMaxJP2FileStreams;

	// Time wait for network server to respond with blocks
	NCSTimeStampMs	m_nBlockingTime;			// Time in ms to block clients that don't support callbacks
	NCSTimeStampMs	m_nRefreshTime;				// Time in ms to wait between refresh callbacks to smart clients

	UINT64			m_nMaximumCacheSize;				// Maximum allowed size of cache

	bool			m_bEnableAutoJ2IGeneration;
	
	bool			m_bPrintPurgeInfo;

	bool			m_bOptimizeForNonProgressive;		// Setting this will optimize the SDK for non progressive mode.  Progressive mode will no longer work.

	bool			m_bOptimizeCachePurgeForRandomMode;

	bool			m_bUseNearestNeighbour;

	bool			m_bUseBufferedIOStream;

	bool			m_bResilientDecoding;
#ifdef AUTO_SCALING
	bool			m_bAutoScaleDown;
#endif
	bool			m_bEnableECWPDiskCacheForSecure;

	bool			m_bEnableECWP3Client;
	UINT32			m_nMaxECWP3BlocksToRequest;
	UINT32			m_nMinECWP3BlocksToRequest;
	UINT32			m_nMaxECWP3ConcurrentDownloads;
	UINT32			m_nECWP3ConnectionTimeout;

	NCS::CString m_sCompressionTempDir;

	CConfig();
	virtual ~CConfig();

	CError SetConfig(NCSEcwConfigType eType, va_list va);
	CError GetConfig(NCSEcwConfigType eType, va_list va);
};
}
}

#ifdef __cplusplus
};
#endif

#endif	// NCSSDKCONFIG_H
