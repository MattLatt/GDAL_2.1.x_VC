/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     CompressedBuffer2D.h $
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CBuffer2D class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**	     [01] 12Dec05 tfl  lint fixes
**		 [02] 25Jan06 tfl  Added Multiply methods to support CFile autoscale feature
 *******************************************************/

#ifndef NCSCOMPRESSEDBUFFER2D_H
#define NCSCOMPRESSEDBUFFER2D_H

#ifndef NCSJPCBuffer2D_H
#include "NCSEcw/SDK/Buffer2D.h"
#endif // NCSJPCBuffer2D_H

#pragma warning(push)
#pragma warning(disable:4251 4127 4275)

namespace NCS {
namespace SDK {
	/**
	 * CBuffer2D class - the base JPC scanline buffer.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #8 $ $Author: scope $ $Date: 2008/05/08 $ 
	 */	
class NCSECW_IMPEX CCompressedBuffer2D: public CBuffer2D {
public:
		/** Buffer is currently compressed. */
	static const UINT32 AT_COMPRESSED;	

		/** Default constructor, initialises members */
	CCompressedBuffer2D();
		/** Virtual destructor */
	virtual ~CCompressedBuffer2D();
	
		/** Assignment operator */
	class CCompressedBuffer2D& CCompressedBuffer2D::operator=( const class CCompressedBuffer2D& src );
	
		/** Free the buffer */
	void Free();

		/** Assign a buffer to this buffer */
	bool Assign(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType, void *pPtr, INT32 nStride = CBuffer2D::MININT32, bool bFree = true);

		/** Release this buffer */
	void Release();

		/** Set flags */
	void SetFlag(UINT32 Flag) { m_Flags |= Flag; };
		/** Unset flags */
	void UnSetFlags(UINT32 Flags) { m_Flags &= ~Flags; };

		/** get pointer to start of buffer (0,0) */
	NCS_INLINE void *GetPtr() { return(m_pBuffer); };
		/**	get pointer to buffer at (x,y) */
	NCS_INLINE void *GetPtr(INT32 x, INT32 y) { return((UINT8*)m_pBuffer + (y - GetY0()) * (INT32)GetStrideBytes() + (x - GetX0()) * (INT32)Size(GetType())); };

	NCS_INLINE ReleasePtr(void *p);
private:
	bool Alloc();
};

}
}
#pragma warning(pop)

#endif // !NCSCOMPRESSEDBUFFER2D_H
