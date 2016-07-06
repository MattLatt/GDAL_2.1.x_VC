/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSThreadPool.h
** CREATED:	2005
** AUTHOR: 	Russell Rogers
** PURPOSE:	NCS Thread Pool Class header.
** EDITS:
** [01] sjc 30Jan06 Moved to include, cleanup headers
**
 ********************************************************/

#if !defined(NCSTHREADPOOL_H)
#define NCSTHREADPOOL_H

#include "NCSUtil.h"
#include "NCSEvent.h"

#include <vector>
#include <list>

#pragma warning(push)
#pragma warning(disable:4251)

namespace NCS {

class NCSUTIL_IMPEX CThreadPool
{
public:
	class NCSUTIL_IMPEX CJob
	{
	public:
		CJob() { m_bIsRunning = false; m_bRun = true; m_bQueued = false; };
		virtual ~CJob() { m_bIsRunning = false; m_bRun = false; m_bQueued = false; }

		virtual void Work() = 0;
		virtual bool IsRunning();
		virtual bool Run() { return(m_bRun); };
		virtual void SetRunning(bool bRunning) { m_bIsRunning = bRunning; };
		virtual void SetRun(bool bRun) { m_bRun = bRun; };
		virtual bool IsQueued();
		virtual void SetQueued(bool bQueued) { m_bQueued = bQueued; };
		virtual bool IsQueuedOrRunning();

		bool IsWorker() { 
			return false; //(sm_tls_bIsWorker); 
		};
		void IsWorker(bool b) { 
			;//sm_tls_bIsWorker = b; 
		};

		void LockStatus();
		void UnLockStatus();
	protected:
		bool	m_bIsRunning;
		bool	m_bRun;
		bool	m_bQueued;
		CMutex m_mStatus;
		//static CThread::T_Local<bool> sm_tls_bIsWorker;
	};
	CThreadPool( UINT32 nMaxThreads = 10, bool bStartThreads = false, bool bPoolIsJobOwner = true,
		bool bCheckIfAlreadyQueued=false, bool bCheckIfRunning=false );
	
	virtual ~CThreadPool();

	void QueueJob( CJob *pJob, bool bHighPriority=false );
	bool CancelJob( CJob *pJob);
	CJob *GetNextJob();
	void Clear();
	INT32 GetQueueSize();
	INT32 GetThreadCount() { return(m_nThreadCount); };
	INT32 GetMaxThreadCount() { return((INT32)m_Threads.size()); };

	/*
	 * If you don't want to use the default Job List you can over-ride the follow
	 * three methods.
	 */
	virtual inline void WorkFinished( CJob *pJob );
protected:
	virtual inline void QueueJobInt( CJob *pJob, bool bHighPriority );
	virtual inline CJob *GetNextJobInt();

	void ResumeThread();
	void StartThread();

	class NCSUTIL_IMPEX CWorkThread : public CThread
	{
	public:
		CWorkThread( CThreadPool *pThreadPool );

		virtual ~CWorkThread();

		void Work( void *pData );
	protected:
		CThreadPool *m_pThreadPool;
	};
	
	// Job list
	typedef std::list<CJob *> CJobList;
	typedef CJobList::iterator CJobListIterator;

	CJobList m_JobList;
	CMutex m_mJobList;

	CJobList m_RunningJobList;
	CMutex m_mRunningJobList;

	// Worker threads
	CEvent m_ResumeEvent;

	typedef std::vector<CWorkThread *> CWorkThreadList;
	typedef CWorkThreadList::iterator CWorkThreadListIterator;

	CWorkThreadList m_Threads;
	UINT32 m_nThreadCount;
	UINT32 m_nIdleThreads;
	bool m_bPoolIsJobOwner;
	bool m_bCheckIfAlreadyQueued;
	bool m_bCheckIfRunning;
	static const NCSTimeStampMs s_tsMaxWaitTime;
};

}

#pragma warning(pop)

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CThreadPool CNCSThreadPool;
typedef NCS::CThreadPool::CJob CNCSThreadPoolJob;
#define CNCSThreadPoolJob CJob
#endif

#endif // !defined(NCSTHREADPOOL_H)
