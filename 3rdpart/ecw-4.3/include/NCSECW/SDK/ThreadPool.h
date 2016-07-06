/********************************************************** 
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
** 
** FILE:   	ThreadPool.h
** CREATED:	10 Jan 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	NCSJPC Thread pool class
** EDITS:
*******************************************************/
/** @file NCSEcw/SDK/ThreadPool.h */

#ifndef NCSSDKTHREADPOOL_H
#define NCSSDKTHREADPOOL_H

#include "NCSThreadPool.h"

namespace NCS {
namespace SDK {

/** @class CThreadPool
 *	@brief This class a thread pool class for NCSJPC derived from CThreadPool.
 */
	class NCSECW_IMPEX CThreadPool: public NCS::CThreadPool {
public:
	INT32	m_nRefs;
	CThreadPool();
	virtual ~CThreadPool();
};

}
}

#endif /* NCSSDKTHREADPOOL_H */
