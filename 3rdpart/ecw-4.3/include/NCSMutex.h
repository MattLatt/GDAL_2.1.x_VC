/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSMutex.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Mutex handling.
** EDITS:
** [01] 03Nov00	 ny	Merged WinCE/PALM SDK changes
** [02] 12Apr01	 ny	Added copy constructor for CMutex
 *******************************************************/

#ifndef NCSMUTEX_H
#define NCSMUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SWIG

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif
#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif
#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif

#ifdef POSIX
#include <pthread.h>
#endif

#if defined( MACINTOSH ) && defined( MAC_PREEMPTIVE )
#	include <Multiprocessing.h>
#endif

/*
** Defines.
*/
/*
#ifdef _WIN32

#define NCS_NULL_MUTEX { 0, 0, 0, 0, 0, 0 }

#elif defined MACINTOSH

#define NCS_NULL_MUTEX { 0, 0, 0, 0, 0 }

#elif defined SOLARIS

#define NCS_NULL_MUTEX PTHREAD_MUTEX_INITIALIZER

#elif defined HPUX

#define NCS_NULL_MUTEX { PTHREAD_MUTEX_INITIALIZER }

#elif defined LINUX

#define NCS_NULL_MUTEX  PTHREAD_MUTEX_INITIALIZER 

#elif defined MACOSX
#define NCS_NULL_MUTEX  PTHREAD_MUTEX_INITIALIZER 

#else /* MACINTOSH */

//ERROR: Define NCS_NULL_MUTEX for this platform

//#endif /* _WIN32 */

typedef void *NCSMutex;

#define NCS_NULL_MUTEX (void*)NULL

/*
** Prototypes.
*/
void NCSMutexInit(NCSMutex *pMutex);
void NCSMutexFini(NCSMutex *pMutex);
void NCSMutexBegin(NCSMutex *pMutex);
BOOLEAN NCSMutexTryBegin(NCSMutex *pMutex);
void NCSMutexEnd(NCSMutex *pMutex);
#ifdef POSIX
pthread_mutex_t *NCSMutexGetNative(NCSMutex *pMutex);
#endif

/* Global (SYSTEM Wide) Locks */
#define NCS_GLOBAL_LOCK_NAME	"NCS GlobalLock"

#endif // SWIG

#ifdef __cplusplus
}

#ifndef NCS_NO_UTIL_LINK

namespace NCS {

// CMutex
class NCSUTIL_IMPEX CMutex {
public:
	CMutex();
	//CMutex(const CMutex &mMutex); /**[02]**/
	virtual ~CMutex();

	virtual void Lock(void);
	virtual bool TryLock(void);
	virtual void UnLock(void);
	virtual bool IsLocked(void);
#ifdef POSIX
	pthread_mutex_t *GetNative() { return(&m_m); };
#endif

private:
#ifdef _WIN32
	typedef struct {
		CRITICAL_SECTION cs;
		INT32			nLockCount;
		DWORD			dwOwnerTID;
	} TryCS;

	CRITICAL_SECTION m_cs;
	TryCS *m_pTryCS;
#elif defined MACINTOSH
	MPCriticalRegionID m_cs;
// FIXME:  No mutex yet!
#elif defined PALM
// FIXME:  No mutex yet!
#elif defined POSIX
	pthread_mutex_t m_m;
	bool		m_bIsLocked;
#else	/* PALM */
ERROR: Define CMutex for this platform
#endif 	/* _WIN32 */
};

#ifndef SWIG

// CMutexLock
class NCSUTIL_IMPEX CMutexLock {

private:
	CMutex	*m_pMutex;
public:
	CMutexLock(CMutex *pmMutex) {
		if( pmMutex ) {
			m_pMutex = pmMutex;
			m_pMutex->Lock();
		} else {
			m_pMutex = NULL;
		}
	}
#ifndef SWIG
	CMutexLock(CMutex &mMutex) {
		m_pMutex = &mMutex;
		m_pMutex->Lock();
	}
#endif
	virtual ~CMutexLock() {
		if( m_pMutex ) {
			m_pMutex->UnLock();
		}
	}
};

class NCSUTIL_IMPEX CGlobalLock {
public:
	CGlobalLock(char *sName = NCS_GLOBAL_LOCK_NAME, bool bProcessLocal = false);
	virtual ~CGlobalLock();
private:
#ifdef _WIN32
	HANDLE	m_hLock;
#else
	static sm_mMutex;
#endif

	bool Lock(char *sName);
	bool UnLock();
};
#endif // SWIG

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CMutex CNCSMutex;
typedef NCS::CMutexLock CNCSMutexLock;
typedef NCS::CGlobalLock CNCSGlobalLock;
#endif

#endif //NCS_NO_UTIL_LINK

#endif
#endif /* NCSMUTEX_H */
