/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/] .
** This document contains proprietary source code of
** ERDAS, Inc.,  
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/

#ifndef NCSSTRING_H
#define NCSSTRING_H

// shut compiler up!
#pragma warning(push)
#pragma warning(disable:4786)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

#ifdef __cplusplus

#include "NCSDefs.h"
#include "NCSMisc.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

namespace NCS {

#ifdef NCS_BUILD_UNICODE
class NCSUTIL_IMPEX CString : public std::wstring
#else
class NCSUTIL_IMPEX CString : public std::string
#endif
{
	// size of Format() buffer.
	enum { _MAX_CHARS = 8096 };

public:

	CString();
	~CString();

	// various constructor flavours
	CString(const CString& arg)	{ assign(arg.c_str()); }
#ifdef NCS_BUILD_UNICODE
	CString(const std::wstring& arg)	{ assign(arg); }
	CString(const char* pArg);
	CString(const std::string& arg)	{ (*this)=arg.c_str(); }
#else
	CString(const std::string& arg)	{ assign(arg); }
#endif
	CString(const NCSTChar* pArg)		{ assign(pArg?pArg:NCS_T("")); }

	// various assignment operator flavours
	CString& operator=(const NCSTChar* pArg)		{ assign(pArg?pArg:NCS_T("")); return (*this); }
#ifdef NCS_BUILD_UNICODE
	CString& operator=(const char* pArg);
	CString& operator=(const std::wstring& arg)	{ assign(arg); return (*this); }
	CString& operator=(const std::string& arg)	{ (*this)=arg.c_str(); return (*this); }
#else
	CString& operator=(const std::string& arg)	{ assign(arg); return (*this); }
#endif
	CString& operator=(const CString& arg)	{ assign(arg.c_str()); return (*this); }

	CString& operator+=(const NCSTChar* pArg)	{ append(pArg?pArg:NCS_T("")); return (*this); }
#ifdef NCS_BUILD_UNICODE
	CString& operator+=(const char* pArg);
	CString& operator+=(const std::wstring& arg)	{ append(arg); return (*this); }
	CString& operator+=(const std::string& arg)	{ (*this)+=arg.c_str(); return (*this); }
#else
	CString& operator+=(const std::string& arg)	{ assign(arg); return (*this); }
#endif

	// the essential MFC member functions
	int Format(const NCSTChar* szFormat,...);
#ifdef NCS_BUILD_UNICODE
	int Format(const char* szFormat,...);
#endif

	// this returns a reference so you can do things like this:
	// CString query = "abc";
	// query += CString().Format("%d %s",1,"abc");
	CString& FormatEx(const NCSTChar* szFormat,...);

		// trim the right hand of the string of whitespace characters
	CString& TrimRight(const NCSTChar* pszDelimiters = NCS_T(" \t\r\n") );

    // trim the right hand of the string of whitespace characters
    CString& TrimLeft(const NCSTChar* pszDelimiters = NCS_T(" \t\r\n") );

    // trim leading and trailing whitespace
    CString& TrimAll(const NCSTChar* pszDelimiters = NCS_T(" \t\r\n") );

	// convert to upper and lower case
	CString& MakeUpper();
	CString& MakeLower();
	bool EndsWith(const CString& sEnd, bool bCompareNoCase=true) const;

	INT32 ToINT32()const;
	INT64 ToINT64()const;
	IEEE8 ToIEEE8()const;

	bool CompareNoCase( const CString& input )const;
	bool CompareNoCase( const CString& input, size_t nChars )const;

	int Split(const CString& delimiter, std::vector<CString>& results, bool bIncludeEmpty=false)const;

	static int Split(const CString& input, const CString& delimiter, std::vector<CString>& results, bool bIncludeEmpty=false);
	static int CompareNoCase( const CString& first, const CString& second );
	static int CompareNoCase( const CString& first, const CString& second, size_t nChars );

// stupid hack for VS2010 need to forward declare npos to resolve lnk2001
#if _MSC_VER == 1600
	static size_t npos;
#endif

#ifdef NCS_BUILD_UNICODE
	CString Substr(size_type pos = 0, size_type n = npos) const;

	const char *a_str()const;
	mutable std::string m_sAsciiString;
	mutable std::string m_utf8String;
#else
	inline CString Substr(size_type pos = 0, size_type n = npos) const
	{
		return substr( pos, n );
	}

	const char *a_str() const { return c_str(); };
#endif
	operator const char*() { return(a_str()); };
#ifdef NCS_BUILD_UNICODE
	operator const wchar_t*() { return(c_str()); };
#endif

	int ReplaceAll( const CString& match, const CString& value );

	void UrlUnEsc();
	static void UrlUnEsc( const CString& src, CString& dest );
	void UrlEsc();
	static void UrlEsc( const CString& src, CString& dest );

	static std::string Utf8Encode (const std::wstring &sValue);
	static std::wstring Utf8Decode (const std::string &sUtf8);

	static void Utf8Encode (std::string& utf8Str, const std::wstring &sValue);
	static void Utf8Decode (std::wstring& sUtf16, const std::string &sUtf8);
#ifdef NCS_BUILD_UNICODE
	const char* utf8_str() const { 
		m_utf8String.clear();
		Utf8Encode(m_utf8String, *this);
		return m_utf8String.c_str(); 
	}
	void FromUtf8(const char* cpUtf8){ if(cpUtf8){ Utf8Decode(*this,  std::string(cpUtf8)); } }
#else
	const char* utf8_str() const { return c_str(); }
	void FromUtf8(const char* cpUtf8) { *this = cpUtf8; }
#endif
	/**
	 * Escape characters that will interfer with xml.
	 *
	 * @param strIn The input string to escape.
	 * @param strOut encoded for insertion into xml.
	 */
	static void EscForXml( const CString& strIn, CString& strOut);
};

}

#pragma warning(pop)

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CString CNCSString;
#endif

#endif //__cplusplus

#endif //NCSSTRING_H
