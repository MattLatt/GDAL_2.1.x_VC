/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:   	NCSDefs.h
** CREATED:	Tue Mar 2 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	General NCS defines
** EDITS:
** [01] sjc 30Apr00 Merged Mac SDK port
** [02]  ny 03Nov00 Merge WinCE/PALM SDK changes
** [03] tfl 14Jul04 Added radians/degrees conversion factors
 *******************************************************/

#ifndef NCSDEFS_H
#define NCSDEFS_H

#define NCS_BUILD_UNICODE

#ifndef _WIN32
#include <wchar.h>
#include <wctype.h>
#else
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#include <stdlib.h>
#include <string.h>

#ifdef LINUX
#ifndef NULL
#define NULL 0
#endif
#endif

#ifdef HPUX
// This should be defined as "inline", but the compiler doenst 
// like our "static inline type function()" prototypes, probably 
// have to change them all to static type inline function()"
// Use the next line when this is sorted.
//#define __inline inline
#define __inline
#define NCS_64BIT
#endif

// When using the sun CC compiler you need to use %ls instead of %s for wide char strings
// also extern C functions must use 'extern "C"' when declared and implemented
#ifdef __SUNPRO_CC
	#define NCS_EXTERN_C_IMP extern "C"
    #ifdef NCS_BUILD_UNICODE
        #define NCS_TS NCS_T("%ls")
    #else
        #define NCS_TS NCS_T("%s")
    #endif
#else
	#define NCS_EXTERN_C_IMP
    #define NCS_TS NCS_T("%s")
#endif
    
#if (defined(MACINTOSH)||defined(SOLARIS)||defined(IRIX)||defined(PALM)||defined(HPUX)||defined(MACOSX))&&(!defined(X86))
#define NCSBO_MSBFIRST
#else	// _WIN32, LINUX (i386)
#define NCSBO_LSBFIRST
#endif

#if !defined(_WIN32_WCE)&&!defined(UNALIGNED)
#define UNALIGNED
#endif	/* !_WIN32_WCE */

#ifdef PALM
// FIXME
#define NCS_PALM_CREATOR_ID 'NCS1'
#endif

#ifdef HPUX
// The system headers clash, so use their versions of MAX/MIN instead.
#include <sys/param.h>

#else

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

#endif

#ifdef WIN32
    #define GCVT _gcvt
#else
    #define GCVT gcvt
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef _WIN32

#ifdef _WIN64
#define NCS_64BIT
#endif
#ifndef MAXDOUBLE
#define MAXDOUBLE   DBL_MAX			//1.7976931348623158e+308
#define NCS_FQNAN	_FPCLASS_QNAN	//0x0002
#define NCS_NAN		_FPCLASS_SNAN
#endif	/* !MAXDOUBLE */

#if defined(_WIN32_WCE)
#define NCS_NO_UNALIGNED_ACCESS
#ifndef MB_TASKMODAL
#define MB_TASKMODAL MB_APPLMODAL          
#endif
#endif // _WIN32_WCE

#elif defined PALM

#define NCS_FQNAN	0x0002

#elif defined MACINTOSH

#define NCS_FQNAN	0x0002
#define NCS_NAN		NAN
#ifndef MAXDOUBLE
#define MAXDOUBLE   DBL_MAX
#endif

#elif defined MACOSX
//#include <values.h>
#include <limits.h>
#include <ctype.h>

#define NCS_FQNAN	0x0002
#define NCS_NAN		NAN
#ifndef MAXDOUBLE
#define MAXDOUBLE   DBL_MAX
#endif

//FIXME: These should probably be moved somewhere else
#define HDC			CGrafPtr
#define LPRECT		Rect *
#define RECT		Rect
#define HBITMAP		PixMapHandle
#define COLORREF	DWORD
#define HRGN		RgnHandle
#define UINT		UINT32
#define HWND		DWORD
#define STDMETHOD(x)	virtual INT32 x
#define LRESULT	INT32
#define HRESULT	long
#define WPARAM	UINT32
#define LPARAM 	INT32
#define VARIANT	void *
#define HPEN		DWORD
#define HBRUSH	DWORD
#define BSTR		short *
#define BOOL		BOOLEAN
#define LPSTR	char *
#define HCURSOR	CursHandle
#define WORD		UINT16
#define STDMETHODIMP	INT32
//#define boolean	BOOLEAN
#define LOWORD(x)		((UINT16)((long)(x) & 0xffff))
#define HIWORD(x)		((UINT16)((long)(x) >> 16))
#define S_OK     ((LRESULT)0x00000000L)
#define S_FALSE  ((LRESULT)0x00000001L)
#define SUCCEEDED(Status) 	((Status) >= 0)
#define FAILED(Status) 		((Status)<0)
#define MB_OK
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))
#define USES_CONVERSION
#define MK_LBUTTON	0x0001
#define MK_RBUTTON	0x0002
#define MK_SHIFT		0x0004
#define MK_CONTROL	0x0008
#define MK_MBUTTON	0x0010
#define VK_SHIFT		0x10
#define SetCursor(X) 0; if( X ) SetCursor(*X)
#define SetCapture(X)
#define ReleaseCapture(X)
#define IntersectRect(A, B, C) SectRect(B, C, A)
#define 	ZeroMemory(DATA, SIZE) memset(DATA, 0, SIZE)

#elif defined SOLARIS || defined LINUX || defined HPUX

#ifdef LINUX
#define wcsicmp wcscasecmp
#endif

#include <values.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>

#ifndef MAXDOUBLE
#define MAXDOUBLE   DBL_MAX			//1.7976931348623158e+308
#endif	/* !MAXDOUBLE */

#ifdef LINUX
#define NCS_FQNAN	NAN	//0x0002
#define NCS_NAN		NAN
#elif defined(SOLARIS)
#include <ieeefp.h>

#define NCS_FQNAN	FP_QNAN
#define NCS_NAN	FP_QNAN
#else
#error NAN not defined
#endif

// Solaris can't access types on unaligned addressed
#define NCS_NO_UNALIGNED_ACCESS

#if defined(SOLARIS) || defined(HPUX)
// SPARC has slow BYTE bit ops
#define NCS_SLOW_CPU_BYTE_OPS
#endif

#else

#error DEFINE SYSTEM INCLUDES FOR TYPES

#endif	/* _WIN32 */



/*Coodinate system defines*/
#define NCS_LINEAR_COORD_SYS	"linear"
#define	NCS_FEET_FACTOR			0.30480061
/*[03]*/
#define NCS_RADIANS_TO_DEGREES	57.29577951L
#define NCS_DEGREES_TO_RADIANS	1.745329252E-2L	

#if defined(_WIN32)

#ifdef NCS_MANAGED_BUILD

#define NCS_EXPORT
#define NCS_IMPORT
#define NCS_GC __gc
#define NCS_VALUE __value
#define NCS_PUBLIC public
#define NCS_PROTECTED NCS_PUBLIC
#define NCS_PRIVATE NCS_PUBLIC
#ifdef __cplusplus
#using <mscorlib.dll>
#endif //__cplusplus

#else // _MANAGED

#if defined(_LIB)||defined(NCSECW_STATIC_LIBS)||defined(SWIG)
#define NCS_EXPORT
#define NCS_IMPORT
#else
#define NCS_EXPORT __declspec(dllexport)
#define NCS_IMPORT __declspec(dllimport)
#endif

#ifndef NCSUTIL_IMPEX
#ifdef NCSUTIL_EXPORTS
  #define NCSUTIL_IMPEX    NCS_EXPORT
#else
  #define NCSUTIL_IMPEX    NCS_IMPORT
#endif
#endif
#ifndef NCSECW_IMPEX
#ifdef NCSECW_EXPORTS
  #define NCSECW_IMPEX    NCS_EXPORT
#else
  #define NCSECW_IMPEX    NCS_IMPORT
#endif
#endif

#define NCS_GC 
#define NCS_VALUE 
#define NCS_PUBLIC public
#define NCS_PROTECTED protected
#define NCS_PRIVATE private

#endif // _MANAGED

#ifdef _WIN32_WCE
#define NCS_CALL
#else
#define NCS_CALL __cdecl
#endif

#define NCS_CB_CALL __cdecl

#else // _WIN32

#define NCS_EXPORT
#define NCS_IMPORT
#define NCS_GC
#define NCS_VALUE
#define NCS_PUBLIC public
#define NCS_PROTECTED protected
#define NCS_PRIVATE private

#define NCS_CALL
#define NCS_CB_CALL
#endif // _WIN32

#ifndef MAX_PATH
#ifdef MACINTOSH

	//	Note: Verify that this is OK for all MAC/OS Platform
#define MAX_PATH	256

#elif defined PALM

#define MAX_PATH 	1024

#elif defined SOLARIS || defined LINUX || defined HPUX

#ifdef PATH_MAX
#define MAX_PATH	PATH_MAX
#else
#define MAX_PATH	1024
#endif

#elif defined MACOSX

#define MAX_PATH 1024

#else	/* PALM */

#define MAX_PATH	PATHNAMELEN

#endif
#endif	/* !MAX_PATH */

#define NCSIsNullString(s) ((s) == (char *)0 || (*(s)) == '\0')
#define NCSIsNullStringW(s) ((s) == (wchar_t *)0 || (*(s)) == '\0')

#if	__ICL >= 700
#ifdef NCS_VECTOR_CC
//Note: need /QaxMiKW /Qvec_report3 /Qrestrict ICC flags to use vectorisation
#define NCS_RESTRICT restrict
#else
#define NCS_RESTRICT
#endif
#else
#define NCS_RESTRICT
#endif

#ifdef _OPENMP
#define NCS_OPENMP
#endif // _OPENMP

#ifdef NCS_OPENMP
#define NCS_VECTOR_CC
#endif // NCS_OPENMP

#ifndef NCS_INLINE
#ifdef SWIG
#define NCS_INLINE 
#elif _WIN32
#define NCS_INLINE __forceinline
#elif defined __GNUC__
#define NCS_INLINE __inline__
#elif __SUNPRO_CC
#define NCS_INLINE inline
#else
#define NCS_INLINE __inline
#endif // _WIN32
#endif // NCS_INLINE

#include "NCSTypes.h"

/* 
 * Unicode/ASCII agnostic macros
 * These reimplement the Win32 "tchar" concept in a platform independent way.  
 * Just enough for what we need, extend as necessary.  Use in lieu of <tchar.h>
 * in any code that requires Unicode support.
 */

#ifndef NCS_TCHAR
#define NCS_TCHAR

#ifdef NCS_BUILD_UNICODE

#ifndef SWIG

#define NCSTChar wchar_t
#define NCS_T(x) L ## x
#define NCSTCmp(x,y) _wcsicmp(x,y)
#define NCSTCat(x,y) wcscat(x,y)
#define NCSTLen(x) wcslen(x)
#define NCSTCpy(x,y) wcscpy(x,y)
#define NCSTNCpy(x,y,z) wcsncpy(x,y,z)

#ifdef _WIN32
#define NCSTLwr _wcslwr
#define NCSTStr wcsstr
#ifdef _WIN32_WCE
#define NCSWinGPA_T NCS_T
#else
#define NCSWinGPA_T(x) x
#endif
#endif

/*
// original
static NCS_INLINE wchar_t *NCSA2WHelper(wchar_t *lpw, const char *lpa, int nChars)
{
	lpw[0] = '\0';
#ifdef _WIN32
	MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpa, -1, (LPWSTR)lpw, nChars);
#else
	mbstowcs(lpw, lpa, nChars);
#endif
	return lpw;
}

static NCS_INLINE char *NCSW2AHelper(char *lpa, const wchar_t *lpw, int nChars)
{
	lpa[0] = '\0';
#ifdef _WIN32
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)lpw, -1, (LPSTR)lpa, nChars, NULL, NULL);
#else
	wcstombs(lpa, lpw, nChars);
#endif
	return lpa;
}


// Shao noted: bug IWS-3332
// _alloca is deprecated, and will cause crash when the size to be allocated is greather than _ALLOCA_S_THRESHOLD
// Using _malloca is safer, but have to free memory. 
// NCSChar2WChar or NCSWChar2Char at NCSTConvert_NoBMG.h may solve the problem
#define OS_STRING(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*) alloca(((int)strlen(lpa)+1)*2*sizeof(wchar_t)), (lpa), ((int)strlen(lpa)+1)))
#define CHAR_STRING(lpw)	(((lpw) == NULL) ? NULL : NCSW2AHelper((char*) alloca(((int)wcslen(lpw)+1)*2), (lpw), ((int)wcslen(lpw)+1)*2))
*/

//
// Shao noted: bug IWS-3332
// _alloca is deprecated, and will cause crash when the size to be allocated is greather than _ALLOCA_S_THRESHOLD
// Using _malloca is safer, but have to free memory. 
// OS_STRING_STATIC and CHAR_STRING_STATIC solve the problem for short string
// NCSChar2WChar or NCSWChar2Char at NCSTConvert_NoBMG.h may also solve the problem for long string

#include <malloc.h>
#include <stdio.h>

static NCS_INLINE wchar_t *NCSA2WHelper(wchar_t *lpw, const char *lpa, int nChars)
{
	if (!lpw) {

		return lpw; // NULL. the caller may use NCSChar2WChar instead

//#ifdef _WIN32
//		__try {
//			lpw = (wchar_t*) _alloca(((int)strlen(lpa)+1)*2*sizeof(wchar_t));
//		}
//		__except( GetExceptionCode() == STATUS_STACK_OVERFLOW )
//		{
//			//printf_s("_alloca failed!\n");
//			return lpw; // NULL. the caller may use NCSChar2WChar instead
//		};
//#else
//		lpw = (wchar_t*) _alloca(((int)strlen(lpa)+1)*2*sizeof(wchar_t));
//#endif
	}

	lpw[0] = '\0';
#ifdef _WIN32
	MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpa, -1, (LPWSTR)lpw, nChars);
#else
	mbstowcs(lpw, lpa, nChars);
#endif
	return lpw;
}

static NCS_INLINE char *NCSW2AHelper(char *lpa, const wchar_t *lpw, int nChars)
{
	if (!lpa) {

	return lpa; // NULL. the caller may use NCSWChar2Char instead

//#ifdef _WIN32
//		__try {
//			lpa = (char*) _alloca(((int)wcslen(lpw)+1)*2);
//		}
//		__except( GetExceptionCode() == STATUS_STACK_OVERFLOW )
//		{
//			//printf_s("_alloca failed!\n");
//			return lpa; // NULL. the caller may use NCSWChar2Char instead
//		};
//#else
//		lpa = (char*) _alloca(((int)wcslen(lpw)+1)*2);
//#endif
	}

	lpa[0] = '\0';
#ifdef _WIN32
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)lpw, -1, (LPSTR)lpa, nChars, NULL, NULL);
#else
	wcstombs(lpa, lpw, nChars);
#endif
	return lpa;
}
//

// safer than original, but may need a little bit time on exception handling
//#define OS_STRING_STATIC(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper(NULL, lpa, ((int)strlen(lpa)+1))) // not work as memory allocated by alloca() is freed locally
//#define CHAR_STRING_STATIC(lpw)		(((lpw) == NULL) ? NULL : NCSW2AHelper(NULL, lpw, ((int)wcslen(lpw)+1)*2)) // not work as memory allocated by alloca() is freed locally
#define MAX_ALLOCA_LENGTH 5000
static wchar_t globalWCharMemAllocated[MAX_ALLOCA_LENGTH];
static char globalCharMemAllocated[MAX_ALLOCA_LENGTH];

/*
#ifdef _WIN32
#include <Winnt.h>
// stack availability
static size_t GetStackAvailabilty()
{
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(&mbi, &mbi, sizeof(mbi));
    // now mbi.AllocationBase = reserved stack memory base address

    VirtualQuery(mbi.AllocationBase, &mbi, sizeof(mbi));
	size_t stackSize_reserved = mbi.RegionSize;
    // now (mbi.BaseAddress, mbi.RegionSize) describe reserved (uncommitted) portion of the stack
    // skip it

    VirtualQuery((char*)mbi.BaseAddress + mbi.RegionSize, &mbi, sizeof(mbi));
    // now (mbi.BaseAddress, mbi.RegionSize) describe the guard page
    // skip it

    VirtualQuery((char*)mbi.BaseAddress + mbi.RegionSize, &mbi, sizeof(mbi));
    // now (mbi.BaseAddress, mbi.RegionSize) describe the committed (i.e. accessed) portion of the stack

	if (stackSize_reserved < mbi.RegionSize)
		return 0;

    return stackSize_reserved - mbi.RegionSize;
}

#define ALLOCA(size) ( ( GetStackAvailabilty() > (size_t)size ) ? alloca(size) : NULL ) // work, but slowest

#define ALLOCA_WCHAR(size) ( \
	( MAX_ALLOCA_LENGTH > size ) ? globalWCharMemAllocated: ALLOCA(size) \
	) // work, fastest
#define ALLOCA_CHAR(size) ( \
	( MAX_ALLOCA_LENGTH > size ) ? globalCharMemAllocated: ALLOCA(size) \
	) // work, fastest
#else
*/

#define ALLOCA_WCHAR(size) ( \
	( MAX_ALLOCA_LENGTH > size ) ? globalWCharMemAllocated: NULL \
	) // work, fastest
#define ALLOCA_CHAR(size) ( \
	( MAX_ALLOCA_LENGTH > size ) ? globalCharMemAllocated: NULL \
	) // work, fastest
//#endif

// if getting NULL, using NCSChar2WChar at NCSTConvert_NoBMG.h may solve the problem for long string
// e.g. (NCSChar2WChar(lpa)).wc_str(); note: make a copy like NCSStrDupT() to hold the string
#define OS_STRING_STATIC(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*)ALLOCA_WCHAR(((int)strlen(lpa)+1)*2*sizeof(wchar_t)), (lpa), ((int)strlen(lpa)+1))) // work for defined ALLOCA_WCHAR, fastest

// if getting NULL, using NCSWChar2Char at NCSTConvert_NoBMG.h may solve the problem for long string
// e.g. (NCSWChar2Char(lpw)).c_str(); note: make a copy like NCSStrDupT() to hold the string
#define CHAR_STRING_STATIC(lpw)		(((lpw) == NULL) ? NULL : NCSW2AHelper((char*)ALLOCA_CHAR(((int)wcslen(lpw)+1)*2), (lpw), ((int)wcslen(lpw)+1)*2)) // work for defined ALLOCA_CHAR, fastest

//#define OS_STRING_STATIC(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*)NULL, lpa, ((int)strlen(lpa)+1))) // not work as auto freed locally
//#define CHAR_STRING_STATIC(lpw)		(((lpw) == NULL) ? NULL : NCSW2AHelper((char*)NULL, lpw, ((int)wcslen(lpw)+1)*2)) // not work as auto freed locally
//#define OS_STRING_STATIC(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*)alloca(((int)strlen(lpa)+1)*2*sizeof(wchar_t)), (lpa), ((int)strlen(lpa)+1))) // work, but may crash
//#define CHAR_STRING_STATIC(lpw)		(((lpw) == NULL) ? NULL : NCSW2AHelper((char*)alloca(((int)wcslen(lpw)+1)*2), (lpw), ((int)wcslen(lpw)+1)*2)) // work, but may crash
//#define OS_STRING_STATIC(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*)ALLOCA(((int)strlen(lpa)+1)*2*sizeof(wchar_t)), (lpa), ((int)strlen(lpa)+1))) // work for defined ALLOCA, but slowest
//#define CHAR_STRING_STATIC(lpw)		(((lpw) == NULL) ? NULL : NCSW2AHelper((char*)ALLOCA(((int)wcslen(lpw)+1)*2), (lpw), ((int)wcslen(lpw)+1)*2)) // work for defined ALLOCA, but slowest
//#define OS_STRING_STATIC(lpa)		( ((lpa) == NULL) ? NULL : ((NCSChar2WChar(lpa)).wc_str()) ) // not work as auto freed locally
//#define CHAR_STRING_STATIC(lpw)		( ((lpw) == NULL) ? NULL : ((NCSWChar2Char(lpw)).c_str()) ) // not work as auto freed locally

// original
#define OS_STRING(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*) alloca(((int)strlen(lpa)+1)*2*sizeof(wchar_t)), (lpa), ((int)strlen(lpa)+1)))
#define CHAR_STRING(lpw)	(((lpw) == NULL) ? NULL : NCSW2AHelper((char*) alloca(((int)wcslen(lpw)+1)*2), (lpw), ((int)wcslen(lpw)+1)*2))

#endif //SWIG

#if defined POSIX
static NCS_INLINE int wcsnicmp(const wchar_t *s1, const wchar_t *s2, int nChars)
{
	int i=0;
	while ((*s2 != L'\0') && (towlower(*s1) == towlower(*s2)) && (i<nChars)) {
		s1++;
		s2++;
		i++;
	}
	return(towlower(*s1) - towlower(*s2));
}
#endif

#if defined SOLARIS || defined MACOSX

static NCS_INLINE int wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
	while ((*s2 != L'\0') && (towlower(*s1) == towlower(*s2))) {
		s1++;
		s2++;
	}
	return(towlower(*s1) - towlower(*s2));
}

static NCS_INLINE wchar_t *wcsdup(const wchar_t *s1)
{
	size_t len = (wcslen(s1) + 1) * sizeof(wchar_t);
	wchar_t *s2 = (wchar_t*)malloc(len);
	memcpy(s2, s1, len);
	return(s2);
}

#endif /* SOLARIS || MACOSX */

#define NCSTDup(x) _wcsdup(x)

#else	/* NCS_BUILD_UNICODE */
/* Define agnostic macros to provide ASCII character support */

#define NCSTChar char
#define NCS_T 
#define NCSTCmp(x,y) stricmp(x,y)
#define NCSTCat(x,y) strcat(x,y)
#define NCSTLen(x) strlen(x)
#define NCSTCpy(x,y) strcpy(x,y)
#define NCSTNCpy(x,y,z) strncpy(x,y,z)
#define NCSTDup(x)	NCSStrDup(x)

#ifdef _WIN32
#define NCSTLwr _strlwr
#define NCSTStr strstr
#define NCSWinGPA_T(x) x
#endif

#define OS_STRING(lpa)		(lpa)
#define CHAR_STRING(lpw)	(char *)(lpw)

#endif	/* NCS_BUILD_UNICODE */

#endif /* NCS_TCHAR */


#if defined(_WIN32)&&!defined(_WIN32_WCE)&&((defined(_M_IX86) && defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 12008804))||(defined(_M_X64) && defined(_MSC_VER) && _MSC_VER >= 1400))
//
// X86 "Multi Media Intrinsics" - ie, MMX, SSE, SSE2 optimisations
//
// Only defined if Win32, X86 and VC6 Processor Pack or newer _OR_ AMD64 and >= VS.NET 2005/AMD64 PlatformSDK compiler (ie, SSE intrinsics support in the compiler)
//
#define NCS_X86_MMI

#ifndef _M_X64
#define NCS_X86_MMI_MMX
#endif

//FIXME MJS compiled out SSE code as its never worked
#if (_MSC_VER >= 1500 || (__GNUC__ == 4 && __GNUC_MINOR__ >=3) || (__GNUC__ > 4) || __INTEL_COMPILER >= 1000)
//#define NCS_X86_MMI_SSSE3
//#define NCS_X86_MMI_SSE41
#endif

#endif

#if defined(_WIN32_WCE)&&defined(_M_ARM)
#define NCS_XSCALE_WMMX
#endif

#ifdef __cplusplus
}
#endif

#endif /* NCSDEFS_H */
