/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSThread.h
** CREATED:	Fri Feb 26 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Thread handling.
** EDITS:
** [01] 03Jan01	 ny	Added CThread wrapper class
** [02] 02May01	 ny	Added GetSysID() method
 *******************************************************/

#ifndef NCSTHREAD_H
#define NCSTHREAD_H

#ifndef NCSTTYPES_H
#include "NCSTypes.h"
#endif

#ifdef _WIN32
#ifndef _WIN32_WCE
#include <process.h>
#endif /* _WIN32_WCE */
#include <windows.h>
#ifdef _WIN32_WINNT
#include <winbase.h>
#endif

#elif defined MACINTOSH
#	ifdef MAC_PREEMPTIVE
#		include <Multiprocessing.h>
#	else
#		include <Threads.h>
#	endif //MAC_PREEMPTIVE
#elif defined POSIX
#include <pthread.h>
#endif

#ifndef SWIG

#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif
#ifndef NCSMALLOC_H
#include "NCSMalloc.h"
#endif
#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif
#include "NCSEvent.h"

#endif // !SWIG

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SWIG

typedef enum {
	NCS_THREAD_PRI_IDLE				= 0,
	NCS_THREAD_PRI_BELOW_NORMAL			= 1,
	NCS_THREAD_PRI_NORMAL				= 2,
	NCS_THREAD_PRI_ABOVE_NORMAL			= 3,
	NCS_THREAD_PRI_REALTIME				= 4
} NCSThreadPriority;

typedef void *NCSThread;
#define NCS_NULL_THREAD_ID (void*)NULL

#if defined(_WIN32)||defined(MACINTOSH)
typedef UINT32 NCSThreadLSKey;
#elif defined(POSIX)
typedef pthread_key_t *NCSThreadLSKey;
#endif

static BOOLEAN s_bThreadException;

static BOOLEAN s_bShowExceptionDialog;
void NCSShowExceptionDialog( BOOLEAN bShow );

BOOLEAN NCSThreadSpawn(NCSThread *pThread, void (*pFunc)(void*), void *pData, BOOLEAN bCreateSuspended);
//void NCSThreadFreeInfo(NCSThread *pThread);
void NCSThreadExit(INT32 dwExitId);
void NCSThreadSuspend(void);
void NCSThreadResume(NCSThread *pThread);
//NCSThread *NCSThreadGetCurrent(void);
BOOLEAN NCSThreadIsRunning(NCSThread *pThread);
BOOLEAN NCSThreadIsSuspended(NCSThread *pThread);
BOOLEAN NCSThreadRun(NCSThread *pThread);
BOOLEAN NCSThreadYield(void);
BOOLEAN NCSThreadSetPriority(NCSThread *pThread, NCSThreadPriority pri);
NCSThreadPriority NCSThreadGetPriority(NCSThread *pThread);
BOOLEAN NCSThreadTerminate(NCSThread *pThread);
#ifdef _WIN32
DWORD NCSThreadGetSysID(NCSThread* t);
#elif defined(POSIX)
pthread_t *NCSThreadGetSysID(NCSThread* t);
#else
ERROR: Need NCSThreadGetSysID() prototype in NCSThread.h
#endif //[07]

NCSThreadLSKey NCSThreadLSAlloc(void);
void NCSThreadLSFree(NCSThreadLSKey Key);
void NCSThreadLSSetValue(NCSThreadLSKey Key, void *pValue);
void *NCSThreadLSGetValue(NCSThreadLSKey Key);

#endif //SWIG

#ifdef __cplusplus
}

#ifndef NCS_NO_UTIL_LINK

#include <list>
#include <algorithm>

namespace NCS {

// CThread
class NCSUTIL_IMPEX CThread: public CMutex {
public:
#ifndef SWIG
	template<class T> class T_Local: public CMutex {
	public:
		T_Local(T defVal) { m_Key = NCSThreadLSAlloc(); };
		virtual ~T_Local() { 
					Lock();
					while(m_FreeList.begin() != m_FreeList.end()) {
						std::list<T*>::iterator it = m_FreeList.begin();
						delete *it;
						m_FreeList.erase(it);
					}
					NCSThreadLSFree(m_Key); 
					UnLock();
				};
		NCS_INLINE operator T&() const { T *p = (T*)::NCSThreadLSGetValue(m_Key); return(p ? *p : (T&)m_default); }
		NCS_INLINE T_Local &operator=(const T &v) { 
					T *p = new T(v);
					Lock();
					m_FreeList.push_back(p);
					UnLock();
					SetValue(p); 
					return(*this); 
				};
		NCS_INLINE T_Local &operator=(const T *p) { SetValue(p); return(*this); };
	protected:
		NCSThreadLSKey m_Key;
		T			   m_default;
		std::list<T*> m_FreeList;

		void SetValue(T *p) {
			T *old = (T*)::NCSThreadLSGetValue(m_Key);
				if(old) {
					Lock();
					std::list<T*>::iterator it = find(m_FreeList.begin(), m_FreeList.end(), old);
					if(it != m_FreeList.end()) {
						delete *it;
						m_FreeList.erase(it);
					}
					UnLock();
				}
				::NCSThreadLSSetValue(m_Key, p);
			};
	};
#endif //SWIG
	typedef enum {
		PRI_IDLE = 0,
		PRI_BELOW_NORMAL = 1,
		PRI_NORMAL = 2,
		PRI_ABOVE_NORMAL = 3,
		PRI_REALTIME = 4
	} Priority;

	void *m_pData;

	CThread();
	virtual ~CThread();
	virtual bool Spawn(void *pData=NULL, bool bCreateSuspended=false);
	
	virtual void Exit(INT32 dwExitId);
	virtual void Suspend(void);
	virtual void Resume();
	//virtual NCSThread *GetCurrent(void);
	virtual bool IsRunning();
	virtual bool IsSuspended();
	virtual bool ThreadYield(void);
	virtual bool SetPriority(Priority pri);
	virtual Priority GetPriority();
	virtual bool Terminate();
	virtual bool Run();
	virtual bool Wait( NCSTimeStampMs tsWait );
	virtual bool Stop(bool bWait = true);
	virtual bool AreThread();
	virtual void Work(void *pData) = 0;

//	virtual NCSThread GetID(void);

#ifdef _WIN32
	virtual DWORD GetSysID(void);
#elif defined MACINTOSH
	virtual MacThreadID GetSysID(void);
#elif defined POSIX
	virtual pthread_t *GetSysID(void);
#endif

protected:
	friend void ::NCSThreadExit(INT32 dwExitId);

#ifdef _WIN32
	DWORD			m_dwTID;
	HANDLE			m_hThread;

#elif defined(POSIX)
	
	pthread_t		m_thread;
	pthread_cond_t	m_suspend_condition;
	bool			m_bSuspended;
	bool			m_bThreadStarted;
    CMutex			m_mSuspendMutex;

#else 
ERROR: define NCS::CThread for this platform
#endif

	bool		m_bRunning;
	bool		m_bRun;
	CEvent	m_evtShutDown;

	static void sThreadFunc(void *pDataParam);
	void FreeInfo();
};

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CThread CNCSThread;
#endif

#endif //NCS_NO_UTIL_LINK


#endif // __cplusplus

#endif /* NCSTHREAD_H */
