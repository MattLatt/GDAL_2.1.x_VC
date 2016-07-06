/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSTimeStamp.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Memory handling routines.
** EDITS:
 *******************************************************/

#ifndef NCSTIMESTAMP_H
#define NCSTIMESTAMP_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif

extern NCSTimeStampMs NCSGetTimeStampMs(void);
extern NCSTimeStampUs NCSGetTimeStampUs(void);

#ifdef __cplusplus
}
#endif

#endif /* NCSTIMESTAMP_H */
