/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] 
** This document contains proprietary source code of
** ERDAS, Inc.
** FILE:   	NCSTimer.h
** CREATED:	Wed Apr 21 00:00:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Timer header.
** EDITS:
 *******************************************************/

#ifndef NCSTIMER_H
#define NCSTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCSUTIL_H
#include "NCSUtil.h"
#endif

typedef struct NCSTimer NCSTimer;

NCSTimer *NCSTimerCreate(NCSTimeStampMs tsPeriod, void	(*pFunc)(void*pData), void *pData);
void NCSTimerStart(NCSTimer *pTimer);
void NCSTimerStop(NCSTimer *pTimer);
void NCSTimerDestroy(NCSTimer *pTimer);
void *NCSTimerGetData(NCSTimer *pTimer);
void NCSTimerSetData(NCSTimer *pTimer, void *pData);

#ifdef __cplusplus
}
#endif

#endif //NCSTIMER_H

