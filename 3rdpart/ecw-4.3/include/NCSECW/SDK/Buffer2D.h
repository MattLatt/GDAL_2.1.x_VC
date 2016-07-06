/********************************************************
** Copyright (c) 2010, ERDAS, Inc. [http://www.erdas.com/]
** This document contains proprietary source code of
** ERDAS, Inc.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     Buffer2D.h 
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CBuffer2D class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**	     [01] 12Dec05 tfl  lint fixes
**		 [02] 25Jan06 tfl  Added Multiply methods to support CFile autoscale feature
 *******************************************************/

#ifndef NCSJPCBuffer2D_H
#define NCSJPCBuffer2D_H

#ifndef NCSJPCRECT_H
#include "NCSEcw/SDK/Rect2D.h"
#endif // NCSJPCRECT_H

#ifndef NCSMISC_H
#include "NCSMisc.h"
#endif
#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif
#ifndef NCSPOOLEDALLOCATOR_H
#include "NCSPooledAllocator.h"
#endif // NCSPOOLEDALLOCATOR_H

#ifndef NCSEVENT_H
#include "NCSEvent.h"
#endif //NCSEVENT_H

namespace tbb {
	template <typename T> struct atomic;
}

#include <vector>

// Don't change this, needs to be inline with the internal VPU implementation
#define NCSVPU_ALIGNMENT 0x10	// original
//#define NCSVPU_ALIGNMENT32 0x20	// for T_VPU<INT16, T_VPU_SSE41>, tested on 29/09/2011

#pragma warning(push)
#pragma warning(disable:4251 4127 4275)

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#undef MAXUINT8
#undef MAXUINT16
#undef MAXUINT32
#undef MAXUINT64
#undef MAXINT8
#undef MAXINT16
#undef MAXINT32
#undef MAXINT64
#undef MINUINT8
#undef MINUINT16
#undef MINUINT32
#undef MINUINT64
#undef MININT8
#undef MININT16
#undef MININT32
#undef MININT64
#endif

namespace NCS {
namespace SDK {
	/**
	 * CBuffer2D class - the base JPC scanline buffer.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #3 $ $Author: jshao $ $Date: 2012/02/21 $ 
	 */	
class NCSECW_IMPEX CBuffer2D: public CRect2D {
public:
		/** Maximum UINT8 value */
	static const UINT32 MAXUINT8;
		/** Maximum UINT16 value */
	static const UINT32 MAXUINT16;
		/** Maximum UINT32 value */
	static const UINT32 MAXUINT32;
		/** Maximum UINT64 value */
	static const UINT64 MAXUINT64;
		/** Minimum UINT8 value */
	static const UINT32 MINUINT8;
		/** Minimum UINT16 value */
	static const UINT32 MINUINT16;
		/** Minimum UINT32 value */
	static const UINT32 MINUINT32;
		/** Minimum UINT64 value */
	static const UINT64 MINUINT64;

		/** Maximum INT8 value */
	static const UINT32 MAXINT8;
		/** Maximum INT16 value */
	static const UINT32 MAXINT16;
		/** Maximum INT32 value */
	static const UINT32 MAXINT32;
		/** Maximum INT64 value */
	static const UINT64 MAXINT64;
		/** Minimum INT8 value */
	static const INT32 MININT8;
		/** Minimum INT16 value */
	static const INT32 MININT16;
		/** Minimum INT32 value */
	static const INT32 MININT32;
		/** Minimum INT64 value */
	static const INT64 MININT64;
	
		/** Minimum IEEE4 value */
	static const IEEE4 MINIEEE4;
		/** Maximum IEEE4 value */
	static const IEEE4 MAXIEEE4;
		/** Minimum IEEE8 value */
	static const IEEE8 MINIEEE8;
		/** Maximum IEEE8 value */
	static const IEEE8 MAXIEEE8;

	typedef enum {
		BT_UINT8        = NCSCT_UINT8,
			/** s16 integer buffer type */
		BT_INT16		= NCSCT_INT16,
			/** s32 integer buffer type */
		BT_INT32		= NCSCT_INT32,
			/** s64 integer buffer type */
		BT_INT64		= NCSCT_INT64,
			/** IEEE4 single precision float buffer type */
		BT_IEEE4		= NCSCT_IEEE4
	} Type;

		/** FLAG: This buffer owns resources */
	static const UINT32 AT_OWNER;	
		/** FLAG: A child node returned the buffer and will free it later */
	static const UINT32 AT_CHILD;	
		/** Buffer is locked and can't be assigned to. */
	static const UINT32 AT_LOCKED;	

		/** Buffer type */
	Type	m_eType;
		/** buffer flags */
	UINT32	m_Flags;

		/** Actual buffer pointer */
	void	*m_pBuffer;

		/** Buffer Stride size to next line */
	int		m_nStride;

		/** Default constructor, initialises members */
	CBuffer2D();
		/** Copy constructor */
	CBuffer2D( const  CBuffer2D& src );
		/** constructor, specify rect values */
	CBuffer2D(UINT32 nWidth, Type eType);
		/** constructor, specify rect values */
	CBuffer2D(UINT32 nWidth, UINT32 nHeight, Type eType);
		/** constructor, specify rect values */
	CBuffer2D(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1, Type eType);
		/** constructor, specify rect values */
	CBuffer2D(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType);
		/** Virtual destructor */
	virtual ~CBuffer2D();
	
		/** Assignment operator */
	CBuffer2D& operator=( const CBuffer2D& src );

		/** Alloc a buffer the specified width and type */
	bool Alloc(UINT32 nWidth, Type eType);
		/** Alloc a buffer the specified width, height and type */
	bool Alloc(UINT32 nWidth, UINT32 nHeight, Type eType);
		/** Alloc a buffer with the specified TL, BR and type */
	bool Alloc(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1, Type eType);
		/** Alloc a buffer with the specified TL, width, height and type */
	bool Alloc(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType);
		/** Alloc a buffer with the specified CRect and type */
	bool Alloc(CRect2D &rect, Type eType);		
		/** Free the buffer */
	void Free();

		/** Assign a buffer to this buffer */
	bool Assign(Type eType, void *pPtr, INT32 nStride = CBuffer2D::MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(UINT32 nWidth, Type eType, void *pPtr, INT32 nStride = MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(UINT32 nWidth, UINT32 nHeight, Type eType, void *pPtr, INT32 nStride = MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1, Type eType, void *pPtr, INT32 nStride = MININT32, bool bFree = true);
		/** Assign a buffer to this buffer */
	bool Assign(INT32 nX0, INT32 nY0, UINT32 nWidth, UINT32 nHeight, Type eType, void *pPtr, INT32 nStride = CBuffer2D::MININT32, bool bFree = true);
	bool Assign(CRect2D &rect, Type eType, void *pPtr, INT32 nStride = CBuffer2D::MININT32, bool bFree = true) {
					return(Assign(rect.GetX0(), rect.GetY0(), rect.GetX1(), rect.GetY1(), eType, pPtr, nStride, bFree));
				}
		/** Release this buffer */
	void Release();

		/** Set flags */
	void SetFlag(UINT32 Flag) { m_Flags |= Flag; };
		/** Unset flags */
	void UnSetFlags(UINT32 Flags) { m_Flags &= ~Flags; };
		/** Get flags */
	UINT32 GetFlags() { return(m_Flags); };
		/** test flags */
	bool TestFlags(UINT32 Flag) const { return((m_Flags & Flag) ? true : false); };

		/** 
		 * Return the size of a cell in bytes
		 * @param		eType		Type of cell
		 * @return      UINT32		calculated buffer size in bytes.
		 */
	static NCS_INLINE UINT32 Size(Type eType) {
			switch(eType) {
				case BT_UINT8:
						return(sizeof(INT8));
					break;
				case BT_INT16:
						return(sizeof(INT16));
					break;
				case BT_INT32:
						return(sizeof(INT32));
					break;
				case BT_INT64:
						return(sizeof(INT64));
					break;
				case BT_IEEE4:
						return(sizeof(IEEE4));
					break;
			}
			return(0);
		}
		/** 
		 * Return the size of a buffer in bytes (default 1 cell).
		 * @param		eType		Type of buffer
		 * @param		nWidth		Number of pixels in buffer.
		 * @return      UINT32		calculated buffer size in bytes.
		 */
	static NCS_INLINE UINT32 Size(Type eType, UINT32 nWidth, UINT32 nHeight = 1) {
			////UINT32 nLineSize = Size(eType) * nWidth;
			
			////if(nHeight != 1 && nLineSize % NCSVPU_ALIGNMENT) {
			//	// Pad scanlines to 32 byte multiples for performance
			//	// when the buffer is > 1 line high 
			////nLineSize = NCSCeilDiv(Size(eType) * nWidth, NCSVPU_ALIGNMENT) * NCSVPU_ALIGNMENT;
			return(NCSCeilDiv(Size(eType) * nWidth, NCSVPU_ALIGNMENT) * NCSVPU_ALIGNMENT * nHeight); // original
			//if (eType == CBuffer2D::BT_INT32)
			//	return(NCSCeilDiv(Size(eType) * nWidth, NCSVPU_ALIGNMENT32) * NCSVPU_ALIGNMENT32 * nHeight); // for T_VPU<INT16, T_VPU_SSE41>
			//else
			//	return(NCSCeilDiv(Size(eType) * nWidth, NCSVPU_ALIGNMENT) * NCSVPU_ALIGNMENT * nHeight); 
		}
		/** Get buffer Stride in cells */
	NCS_INLINE UINT32 GetStride() const { return(m_nStride); };
		/** Get buffer Stride in bytes */
	NCS_INLINE UINT32 GetStrideBytes() const { return(m_nStride * Size(m_eType)); };
		/** get buffer type */
	NCS_INLINE Type GetType() const { return(m_eType); };
		/** get pointer to start of buffer (0,0) */
	NCS_INLINE void *GetPtr() { return(m_pBuffer); };
		/**	get pointer to buffer at (x,y) */
	NCS_INLINE void *GetPtr(INT32 x, INT32 y) const { 
		return((UINT8*)m_pBuffer + (y - GetY0()) * (INT32)GetStrideBytes() + (x - GetX0()) * (INT32)Size(GetType())); };

		/** 
		 * Return the size of the Wavelet Tranform border for the specified pipeline type.
		 * @param		eType		Type of buffer
		 * @return      UINT32		Width/Height of border in CELLS.
		 */
	static NCS_INLINE UINT32 Border(Type eType) {
			switch(eType) {
				case BT_UINT8:
				case BT_INT16:
				case BT_INT32:
				case BT_INT64:
						return(1);
					//break;
				case BT_IEEE4:
						return(2);
					//break;
			}
			return(0);
		}

		/** Clear buffer to 0 */
	void Clear();
	static bool Clear(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, Type eType);
	bool Clear(CRect2D &clip);
		/** 
		 * Convert a line to a line of type T.
		 * @param		pDst		Pointer to Destination buffer
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Convert(UINT8 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT8, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(UINT16 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT16, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(UINT32 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT32, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(UINT64 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT64, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT8 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT8, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT16 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT16, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT32 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT32, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(INT64 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT64, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(IEEE4 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_IEEE4, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
	virtual bool Convert(IEEE8 *pDst, UINT8 nStep, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_IEEE8, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), bCanDoScaling, nScaleRatio));
	}
#ifdef AUTO_SCALE
	virtual bool Convert(UINT8 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT8, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(UINT16 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT16, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(UINT32 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT32, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(UINT64 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_UINT64, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(INT8 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT8, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(INT16 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT16, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(INT32 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT32, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(INT64 *pDst, UINT8 nStep, INT64 nScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_INT64, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), nScaleOffset, dScaleFactor));
	}
	virtual bool Convert(IEEE4 *pDst, UINT8 nStep, IEEE8 dScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_IEEE4, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), dScaleOffset, dScaleFactor));
	}
	virtual bool Convert(IEEE8 *pDst, UINT8 nStep, IEEE8 dScaleOffset, IEEE4 dScaleFactor) const {
		return(CBuffer2D::ConvType(pDst, NCSCT_IEEE8, nStep, m_pBuffer, (NCSEcwCellType)m_eType, 1, GetWidth(), dScaleOffset, dScaleFactor));
	}
#endif

		/** 
		 * Read buffer to a type T.
		 * @param		pDst		Pointer to Destination buffer
		 * @return      bool		true on succes, else false.
		 */
	template <class T> bool Read(T *pDst, UINT8 nStep = 1, bool bCanDoScaling = false, UINT8 nScaleRatio = 2) const {
			for(UINT32 y = 0; y < GetHeight(); y++) {
				Convert(pDst, nStep, bCanDoScaling, nScaleRatio);
			}
			return(true);
		}

	template <class DT, class ST> bool Read(DT *pDst, UINT8 nStep, ST SrcMin, ST SrcMax, DT DstMin, DT DstMax) const {
			switch(GetType()) {
				case BT_UINT8:
					return Read<DT, ST, UINT8>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_INT16:
					return Read<DT, ST, INT16>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_INT32:
					return Read<DT, ST, INT32>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_INT64:
					return Read<DT, ST, INT64>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				case BT_IEEE4:
					return Read<DT, ST, IEEE4>(pDst, nStep, SrcMin, SrcMax, DstMin, DstMax);
				default:
					break;
			}
			return(false);
		}

	NCS_INLINE bool Read(CBuffer2D *pDst, CRect2D &clip) const {
			CBuffer2D tmp;
			CRect2D intersect = clip & *pDst;
			tmp.Assign(intersect,
					   pDst->GetType(), 
					   pDst->GetPtr(intersect.GetX0(), intersect.GetY0()), 
					   pDst->GetStride());
			return(Read(&tmp));
		}
		/** Read from the buffer to the destination buffer, copying only the intersecting area */
	NCS_INLINE bool Read(CBuffer2D *pDst) const {
			CRect2D r(*this & *pDst);

			if(pDst->GetType() == GetType()) {
//				return(Copy(r.GetWidth(), r.GetHeight(), 
//					(UINT8*)pDst->GetPtr() + (r.GetY0() - pDst->GetY0()) * pDst->GetStrideBytes() + Size(m_eType, r.GetX0() - pDst->GetX0()), 
//					pDst->GetStrideBytes(), 
//					(UINT8*)GetPtr() + (r.GetY0() - GetY0()) * GetStrideBytes() + Size(m_eType, r.GetX0() - GetX0()), 
//					GetStrideBytes(), 
//					m_eType));
				return(Copy(r.GetWidth(), r.GetHeight(), 
							pDst->GetPtr(r.GetX0(), r.GetY0()),
							pDst->GetStrideBytes(), 
							GetPtr(r.GetX0(), r.GetY0()),
							GetStrideBytes(), 
							m_eType));
			} else {
				for(UINT32 y = 0; y < r.GetHeight(); y++) {
					CBuffer2D tmp;
					tmp.Assign(r.GetX0(), r.GetY0(), r.GetWidth(), r.GetHeight(), GetType(), GetPtr(r.GetX0(), r.GetY0()), GetStride());
					switch(pDst->GetType()) {
						case BT_UINT8:
								tmp.Convert((UINT8*)pDst->GetPtr(r.GetX0(), r.GetY0()), 1);
							break;
						case BT_INT16:
								tmp.Convert((INT16*)pDst->GetPtr(r.GetX0(), r.GetY0()), 1);
							break;
						case BT_INT32:
								tmp.Convert((INT32*)pDst->GetPtr(r.GetX0(), r.GetY0()), 1);
							break;
						case BT_INT64:
								tmp.Convert((INT64*)pDst->GetPtr(r.GetX0(), r.GetY0()), 1);
							break;
						case BT_IEEE4:
								tmp.Convert((IEEE4*)pDst->GetPtr(r.GetX0(), r.GetY0()), 1);
							break;
					}
					tmp.Release();
/*					CBuffer2D tmp;
					tmp.Assign(r.GetX0(), r.GetY0(), r.GetWidth(), r.GetHeight(), GetType(), (UINT8*)GetPtr() + (y + r.GetY0() - GetY0()) * GetStrideBytes() + Size(m_eType, r.GetX0() - GetX0()));
					switch(pDst->GetType()) {
						case BT_INT16:
								tmp.Convert((INT16*)((UINT8*)pDst->GetPtr() + (y + r.GetY0() - pDst->GetY0()) * pDst->GetStrideBytes() + Size(m_eType, r.GetX0() - pDst->GetX0())), 1);
							break;
						case BT_INT32:
								tmp.Convert((INT32*)((UINT8*)pDst->GetPtr() + (y + r.GetY0() - pDst->GetY0()) * pDst->GetStrideBytes() + Size(m_eType, r.GetX0() - pDst->GetX0())), 1);
							break;
						case BT_INT64:
								tmp.Convert((INT64*)((UINT8*)pDst->GetPtr() + (y + r.GetY0() - pDst->GetY0()) * pDst->GetStrideBytes() + Size(m_eType, r.GetX0() - pDst->GetX0())), 1);
							break;
						case BT_IEEE4:
								tmp.Convert((IEEE4*)((UINT8*)pDst->GetPtr() + (y + r.GetY0() - pDst->GetY0()) * pDst->GetStrideBytes() + Size(m_eType, r.GetX0() - pDst->GetX0())), 1);
							break;
					}
					tmp.Release();*/
				}
			}
			return(true);
		}

		/** 
		 * Clip valus in the buffer to the specified range.
		 * @param		nMin		Min value
		 * @param		nMax		Max value
		 * @return      bool		true on success, else false & error set.
		 */
	bool Clip(INT64 nMin, INT64 nMax);
		/** 
		 * Clip valus in a buffer to the specified range.
		 * @param		fMin		Min value
		 * @param		fMax		Max value
		 * @return      bool		true on success, else false & error set.
		 */
	bool Clip(IEEE4 fMin, IEEE4 fMax);
		/** 
		 * Add a constant value to the buffer.
		 * @param		nValue		Constant value to add
		 * @param		nScale		Scale factor, *= NCS2Pow(-nScale)
		 * @return      bool		true on succes, else false.
		 */
	bool Add(const INT64 nValue, INT16 nScale = 0);
		/** 
		 * Add a constant value to the line.
		 * @param		fValue		Constant value to add
		 * @param		nScale		Scale factor, *= fScale
		 * @return      bool		true on succes, else false.
		 */
	bool Add(const IEEE4 fValue, INT16 nScale = 0);
		/** 
		 * Change bit depth (data range) of a buffer
		 * @param		nCurrentBits	Current bit depth
		 * @param		nNewBits		New bit depth
		 * @return      bool		true on succes, else false.
		 */
	bool OffsetShift(const INT64 nValue, UINT8 nCurrentBits, UINT8 nNewBits);
		/**
		 * Multiply the contents of the buffer by an integer.
		 * @param		nValue		Multiplication factor
		 * @return		bool		true on success, else false.
		 */
	bool Multiply(const IEEE4 fValue);
		/**
		 * Multiply the contents of the buffer by an integer.
		 * @param		nValue		Multiplication factor
		 * @return		bool		true on success, else false.
		 */
	bool Multiply(const INT32 nValue);
		/**
		 * Separate the contents of the buffer into low and hi values 
		 * @param		nLowValue	Low Value to set it to if less than Mid Value
		 * @param		nMidValue	MidValue 
		 * @param		nHighValue	High Value to set it to if greater than than Mid Value
		 * @return		bool		true on success, else false.
		 */
	//bool Separate(INT16 nLowValue, INT16 nMidValue, INT16 nHighValue, NCSEcwCellType eSrcType);
	bool Separate(INT64 nLowValue, INT64 nMidValue, INT64 nHighValue, NCSEcwCellType eSrcType);
	/** 
		 * Copy from one pointer to another (may overlap).
		 * @param		nWidth		Width in pixels.
		 * @param		nHeight		Height in pixels.
		 * @param		pDst		Dest buffer
		 * @param		nDstStep	Dest buffer step size
		 * @param		pSrc		Src buffer
		 * @param		nSrcStep	Src buffer step size
		 * @param		eType		Buffer Type
		 * @return      bool		true on success, else false & error set.
		 */
	static bool Copy(UINT32 nWidth, UINT32 nHeight, void *pDst, INT32 nDstStride, const void *pSrc, INT32 nSrcStride, Type eType);
	static bool CopyFlip(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, const void *pSrc, int nSrcStride, Type eType, bool bVertical);
	static bool CopyMultiply(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, const void *pSrc, int nSrcStride, Type eType, const INT64 nValue);
	static bool CopyShift(UINT32 nWidth, UINT32 nHeight, void *pDst, int nDstStride, const void *pSrc, int nSrcStride, Type eType, const int nShift);

	static bool ConvType(void *pDst, Type eDstType, UINT8 nDstStep, void *pSrc, Type eSrcType, UINT8 nSrcStep, UINT32 nWidth, bool bCanDoScaling = false, UINT8 nScaleRatio = 2);
	static bool ConvType(void *pDst, NCSEcwCellType eDstType, UINT8 nDstStep, void *pSrc, NCSEcwCellType eSrcType, UINT8 nSrcStep, UINT32 nWidth, bool bCanDoScaling = false, UINT8 nScaleRatio = 2);
	
#ifdef AUTO_SCALE
	static bool ConvType(void *pDst, NCSEcwCellType eDstType, UINT8 nDstStep, void *pSrc, NCSEcwCellType eSrcType, UINT8 nSrcStep, UINT32 nWidth, INT64 nScaleOffset, IEEE4 dScaleFactor);
	static bool ConvType(void *pDst, NCSEcwCellType eDstType, UINT8 nDstStep, void *pSrc, NCSEcwCellType eSrcType, UINT8 nSrcStep, UINT32 nWidth, IEEE8 nScaleOffset, IEEE4 dScaleFactor);
#endif

	static Type GetTypeFromCellType(NCSEcwCellType eType);
	
	static void sPurgeFreeQueue();

	static void Init();
	static void Fini();
protected:
	static CPooledAllocator sm_Allocator;
private:
	bool Alloc();
	//static tbb::cache_aligned_allocator<UINT8> sm_allocator;


	template <class DT, class ST, class BT> bool Read(DT *pDst, UINT8 nStep, ST SrcMin, ST SrcMax, DT DstMin, DT DstMax) const {
		DT *pD = pDst;
		IEEE4 nF = DstMax - DstMin;
		nF /= (SrcMax - SrcMin);
		for(UINT32 y = GetY0(); y < GetY1(); y++) {
			BT *pS = (BT*)GetPtr(GetX0(), y);
			for(UINT32 x = 0; x < GetWidth(); x++) {
				//*pD = (DT)(((pS[x] - SrcMin) * DstMax) / (SrcMax - SrcMin));
				*pD = MIN(MAX((DT)((pS[x] - SrcMin) * nF), DstMin), DstMax);
				pD += nStep;
			}
		}
		return true;
	}

	public:
		/** Buffer copy US */
	static tbb::atomic<NCSTimeStampUs>	*sm_pusCopy;
		/** buffer copy bytes */
	static tbb::atomic<UINT64>			*sm_pnCopyBytes;
		/** buffer clear US */
	static tbb::atomic<NCSTimeStampUs>	*sm_pusClear;
		/** buffer clear bytes */
	static tbb::atomic<UINT64>			*sm_pnClearBytes;
		/** buffer Alloc US */
	static tbb::atomic<NCSTimeStampUs>	*sm_pusAlloc;
		/** Current buffer alloc bytes */
	static tbb::atomic<UINT64>	*sm_pnAllocBytes;
		/** buffer free US */
	static tbb::atomic<NCSTimeStampUs>	*sm_pusFree;
};

typedef std::vector<CBuffer2D> CBuffer2DVector;

class NCSECW_IMPEX CBuffer2DCache {
public:
	virtual ~CBuffer2DCache() {};
	virtual void SetBuffers(UINT32 nBuffers);
	virtual CBuffer2D *GetBuffer(UINT32 nBuffer);
	virtual CBuffer2D *FindBuffer(UINT32 &nBuffer, CRect2D &r, bool &bFound, CBuffer2D::Type eType);
private:
	CBuffer2DVector m_Buffers;
};

class NCSECW_IMPEX CBuffer2DQueue: public std::vector<CBuffer2DVector*>, public CEvent, public CMutex {
public:
	CBuffer2DQueue();
	virtual ~CBuffer2DQueue();
	virtual CError Alloc(UINT32 nLength, UINT32 nBuffers);
	virtual CError Free();
	virtual CBuffer2DVector *NextBuffer();
	virtual void PushBuffer(CBuffer2DVector *pBuffer);
};
}
}
#pragma warning(pop)

#endif // !NCSJPCBuffer2D_H
