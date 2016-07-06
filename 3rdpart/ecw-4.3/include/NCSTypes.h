/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSTypes.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	General NCS typedefs
** EDITS:
** [01] sjc 30Apr00 Merged Mac SDK port
** [02]  ny 03Nov00 Mac port changes
** [03] tfl 02Jul04 Moved NCSEcwCellType enum declaration from NCSEcwClient to here
 *******************************************************/

#include "NCSDefs.h"

#ifndef NCSTYPES_H
#define NCSTYPES_H

/*	Platform determination stuff.
	Should be moved to a better "central" location.
*/
#ifdef macintosh
//#define MACINTOSH
//#include "SimpleWinTypes.h"  // include mac types
#ifndef MACINTOSH
#	undef _WIN32
#	define MACINTOSH
#	define _MAC
#	undef _DEBUG_NEVER
#	define uint2	unsigned short
#	define uint	unsigned long		
#endif

#elif defined __PALMOS_TRAP__

#define PALM

#elif defined sun

#ifndef SOLARIS
#define SOLARIS
#endif // SOLARIS

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ERSTYPES_H

/*
** Includes
*/
#ifdef MACINTOSH
#include <Types.h>
#elif !defined(_WIN32_WCE)	//Pocket PC 2002
#ifdef _WIN32_WCE
	#include <nothing.h>
#endif


	#include <sys/types.h>
#endif

#if (defined(_WIN32) || defined(_WIN32_WCE))	//Pocket PC 2002

#ifdef NCS_BUILD_UNICODE
	#ifndef UNICODE
		#define UNICODE
		#define _UNICODE
	#endif
#endif 

#include <windows.h>
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>

#if (_WIN32_WCE > 300) //Pocket PC 2002
#include <stddef.h>
#endif

#elif defined(MACINTOSH)
#include <MacWindows.h>
#include <stdarg.h>
#include <stddef.h>
#include <MacTypes.h>
#include <Math64.h>
#include <time.h>
#elif defined POSIX
#include <stdarg.h>
#endif /* _WIN32 */

#ifdef HPUX
//#include <sys/types.h>
#endif

/*
 * #defines
 */
#ifdef _WIN32
#elif defined MACINTOSH

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#elif defined PALM
#endif
/*
** Typedefs
*/

/*
** General data sizes
*/
#if (defined(_WIN32) || defined(_WIN32_WCE))	//pocket pc 2002
typedef signed char		INT8;		/*  8 bit signed integer		*/
typedef unsigned char	UINT8;		/*  8 bit unsigned integer		*/
typedef signed short	INT16;		/* 16 bit signed integer		*/
typedef unsigned short	UINT16;		/* 16 bit unsigned integer		*/
#if !defined(_BASETSD_H_)||defined(SWIG)
typedef signed int		INT32;		/* 32 bit signed integer		*/
typedef unsigned int	UINT32;		/* 32 bit unsigned integer		*/
typedef __int64 		INT64;		/* 64 bit signed integer		*/
typedef unsigned __int64 UINT64;		/* 64 bit unsigned integer		*/
typedef UINT8			BOOLEAN;	/* for use as a True/False or On/Off type */
#endif

#else	/* _WIN32 */

#ifdef MACINTOSH

#if TARGET_API_MAC_CARBON
#define MAC_PREEMPTIVE
#endif // TARGET_API_MAC_CARBON

//	Define Data Types for the Macintosh
typedef signed char			INT8;		/*  8 bit signed integer		*/
typedef unsigned char		UINT8;		/*  8 bit unsigned integer		*/
typedef signed short		INT16;		/* 16 bit signed integer		*/
typedef unsigned short		UINT16;		/* 16 bit unsigned integer		*/
typedef signed long			INT32;		/* 32 bit signed integer		*/
typedef unsigned long		UINT32;		/* 32 bit unsigned integer		*/
typedef long long			INT64;		/* 64 bit signed integer		*/
typedef unsigned long long	UINT64;		/* 64 bit unsigned integer		*/
typedef int  				BOOLEAN;	/* boolean						*/

typedef void *HANDLE;
#define DWORD UINT32
#define BYTE	UINT8

#else	/* MACINTOSH */

#ifdef POSIX

typedef signed char	INT8;		/*  8 bit signed integer		*/
typedef unsigned char	UINT8;		/*  8 bit unsigned integer		*/
typedef signed short	INT16;		/* 16 bit signed integer		*/
typedef unsigned short	UINT16;		/* 16 bit unsigned integer		*/
#ifndef _BASETSD_H_
typedef signed int	INT32;		/* 32 bit signed integer		*/
typedef unsigned int	UINT32;		/* 32 bit unsigned integer		*/
#endif
#define BYTE	UINT8

#if defined(SOLARIS)

#define DWORD UINT32
typedef struct tagRECT {
	long left;
	long top;
	long right;
	long bottom;
} RECT, *PRECT;

typedef longlong_t 	INT64;		/* 64 bit signed integer		*/
typedef u_longlong_t 	UINT64;		/* 64 bit unsigned integer		*/

#elif defined(LINUX)

#define DWORD UINT32
typedef struct tagRECT {
	long left;
	long top;
	long right;
	long bottom;
} RECT, *PRECT;

typedef int64_t		INT64;
typedef u_int64_t	UINT64;

#elif defined(MACOSX)

typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef char BOOLEAN;
//typedef int boolean;
#define DWORD UINT32
#define BYTE	UINT8
#ifndef NULL
#	define NULL 0
#endif

#elif defined(HPUX)

#if !defined(_BASETSD_H_) 
//&& !defined(_DLFCN_H)
typedef int64_t		INT64;
//typedef uint64_t	UINT64;
#include <dlfcn.h>	// include this globally to get the correct definition of UINT64
#endif

#endif

#if !(defined(_BASETSD_H_)||defined(_UTILITY_H)) && !(defined(MACOSX))
typedef int BOOLEAN;		/* for use as a True/False or On/Off type */
#endif

#else	/* SOLARIS */
#ifdef IRIX

typedef signed char		INT8;		/*  8 bit signed integer		*/
typedef unsigned char	UINT8;		/*  8 bit unsigned integer		*/
typedef signed short	INT16;		/* 16 bit signed integer		*/
typedef unsigned short	UINT16;		/* 16 bit unsigned integer		*/
#ifndef _BASETSD_H_
typedef signed int		INT32;		/* 32 bit signed integer		*/
typedef unsigned int	UINT32;		/* 32 bit unsigned integer		*/
#endif
typedef __int64_t 		INT64;		/* 64 bit signed integer		*/
typedef __uint64_t 		UINT64;		/* 64 bit unsigned integer		*/
#ifndef _BASETSD_H_
typedef UINT8 BOOLEAN;				/* for use as a True/False or On/Off type */
#endif


#else	/* IRIX */

#error unknown machine type will require custom 64 bit variables

#endif	/* IRIX */
#endif	/* SOLARIS */
#endif	/* MACINTOSH */
#endif	/* _WIN32 */

typedef float			IEEE4;		/* 4 byte IEEE floating point	*/
typedef double			IEEE8;		/* 8 byte IEEE floating point	*/
typedef time_t			DATETIME;	/* Datestamp					*/

#endif /* #ifndef ERSTYPES_H */

/*
** NCS Specific typedefs
*/
typedef UINT32			NCSBlockId;		/* Unique (per file) Block ID		*/
typedef UINT64			NCSClientUID;	/* Unique client ID (per server)	*/
typedef UINT64			NCSSequenceNr;	/* Packet sequence number			*/
typedef INT64			NCSTimeStampMs;	/* msec timestamp - wraps every 2^64 ms (10^6 years) */
typedef INT64			NCSTimeStampUs;	/* usec timestamp - wraps every 2^64 us (10^3 years) */

/*
** NCS Color typedefs : Note that these are compatible with win32 api calls for COLORREF
*/
typedef UINT32			NCSColor;

#define NCSColorRGB(r,g,b)          ((NCSColor)(((UINT8)(r)|((UINT16)((UINT8)(g))<<8))|(((UINT32)(UINT8)(b))<<16)))
#define NCSColorRGBA(r,g,b,a)		((NCSColor)(((UINT8)(r)|((UINT16)((UINT8)(g))<<8))|(((UINT32)(UINT8)(b))<<16))|(((UINT32)(UINT8)(a))<<24))
#define NCSColorGetRValue(nColor)   ((UINT8)(nColor))
#define NCSColorGetGValue(nColor)   ((UINT8)(((UINT16)(nColor)) >> 8))
#define NCSColorGetBValue(nColor)   ((UINT8)((nColor)>>16))
#define NCSColorGetAValue(nColor)	((UINT8)((nColor)>>24))

/*
** Enum type for different kinds of cell sample [03]
*/
typedef enum {						
	NCSCT_UINT8				=	0,	
	NCSCT_UINT16			=	1,	
	NCSCT_UINT32			=	2,	
	NCSCT_UINT64			=	3,	
	NCSCT_INT8				=	4,	
	NCSCT_INT16				=	5,	
	NCSCT_INT32				=	6,	
	NCSCT_INT64				=	7,	
	NCSCT_IEEE4				=	8,	
	NCSCT_IEEE8				=	9,

	NCSCT_NUMVALUES			=   10
} NCSEcwCellType;					

/*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/

typedef enum {
	NCSCS_RAW	= 0,
	NCSCS_UTM	= 1,
	NCSCS_LL	= 2
} NCSCoordSys;

#ifdef __cplusplus

namespace NCS {

/**
 * NCSUUID - UUID class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2011/11/17 $ 
 */	
class NCSUTIL_IMPEX UUID {
public:
	UINT8 m_UUID[16];
	
	UUID() {
		memset(m_UUID, 0, sizeof(m_UUID));
	};
	UUID(const UINT8 Src[16]) {
		memcpy(m_UUID, Src, sizeof(m_UUID));
	};
	UUID(const UUID &Src) {
		memcpy(m_UUID, Src.m_UUID, sizeof(m_UUID));
	};
	virtual ~UUID() {};

	NCS_INLINE bool operator==(const UUID &s) {
		return(memcmp(m_UUID, s.m_UUID, sizeof(m_UUID)) == 0);
	}
	NCS_INLINE bool operator!=(const UUID &s) {
		return(memcmp(m_UUID, s.m_UUID, sizeof(m_UUID)) != 0);
	}
};

	/** packet ID type */
typedef NCSBlockId PacketId;	

#ifndef SWIG
class NCSUTIL_IMPEX CPacketId {
public:
	CPacketId() { m_id = (NCSBlockId)-1; };
	CPacketId(const CPacketId &s) { *this = s; };
	CPacketId(const NCSBlockId &s) { *this = s; };
	NCS_INLINE CPacketId &operator=(const CPacketId &s) { m_id = s.m_id; return(*this); };
	NCS_INLINE CPacketId &operator=(const NCSBlockId &s) { m_id = s; return(*this); };
	NCS_INLINE operator NCSBlockId() const { return(m_id); };
protected:
	NCSBlockId m_id;
};
#endif // !SWIG

}

}
#endif

#endif /* #ifndef NCSTYPES_H */
