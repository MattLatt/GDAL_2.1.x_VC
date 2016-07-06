/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Defs.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPC Defines
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCDEFS_H
#define NCSJPCDEFS_H

//
// Disable dll-interface warning, compiler gives it on protected/private members.
// Disable truncated name mangling warning
//

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif // NCSDEFS_H

//
// Use LCMS for ICC->RGB conversions, supports both
// restricted and full ICC profiles.
//
#define NCSJPC_USE_LCMS

//
// Use TinyXML for XML DOM Parsing
//
#define NCSJPC_USE_TINYXML

//
// Include ECW Compression in lib
//
#define ECW_COMPRESS

#define NCSJPC_2POW11 2048

#define NCSJP2_STRIP_HEIGHT	64

#define NCSECW_GLOBAL_LOCK_NAME "NCSEcw GlobalLock"

#endif // NCSJPCDEFS_H
