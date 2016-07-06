/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSMisc.h
** CREATED:	Wed Oct 13 09:19:00 WST 1999
** AUTHOR: 	David Hayward
** PURPOSE:	Miscellaneous prototypes of useful functions
**			for the public SDKs.
**
**			NOTE: Must be kept in sync with the private
**				  includes.
**
** EDITS:
** [01] 	08-12-05 tfl lint fixes
 *******************************************************/

#ifndef NCSMISC_H
#define NCSMISC_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif

#ifndef SWIG

#ifdef _WIN32_WCE
#define NCSMin MIN
#define NCSMax MAX
#elif defined POSIX
#define NCSMin(a, b)   ((a) > (b) ? (b) : (a))
#define NCSMax(a, b)   ((a) < (b) ? (b) : (a))
#else
#define NCSMin min
#define NCSMax max
#endif // _WIN32_WCE


typedef enum {
	NCS_UNKNOWN	= 0,	/* Don't know what this platform is 	*/
	NCS_WINDOWS_9X	= 1,	/* Windows 9x (95, 98)			*/
	NCS_WINDOWS_NT	= 2,	/* Windows NT (NT4, 2000, 2003, 2008R1,R2, Vista, 7) */
	NCS_WINDOWS_CE	= 3,	/* Windows CE (CE, PocketPC)		*/
	NCS_MACINTOSH	= 4,	/* Macintosh (Sys 8/9)			*/
	NCS_MACINTOSH_OSX= 5,	/* Macintosh OSX			*/
	NCS_LINUX	= 6,	/* Linux				*/
	NCS_PALM	= 7,	/* PalmOS (v2+)				*/
	NCS_SOLARIS	= 8,	/* Solaris 2.5+				*/
	NCS_HPUX	= 9	/* HP-UX 11.0(64bit)			*/
} NCSPlatform;

void NCSFormatSizeText(INT64 nSizeBytes, char *buf);
NCSPlatform NCSGetPlatform(void);

#ifdef _WIN32

typedef enum {
	NCS_WINVER_00,
	NCS_WINVER_5X,
	NCS_WINVER_60,
	NCS_WINVER_61
} NCSWinVer;

NCSWinVer NCSGetWindowsVersion(void);

#endif


/*
**	[06] Fast Float to UINT8 conversion logic
*/

#if (defined(_WIN32) && (defined(_M_IX86)||defined(_M_X64)))

#define FLT_TO_INT_INIT() { unsigned int old_controlfp_val = _controlfp(_RC_NEAR, _MCW_RC)
#define FLT_TO_INT_INIT_FLR() { unsigned int old_controlfp_val = _controlfp(_RC_DOWN, _MCW_RC)
#define FLT_TO_INT_FINI() _controlfp(old_controlfp_val, _MCW_RC); }

#ifdef _M_IX86

#define FLT_TO_UINT8(a, b)								\
	{                                                   \
		INT32 FLT_TO_INT_rval;							\
		__asm                                           \
		{                                               \
			__asm fld dword ptr [b]                     \
			__asm fistp dword ptr [FLT_TO_INT_rval]     \
		}                                               \
		a = (UINT8)FLT_TO_INT_rval;							\
	}
#define FLT_TO_INT32(a, b)						\
		__asm                                   \
		{                                       \
			__asm fld dword ptr [b]             \
			__asm fistp dword ptr [a]           \
		}

#else

#define FLT_TO_UINT8(a, b)								\
	   { a = (UINT8) b; }
#define FLT_TO_INT32(a,b)								\
	   { a = (INT32) b; }

#endif

#else	/* _WIN32 && _M_IX86 */

#define FLT_TO_INT_INIT() { 
#define FLT_TO_INT_INIT_FLR() {
#define FLT_TO_INT_FINI()  }

#ifdef MACOSX	/**[16]**/

#define FLT_TO_INT32(a,b) a = rint(b)
//#define FLT_TO_UINT8(a, b) a = (UINT8) b
// rar (24-1-01): added from ECW mac port
#define FLT_TO_UINT8(a,b) 							\
	{ 												\
	UINT32	_x;										\
	FLT_TO_INT32(_x,b);								\
	a = (UINT8)_x;									\
	}
				   
#else	/* MACINTOSH */

#define FLT_TO_UINT8(a, b)								\
	   { a = (UINT8) b; }
#define FLT_TO_INT32(a,b)								\
	   { a = (INT32) b; }

#endif	/* MACINTOSH */
#endif	/* _WIN32 && _M_IX86 */

#define NCS_INLINE_FUNCS
#ifdef NCS_INLINE_FUNCS

#if (defined(_WIN32) && defined(_M_IX86))

static NCS_INLINE INT32 NCSfloatToInt32_RM(IEEE4 f) {
	INT32 i32;
	FLT_TO_INT32(i32, f);
	return(i32);
}

static NCS_INLINE INT32 NCSdoubleToInt32_RM(IEEE8 x) {
	return(NCSfloatToInt32_RM((IEEE4)x));
}

// Convert a float between 0.0 and 1.0 to an INT32
static NCS_INLINE INT32 NCSfloatToInt32_0_1(IEEE4 x)
{
    IEEE4 y = x + 1.f;
    return((*(INT32 *)&y) & 0x7FFFFF);	// last 23 bits
}

// Convert a float to an INT32
static NCS_INLINE INT32 NCSfloatToInt32(IEEE4 x)
{
	INT32 FltInt = *(INT32 *)&x;
	INT32 MyInt;
	INT32 mantissa = (FltInt & 0x07fffff) | 0x800000;
	INT32 exponent = 150 - ((FltInt >> 23) & 0xff);

	if (exponent < -(8*(int)sizeof(mantissa)-1)) {
		MyInt = (mantissa << (8*(int)sizeof(mantissa)-1));		      
	} else if(exponent < 0) {
		MyInt = (mantissa << -exponent);		      
	} else if(exponent > (8*(int)sizeof(mantissa)-1)) {
		MyInt = (mantissa >> (8*(int)sizeof(mantissa)-1));
	} else {
		MyInt = (mantissa >> exponent);
	}

	if (FltInt & 0x80000000)
		MyInt = -MyInt;
	return(MyInt);
}

// Convert a double to an INT32
static NCS_INLINE INT32 NCSdoubleToInt32(IEEE8 x)
{
	INT64 DblInt = *(INT64 *)&x;
	INT32 MyInt;
	INT64 mantissa = (DblInt & 0xfffffffffffff) | 0x10000000000000;
	INT32 exponent = (INT32)(1075 - ((DblInt >> 52) & 0x7ff));

	if (exponent < -(8*(int)sizeof(mantissa)-1)) {
		MyInt = (INT32)(mantissa << (8*(int)sizeof(mantissa)-1));		      
	} else if(exponent < 0) {
		MyInt = (INT32)(mantissa << -exponent);		      
	} else if(exponent > (8*(int)sizeof(mantissa)-1)) {
		MyInt = (INT32)(mantissa >> (8*(int)sizeof(mantissa)-1));
	} else {
		MyInt = (INT32)(mantissa >> exponent);
	}

	if (DblInt & 0x8000000000000000)
		MyInt = -MyInt;
	return(MyInt);
}

#else // _WIN32 & X86

#define NCSfloatToInt32_RM(f) ((INT32)(f))
#define NCSfloatToInt32_0_1(x) ((INT32)(x))
#define NCSfloatToInt32(x) ((INT32)(x))
#define NCSdoubleToInt32(x) ((INT32)(x))
#define NCSdoubleToInt32_RM(x) ((INT32)(x))

#endif // _WIN32 && X86

#define NCSfloatToInt64_RM(f) ((INT64)(f))
#define NCSfloatToInt64_0_1(x) ((INT64)(x))
#define NCSdoubleToInt64_RM(x) ((INT64)(x))
#define NCSfloatToInt64(x) ((INT64)(x))
#define NCSdoubleToInt64(x) ((INT64)(x))

// Get the 23bit mantissa from a float
static NCS_INLINE UINT32 NCSGetIEEE4Mantissa(IEEE4 x)
{
	INT32 FltInt = *(INT32 *)&x;
	return(FltInt & 0x07fffff);
}

// Get the 8bit exponent from a float
static NCS_INLINE UINT8 NCSGetIEEE4Exponent(IEEE4 x)
{
	INT32 FltInt = *(INT32 *)&x;
	return((UINT8)((FltInt >> 23) & 0xff));
}

// Get the 1bit sign from a float
static NCS_INLINE UINT8 NCSGetIEEE4Sign(IEEE4 x)
{
	INT32 FltInt = *(INT32 *)&x;
	return((UINT8)(FltInt >> 31));
}

// Create a Float from a 23bit mantissa, 8bit exponent and 1bit sign.
static NCS_INLINE IEEE4 NCSGetIEEE4(UINT32 Mantissa, UINT8 Exponent, UINT8 Sign)
{
	INT32 FltInt = Mantissa | (Exponent << 23) | (Sign << 31);
	return(*(IEEE4 *)&FltInt);
}

// Get the 52bit mantissa from a float
static NCS_INLINE UINT64 NCSGetIEEE8Mantissa(IEEE8 x)
{
	INT64 FltInt = *(INT64 *)&x;
	return(FltInt & 0xfffffffffffff);
}

// Get the 11bit exponent from a float
static NCS_INLINE UINT16 NCSGetIEEE8Exponent(IEEE8 x)
{
	INT64 FltInt = *(INT64 *)&x;
	return((UINT16)((FltInt >> 52) & 0x7ff));
}

// Get the 1bit sign from a float
static NCS_INLINE UINT8 NCSGetIEEE8Sign(IEEE8 x)
{
	INT64 FltInt = *(INT64 *)&x;
	return((UINT8)(FltInt >> 63));
}

// Create a Float from a 52bit mantissa, 11bit exponent and 1bit sign.
static NCS_INLINE IEEE8 NCSGetIEEE8(UINT64 Mantissa, UINT16 Exponent, UINT8 Sign)
{
	INT64 FltInt = Mantissa | (((INT64)Exponent) << 52) | (((INT64)Sign) << 63);
	return(*(IEEE8 *)&FltInt);
}

static NCS_INLINE INT32 NCSCeil(double a)
{
	if(a >= 0.0) {
		INT32 v = NCSdoubleToInt32(a);
		return(v + ((a != v) ? 1 : 0));
	} else {
		return(NCSdoubleToInt32(a));
	}
}

static NCS_INLINE INT64 NCSCeil64(double a)
{
	if(a >= 0.0) {
		INT64 v = NCSdoubleToInt64(a);
		return(v + ((a != v) ? 1 : 0));
	} else {
		return(NCSdoubleToInt64(a));
	}
}

static NCS_INLINE INT32 NCSFloor(double a)
{
	if(a >= 0.0) {
		return(NCSdoubleToInt32(a));
	} else {
		INT32 v = NCSdoubleToInt32(a);
		return(v - ((a != v) ? 1 : 0));		
	}
}

static NCS_INLINE INT64 NCSFloor64(double a)
{
	if(a >= 0.0) {
		return(NCSdoubleToInt64(a));
	} else {
		INT64 v = NCSdoubleToInt64(a);
		return(v - ((a != v) ? 1 : 0));		
	}
}

static NCS_INLINE INT32 NCSCeilDiv(INT32 n, INT32 d)
{
	if(d == 0) {
		return(0x7fffffff);
	} else if(n >= 0 && d > 0) {
		return((n / d + ((n % d) ? 1 : 0)));
	} else {
		return(n / d);
	}
}

static NCS_INLINE INT64 NCSCeilDiv64(INT64 n, INT64 d)
{
	if(d == 0) {
		return(0x7fffffffffffffff);
	} else if(n >= 0 && d > 0) {
		return((n / d + ((n % d) ? 1 : 0)));
	} else {
		return(n / d);
	}
}

static NCS_INLINE INT32 NCSFloorDiv(INT32 n, INT32 d)
{
	switch(d) {
		case 1: return(n); 
			//break;
		case 2: return(n >> 1); 
			//break;
		case 4: return(n >> 2); 
			//break;
		default:
				if(n < 0 || d < 0) {
					return((INT32)NCSFloor(n / (double)d));
				} else {
					return(n / d);
				}
			//break;
	}
}

static NCS_INLINE INT64 NCSFloorDiv64(INT64 n, INT64 d)
{
	switch(d) {
		case 1: return(n); break;
		case 2: return(n >> 1); break;
		case 4: return(n >> 2); break;
		default:
				if(n < 0 || d < 0) {
					return((INT64)NCSFloor64(n / (double)d));
				} else {
					return(n / d);
				}
			break;
	}
}

static NCS_INLINE UINT32 NCS2Pow(UINT32 n)
{
//	return(pow(2, n));
	return(1 << n);
}

static NCS_INLINE UINT64 NCS2Pow64(UINT64 n)
{
	return(((UINT64)1) << n);
}

static NCS_INLINE IEEE8 NCS2PowS(INT32 n)
{
//	return(pow(2, n));
	if(n >= 0) {
		return((IEEE8)(1 << n));
	} else {
		return(1.0 / (1 << -n));
	}
}

static NCS_INLINE INT32 NCSLog2(INT32 n)
{
    INT32 nLog;
    for(nLog = 0; n > 1; nLog++) {
        n >>= 1;
    }
    return nLog;
}

static NCS_INLINE INT64 NCSLog2_64(INT64 n)
{
    INT32 nLog;
    for(nLog = 0; n > 1; nLog++) {
        n >>= 1;
    }
    return nLog;
}

static NCS_INLINE UINT64 NCSAbs(INT64 a)
{
//	return(abs(a));
	return((a < 0) ? -a : a);
}

static NCS_INLINE BOOLEAN NCSIsPow2(UINT32 nValue) 
{
	if(NCS2Pow(NCSLog2(nValue)) == nValue) {
		return(TRUE);
	}
	return(FALSE);
}

#else

#ifdef __cplusplus
extern void *NCSNew(INT32 nSize, bool bClear = false);
extern void NCSDelete(void *p);
#endif // __cplusplus

extern INT32 NCSCeil(double a);
extern INT32 NCSFloor(double a);
extern INT32 NCSCeilDiv(INT32 n, INT32 d);
extern INT32 NCSFloorDiv(INT32 n, INT32 d);
extern UINT32 NCS2Pow(UINT32 n);
extern IEEE8 NCS2PowS(INT32 n);
extern UINT64 NCSAbs(INT64 a);

#endif /* NCS_INLINE_FUNCS */
// #endif // !MACOSX

#endif //SWIG

#ifdef __cplusplus
}
#endif
#endif /* NCSMISC_H */
