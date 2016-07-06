/********************************************************** 
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
** 
** FILE:   	Stats.h
** CREATED:	15 Dec 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	Ecw stats class header for SDK instrumentation
** EDITS:
** [01] sjc	15Dec06 Created file
** 
** NOTES
**
********************************************************/

#ifndef NCSECWSTATS_H
#define NCSECWSTATS_H

#include "NCSDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ECW_STATS_LOCK_NAME "NCS StatsLock"

namespace NCS {
namespace SDK {

class NCS_EXPORT CStats {
public:
	class NCS_EXPORT CRaw32Value {
	public:
		CRaw32Value(class CRaw32Value *pParent = NULL) {
				m_pValue = &m_nValue;
				m_pParent = pParent;
			};
		virtual ~CRaw32Value() { 
				m_pValue = NULL;
				m_pParent = NULL;
			};
#if defined(_WIN32)&&!defined(NCS_MINDEP_BUILD)
		void Init(ULONG &p) {
			m_pValue = (INT32*)&p;
		}
#endif
		NCS_INLINE const INT32& operator++() const {
				CRaw32Value *p = (CRaw32Value*)this;
				while(p) {
					CStats::sIncrement(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(*m_pValue);
			};
		NCS_INLINE const INT32 operator++(int) const {
				INT32 v = *m_pValue;
				CRaw32Value *p = (CRaw32Value*)this;
				while(p) {
					CStats::sIncrement(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(v);
			};
		NCS_INLINE const INT32 operator+=(INT32 n) const {
				CRaw32Value *p = (CRaw32Value*)this;
				while(p) {
					CStats::sIncrement(p->m_pValue, n);
					p = p->m_pParent;
				}
				return(*m_pValue);
			};
		NCS_INLINE const INT32 operator--() const {
				CRaw32Value *p = (CRaw32Value*)this;
				while(p) {
					CStats::sDecrement(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(*m_pValue);
			};

		NCS_INLINE const INT32 operator--(int) const {
				INT32 v = *m_pValue;
				CRaw32Value *p = (CRaw32Value*)this;
				while(p) {
					CStats::sDecrement(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(v);
			};
		NCS_INLINE const INT32 operator-=(INT32 n) const {
				CRaw32Value *p = (CRaw32Value*)this;
				while(p) {
					CStats::sDecrement(p->m_pValue, n);
					p = p->m_pParent;
				}
				return(*m_pValue);
			};
		NCS_INLINE operator const INT32&() { return(*m_pValue); };
		NCS_INLINE operator const INT32*() { return(m_pValue); };
	private:
		INT32 m_nValue;
		INT32 *m_pValue;
		class CRaw32Value *m_pParent;
	};
	class NCS_EXPORT CRaw64Value {
	public:
		//friend const INT64 operator-=(class CRaw64Value o, INT64 n);
		CRaw64Value(class CRaw64Value *pParent = NULL) {
				m_pValue = &m_nValue;
				m_pParent = pParent;
			};
		virtual ~CRaw64Value() { 
				m_pValue = NULL; 
				m_pParent = NULL;
			};
#if defined(_WIN32)&&!defined(NCS_MINDEP_BUILD)
		void Init(ULONGLONG &p) {
			m_pValue = (INT64*)&p;
		}
#endif
		NCS_INLINE CRaw64Value& operator++() {
				CRaw64Value *p = (CRaw64Value*)this;
				while(p) {
					CStats::sIncrement64(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(*this);
			};
		NCS_INLINE INT64 operator++(int) {
				INT64 v = *this;
				CRaw64Value *p = (CRaw64Value*)this;
				while(p) {
					CStats::sIncrement64(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(v);
			};
		NCS_INLINE CRaw64Value& operator+=(INT64 n) {
				CRaw64Value *p = (CRaw64Value*)this;
				while(p) {
					CStats::sIncrement64(p->m_pValue, n);
					p = p->m_pParent;
				}
				return(*this);
			};
		NCS_INLINE CRaw64Value& operator--() {
				CRaw64Value *p = (CRaw64Value*)this;
				while(p) {
					CStats::sDecrement64(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(*this);
			};
		NCS_INLINE INT64 operator--(int) {
				INT64 v = *this;
				CRaw64Value *p = (CRaw64Value*)this;
				while(p) {
					CStats::sDecrement64(p->m_pValue, 1);
					p = p->m_pParent;
				}
				return(v);
			};
		NCS_INLINE CRaw64Value& operator-=(INT64 n) {
				CRaw64Value *p = (CRaw64Value*)this;
				while(p) {
					CStats::sDecrement64(p->m_pValue, n);
					p = p->m_pParent;
				}
				return(*this);
			};
		NCS_INLINE operator const INT64&() { return(*m_pValue); };
		NCS_INLINE operator const INT64*() { return(m_pValue); };

	private:
//		friend const INT64 operator+=(NCS::SDK::CStats::CRaw64Value o, INT64 n);
//		friend const INT64 operator-=(NCS::SDK::CStats::CRaw64Value o, INT64 n);
		INT64 m_nValue;
		INT64 *m_pValue;
		class CRaw64Value *m_pParent;
	};
	class NCS_EXPORT _Cache {
	public:
		CRaw64Value Bytes;
		CRaw64Value SetViewHits;
		CRaw64Value SetViewMisses;
		CRaw64Value ReadHits;
		CRaw64Value ReadUnpackedHits;
		CRaw64Value ReadMisses;
		CRaw64Value ReadBypass;

		CRaw64Value ECWBytes;
		CRaw64Value ECWPackedBytes;
		CRaw64Value ECWUnPackedBytes;
		CRaw64Value ECWCachedPrecincts;
		CRaw64Value ECWCachedBlocks;
		CRaw64Value SetViewECWHits;
		CRaw64Value SetViewECWMisses;
		CRaw64Value ReadECWHits;
		CRaw64Value ReadECWUnpackedHits;
		CRaw64Value ReadECWMisses;
		CRaw64Value ReadECWBypass;

		CRaw64Value JP2Bytes;
		CRaw64Value JP2PackedBytes;
		CRaw64Value JP2UnPackedBytes;
		CRaw64Value JP2CachedPrecincts;
		CRaw64Value SetViewJP2Hits;
		CRaw64Value SetViewJP2Misses;
		CRaw64Value ReadJP2Hits;
		CRaw64Value ReadJP2UnpackedHits;
		CRaw64Value ReadJP2Misses;
		CRaw64Value ReadJP2Bypass;

		_Cache();
		virtual ~_Cache();
		void Init();
	};
	_Cache Cache;
	class NCS_EXPORT _Files {
	public:
		CRaw32Value Open;
		CRaw32Value Cached;
		CRaw32Value CacheHits;
		CRaw32Value CacheMisses;

		CRaw32Value ECWOpen;
		CRaw32Value ECWCached;
		CRaw32Value ECWCacheHits;
		CRaw32Value ECWCacheMisses;

		CRaw32Value JP2Open;
		CRaw32Value JP2Cached;
		CRaw32Value JP2CacheHits;
		CRaw32Value JP2CacheMisses;

		_Files();
		virtual ~_Files();
		void Init();
	};
	_Files Files;

	class NCS_EXPORT _FileViews {
	public:
		CRaw32Value Open;
		CRaw32Value Cached;

		CRaw32Value ECWOpen;
		CRaw32Value ECWCached;

		CRaw32Value JP2Open;
		CRaw32Value JP2Cached;

		_FileViews();
		virtual ~_FileViews();
		void Init();
	};
	_FileViews FileViews;

	class NCS_EXPORT _ECWP {
	public:
		CRaw64Value RequestsSent;
		CRaw64Value CancelsSent;
		CRaw64Value BlocksReceived;
		CRaw64Value CancelledBlocksReceived;
		CRaw64Value RequestsXmitPending;
		CRaw64Value CancelsXmitPending;

		CRaw64Value ECWRequestsSent;
		CRaw64Value ECWCancelsSent;
		CRaw64Value ECWBlocksReceived;
		CRaw64Value ECWCancelledBlocksReceived;
		CRaw64Value ECWRequestsXmitPending;
		CRaw64Value ECWCancelsXmitPending;

		CRaw64Value JP2RequestsSent;
		CRaw64Value JP2CancelsSent;
		CRaw64Value JP2BlocksReceived;
		CRaw64Value JP2CancelledBlocksReceived;
		CRaw64Value JP2RequestsXmitPending;
		CRaw64Value JP2CancelsXmitPending;

		_ECWP();
		virtual ~_ECWP();
		void Init();
	};
	_ECWP ECWP;

	class NCS_EXPORT _Apps {
	public:
		CRaw32Value Open;

		_Apps();
		virtual ~_Apps();
		void Init();
	};
	_Apps Apps;

	CStats();
	virtual ~CStats();
	void Init();

public:
	
	static void sIncrement(INT32 *pVal, INT32 n);
	static void sDecrement(INT32 *pVal, INT32 n);
	static void sIncrement64(INT64 *pVal, INT64 n);
	static void sDecrement64(INT64 *pVal, INT64 n);
};
}
}

//NCS_INLINE const INT64 operator+=(NCS::SDK::CStats::CRaw64Value o, INT64 n) {
//	return(o._Increment(n));
//};

//NCS_INLINE const INT64 operator-=(NCS::SDK::CStats::CRaw64Value o, INT64 n) {
//	return(o._Decrement(n));
//};

#ifdef __cplusplus
};
#endif

#endif	// NCSECWSTATS_H
